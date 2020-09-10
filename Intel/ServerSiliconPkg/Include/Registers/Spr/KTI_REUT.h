
/** @file
  KTI_REUT.h

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
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _KTI_REUT_h
#define _KTI_REUT_h
#include <Base.h>

/* VID_1_KTI_REUT_REG supported on:                                             */
/*      SPRA0 (0x11e0a000)                                                      */
/*      SPRB0 (0x11e0a000)                                                      */
/*      SPRHBM (0x11e0a000)                                                     */
/*      SPRC0 (0x11e0a000)                                                      */
/*      SPRMCC (0x11e0a000)                                                     */
/*      SPRUCC (0x11e0a000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Vendor ID Register
*/


#define VID_1_KTI_REUT_REG 0x09110000

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

} VID_1_KTI_REUT_STRUCT;

/* DID_1_KTI_REUT_REG supported on:                                             */
/*      SPRA0 (0x11e0a002)                                                      */
/*      SPRB0 (0x11e0a002)                                                      */
/*      SPRHBM (0x11e0a002)                                                     */
/*      SPRC0 (0x11e0a002)                                                      */
/*      SPRMCC (0x11e0a002)                                                     */
/*      SPRUCC (0x11e0a002)                                                     */
/* Register default value on SPRA0: 0x00003241                                  */
/* Register default value on SPRB0: 0x00003241                                  */
/* Register default value on SPRHBM: 0x00003241                                 */
/* Register default value on SPRC0: 0x00003242                                  */
/* Register default value on SPRMCC: 0x00003242                                 */
/* Register default value on SPRUCC: 0x00003242                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Device Identification Number
*/


#define DID_1_KTI_REUT_REG 0x09110002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00003241*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_1_KTI_REUT_STRUCT;

/* PCICMD_1_KTI_REUT_REG supported on:                                          */
/*      SPRA0 (0x11e0a004)                                                      */
/*      SPRB0 (0x11e0a004)                                                      */
/*      SPRHBM (0x11e0a004)                                                     */
/*      SPRC0 (0x11e0a004)                                                      */
/*      SPRMCC (0x11e0a004)                                                     */
/*      SPRUCC (0x11e0a004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command Register
*/


#define PCICMD_1_KTI_REUT_REG 0x09110004

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

} PCICMD_1_KTI_REUT_STRUCT;

/* PCISTS_1_KTI_REUT_REG supported on:                                          */
/*      SPRA0 (0x11e0a006)                                                      */
/*      SPRB0 (0x11e0a006)                                                      */
/*      SPRHBM (0x11e0a006)                                                     */
/*      SPRC0 (0x11e0a006)                                                      */
/*      SPRMCC (0x11e0a006)                                                     */
/*      SPRUCC (0x11e0a006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status Register
*/


#define PCISTS_1_KTI_REUT_REG 0x09110006

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

} PCISTS_1_KTI_REUT_STRUCT;

/* RID_CCR_1_KTI_REUT_REG supported on:                                         */
/*      SPRA0 (0x21e0a008)                                                      */
/*      SPRB0 (0x21e0a008)                                                      */
/*      SPRHBM (0x21e0a008)                                                     */
/*      SPRC0 (0x21e0a008)                                                      */
/*      SPRMCC (0x21e0a008)                                                     */
/*      SPRUCC (0x21e0a008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_1_KTI_REUT_REG 0x09120008

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

} RID_CCR_1_KTI_REUT_STRUCT;

/* CLSR_1_KTI_REUT_REG supported on:                                            */
/*      SPRA0 (0x1e0a00c)                                                       */
/*      SPRB0 (0x1e0a00c)                                                       */
/*      SPRHBM (0x1e0a00c)                                                      */
/*      SPRC0 (0x1e0a00c)                                                       */
/*      SPRMCC (0x1e0a00c)                                                      */
/*      SPRUCC (0x1e0a00c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Cache Line Size Register
*/


#define CLSR_1_KTI_REUT_REG 0x0910000C

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

} CLSR_1_KTI_REUT_STRUCT;

/* PLAT_1_KTI_REUT_REG supported on:                                            */
/*      SPRA0 (0x1e0a00d)                                                       */
/*      SPRB0 (0x1e0a00d)                                                       */
/*      SPRHBM (0x1e0a00d)                                                      */
/*      SPRC0 (0x1e0a00d)                                                       */
/*      SPRMCC (0x1e0a00d)                                                      */
/*      SPRUCC (0x1e0a00d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Latency Timer
*/


#define PLAT_1_KTI_REUT_REG 0x0910000D

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

} PLAT_1_KTI_REUT_STRUCT;

/* HDR_1_KTI_REUT_REG supported on:                                             */
/*      SPRA0 (0x1e0a00e)                                                       */
/*      SPRB0 (0x1e0a00e)                                                       */
/*      SPRHBM (0x1e0a00e)                                                      */
/*      SPRC0 (0x1e0a00e)                                                       */
/*      SPRMCC (0x1e0a00e)                                                      */
/*      SPRUCC (0x1e0a00e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Header Type
*/


#define HDR_1_KTI_REUT_REG 0x0910000E

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

} HDR_1_KTI_REUT_STRUCT;

/* BIST_1_KTI_REUT_REG supported on:                                            */
/*      SPRA0 (0x1e0a00f)                                                       */
/*      SPRB0 (0x1e0a00f)                                                       */
/*      SPRHBM (0x1e0a00f)                                                      */
/*      SPRC0 (0x1e0a00f)                                                       */
/*      SPRMCC (0x1e0a00f)                                                      */
/*      SPRUCC (0x1e0a00f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI BIST Register
*/


#define BIST_1_KTI_REUT_REG 0x0910000F

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

} BIST_1_KTI_REUT_STRUCT;

/* SVID_1_KTI_REUT_REG supported on:                                            */
/*      SPRA0 (0x11e0a02c)                                                      */
/*      SPRB0 (0x11e0a02c)                                                      */
/*      SPRHBM (0x11e0a02c)                                                     */
/*      SPRC0 (0x11e0a02c)                                                      */
/*      SPRMCC (0x11e0a02c)                                                     */
/*      SPRUCC (0x11e0a02c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID Register
*/


#define SVID_1_KTI_REUT_REG 0x0911002C

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

} SVID_1_KTI_REUT_STRUCT;

/* SDID_1_KTI_REUT_REG supported on:                                            */
/*      SPRA0 (0x11e0a02e)                                                      */
/*      SPRB0 (0x11e0a02e)                                                      */
/*      SPRHBM (0x11e0a02e)                                                     */
/*      SPRC0 (0x11e0a02e)                                                      */
/*      SPRMCC (0x11e0a02e)                                                     */
/*      SPRUCC (0x11e0a02e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID Register
*/


#define SDID_1_KTI_REUT_REG 0x0911002E

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

} SDID_1_KTI_REUT_STRUCT;

/* CAPPTR_1_KTI_REUT_REG supported on:                                          */
/*      SPRA0 (0x1e0a034)                                                       */
/*      SPRB0 (0x1e0a034)                                                       */
/*      SPRHBM (0x1e0a034)                                                      */
/*      SPRC0 (0x1e0a034)                                                       */
/*      SPRMCC (0x1e0a034)                                                      */
/*      SPRUCC (0x1e0a034)                                                      */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Capability Pointer Register
*/


#define CAPPTR_1_KTI_REUT_REG 0x09100034

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

} CAPPTR_1_KTI_REUT_STRUCT;

/* INTL_1_KTI_REUT_REG supported on:                                            */
/*      SPRA0 (0x1e0a03c)                                                       */
/*      SPRB0 (0x1e0a03c)                                                       */
/*      SPRHBM (0x1e0a03c)                                                      */
/*      SPRC0 (0x1e0a03c)                                                       */
/*      SPRMCC (0x1e0a03c)                                                      */
/*      SPRUCC (0x1e0a03c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Line Register
*/


#define INTL_1_KTI_REUT_REG 0x0910003C

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

} INTL_1_KTI_REUT_STRUCT;

/* INTPIN_1_KTI_REUT_REG supported on:                                          */
/*      SPRA0 (0x1e0a03d)                                                       */
/*      SPRB0 (0x1e0a03d)                                                       */
/*      SPRHBM (0x1e0a03d)                                                      */
/*      SPRC0 (0x1e0a03d)                                                       */
/*      SPRMCC (0x1e0a03d)                                                      */
/*      SPRUCC (0x1e0a03d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Pin Register
*/


#define INTPIN_1_KTI_REUT_REG 0x0910003D

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

} INTPIN_1_KTI_REUT_STRUCT;

/* MINGNT_1_KTI_REUT_REG supported on:                                          */
/*      SPRA0 (0x1e0a03e)                                                       */
/*      SPRB0 (0x1e0a03e)                                                       */
/*      SPRHBM (0x1e0a03e)                                                      */
/*      SPRC0 (0x1e0a03e)                                                       */
/*      SPRMCC (0x1e0a03e)                                                      */
/*      SPRUCC (0x1e0a03e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Min Grant Register
*/


#define MINGNT_1_KTI_REUT_REG 0x0910003E

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

} MINGNT_1_KTI_REUT_STRUCT;

/* MAXLAT_1_KTI_REUT_REG supported on:                                          */
/*      SPRA0 (0x1e0a03f)                                                       */
/*      SPRB0 (0x1e0a03f)                                                       */
/*      SPRHBM (0x1e0a03f)                                                      */
/*      SPRC0 (0x1e0a03f)                                                       */
/*      SPRMCC (0x1e0a03f)                                                      */
/*      SPRUCC (0x1e0a03f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Max Latency Register
*/


#define MAXLAT_1_KTI_REUT_REG 0x0910003F

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

} MAXLAT_1_KTI_REUT_STRUCT;

/* PXPCAP_1_KTI_REUT_REG supported on:                                          */
/*      SPRA0 (0x21e0a040)                                                      */
/*      SPRB0 (0x21e0a040)                                                      */
/*      SPRHBM (0x21e0a040)                                                     */
/*      SPRC0 (0x21e0a040)                                                      */
/*      SPRMCC (0x21e0a040)                                                     */
/*      SPRUCC (0x21e0a040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_1_KTI_REUT_REG 0x09120040

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

} PXPCAP_1_KTI_REUT_STRUCT;

/* PCIE_RSVD0_1_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a044)                                                      */
/*      SPRB0 (0x21e0a044)                                                      */
/*      SPRHBM (0x21e0a044)                                                     */
/*      SPRC0 (0x21e0a044)                                                      */
/*      SPRMCC (0x21e0a044)                                                     */
/*      SPRUCC (0x21e0a044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD0_1_KTI_REUT_REG 0x09120044

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

} PCIE_RSVD0_1_KTI_REUT_STRUCT;

/* PCIE_RSVD1_1_N0_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a048)                                                      */
/*      SPRB0 (0x21e0a048)                                                      */
/*      SPRHBM (0x21e0a048)                                                     */
/*      SPRC0 (0x21e0a048)                                                      */
/*      SPRMCC (0x21e0a048)                                                     */
/*      SPRUCC (0x21e0a048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_1_N0_KTI_REUT_REG 0x09120048

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

} PCIE_RSVD1_1_N0_KTI_REUT_STRUCT;

/* PCIE_RSVD1_1_N1_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a04c)                                                      */
/*      SPRB0 (0x21e0a04c)                                                      */
/*      SPRHBM (0x21e0a04c)                                                     */
/*      SPRC0 (0x21e0a04c)                                                      */
/*      SPRMCC (0x21e0a04c)                                                     */
/*      SPRUCC (0x21e0a04c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_1_N1_KTI_REUT_REG 0x0912004C

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

} PCIE_RSVD1_1_N1_KTI_REUT_STRUCT;

/* PCIE_RSVD2_1_N0_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a050)                                                      */
/*      SPRB0 (0x21e0a050)                                                      */
/*      SPRHBM (0x21e0a050)                                                     */
/*      SPRC0 (0x21e0a050)                                                      */
/*      SPRMCC (0x21e0a050)                                                     */
/*      SPRUCC (0x21e0a050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_1_N0_KTI_REUT_REG 0x09120050

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

} PCIE_RSVD2_1_N0_KTI_REUT_STRUCT;

/* PCIE_RSVD2_1_N1_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a054)                                                      */
/*      SPRB0 (0x21e0a054)                                                      */
/*      SPRHBM (0x21e0a054)                                                     */
/*      SPRC0 (0x21e0a054)                                                      */
/*      SPRMCC (0x21e0a054)                                                     */
/*      SPRUCC (0x21e0a054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_1_N1_KTI_REUT_REG 0x09120054

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

} PCIE_RSVD2_1_N1_KTI_REUT_STRUCT;

/* PCIE_RSVD3_1_N0_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a058)                                                      */
/*      SPRB0 (0x21e0a058)                                                      */
/*      SPRHBM (0x21e0a058)                                                     */
/*      SPRC0 (0x21e0a058)                                                      */
/*      SPRMCC (0x21e0a058)                                                     */
/*      SPRUCC (0x21e0a058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_1_N0_KTI_REUT_REG 0x09120058

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

} PCIE_RSVD3_1_N0_KTI_REUT_STRUCT;

/* PCIE_RSVD3_1_N1_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a05c)                                                      */
/*      SPRB0 (0x21e0a05c)                                                      */
/*      SPRHBM (0x21e0a05c)                                                     */
/*      SPRC0 (0x21e0a05c)                                                      */
/*      SPRMCC (0x21e0a05c)                                                     */
/*      SPRUCC (0x21e0a05c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_1_N1_KTI_REUT_REG 0x0912005C

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

} PCIE_RSVD3_1_N1_KTI_REUT_STRUCT;

/* PCIE_RSVD4_1_N0_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a060)                                                      */
/*      SPRB0 (0x21e0a060)                                                      */
/*      SPRHBM (0x21e0a060)                                                     */
/*      SPRC0 (0x21e0a060)                                                      */
/*      SPRMCC (0x21e0a060)                                                     */
/*      SPRUCC (0x21e0a060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_1_N0_KTI_REUT_REG 0x09120060

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

} PCIE_RSVD4_1_N0_KTI_REUT_STRUCT;

/* PCIE_RSVD4_1_N1_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a064)                                                      */
/*      SPRB0 (0x21e0a064)                                                      */
/*      SPRHBM (0x21e0a064)                                                     */
/*      SPRC0 (0x21e0a064)                                                      */
/*      SPRMCC (0x21e0a064)                                                     */
/*      SPRUCC (0x21e0a064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_1_N1_KTI_REUT_REG 0x09120064

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

} PCIE_RSVD4_1_N1_KTI_REUT_STRUCT;

/* PCIE_RSVD5_1_N0_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a068)                                                      */
/*      SPRB0 (0x21e0a068)                                                      */
/*      SPRHBM (0x21e0a068)                                                     */
/*      SPRC0 (0x21e0a068)                                                      */
/*      SPRMCC (0x21e0a068)                                                     */
/*      SPRUCC (0x21e0a068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_1_N0_KTI_REUT_REG 0x09120068

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

} PCIE_RSVD5_1_N0_KTI_REUT_STRUCT;

/* PCIE_RSVD5_1_N1_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a06c)                                                      */
/*      SPRB0 (0x21e0a06c)                                                      */
/*      SPRHBM (0x21e0a06c)                                                     */
/*      SPRC0 (0x21e0a06c)                                                      */
/*      SPRMCC (0x21e0a06c)                                                     */
/*      SPRUCC (0x21e0a06c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_1_N1_KTI_REUT_REG 0x0912006C

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

} PCIE_RSVD5_1_N1_KTI_REUT_STRUCT;

/* PCIE_RSVD6_1_N0_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a070)                                                      */
/*      SPRB0 (0x21e0a070)                                                      */
/*      SPRHBM (0x21e0a070)                                                     */
/*      SPRC0 (0x21e0a070)                                                      */
/*      SPRMCC (0x21e0a070)                                                     */
/*      SPRUCC (0x21e0a070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_1_N0_KTI_REUT_REG 0x09120070

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

} PCIE_RSVD6_1_N0_KTI_REUT_STRUCT;

/* PCIE_RSVD6_1_N1_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a074)                                                      */
/*      SPRB0 (0x21e0a074)                                                      */
/*      SPRHBM (0x21e0a074)                                                     */
/*      SPRC0 (0x21e0a074)                                                      */
/*      SPRMCC (0x21e0a074)                                                     */
/*      SPRUCC (0x21e0a074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_1_N1_KTI_REUT_REG 0x09120074

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

} PCIE_RSVD6_1_N1_KTI_REUT_STRUCT;

/* PCIE_RSVD7_1_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a078)                                                      */
/*      SPRB0 (0x21e0a078)                                                      */
/*      SPRHBM (0x21e0a078)                                                     */
/*      SPRC0 (0x21e0a078)                                                      */
/*      SPRMCC (0x21e0a078)                                                     */
/*      SPRUCC (0x21e0a078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD7_1_KTI_REUT_REG 0x09120078

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

} PCIE_RSVD7_1_KTI_REUT_STRUCT;

/* KTITGTO_KTI_REUT_REG supported on:                                           */
/*      SPRA0 (0x21e0a0dc)                                                      */
/*      SPRB0 (0x21e0a0dc)                                                      */
/*      SPRHBM (0x21e0a0dc)                                                     */
/*      SPRC0 (0x21e0a0dc)                                                      */
/*      SPRMCC (0x21e0a0dc)                                                     */
/*      SPRUCC (0x21e0a0dc)                                                     */
/* Register default value on SPRA0: 0x00426800                                  */
/* Register default value on SPRB0: 0x00426800                                  */
/* Register default value on SPRHBM: 0x00426800                                 */
/* Register default value on SPRC0: 0x00426800                                  */
/* Register default value on SPRMCC: 0x00426800                                 */
/* Register default value on SPRUCC: 0x00426800                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* ktitgto
*/


#define KTITGTO_KTI_REUT_REG 0x091200DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 kti_0_disable_timeout : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Disable timer in kti_physical_layer_timeout. */
    UINT32 not_used : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* not_used */
    UINT32 rsvd : 6;

                            /* Bits[7:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 kti_physical_layer_timeout : 16;

                            /* Bits[23:8], Access Type=RW/P, default=0x00004268*/

                            /*
                               Value used by PCode to timeout on Intel UPI
                               training. Default value of about 17000uS
                               (0x4268) will work for slow boot. And BIOS will
                               adjust the value for full speed initialization.
                               Value defined in micro-seconds (uS) 0x0-0xFFFF
                               uSec
                            */
    UINT32 spare : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /* spare */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTITGTO_KTI_REUT_STRUCT;

/* PXPENHCAP_1_KTI_REUT_REG supported on:                                       */
/*      SPRA0 (0x21e0a100)                                                      */
/*      SPRB0 (0x21e0a100)                                                      */
/*      SPRHBM (0x21e0a100)                                                     */
/*      SPRC0 (0x21e0a100)                                                      */
/*      SPRMCC (0x21e0a100)                                                     */
/*      SPRUCC (0x21e0a100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_1_KTI_REUT_REG 0x09120100

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

} PXPENHCAP_1_KTI_REUT_STRUCT;

/* KTIREUT_HDR_CAP_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a104)                                                      */
/*      SPRB0 (0x21e0a104)                                                      */
/*      SPRHBM (0x21e0a104)                                                     */
/*      SPRC0 (0x21e0a104)                                                      */
/*      SPRMCC (0x21e0a104)                                                     */
/*      SPRUCC (0x21e0a104)                                                     */
/* Register default value on SPRA0: 0x0F000001                                  */
/* Register default value on SPRB0: 0x0F000001                                  */
/* Register default value on SPRHBM: 0x0F000001                                 */
/* Register default value on SPRC0: 0x0F000001                                  */
/* Register default value on SPRMCC: 0x0F000001                                 */
/* Register default value on SPRUCC: 0x0F000001                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_hdr_cap
*/


#define KTIREUT_HDR_CAP_KTI_REUT_REG 0x09120104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vsecid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000001*/

                            /*
                               This field is a Intel-defined ID number that
                               indicates the nature and format of the VSEC
                               structure. 01h is the ID Council defined for
                               REUT engines. Software must qualify the Vendor
                               ID before interpreting this field.
                            */
    UINT32 vsecidrev : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000000*/

                            /*
                               This field is defined as the version number that
                               indicates the nature and format of the VSEC
                               structure. Software must quality the Vendor ID
                               before interpreting this field.
                            */
    UINT32 vseclength : 12;

                            /* Bits[31:20], Access Type=RO, default=0x000000F0*/

                            /*
                               This field defines the length of the REUT
                               capability body. Currently the size of the body
                               is 184 bytes or 46 DWORDs.
                            */

  } Bits;
  UINT32 Data;

} KTIREUT_HDR_CAP_KTI_REUT_STRUCT;

/* KTIREUT_ENG_OVR_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a108)                                                      */
/*      SPRB0 (0x21e0a108)                                                      */
/*      SPRHBM (0x21e0a108)                                                     */
/*      SPRC0 (0x21e0a108)                                                      */
/*      SPRMCC (0x21e0a108)                                                     */
/*      SPRUCC (0x21e0a108)                                                     */
/* Register default value on SPRA0: 0x00400000                                  */
/* Register default value on SPRB0: 0x00400000                                  */
/* Register default value on SPRHBM: 0x00400000                                 */
/* Register default value on SPRC0: 0x00400000                                  */
/* Register default value on SPRMCC: 0x00400000                                 */
/* Register default value on SPRUCC: 0x00400000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_eng_ovr
*/


#define KTIREUT_ENG_OVR_KTI_REUT_REG 0x09120108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txrxonmultiplelinks : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               1 : REUT Engine can transmit and/or receive on
                               multiple Links simultaneously.
                            */
    UINT32 canconrlpartialsellinks : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               1 : REUT engine can control any possible subset
                               of multiple Links simultaneously.
                            */
    UINT32 separatesmctrl : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Separate PHY state machine control0: Only one
                               LTSSM for each link. 1: There is more than one
                               LTSSM for each link.
                            */
    UINT32 rsvd : 8;

                            /* Bits[10:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 reutengineid : 6;

                            /* Bits[16:11], Access Type=RO, default=0x00000000*/

                            /* REUT Engine ID[5:0] */
    UINT32 reutspecrev : 8;

                            /* Bits[24:17], Access Type=RO, default=0x00000020*/

                            /*
                               REUT revision[5:0][7:5]: major revision = 001b
                               [4:0]: minor revision = 0 0000b
                            */
    UINT32 numlinks : 5;

                            /* Bits[29:25], Access Type=RO, default=0x00000000*/

                            /*
                               Number of links supported. The number of links
                               supported +1 is the total number of separate
                               Links that this REUT Engine is designed to
                               support.
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_ENG_OVR_KTI_REUT_STRUCT;

/* KTIREUT_ENG_LD_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a10c)                                                      */
/*      SPRB0 (0x21e0a10c)                                                      */
/*      SPRHBM (0x21e0a10c)                                                     */
/*      SPRC0 (0x21e0a10c)                                                      */
/*      SPRMCC (0x21e0a10c)                                                     */
/*      SPRUCC (0x21e0a10c)                                                     */
/* Register default value on SPRA0: 0x00086273                                  */
/* Register default value on SPRB0: 0x00086273                                  */
/* Register default value on SPRHBM: 0x00086273                                 */
/* Register default value on SPRC0: 0x00086273                                  */
/* Register default value on SPRMCC: 0x00086273                                 */
/* Register default value on SPRUCC: 0x00086273                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_eng_ld
*/


#define KTIREUT_ENG_LD_KTI_REUT_REG 0x0912010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txlinkwidth : 5;

                            /* Bits[4:0], Access Type=RO, default=0x00000013*/

                            /*
                               Tx Link Width Width of the Link indicated by
                               LinkSelect in REUT-ENG-LS. Actual width is N+1.
                               13h = 19dec. Actual width is 19 + 1 = 20 lanes.
                               If link is asymmetric then only the RX width is
                               reported here.
                            */
    UINT32 rxlinkwidth : 5;

                            /* Bits[9:5], Access Type=RO, default=0x00000013*/

                            /*
                               Rx Link Width Width of the Link indicated by
                               LinkSelect in REUT-ENG-LS. Actual width is N+1.
                               13h = 19dec. Actual width is 19 + 1 = 20 lanes.
                               If link is asymmetric then only the RX width is
                               reported here.
                            */
    UINT32 rsvd : 3;

                            /* Bits[12:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 reutdevid : 8;

                            /* Bits[20:13], Access Type=RO, default=0x00000043*/

                            /*
                               Device/Function number. Describes the unique
                               Device/Fn number for the selected Link
                               (indicated by the LinkSelect setting). Device/Fn
                               number is a unique identifier that is component
                               specific that is not repeated for any two Links
                               of within the component (i.e. every Intel UPI
                               Link must have a unique device number but a
                               Intel UPI link can share a similar device
                               number. Intel UPI port 0 located at: Device8 =
                               01000b, function3 = 011b. ReutDevID = 0100 0011b
                               Intel UPI port 1 located at: Device9 = 01000b,
                               function3 = 011b. ReutDevID = 0100 1011b Intel
                               UPI port 2 located at: Device10 = 01010b,
                               function3 = 011b. ReutDevID = 0101 0011b
                            */
    UINT32 rsvd_21 : 2;

                            /* Bits[22:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 linkinactive : 1;

                            /* Bits[23:23], Access Type=RO/V, default=0x00000000*/

                            /*
                               Link Inactive: Indicates whether the current
                               Link (indicated by the Link Select) is in use.
                               In general this bit is used to indicate that
                               this link is disabled and no testing can be
                               performed. Intel UPI Link Behavior: Indicates
                               whether the selected Link is disabled.
                            */
    UINT32 linktype : 6;

                            /* Bits[29:24], Access Type=RO, default=0x00000000*/

                            /*
                               Link TypeIndicates the type of Link selected by
                               Link Select. 000000: Intel UPI
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_ENG_LD_KTI_REUT_STRUCT;

/* KTIREUT_ENG_LSCAP_KTI_REUT_REG supported on:                                 */
/*      SPRA0 (0x21e0a110)                                                      */
/*      SPRB0 (0x21e0a110)                                                      */
/*      SPRHBM (0x21e0a110)                                                     */
/*      SPRC0 (0x21e0a110)                                                      */
/*      SPRMCC (0x21e0a110)                                                     */
/*      SPRUCC (0x21e0a110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_eng_lscap
*/


#define KTIREUT_ENG_LSCAP_KTI_REUT_REG 0x09120110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 currpllratio : 7;

                            /* Bits[6:0], Access Type=RO, default=0x00000000*/

                            /* Current PLL Ratio This is current PLL ratio. */
    UINT32 rsvd : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pllratiomask : 6;

                            /* Bits[13:8], Access Type=RO, default=0x00000000*/

                            /*
                               This indicates the range of options for
                               programming the I/O PLL ratio. Examples: 001100
                               - Indicates that only ratio multiples of four
                               and eight can be programmed. 010001 - Indicates
                               that only ratio multiples of sixteen and one can
                               be programmed.
                            */
    UINT32 rsvd_14 : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 minclkratio : 6;

                            /* Bits[21:16], Access Type=RO, default=0x00000000*/

                            /*
                               Minimum Clock Ratio This is the minimum value
                               usable for the PLL ratio.
                            */
    UINT32 rsvd_22 : 2;

                            /* Bits[23:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 maxclkratio : 6;

                            /* Bits[29:24], Access Type=RO, default=0x00000000*/

                            /*
                               Maximum Clock Ratio This is the maximum value
                               usable for the PLL ratio.
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_ENG_LSCAP_KTI_REUT_STRUCT;

/* KTIREUT_ENG_LSCTR_KTI_REUT_REG supported on:                                 */
/*      SPRA0 (0x21e0a114)                                                      */
/*      SPRB0 (0x21e0a114)                                                      */
/*      SPRHBM (0x21e0a114)                                                     */
/*      SPRC0 (0x21e0a114)                                                      */
/*      SPRMCC (0x21e0a114)                                                     */
/*      SPRUCC (0x21e0a114)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_eng_lsctr
*/


#define KTIREUT_ENG_LSCTR_KTI_REUT_REG 0x09120114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nextpllratio : 7;

                            /* Bits[6:0], Access Type=RO, default=0x00000000*/

                            /*
                               Next PLL Ratio after link reset Allows for
                               controlling the speed of the Link by setting the
                               next PLL ratio to be used after an Inband Reset.
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_ENG_LSCTR_KTI_REUT_STRUCT;

/* KTIREUT_ENG_LTRCAP_KTI_REUT_REG supported on:                                */
/*      SPRA0 (0x21e0a118)                                                      */
/*      SPRB0 (0x21e0a118)                                                      */
/*      SPRHBM (0x21e0a118)                                                     */
/*      SPRC0 (0x21e0a118)                                                      */
/*      SPRMCC (0x21e0a118)                                                     */
/*      SPRUCC (0x21e0a118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_eng_ltrcap
*/


#define KTIREUT_ENG_LTRCAP_KTI_REUT_REG 0x09120118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 linkcapability : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Link Capability - Link Capability represents
                               each of the Links indicated by # of Links
                               Supported by using a one hot it for each of the
                               Links. Intel UPI link Behavior Every even bit
                               represents a Link. [0]: Link 0 [1]: Reserved
                               [2]: Link 1 [3]: Reserved [4]: Link 2 Etc.. Odd
                               bits are reserved.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_ENG_LTRCAP_KTI_REUT_STRUCT;

/* KTIREUT_ENG_LTRCON_KTI_REUT_REG supported on:                                */
/*      SPRA0 (0x21e0a11c)                                                      */
/*      SPRB0 (0x21e0a11c)                                                      */
/*      SPRHBM (0x21e0a11c)                                                     */
/*      SPRC0 (0x21e0a11c)                                                      */
/*      SPRMCC (0x21e0a11c)                                                     */
/*      SPRUCC (0x21e0a11c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_eng_ltrcon
*/


#define KTIREUT_ENG_LTRCON_KTI_REUT_REG 0x0912011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 linkstocontrol : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*
                               Link Control Dependency on Link Control is
                               called out explicitly throughout various REUT
                               registers definition when it occurs. Every even
                               bit in Link Control represents a Link. Bit 0 =
                               Link 0 Bit 2 = Link 1 Bit 4 = Link 2 Etc. The
                               following Rules apply to the Even bits of Link
                               Control. If Can Transmit or Receive on Multiple
                               Links? = 0 then the even Bits of Link Control
                               are not writable and must always be equal to
                               Link Capability. Link Select must be the sole
                               method to determine which Link is being accessed
                               and controlled. If Can Transmit or Receive on
                               Multiple Links? = 1 then the REUT engine can
                               transmit and/or receive on multiple links based
                               on the following rules. If Can Select a Partial
                               Set Of Link Control is set to 0 then the even
                               Bits of Link Control are not writable and must
                               always be equal to Link Capability Else, If Can
                               Select a Partial Set Of Link Control is set to 1
                               then the even bits of Link Control can be set to
                               0 or 1 for any combination of Links which are
                               set to ?1? in the Link Capability. Writing to
                               register dependent on Link Control will cause
                               only the selected Links to be written to while
                               leaving non selected Link register values
                               unchanged. The following Rules apply to the odd
                               bits of Link Control. UPI link Behavior: Odd
                               bits are reserved.
                            */
    UINT32 rsvd : 7;

                            /* Bits[22:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 linkautoconfiguration : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               Link Auto Configuration: If Link Auto
                               Configuration Capability is set to 0 then Link
                               Auto Configuration is always reserved as 0.
                               Else, If Link Auto Configuration is set to 0
                               then all odd Link Control bits that are set to 1
                               will condition how the Pre Configuration Lane ID
                               values are set. If Link Auto Configuration is
                               set to 1 then all odd Link Control bits are
                               ignored and are assumed to be 0. See Link
                               Control for more details.
                            */
    UINT32 reutdisable : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /*
                               REUT disable 0: REUT enable 1: REUT disable By
                               writing this bit the whole REUT PCI Function
                               will disappear from the perspective of software
                               (i.e. PCI CSR space). The REUT engine will only
                               be made visible again upon the next Cold Reset
                               or some other implementation specific means.
                               This is intended to act as a security mechanism
                               to prevent any unwanted usage of the REUT
                               registers. Note: Support for this feature is
                               optional. ( It was RW-O).
                            */
    UINT32 linkselect : 4;

                            /* Bits[28:25], Access Type=RO, default=0x00000000*/

                            /*
                               Link Select -This bit is used in conjunction
                               with many other REUT register bits which are
                               explicitly called out within the description
                               field of the particular bit where it has a
                               potential affect. Note: We have only one Intel
                               UPI link so this bit field cannot be written.
                            */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_ENG_LTRCON_KTI_REUT_STRUCT;

/* KTIREUT_PH_CPR_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a128)                                                      */
/*      SPRB0 (0x21e0a128)                                                      */
/*      SPRHBM (0x21e0a128)                                                     */
/*      SPRC0 (0x21e0a128)                                                      */
/*      SPRMCC (0x21e0a128)                                                     */
/*      SPRUCC (0x21e0a128)                                                     */
/* Register default value on SPRA0: 0x00020070                                  */
/* Register default value on SPRB0: 0x00020070                                  */
/* Register default value on SPRHBM: 0x00020070                                 */
/* Register default value on SPRC0: 0x00020070                                  */
/* Register default value on SPRMCC: 0x00020070                                 */
/* Register default value on SPRUCC: 0x00020070                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_cpr
*/


#define KTIREUT_PH_CPR_KTI_REUT_REG 0x09120128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 kti_version : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /*
                               Intel UPI Version 0h: Intel UPI v1.0 all other
                               encoding are reserved.
                            */
    UINT32 ccl0p : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /* L0p Support 1:Supports L0p */
    UINT32 ccl1 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /* L1 Support 1: supports L1 */
    UINT32 ccdeterminism : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000001*/

                            /*
                               Determinism Support 1: supports latency fixing
                               and alignment of exit from init on a clean flit
                               boundary and aligned to external signal
                            */
    UINT32 cctxadaptsettings : 8;

                            /* Bits[14:7], Access Type=RO, default=0x00000000*/

                            /*
                               Number of Tx adapt settings that can be tried is
                               one iteration. Note: No plan to support Tx Adapt
                               in processor. Errata: SPEC to RTL Mismatch. -
                               Unused register
                            */
    UINT32 cctdeterminism : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* Supports tester determinism */
    UINT32 ccfailover : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               0: not capable of failover on fault. 1:
                               capabable of failover on fault
                            */
    UINT32 ccbdeterminism : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000001*/

                            /*
                               : Supports basic determinism i.e. latency
                               fixing; enabled by CLatFixEn
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_CPR_KTI_REUT_STRUCT;

/* KTIREUT_PH_CTR1_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a12c)                                                      */
/*      SPRB0 (0x21e0a12c)                                                      */
/*      SPRHBM (0x21e0a12c)                                                     */
/*      SPRC0 (0x21e0a12c)                                                      */
/*      SPRMCC (0x21e0a12c)                                                     */
/*      SPRUCC (0x21e0a12c)                                                     */
/* Register default value on SPRA0: 0x28000020                                  */
/* Register default value on SPRB0: 0x28000020                                  */
/* Register default value on SPRHBM: 0x28000020                                 */
/* Register default value on SPRC0: 0x28000020                                  */
/* Register default value on SPRMCC: 0x28000020                                 */
/* Register default value on SPRUCC: 0x28000020                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* ktireut_ph_ctr1
*/


#define KTIREUT_PH_CTR1_KTI_REUT_REG 0x0912012C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cp_reset : 1;

                            /* Bits[0:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               1: initiates reset (resulting hw actions restore
                               all status RO-V bits to default values); cleared
                               by hardware on entering ResetC
                            */
    UINT32 c_single_step : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               1: enable Single step; forward progress on
                               clearing of SPSSHold. Setting this bit also sets
                               SPSSHold
                            */
    UINT32 c_fia : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               1: enable Freeze on init abort; backward
                               progress on clearing of SPSSHold
                            */
    UINT32 c_ate : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               Not supported on processor. 1: enable ATE mode;
                               forward progress on time-out
                            */
    UINT32 c_op_speed : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               1: operational speed 0: slow speed Note: The
                               operational speed is determined by BIOS setup of
                               the PLL using BIOS-to-PCode Mailbox
                            */
    UINT32 c_init_begin : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* 0: blocks exit from ResetC */
    UINT32 c_init : 2;

                            /* Bits[7:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Init to L0 1: CLocalLBM 2:
                               CLocalComplianceSlave 3: CLocalComplianceMaster
                            */
    UINT32 c_det_en : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* 0: no determinism 1: determinism enabled */
    UINT32 c_det_slave : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* 0: Determinism master 1: Determinism slave */
    UINT32 c_reinit_prbs : 2;

                            /* Bits[11:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: no reinit of PRBS generating LFSRs 1: reinit
                               after every EIEOS 2: reserved 3: reserved
                            */
    UINT32 c_la_load_disable : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               1: disable loading of effective values from late
                               action registers in reset
                            */
    UINT32 c_numinit : 8;

                            /* Bits[20:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: no limit on consecutive init iterations
                               Others: number of init iterations attempted
                               before coming to rest in ResetC.
                            */
    UINT32 c_enable_phy_reset : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               1: drift alarm initiates a PHY reset; late
                               action
                            */
    UINT32 c_l0p_en : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* 1: enables L0p */
    UINT32 c_l1_en : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* 1: enables L1 */
    UINT32 rsvd : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 c_enh_qual_en : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* 1: Enables enhanced lane qualification */
    UINT32 c_comp_byp_en : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               1: Ignore wake detection, adaptation etc prior
                               to bitlock. Used by compliance slave
                            */
    UINT32 c_failover_en : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000001*/

                            /* 1: Enable failover */
    UINT32 c_enable_phy_reset_w : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               1: Loss of acquired wake detect during init
                               initiates phy reset
                            */
    UINT32 c_rc_terms_off : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000001*/

                            /* 1: Rx pulldowns are off in ResetC */
    UINT32 c_rx_reset_dis : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /* 1: Disables RxReset for DFx purposes */
    UINT32 c_lat_fix_en : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /* : Enables latency fixing */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_CTR1_KTI_REUT_STRUCT;

/* KTIREUT_PH_CTR2_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a130)                                                      */
/*      SPRB0 (0x21e0a130)                                                      */
/*      SPRHBM (0x21e0a130)                                                     */
/*      SPRC0 (0x21e0a130)                                                      */
/*      SPRMCC (0x21e0a130)                                                     */
/*      SPRUCC (0x21e0a130)                                                     */
/* Register default value on SPRA0: 0x00001000                                  */
/* Register default value on SPRB0: 0x00001000                                  */
/* Register default value on SPRHBM: 0x00001000                                 */
/* Register default value on SPRC0: 0x00001000                                  */
/* Register default value on SPRMCC: 0x00001000                                 */
/* Register default value on SPRUCC: 0x00001000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* ktireut_ph_ctr2
*/


#define KTIREUT_PH_CTR2_KTI_REUT_REG 0x09120130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 c_tx_adapt_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enables Tx Adapt Flow Note: unsupported in
                               processor
                            */
    UINT32 c_tx_adapt_settings : 8;

                            /* Bits[8:1], Access Type=RO, default=0x00000000*/

                            /*
                               Number of TX adapt settings that are tried in
                               one iteration Note: unsupported in processor
                               Errata ; SPEC to RTL Mismatch. - Unused register
                            */
    UINT32 c_tx_adapt_pat : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               1: use loopback master pattern based on pat gen
                               mux 0: use L0 PRBS Note: unsupported in
                               processor Errata: SPEC to RTL Mismatch. - Unused
                               register
                            */
    UINT32 c_sb_back_channel : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               0: use slow mode backchannel 1: use sideband
                               backchannel Note: Unsupported in processor.
                               Errata: SPEC to RTL Mismatch. - Here is encoding
                               as per UPI Spec 0: Use slow mode backchannel 1:
                               Use sideband backchannel - SPR Logphy supports
                               only one mode i.e. slow mode back channel -
                               therefore though it is mismatch w.r.t. spec but
                               no issue
                            */
    UINT32 rsvd : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cecnrdp : 1;

                            /* Bits[12:12], Access Type=RW/V/P, default=0x00000001*/

                            /*
                               1'b1 : extender or re-driver on channel with no
                               Rx Detect Propagation is supported
                            */
    UINT32 c_pipe_clk_gate_override : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Pipe control clock gate enable override. Errata:
                               SPEC to RTL Mismatch. Reserved bit as per spec
                               and RTL used them as clock gate override.
                            */
    UINT32 c_msgbus_clk_gate_override : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Message bus clock gate enable override. Errata:
                               SPEC to RTL Mismatch. Reserved bit as per spec
                               and RTL used them as clock gate override.
                            */
    UINT32 c_slow_mode_clk_gate : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               upiphr slowmode and nearend loopback clock gate
                               enable override. Errata: SPEC to RTL Mismatch.
                               Reserved bit as per spec and RTL used them as
                               clock gate override.
                            */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_CTR2_KTI_REUT_STRUCT;

/* KTIREUT_PH_LCS_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a134)                                                      */
/*      SPRB0 (0x21e0a134)                                                      */
/*      SPRHBM (0x21e0a134)                                                     */
/*      SPRC0 (0x21e0a134)                                                      */
/*      SPRMCC (0x21e0a134)                                                     */
/*      SPRUCC (0x21e0a134)                                                     */
/* Register default value on SPRA0: 0x00001210                                  */
/* Register default value on SPRB0: 0x00001210                                  */
/* Register default value on SPRHBM: 0x00001210                                 */
/* Register default value on SPRC0: 0x00001210                                  */
/* Register default value on SPRMCC: 0x00001210                                 */
/* Register default value on SPRUCC: 0x00001210                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* ktireut_ph_lcs
*/


#define KTIREUT_PH_LCS_KTI_REUT_REG 0x09120134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_enable_phy_reset : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Updated from CEnablePhyReset on exit from ResetC */
    UINT32 rsvd : 2;

                            /* Bits[2:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 s_include_dropped_lanes : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Updated from CIncludeDroppedLanes on exit from
                               ResetC
                            */
    UINT32 s_tl0c : 8;

                            /* Bits[11:4], Access Type=RO/V/P, default=0x00000021*/

                            /*
                               L0c interval. Updated from PHTL0C_SL.t_l0c or
                               PHTL0C_OP.t_l0c depending on slow mode
                            */
    UINT32 s_tl0cq : 4;

                            /* Bits[15:12], Access Type=RO/V/P, default=0x00000001*/

                            /* L0c duration. Always 1 */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_LCS_KTI_REUT_STRUCT;

/* KTIREUT_PH_TDC_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a138)                                                      */
/*      SPRB0 (0x21e0a138)                                                      */
/*      SPRHBM (0x21e0a138)                                                     */
/*      SPRC0 (0x21e0a138)                                                      */
/*      SPRMCC (0x21e0a138)                                                     */
/*      SPRUCC (0x21e0a138)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_tdc
*/


#define KTIREUT_PH_TDC_KTI_REUT_REG 0x09120138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdatalanedisable : 24;

                            /* Bits[23:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Tx Data Lane Disable A bit mask used for
                               selectively enabling/disabling data TX Lanes.
                               Used for debug and validation purposes. A bit
                               value of 1 indicates the corresponding lane is
                               disabled. Bit 0: Controls Lane 0. Bit 1:
                               Controls Lane 1. .. and so on.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TDC_KTI_REUT_STRUCT;

/* KTIREUT_PH_TDS_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a13c)                                                      */
/*      SPRB0 (0x21e0a13c)                                                      */
/*      SPRHBM (0x21e0a13c)                                                     */
/*      SPRC0 (0x21e0a13c)                                                      */
/*      SPRMCC (0x21e0a13c)                                                     */
/*      SPRUCC (0x21e0a13c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_tds
*/


#define KTIREUT_PH_TDS_KTI_REUT_REG 0x0912013C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txlanedetectstat : 24;

                            /* Bits[23:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Tx Data Lane Detection Status A bit value of 1
                               indicates the TX lane is dropped. Bit 0: Status
                               of lane 0. Bit 1: Status of lane 1. .. and so
                               on..
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TDS_KTI_REUT_STRUCT;

/* KTIREUT_PH_RDS_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a140)                                                      */
/*      SPRB0 (0x21e0a140)                                                      */
/*      SPRHBM (0x21e0a140)                                                     */
/*      SPRC0 (0x21e0a140)                                                      */
/*      SPRMCC (0x21e0a140)                                                     */
/*      SPRUCC (0x21e0a140)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_rds
*/


#define KTIREUT_PH_RDS_KTI_REUT_REG 0x09120140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdatareadystat : 24;

                            /* Bits[23:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Rx Data Lane Ready Status A bit value of 1
                               indicates Rx lane is dropped. Bit 0: Status of
                               Lane 0 Bit 1: Status of Lane 1. .. and so on.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_RDS_KTI_REUT_STRUCT;

/* KTIREUT_PH_RDC_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a144)                                                      */
/*      SPRB0 (0x21e0a144)                                                      */
/*      SPRHBM (0x21e0a144)                                                     */
/*      SPRC0 (0x21e0a144)                                                      */
/*      SPRMCC (0x21e0a144)                                                     */
/*      SPRUCC (0x21e0a144)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_rdc
*/


#define KTIREUT_PH_RDC_KTI_REUT_REG 0x09120144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxdatalanedisable : 24;

                            /* Bits[23:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Rx Data Lane Disable A bit mask used for
                               selectively enabling/disabling data RX Lanes.
                               Used for debug and validation purposes. A bit
                               value of 1 indicates the corresponding RX lane
                               is disabled. Bit 0: Controls RX on Lane 0. Bit
                               1: Controls RX on Lane 1. .. and so on.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_RDC_KTI_REUT_STRUCT;

/* KTIREUT_PH_CIS_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a148)                                                      */
/*      SPRB0 (0x21e0a148)                                                      */
/*      SPRHBM (0x21e0a148)                                                     */
/*      SPRC0 (0x21e0a148)                                                      */
/*      SPRMCC (0x21e0a148)                                                     */
/*      SPRUCC (0x21e0a148)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_cis
*/


#define KTIREUT_PH_CIS_KTI_REUT_REG 0x09120148

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_op_speed : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               0: slow speed 1: op speed Updated at entry and
                               exit from ResetC
                            */
    UINT32 sp_rx_calib : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               1: Rx calib done; sw clears by writing 1 prior
                               to speed change to force re-calib.
                            */
    UINT32 sp_detect : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* 1: remote Rx detected by Tx */
    UINT32 sp_tx_calib : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               1: Tx calib done; sw clears by writing 1 prior
                               to speed change to force re-calib.
                            */
    UINT32 s_wake_detected : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* 1: Wake detected on one or more lanes. */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sp_use_back_channel : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               1: backchannel in use after Tx Adapt done; sw
                               clears by writing 1 after backchannel operations
                               completed Note: unsupported in processor
                            */
    UINT32 sp_tx_adapt : 8;

                            /* Bits[14:7], Access Type=RO, default=0x00000000*/

                            /*
                               Tx Adapt Index Note: unsupported in processor
                               Errata: SPEC to RTL Mismatch. - Unused register
                            */
    UINT32 s_link_up : 2;

                            /* Bits[16:15], Access Type=RO/V, default=0x00000000*/

                            /*
                               00: STxLinkUp FALSE, SRxLinkUp FALSE 01:
                               STxLinkUp FALSE, SRxLinkUp TRUE 10: STxLinkUp
                               TRUE, SRxLinkUp FALSE 11: STxLinkUp TRUE,
                               SRxLinkUp TRUE
                            */
    UINT32 s_init_count : 8;

                            /* Bits[24:17], Access Type=RO/V, default=0x00000000*/

                            /* Init iterations count (no rollover) */
    UINT32 rsvd_25 : 2;

                            /* Bits[26:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 s_remote_lbm : 1;

                            /* Bits[27:27], Access Type=RO/V, default=0x00000000*/

                            /*
                               Set if received TS destination state is loopback
                               master.
                            */
    UINT32 s_rx_lane_rev : 1;

                            /* Bits[28:28], Access Type=RO/V, default=0x00000000*/

                            /* 1: lane reversal at Rx */
    UINT32 s_pss_hold : 1;

                            /* Bits[29:29], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               State Machine Hold 1 - Physical layer state
                               machine is holding on single step or freezing on
                               initialization abort Clearing this bit will
                               cause state machine to advance. sw must also
                               clear this bit when CSingleStep or CFIA is
                               cleared.
                            */
    UINT32 s_init_fail : 1;

                            /* Bits[30:30], Access Type=RO/V, default=0x00000000*/

                            /*
                               0: init progress 1: secondary timeout and init
                               fail
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_CIS_KTI_REUT_STRUCT;

/* KTIREUT_PH_PIS_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a14c)                                                      */
/*      SPRB0 (0x21e0a14c)                                                      */
/*      SPRHBM (0x21e0a14c)                                                     */
/*      SPRC0 (0x21e0a14c)                                                      */
/*      SPRMCC (0x21e0a14c)                                                     */
/*      SPRUCC (0x21e0a14c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_pis
*/


#define KTIREUT_PH_PIS_KTI_REUT_REG 0x0912014C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_op_speed_p : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CIS
                               register, but this is copy of prior to last init
                               abort.
                            */
    UINT32 sp_rx_calib_p : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CIS
                               register, but this is copy of prior to last init
                               abort.
                            */
    UINT32 sp_detect_p : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CIS
                               register, but this is copy of prior to last init
                               abort.
                            */
    UINT32 sp_tx_calib_p : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CIS
                               register, but this is copy of prior to last init
                               abort.
                            */
    UINT32 s_wake_detected_p : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CIS
                               register, but this is copy of prior to last init
                               abort.
                            */
    UINT32 rsvd : 1;

                            /* Bits[5:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sp_use_back_channel_p : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CIS
                               register, but this is copy of prior to last init
                               abort.
                            */
    UINT32 sp_tx_adapt_p : 8;

                            /* Bits[14:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CIS
                               register, but this is copy of prior to last init
                               abort. Errata: SPEC to RTL Mismatch. - Unused
                               register
                            */
    UINT32 s_link_up_p : 2;

                            /* Bits[16:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CIS
                               register, but this is copy of prior to last init
                               abort.
                            */
    UINT32 s_init_count_p : 8;

                            /* Bits[24:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CIS
                               register, but this is copy of prior to last init
                               abort.
                            */
    UINT32 rsvd_25 : 2;

                            /* Bits[26:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 s_remote_lbm_p : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CIS
                               register, but this is copy of prior to last init
                               abort.
                            */
    UINT32 s_rx_lane_rev_p : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CIS
                               register, but this is copy of prior to last init
                               abort.
                            */
    UINT32 s_pss_hold_p : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CIS
                               register, but this is copy of prior to last init
                               abort.
                            */
    UINT32 s_init_fail_p : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CIS
                               register, but this is copy of prior to last init
                               abort.
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_PIS_KTI_REUT_STRUCT;

/* KTIREUT_PH_CSS_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a150)                                                      */
/*      SPRB0 (0x21e0a150)                                                      */
/*      SPRHBM (0x21e0a150)                                                     */
/*      SPRC0 (0x21e0a150)                                                      */
/*      SPRMCC (0x21e0a150)                                                     */
/*      SPRUCC (0x21e0a150)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_css
*/


#define KTIREUT_PH_CSS_KTI_REUT_REG 0x09120150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_tx_state : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Tx State (encoding defined in Intel UPI
                               specification)
                            */
    UINT32 s_rx_state : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               Rx State (encoding defined in Intel UPI
                               specification)
                            */
    UINT32 s_clm : 3;

                            /* Bits[18:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               lane map code determined at Rx and sent in
                               TX.Config for remote Tx; also updated on L0p
                               entry/exit. Encoded as follows: 0b111 - lanes
                               0..19 0b001 - lanes 0..7 0b100 - lanes 12..19
                            */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_CSS_KTI_REUT_STRUCT;

/* KTIREUT_PH_PSS_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a154)                                                      */
/*      SPRB0 (0x21e0a154)                                                      */
/*      SPRHBM (0x21e0a154)                                                     */
/*      SPRC0 (0x21e0a154)                                                      */
/*      SPRMCC (0x21e0a154)                                                     */
/*      SPRUCC (0x21e0a154)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_pss
*/


#define KTIREUT_PH_PSS_KTI_REUT_REG 0x09120154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_tx_state_p : 8;

                            /* Bits[7:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CSS
                               register, but this is copy of prior to last phy
                               reset
                            */
    UINT32 s_rx_state_p : 8;

                            /* Bits[15:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CSS
                               register, but this is copy of prior to last phy
                               reset
                            */
    UINT32 s_clm_p : 3;

                            /* Bits[18:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_CSS
                               register, but this is copy of prior to last phy
                               reset
                            */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_PSS_KTI_REUT_STRUCT;

/* KTIREUT_PH_YIS_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a158)                                                      */
/*      SPRB0 (0x21e0a158)                                                      */
/*      SPRHBM (0x21e0a158)                                                     */
/*      SPRC0 (0x21e0a158)                                                      */
/*      SPRMCC (0x21e0a158)                                                     */
/*      SPRUCC (0x21e0a158)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_yis
*/


#define KTIREUT_PH_YIS_KTI_REUT_REG 0x09120158

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx_pol_inv : 24;

                            /* Bits[23:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               A bit value of 1 indicates Rx lane has detected
                               polarity inversion. Bit 0: Status of Physical
                               Lane 0 Bit 1: Status of Physical Lane 1. .. and
                               so on.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_YIS_KTI_REUT_STRUCT;

/* KTIREUT_PH_DIS_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a15c)                                                      */
/*      SPRB0 (0x21e0a15c)                                                      */
/*      SPRHBM (0x21e0a15c)                                                     */
/*      SPRC0 (0x21e0a15c)                                                      */
/*      SPRMCC (0x21e0a15c)                                                     */
/*      SPRUCC (0x21e0a15c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_dis
*/


#define KTIREUT_PH_DIS_KTI_REUT_REG 0x0912015C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_lat_added : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               8:7 - Reserved 6:0 - Latency added after deskew
                               div 8 UI, i.e., 8 UI granularity
                            */
    UINT32 s_rx_obs_latency : 12;

                            /* Bits[19:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               Rx sync counter value when EIEOS of TS.Polling
                               received at latency buffer.
                            */
    UINT32 rsvd : 4;

                            /* Bits[23:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 s_lat_buff_error : 1;

                            /* Bits[24:24], Access Type=RO/V, default=0x00000000*/

                            /*
                               1 - latency not fixable i.e. fix value larger
                               than latency buffer depth; init continues
                               without latency fixing.
                            */
    UINT32 rsvd_25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_DIS_KTI_REUT_STRUCT;

/* KTIREUT_PH_CLS_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a160)                                                      */
/*      SPRB0 (0x21e0a160)                                                      */
/*      SPRHBM (0x21e0a160)                                                     */
/*      SPRC0 (0x21e0a160)                                                      */
/*      SPRMCC (0x21e0a160)                                                     */
/*      SPRUCC (0x21e0a160)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_cls
*/


#define KTIREUT_PH_CLS_KTI_REUT_REG 0x09120160

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_l1r_err : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* L1r received at wrong time */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sp_drift_alarm : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* 1: Indicates drift buffer alarm */
    UINT32 s_drift_alarm_lane_address : 5;

                            /* Bits[7:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               The lane ID of first lane that caused Drift
                               Buffer Alarm to set. Valid only when alarm is
                               set.
                            */
    UINT32 s_l0pe_fail : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* 1: L0p exit failed at Rx */
    UINT32 s_l0pe_lat_sub : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /*
                               1: Latency subtracted on at least one idle lane
                               during L0p exit
                            */
    UINT32 s_l0pe_lat_add : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /*
                               1: Latency added on at least one idle lane
                               during L0p exit
                            */
    UINT32 s_l1r_out : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /* Set when QL1r sent; cleared when QL1n received */
    UINT32 s_l1n_pend : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /* Set when QL1r received; cleared when QL1n sent */
    UINT32 s_rst_rcvd : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /* 1: QRst code received */
    UINT32 s_mm_l0c : 8;

                            /* Bits[21:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               Mismatched pattern by lane when L0c mismatch
                               causes a reset. will be 1 for bad lane, 0 for
                               good lane
                            */
    UINT32 s_sent_l0c : 4;

                            /* Bits[25:22], Access Type=RO/V, default=0x00000000*/

                            /* Sent L0c code */
    UINT32 s_rcvd_l0c : 4;

                            /* Bits[29:26], Access Type=RO/V, default=0x00000000*/

                            /* Received L0c code */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_CLS_KTI_REUT_STRUCT;

/* KTIREUT_PH_PLS_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a164)                                                      */
/*      SPRB0 (0x21e0a164)                                                      */
/*      SPRHBM (0x21e0a164)                                                     */
/*      SPRC0 (0x21e0a164)                                                      */
/*      SPRMCC (0x21e0a164)                                                     */
/*      SPRUCC (0x21e0a164)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_pls
*/


#define KTIREUT_PH_PLS_KTI_REUT_REG 0x09120164

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_l1r_err_p : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_PLS
                               register, but this is copy of prior to last phy
                               reset.
                            */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sp_drift_alarm_p : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               : Indicates drift buffer underflow or overflow
                               alarm.
                            */
    UINT32 s_drift_alarm_lane_address_p : 5;

                            /* Bits[7:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_PLS
                               register, but this is copy of prior to last phy
                               reset.
                            */
    UINT32 s_l0pe_fail_p : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_PLS
                               register, but this is copy of prior to last phy
                               reset.
                            */
    UINT32 s_l0pe_lat_sub_p : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_PLS
                               register, but this is copy of prior to last phy
                               reset.
                            */
    UINT32 s_l0pe_lat_add_p : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_PLS
                               register, but this is copy of prior to last phy
                               reset.
                            */
    UINT32 s_l1r_out_p : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_PLS
                               register, but this is copy of prior to last phy
                               reset.
                            */
    UINT32 s_l1n_pend_p : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_PLS
                               register, but this is copy of prior to last phy
                               reset.
                            */
    UINT32 s_rst_rcvd_p : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_PLS
                               register, but this is copy of prior to last phy
                               reset.
                            */
    UINT32 s_mm_l0c_p : 8;

                            /* Bits[21:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_PLS
                               register, but this is copy of prior to last phy
                               reset.
                            */
    UINT32 s_sent_l0c_p : 4;

                            /* Bits[25:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_PLS
                               register, but this is copy of prior to last phy
                               reset.
                            */
    UINT32 s_rcvd_l0c_p : 4;

                            /* Bits[29:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Same description as bit field in KTIREUT_PH_PLS
                               register, but this is copy of prior to last phy
                               reset
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_PLS_KTI_REUT_STRUCT;

/* KTIREUT_PH_WES_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a168)                                                      */
/*      SPRB0 (0x21e0a168)                                                      */
/*      SPRHBM (0x21e0a168)                                                     */
/*      SPRC0 (0x21e0a168)                                                      */
/*      SPRMCC (0x21e0a168)                                                     */
/*      SPRUCC (0x21e0a168)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_wes
*/


#define KTIREUT_PH_WES_KTI_REUT_REG 0x09120168

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_rx_lane_wake_err : 24;

                            /* Bits[23:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               A bit value of 1 indicates Rx lane wake detect
                               failed. Bit 0: Status of Physical Lane 0 Bit 1:
                               Status of Physical Lane 1 ... and so on
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_WES_KTI_REUT_STRUCT;

/* KTIREUT_PH_KES_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a16c)                                                      */
/*      SPRB0 (0x21e0a16c)                                                      */
/*      SPRHBM (0x21e0a16c)                                                     */
/*      SPRC0 (0x21e0a16c)                                                      */
/*      SPRMCC (0x21e0a16c)                                                     */
/*      SPRUCC (0x21e0a16c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_kes
*/


#define KTIREUT_PH_KES_KTI_REUT_REG 0x0912016C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_rx_lane_deskew_err : 24;

                            /* Bits[23:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               A bit value of 1 indicates Rx lane deskew
                               failed. Bit 0: Status of Physical Lane 0 Bit 1:
                               Status of Physical Lane 1 ... and so on
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_KES_KTI_REUT_STRUCT;

/* KTIREUT_PH_DAS_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a170)                                                      */
/*      SPRB0 (0x21e0a170)                                                      */
/*      SPRHBM (0x21e0a170)                                                     */
/*      SPRC0 (0x21e0a170)                                                      */
/*      SPRMCC (0x21e0a170)                                                     */
/*      SPRUCC (0x21e0a170)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_das
*/


#define KTIREUT_PH_DAS_KTI_REUT_REG 0x09120170

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_rx_lane_drift_alarm : 24;

                            /* Bits[23:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               A bit value of 1 indicates Rx lane drift alarm.
                               Bit 0: Status of Physical Lane 0 Bit 1: Status
                               of Physical Lane 1 ... and so on
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_DAS_KTI_REUT_STRUCT;

/* KTIREUT_PH_TES_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a174)                                                      */
/*      SPRB0 (0x21e0a174)                                                      */
/*      SPRHBM (0x21e0a174)                                                     */
/*      SPRC0 (0x21e0a174)                                                      */
/*      SPRMCC (0x21e0a174)                                                     */
/*      SPRUCC (0x21e0a174)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_tes
*/


#define KTIREUT_PH_TES_KTI_REUT_REG 0x09120174

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_opt_index : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Optimal Tx Adapt index for lane addressed in
                               PH_TEC
                            */
    UINT32 s_al_metric : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /* Rx metric for addressed lane */
    UINT32 s_al_metric_max : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /*
                               Rx metric max for addressed lane (for
                               normalization); 0 means metric not implemented.
                            */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TES_KTI_REUT_STRUCT;

/* KTIREUT_PH_LDC_KTI_REUT_REG supported on:                                    */
/*      SPRA0 (0x21e0a178)                                                      */
/*      SPRB0 (0x21e0a178)                                                      */
/*      SPRHBM (0x21e0a178)                                                     */
/*      SPRC0 (0x21e0a178)                                                      */
/*      SPRMCC (0x21e0a178)                                                     */
/*      SPRUCC (0x21e0a178)                                                     */
/* Register default value on SPRA0: 0x00000804                                  */
/* Register default value on SPRB0: 0x00000804                                  */
/* Register default value on SPRHBM: 0x00000804                                 */
/* Register default value on SPRC0: 0x00000804                                  */
/* Register default value on SPRMCC: 0x00000804                                 */
/* Register default value on SPRUCC: 0x00000804                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_ldc
*/


#define KTIREUT_PH_LDC_KTI_REUT_REG 0x09120178

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 driftbufalarmthr : 6;

                            /* Bits[5:0], Access Type=RW/P/L, default=0x00000004*/

                            /*
                               Drift Buffer Alarm Threshold When the difference
                               (both underflow and overflow) between drift
                               buffer read and write pointers (depth) is less
                               than the value in this field, drift buffer alarm
                               status will be set.
                            */
    UINT32 rsvd : 2;

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 inidriftbufflatency : 6;

                            /* Bits[13:8], Access Type=RW/P/L, default=0x00000008*/

                            /*
                               Initial Drift Buffer Latency Drift Buffer Depth
                               refers to the initialized difference between
                               read and writer pointers in RX drift buffer. The
                               field indicates the difference between the drift
                               buffer read and write pointers in UI, to be set
                               during Polling.BitLock, in all lanes.
                            */
    UINT32 targetlinklatency : 9;

                            /* Bits[22:14], Access Type=RW/P/L, default=0x00000000*/

                            /* Target latency at Rx in 8 UI granularity. */
    UINT32 rsvd_23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_LDC_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTCAP_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a188)                                                      */
/*      SPRB0 (0x21e0a188)                                                      */
/*      SPRHBM (0x21e0a188)                                                     */
/*      SPRC0 (0x21e0a188)                                                      */
/*      SPRMCC (0x21e0a188)                                                     */
/*      SPRUCC (0x21e0a188)                                                     */
/* Register default value on SPRA0: 0x04A40000                                  */
/* Register default value on SPRB0: 0x04A40000                                  */
/* Register default value on SPRHBM: 0x04A40000                                 */
/* Register default value on SPRC0: 0x04A40000                                  */
/* Register default value on SPRMCC: 0x04A40000                                 */
/* Register default value on SPRUCC: 0x04A40000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_ttcap
*/


#define KTIREUT_PH_TTCAP_KTI_REUT_REG 0x09120188

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 test_exe_control_cap : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               If 0, this field indicates that the
                               KTIREUT_PH_TTPGC register is used for the Start
                               Test and Stop Test control bits. If 1, this
                               field indicates that the KTIREUT_PH_TTTEC
                               register is used for the Start Test and Stop
                               Test control bits.
                            */
    UINT32 test_exe_delay_cap : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               If set, this field indicates that the Test
                               Execution Delay register is available to set the
                               delay between setting Start Test and the actual
                               start of the test. This bit MUST be a 1 when the
                               Start Coordination Capability is 1. 0 - Test
                               Execution Delay register (KTIREUT_PH_TTTED) is
                               not implemented 1 - Test Execution Delay
                               register is implemente
                            */
    UINT32 test_exe_status : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               If set, this field indicates that the Test
                               Execution Status register is implemented. 0 -
                               Test Execution Status register
                               (KTIREUT_PH_TTTES) is not implemented 1 - Test
                               Execution Status register is implemented
                            */
    UINT32 selerrmasktype : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               1 - Symbol/Bit and Masking Modes Single bit
                               checks are specified in terms of a pattern
                               buffer symbol number and the bit position within
                               that symbol. For interval testing, the mask
                               logic provides a bit mask that is always aligned
                               to the receive interpolator for isolation. 0 -
                               Start_bit/Interval mode Single bit checks are
                               specified in terms of the pattern buffer bit to
                               check. Interval checking specifies the starting
                               pattern buffer bit and the interval between bit
                               checks.
                            */
    UINT32 rsvd : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 slave_ib_para_extract : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               If set, slave is capable of extracting
                               electrical parameter from master's TS.Loopback
                               and applying during the test.
                            */
    UINT32 startcoordcap : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Start Coordination Capability If set, this field
                               indicates that the Start Test field has the
                               ability to be further qualified by an
                               implementation specific control signal from a
                               component wide Global Event.
                            */
    UINT32 txerrorinjcap : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Only valid if Selective Error Mask Type is 1. If
                               set, this field indicates that the transmit side
                               of the Loopback Master can inject bit errors.
                            */
    UINT32 rsvd_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 flatpatbufcap : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               If set, this field indicates that the Pattern
                               Buffer Access registers are not indexed, but are
                               flattened.
                            */
    UINT32 rsvd_10 : 6;

                            /* Bits[15:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 loop_cnt_inhibit_cap : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               If set, this field indicates that the Loop
                               Counter Inhibit is implemented and the user can
                               stall the Loop Counter. 0 - Loop Counter Inhibit
                               is not implemented 1 - Loop Counter Inhibit is
                               implemented
                            */
    UINT32 rsvd_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lfsrstaggercap : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000001*/

                            /*
                               If set, this field indicates that the LFSR1
                               Stagger Enable and LFSR2 Stagger Enable fields
                               are implemented and the user can choose between
                               staggereing of each LFSR or not. 1 - LRSR1
                               Stagger Enable and LFSR2 Stagger Enable fields
                               exist 0 - LFSR1 Stagger Enable and LFSR2 Stagger
                               Enable fields are missing
                            */
    UINT32 dcpolarityselcap : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               If set, this field indicates that the Drive DC
                               Zero or DC One field is implemented and the user
                               can choose between driving a 0 or a 1 on those
                               lanes set to drive a DC value in. 1 - Drive DC
                               Zero or DC One field exists 0 - Drive DC Zero or
                               DC One field is missing To force 0 on lanes
                               during PATTERN use PQ_CS_IBTXLER.tx_lane_en
                               [23:0] To force 1 on lanes during PATTERN use
                               PH_TTDDC.txdatlanctr[23:0]
                            */
    UINT32 rsvd_20 : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 err_cnt_inhibit : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000001*/

                            /*
                               If set, design is capable of inhibing error
                               counters.
                            */
    UINT32 polyselectcap : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               If set, this field indicates that the LFSR
                               Polynomial Select field is implemented and the
                               user can choose between the two LFSR
                               polynomials. 1 - LFSR Polynomial can be selected
                               between the ITU polynomial and the historical
                               Intel UPI polynomial (reciprocal of ITU
                               polynomial). 0 - Only the spec polynomial is
                               available
                            */
    UINT32 patbufselrotcap : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000001*/

                            /*
                               If set, user can enable rotation of the
                               KTIREUT_PH_TTPG2S register selects during
                               testing. 1 - Buffer Select can rotate 0 - Buffer
                               Select is static
                            */
    UINT32 rsvd_24 : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ext_err_cnt_lane_reassign_cap : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000001*/

                            /*
                               This field indicates that the Extended Error
                               Counter Lane Assign field is implemented and the
                               user can update the assigned lane for the
                               Extended Error Counters(s). 0 - Feature not
                               implemented 1 - Extended Error Counter can be
                               reassigned
                            */
    UINT32 extcntalllanescap : 1;

                            /* Bits[27:27], Access Type=RO, default=0x00000000*/

                            /*
                               If set, this field indicates that the Extended
                               Error Count All Lanes field is implemented and
                               the user can enable counting of all bit errors
                               for the complete set of lanes for the tested
                               lanes, starting from the lane selected in
                               Extended Error Counter Lane Select and including
                               any consecutive higher lanes included in the
                               testing. 1 - Extended Error Counter can
                               accumulate all errors 0 - Feature not
                               implemented
                            */
    UINT32 numextcntcap : 4;

                            /* Bits[31:28], Access Type=RO, default=0x00000000*/

                            /*
                               This field is used to enumerate the total number
                               of Extended Error Counters. Each will have a
                               Each will have a PH_TTPCC and PH_TTLECR. If the
                               quantity of Extended Error Counters is other
                               than 1 then that number is recorded in this
                               field as a N-1 value.
                            */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTCAP_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTPGC_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a18c)                                                      */
/*      SPRB0 (0x21e0a18c)                                                      */
/*      SPRHBM (0x21e0a18c)                                                     */
/*      SPRC0 (0x21e0a18c)                                                      */
/*      SPRMCC (0x21e0a18c)                                                     */
/*      SPRUCC (0x21e0a18c)                                                     */
/* Register default value on SPRA0: 0x06060000                                  */
/* Register default value on SPRB0: 0x06060000                                  */
/* Register default value on SPRHBM: 0x06060000                                 */
/* Register default value on SPRC0: 0x06060000                                  */
/* Register default value on SPRMCC: 0x06060000                                 */
/* Register default value on SPRUCC: 0x06060000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_ttpgc
*/


#define KTIREUT_PH_TTPGC_KTI_REUT_REG 0x0912018C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 strttst : 1;

                            /* Bits[0:0], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               Start test KTIRUET_PH_TTCAP(0) is 0, test starts
                               if this bit is set. 1 - Start the test -
                               Initiates transition from Loopback.Marker to
                               Loopback.Pattern. The bit is cleared when
                               Loopback.Pattern is exited.
                            */
    UINT32 stponerr : 1;

                            /* Bits[1:1], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Stop test on Error Exit Loopback.Pattern to
                               Loopback.Marker on error - Flag 0 - Do not stop
                               test on error 1 - Stop test and latch Loop
                               Status on first error in pattern checker.
                            */
    UINT32 autoinv : 1;

                            /* Bits[2:2], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Auto inversion rotate enable If set, this bit
                               will enable logical rotation of the
                               KTIREUT_PH_TTPIS register during the test. Each
                               lane begins by selecting the appropriate bit in
                               the Pattern Invert Select field, and
                               sequentially the next higher bit, wrapping back
                               to bit 0, following each loop. 1 - Logical
                               rotation of Buffer Invert Select 0 - No logical
                               rotation of Buffer INvert Select
                            */
    UINT32 psuedorandomen1 : 1;

                            /* Bits[3:3], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Pseudo-random Enable1 Enable Pseudo random
                               pattern generation in Pattern Generator Buffer 1
                               0 - Use Pattern Buffer 1 as 128 bit pattern. 1 -
                               Use Pattern Buffer 1 XORed with the LFSR.
                            */
    UINT32 psuedorandomen2 : 1;

                            /* Bits[4:4], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Pseudo-random Enable2 Enable Pseudo random
                               pattern generation in Pattern Generator Buffer 2
                               0 - Use Pattern Buffer 2 as 128 bit pattern. 1 -
                               Use Pattern Buffer 2 XORed with the LFSR.
                            */
    UINT32 slaveignorremoteparam : 1;

                            /* Bits[5:5], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable application of electrical parameters. If
                               set override electrical parameters will not be
                               applied applied in Loopback.Pattern.
                            */
    UINT32 start_coordination : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Valid only if Start Coordination Capability is
                               1. If set, the Start Test bit is further
                               qualified by an implementation specific control
                               signal from a component wide Global Event. Note:
                               not supported in SKX Errata: SPEC to RTL
                               Mismatch. - Unused register
                            */
    UINT32 incldroppedlanes : 1;

                            /* Bits[7:7], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               If set, include Tx lanes marked TxLaneDropped
                               and Rx lanes marked RxLaneDropped in
                               Loopback.Pattern with deskew setting of 0. Late
                               action.
                            */
    UINT32 rsvd : 1;

                            /* Bits[8:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pattbuffselect : 5;

                            /* Bits[13:9], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Pattern buffer selection used to index pattern
                               buffers through the PH_TTPGB register (see
                               PH_TTPGB: Pattern Generator Buffer Access
                               Register). Since PH_TTPGB is a through register,
                               target registers must be provided for each
                               address below. x00-x03 - Access pattern buffer 1
                               LS-Dword (lsdw) to MS-Dword (msfw) x04-x07 -
                               Access pattern buffer 2 LD-Dword to MSDword. x08
                               - Access Loopback LFSR1 seed x09- Access
                               Loopback LFSR3 seed x0a Access Loopback LFSR5
                               seed x0b- Access Loopback LFSR7 seed x0c-x1f:
                               reserved
                            */
    UINT32 rsvd_14 : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 loop_cnt_inhibit : 1;

                            /* Bits[16:16], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Valid only if the Loop Counter Inhibit
                               Capability is 1. This field may be used to
                               inhibit the counting in the Loop Counter when it
                               is set. Normally this would be used when setting
                               Error Counter Inhibit, so there is an accurate
                               reading of the Loop Counter Status. 0 - Normal
                               counting of pattern check loops 1 - Inhibit
                               counting of pattern check loops Errata: SPEC to
                               RTL Mismatch. - Unused register
                            */
    UINT32 lfsr1_stagger_en : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000001*/

                            /*
                               Selects a staggered scrambling pattern to be
                               generated when LFSR1 is selected for a
                               pseudorandom pattern. 0 - Do not stagger the
                               LFSR1 pattern(not supported) 1 - Enable
                               staggering of the LFSR1 pattern(default) Errata:
                               SPEC to RTL Mismatch. - Unused register
                            */
    UINT32 lfsr2_stagger_en : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000001*/

                            /*
                               Selects a staggered scrambling pattern to be
                               generated when LFSR2 is selected for a
                               pseudorandom pattern. 0 - Do not stagger the
                               LFSR2 pattern(not supported) 1 - Enable
                               staggering of the LFSR2 pattern(default) Errata:
                               SPEC to RTL Mismatch. - Unused register
                            */
    UINT32 drive0or1 : 1;

                            /* Bits[19:19], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Drive DC Zero Instead of One Selects the DC
                               Logic Value to use when KTIREUT_PH_TTDDC selects
                               a DC pattern override. 0 - Drive a logic 1 for
                               all lanes selected in KTIREUT_PH_TTDDC 1 - Drive
                               a logic 0 for all lanes selected in
                               KTIREUT_PH_TTDDC Errata: SPEC to RTL Mismatch. -
                               Unused register
                            */
    UINT32 stoptest : 1;

                            /* Bits[20:20], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               if KTIREUT_PH_TTCAP(0) is 0, setting this bit
                               stops the test. 1 - Stops the test - forces an
                               exit from Loopback.Pattern. The bit is cleared
                               when Loopback.Pattern is exited
                            */
    UINT32 errcntstall : 1;

                            /* Bits[21:21], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Error Counters Inhibit Inhibits the counting of
                               errors when in Loopback.Pattern. 0 - Normal
                               Error counting 1 - Inhibit Error counting
                            */
    UINT32 lfsrpolynomialsel : 1;

                            /* Bits[22:22], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Polynomial Select 0 - Use the spec polynomial 1
                               - RSVD Errata: SPEC to RTL Mismatch. - Unused
                               register
                            */
    UINT32 autobufselenable : 1;

                            /* Bits[23:23], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Rotate Pattern Buffer Selection If set, this bit
                               will enable rotation of the KTIREUT_PH_TTPG2S
                               register during the test. The actual register is
                               not rotated, but an operating copy is made at
                               the beginning of the test, and then the copy is
                               rotated across all lanes of the interface. 0 -
                               Rotation is disabled 1 - Rotation is enabled
                            */
    UINT32 exploopcnt : 5;

                            /* Bits[28:24], Access Type=RW/P/L, default=0x00000006*/

                            /*
                               Exponential Loop Count Exponential Test Loop
                               Count. Loop count is 2(count in this field) Each
                               increment in loop counter indicates 128 UI of
                               Pattern Buffer Register content. 0 - Infinite
                               count. Use Soft Reset to exit test.
                            */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTPGC_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTTEC_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a190)                                                      */
/*      SPRB0 (0x21e0a190)                                                      */
/*      SPRHBM (0x21e0a190)                                                     */
/*      SPRC0 (0x21e0a190)                                                      */
/*      SPRMCC (0x21e0a190)                                                     */
/*      SPRUCC (0x21e0a190)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_tttec
*/


#define KTIREUT_PH_TTTEC_KTI_REUT_REG 0x09120190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_test : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Test started on change from 0 to 1 (edge detect)
                               if in Compliance or Loopback.Marker. Note:
                               Writing Start Test from a 0 to 1 while also
                               writing Stop Test to a 1 will result in no test
                               being executed. Clearing Start Test while a test
                               is in process will corrupt (clear) the test
                               progress status bits in the KTIREUT_PH_TTTES
                               register. Start Test should only be cleared
                               after all test results have been examined.
                               Register not supported in processor Errata: SPEC
                               to RTL Mismatch. - Unused register
                            */
    UINT32 stop_test : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Test stopped on change from 0 to 1 (edge detect)
                               if in Loopback.Pattern. Register not supported
                               in processor Errata: SPEC to RTL Mismatch. -
                               Unused register
                            */
    UINT32 start_delay_sel_index : 2;

                            /* Bits[3:2], Access Type=RO, default=0x00000000*/

                            /*
                               Start Delay Select Index into PH_TTTED Selects
                               the execution delay in PH_TTTED. Register not
                               supported in processor Errata: SPEC to RTL
                               Mismatch. - Unused register
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTTEC_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTTED_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a194)                                                      */
/*      SPRB0 (0x21e0a194)                                                      */
/*      SPRHBM (0x21e0a194)                                                     */
/*      SPRC0 (0x21e0a194)                                                      */
/*      SPRMCC (0x21e0a194)                                                     */
/*      SPRUCC (0x21e0a194)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_ttted
*/


#define KTIREUT_PH_TTTED_KTI_REUT_REG 0x09120194

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_test_delay_ctrl0 : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*
                               Test execution delay Value number 0. Following
                               the setting of the Start Test bit, the selected
                               delay value as indexed in Start Delay Select is
                               used to delay the actual start of the test by
                               the programmed number of TSL. If Start
                               Coordination is set then the timer will not
                               begin until the input trigger has happened.
                               Register not supported in processor Errata: SPEC
                               to RTL Mismatch. - Unused register
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTTED_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTPCC_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a198)                                                      */
/*      SPRB0 (0x21e0a198)                                                      */
/*      SPRHBM (0x21e0a198)                                                     */
/*      SPRC0 (0x21e0a198)                                                      */
/*      SPRMCC (0x21e0a198)                                                     */
/*      SPRUCC (0x21e0a198)                                                     */
/* Register default value on SPRA0: 0x04000002                                  */
/* Register default value on SPRB0: 0x04000002                                  */
/* Register default value on SPRHBM: 0x04000002                                 */
/* Register default value on SPRC0: 0x04000002                                  */
/* Register default value on SPRMCC: 0x04000002                                 */
/* Register default value on SPRUCC: 0x04000002                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_ttpcc
*/


#define KTIREUT_PH_TTPCC_KTI_REUT_REG 0x09120198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 init_check_mask : 4;

                            /* Bits[3:0], Access Type=RW/P/L, default=0x00000002*/

                            /*
                               InitialCheckMask The Initial Check Mask is used
                               for a settling time for any Electrical Parameter
                               offsets that are applied upon entry to
                               Loopback.Pattern and for settling time for any
                               Electrical Parameter Global values applied upon
                               re-entry to Loopback.Marker. Upon entry to
                               Loopback.Pattern, no RX lane errors will be
                               logged during the Initial Check Mask period.
                               Initial check mask period is 2(Count in this
                               field) * 1TSL. 0 - Indicates no masking.
                            */
    UINT32 rsvd : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sel_err_check_int : 3;

                            /* Bits[7:5], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Selective Error Check Interval = 2Count in this
                               Field. Checks error for 1UI at every Error Check
                               Interval starting from Error Check Start.
                            */
    UINT32 sel_err_check_start : 7;

                            /* Bits[14:8], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               The initial UI in the pattern buffer to start
                               checking errors in the arriving pattern
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_err_counter_lane_sel : 5;

                            /* Bits[20:16], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               The value of RX Error Counter Lane Select is
                               used to indicate which Lane will have the full
                               32 bit error counter and the other Lanes will
                               have only 8 bit error counters that are
                               observable using RX Error Counter Lane Select.
                               0: Rx Lane 0 Selected. 1: Rx Lane 1 Selected.
                               Etc..
                            */
    UINT32 rsvd_21 : 5;

                            /* Bits[25:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ext_err_ctr_lane_assign : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000001*/

                            /*
                               This field is valid only if Extended Count Lane
                               Assign Capability is set. This bit is used to
                               force a reassignment of the 31-bit error
                               counter. Entry into Loopback.Pattern continues
                               to also assign the long error counter lane. 1 -
                               Use the current RxErrCntLaneSel value for
                               assigning the lane that will use the 31-bit
                               error counter 0 - Only normal entry to
                               Loopback.Pattern will cause the lane assignment
                               to use the 31-bit error counter Errata: SPEC to
                               RTL Mismatch. - Unused register
                            */
    UINT32 ext_err_ctr_all_lanes : 1;

                            /* Bits[27:27], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field is only valid if Extended Count All
                               Lanes Capability is 1. The user can enable
                               counting of all bit errors for the complete set
                               of lanes for the tested lanes, starting from the
                               lane selected in Extended Error Counter Lane
                               Select and including any consecutive higher
                               lanes included in the testing. These errors are
                               accumulated using the Extended Error Counter.
                               Any per-lane error counters are invalid for the
                               tested lanes. 1 - Accumulate all errors for all
                               lanes 0 - Per-lane error counters are used
                               Errata: SPEC to RTL Mismatch. - Unused register
                            */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTPCC_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTPGB_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a19c)                                                      */
/*      SPRB0 (0x21e0a19c)                                                      */
/*      SPRHBM (0x21e0a19c)                                                     */
/*      SPRC0 (0x21e0a19c)                                                      */
/*      SPRMCC (0x21e0a19c)                                                     */
/*      SPRUCC (0x21e0a19c)                                                     */
/* Register default value on SPRA0: 0x4DAE1577                                  */
/* Register default value on SPRB0: 0x4DAE1577                                  */
/* Register default value on SPRHBM: 0x4DAE1577                                 */
/* Register default value on SPRC0: 0x4DAE1577                                  */
/* Register default value on SPRMCC: 0x4DAE1577                                 */
/* Register default value on SPRUCC: 0x4DAE1577                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_ttpgb
*/


#define KTIREUT_PH_TTPGB_KTI_REUT_REG 0x0912019C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reutpattbuff : 32;

                            /* Bits[31:0], Access Type=RW/V/P/L, default=0x4DAE1577*/

                            /*
                               All words of Pattern generator buffers are
                               accessed through this register. A particular
                               pattern buffer is indexed using Pattern buffer
                               selection bits in the KTIREUT_PH_TTPGC register
                               Patterns larger than 32 bits are indexed using
                               pattern buffer word selection field in the
                               KTIREUT_PH_TTPGC register. Each pattern buffer
                               is 128 bit, accessed 32 bit at a time. Least
                               significant bit is sent out first but pattern is
                               not shifted. PRBS pattern is generated by the 23
                               bit LFSR using ITU-T PRBS23 polynomial x23 + x18
                               + 1 and the initial seed is programmed using
                               bits 0:23 PatBuf1 Default (msdw = most
                               significant Dword) msdw (D3): ACE3_C3E0h msdw-1
                               (D2): 100F_F7FFh msdw-2 (D1): ACBC_B211h lsdw
                               (D0): 4DAE_1577h
                            */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTPGB_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTPPG_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a1a0)                                                      */
/*      SPRB0 (0x21e0a1a0)                                                      */
/*      SPRHBM (0x21e0a1a0)                                                     */
/*      SPRC0 (0x21e0a1a0)                                                      */
/*      SPRMCC (0x21e0a1a0)                                                     */
/*      SPRUCC (0x21e0a1a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_ttppg
*/


#define KTIREUT_PH_TTPPG_KTI_REUT_REG 0x091201A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 c_reinit_lfsr : 16;

                            /* Bits[15:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Reinit LFSR with seed after CReinitLFSR TSL.
                               (This implies that LFSR seed registers are
                               distinct from the LFSR). 0: do not re-init LFSR
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTPPG_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTPG2S_KTI_REUT_REG supported on:                                 */
/*      SPRA0 (0x21e0a1a4)                                                      */
/*      SPRB0 (0x21e0a1a4)                                                      */
/*      SPRHBM (0x21e0a1a4)                                                     */
/*      SPRC0 (0x21e0a1a4)                                                      */
/*      SPRMCC (0x21e0a1a4)                                                     */
/*      SPRUCC (0x21e0a1a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_ttpg2s
*/


#define KTIREUT_PH_TTPG2S_KTI_REUT_REG 0x091201A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pattbuflanesel : 24;

                            /* Bits[23:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Lane selects Pattern buffer 2 for its testing.
                               One bit per each lane Bit 0 - Controls Lane 0
                               Bit 1 - Controls Lane 1 ..., and so on. 1 -
                               Corresponding lane will use Pattern Buffer 2 for
                               test.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTPG2S_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTREC_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a1a8)                                                      */
/*      SPRB0 (0x21e0a1a8)                                                      */
/*      SPRHBM (0x21e0a1a8)                                                     */
/*      SPRC0 (0x21e0a1a8)                                                      */
/*      SPRMCC (0x21e0a1a8)                                                     */
/*      SPRUCC (0x21e0a1a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_ttrec
*/


#define KTIREUT_PH_TTREC_KTI_REUT_REG 0x091201A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxerrchdisable : 24;

                            /* Bits[23:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Rx Lane Error Check Disable One bit for each Rx
                               lane, if set, will disable error check and
                               accumulation during loop back test. Bit 0 - Rx
                               Lane 0. Bit 1 - Rx Lane 1. ... Bit 23 - Rx Lane
                               23
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTREC_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTPIS_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a1ac)                                                      */
/*      SPRB0 (0x21e0a1ac)                                                      */
/*      SPRHBM (0x21e0a1ac)                                                     */
/*      SPRC0 (0x21e0a1ac)                                                      */
/*      SPRMCC (0x21e0a1ac)                                                     */
/*      SPRUCC (0x21e0a1ac)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_ttpis
*/


#define KTIREUT_PH_TTPIS_KTI_REUT_REG 0x091201AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 patterninvert : 24;

                            /* Bits[23:0], Access Type=RW/P/L, default=0x00000001*/

                            /*
                               Pattern Invert Selection One bit for each Tx
                               lane. Bit 0 - Tx Lane 0 Bit 1 - Tx Lane 1 ..,
                               and so on. 1 - Invert the pattern in
                               corresponding Tx lane. This register sets the
                               initial inversion phase and is not dynamically
                               updated as the pattern progresses.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTPIS_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTDDC_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a1b0)                                                      */
/*      SPRB0 (0x21e0a1b0)                                                      */
/*      SPRHBM (0x21e0a1b0)                                                     */
/*      SPRC0 (0x21e0a1b0)                                                      */
/*      SPRMCC (0x21e0a1b0)                                                     */
/*      SPRUCC (0x21e0a1b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_ttddc
*/


#define KTIREUT_PH_TTDDC_KTI_REUT_REG 0x091201B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txdatlanctr : 24;

                            /* Bits[23:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               TX DC Data Lane Control A bit mask used for
                               selectively overriding the data TX to output a
                               DC logical '1' condition during
                               Loopback.Pattern. Used for debug and validation
                               purposes to quiet neighboring crosstalk lines,
                               and is only applied during Loopback.Pattern.
                               Only the KTIPHTDC register will override the
                               behavior of this register. A bit value of 1
                               indicates the corresponding lane is transmitting
                               a logical 1. Bit 0: Controls Lane 0. Bit 1:
                               Controls Lane 1. .. and so on.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTDDC_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTLECR_KTI_REUT_REG supported on:                                 */
/*      SPRA0 (0x21e0a1b4)                                                      */
/*      SPRB0 (0x21e0a1b4)                                                      */
/*      SPRHBM (0x21e0a1b4)                                                     */
/*      SPRC0 (0x21e0a1b4)                                                      */
/*      SPRMCC (0x21e0a1b4)                                                     */
/*      SPRUCC (0x21e0a1b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_ttlecr
*/


#define KTIREUT_PH_TTLECR_KTI_REUT_REG 0x091201B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 laneerrcnt : 8;

                            /* Bits[7:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               <p>Lane Error Counter This field will contain
                               the lower bits of the Extended Lane Error
                               Counter, or the error count bits of the short
                               error counter for the lane selected by the
                               current RX Error Counter Lane Select. To
                               properly read all 32 bits of the Extended Lane
                               Error Counter, the current value in the RX Error
                               Counter Lane Select field must match the
                               assigned lane designated on entry to Pattern. If
                               Extended Error Count All Lanes is set, then the
                               value of RX Error Counter Lane Select should not
                               be changed from the assigned lane designated
                               upon entry to Pattern. Any per-lane Error
                               Counters will be cleared by software writing 1
                               to the appropriate bit in the Receive Error
                               Status register, writing 1 to the appropriate
                               bit in the Receive Error Check Disable field, or
                               by a Cold Reset. Writing to this field will have
                               no affect on any per-Lane Error
                               Counters.&nbsp;</p> <p>Any per-lane Error
                               Counters will be cleared by software writing 1
                               to the appropriate bit in the Receive Error
                               Status register, or by a Cold Reset. Writing to
                               this field will have no affect on any per-Lane
                               Error Counters.&nbsp;</p>
                            */
    UINT32 ex_laneerrcnt : 23;

                            /* Bits[30:8], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               <p>Extended Lane Error Counter Field is assigned
                               using the RxError Counter Lane Select on entry
                               to Loopback.Pattern. It may also be reassigned
                               at any time by setting the Extended Error
                               Counter Lane Assign. The Extended Lane Error
                               Counter can be cleared through software by
                               writing 1 the bit in TTRES that matches the
                               currently selected lane in TTPCC[20:16].</p>
                               <p>Note: &nbsp;This field
                               is&nbsp;cleared&nbsp;when&nbsp;selected lane in
                               TTPCC[20:16] is changed.</p>
                            */
    UINT32 errovrflow : 1;

                            /* Bits[31:31], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               <p>Error Overflow If set, indicates that the
                               error counter has overflown in selected lane.
                               Cleared by writing a 1 to selected lane in
                               TT_RES.</p>
                            */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTLECR_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTTLS_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a1b8)                                                      */
/*      SPRB0 (0x21e0a1b8)                                                      */
/*      SPRHBM (0x21e0a1b8)                                                     */
/*      SPRC0 (0x21e0a1b8)                                                      */
/*      SPRMCC (0x21e0a1b8)                                                     */
/*      SPRUCC (0x21e0a1b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_tttls
*/


#define KTIREUT_PH_TTTLS_KTI_REUT_REG 0x091201B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_tx_loopcntstat : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Tx Loop Count The Loop Count Status register
                               represents the current received Loop Count while
                               in Loopback.Pattern or the final transmitted
                               Loop Count after a test. The Loop Count Status
                               is continuously incremented by 1 on the
                               transmission of a complete pattern buffer
                               boundary while in Loopback.Pattern and rolls
                               over to all 0s upon hitting the maximum value.
                               Errata: SPEC to RTL Mismatch. - same behaviour
                               as ICX.
                            */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTTLS_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTLCS_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a1bc)                                                      */
/*      SPRB0 (0x21e0a1bc)                                                      */
/*      SPRHBM (0x21e0a1bc)                                                     */
/*      SPRC0 (0x21e0a1bc)                                                      */
/*      SPRMCC (0x21e0a1bc)                                                     */
/*      SPRUCC (0x21e0a1bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_ttlcs
*/


#define KTIREUT_PH_TTLCS_KTI_REUT_REG 0x091201BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s_rx_loopcntstat : 32;

                            /* Bits[31:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Rx Loop Count The Loop Count Status register
                               represents the current received Loop Count while
                               in Loopback.Pattern or the final checked Loop
                               Count after a test. The Loop Count Status is
                               continuously incremented by 1 on the reception
                               of a complete pattern buffer boundary while in
                               Loopback.Pattern and rolls over to all 0s upon
                               hitting the maximum value. When Stop on Error is
                               set to 1 and a specific LFSR is used to scramble
                               either Pattern Buffer 1 or 2, the Loop Count
                               Status can be used to recreate the region of the
                               failing pattern by software replicating the LFSR
                               using the resultant Loop Count Status. The Loop
                               Count Status can be cleared through software
                               writing 1s to this register, or a Cold Reset.
                            */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTLCS_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTRES_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a1c0)                                                      */
/*      SPRB0 (0x21e0a1c0)                                                      */
/*      SPRHBM (0x21e0a1c0)                                                     */
/*      SPRC0 (0x21e0a1c0)                                                      */
/*      SPRMCC (0x21e0a1c0)                                                     */
/*      SPRUCC (0x21e0a1c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_ttres
*/


#define KTIREUT_PH_TTRES_KTI_REUT_REG 0x091201C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx_lane_error_status : 24;

                            /* Bits[23:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Rx Lane Error Status One bit per each Rx lane
                               Bit 0 - Status of Lane 0 Bit 1 - Status of Lane
                               1 ..., and so on. Cleared with writing 1.
                               Clearing a lane status also clears the lane
                               error counter for that lane (but not the
                               extended error lane counter/overflow). 1 -
                               Corresponding lane has received error.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTRES_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTLEO_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a1c4)                                                      */
/*      SPRB0 (0x21e0a1c4)                                                      */
/*      SPRHBM (0x21e0a1c4)                                                     */
/*      SPRC0 (0x21e0a1c4)                                                      */
/*      SPRMCC (0x21e0a1c4)                                                     */
/*      SPRUCC (0x21e0a1c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_ttleo
*/


#define KTIREUT_PH_TTLEO_KTI_REUT_REG 0x091201C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxlaneerrsoverstat : 24;

                            /* Bits[23:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Rx Lane Error Overflow Status Indicates that a
                               particular lane encountered a sufficient number
                               of errors during Loopback.Pattern to overflow
                               any per-lane error counter. The counter may be
                               as simple as 1 bit, in which case this can be
                               interpreted as ?multiple errors?. A value of 1
                               indicates that the particular lane has received
                               an overflow of errors during Loopback.Pattern.
                               One bit per Rx lane. Bit 0 - Status of Lane 0
                               Bit 1 - Status of Lane 1 ... and so on. Cleared
                               by writing 1.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTLEO_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTTES_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a1c8)                                                      */
/*      SPRB0 (0x21e0a1c8)                                                      */
/*      SPRHBM (0x21e0a1c8)                                                     */
/*      SPRC0 (0x21e0a1c8)                                                      */
/*      SPRMCC (0x21e0a1c8)                                                     */
/*      SPRUCC (0x21e0a1c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktireut_ph_tttes
*/


#define KTIREUT_PH_TTTES_KTI_REUT_REG 0x091201C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 test_initiated : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Master Tx SDS sent */
    UINT32 error_check_start : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Master Rx SDS received, error checking has
                               begun. Bit is automatically reset to default
                               state, '0',upon 'Start Test' being set to '0'.
                            */
    UINT32 test_completed : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Full test has been transmitted and checked. Bit
                               is automatically reset to default state, '0',
                               upon 'Start Test' being set to '0'.
                            */
    UINT32 marker_relock_achieved : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Post-test execution transition back to Marker
                               and bit/byte lock achieved. Remains persistent
                               until the TX SDS is sent at which time it is
                               reset to its default state of 0.
                            */
    UINT32 any_lane_fails : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Any lane fails. Bit is automatically reset to
                               default state, '0',upon 'Start Test' being set
                               to '0'.
                            */
    UINT32 all_lanes_fail : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               All lanes fail. Bit is automatically reset to
                               default state, '0',upon 'Start Test' being set
                               to '0'.
                            */
    UINT32 any_lane_overflows : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Any lane error counter overflow. Bit is
                               automatically reset to default state, '0',upon
                               'Start Test' being set to '0'.
                            */
    UINT32 all_lanes_overflow : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               All lanes overflow. Bit is automatically reset
                               to default state, '0',upon 'Start Test' being
                               set to '0'.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTTES_KTI_REUT_STRUCT;

/* KTIREUT_PH_TTSNL_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a1cc)                                                      */
/*      SPRB0 (0x21e0a1cc)                                                      */
/*      SPRHBM (0x21e0a1cc)                                                     */
/*      SPRC0 (0x21e0a1cc)                                                      */
/*      SPRMCC (0x21e0a1cc)                                                     */
/*      SPRUCC (0x21e0a1cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_ttsnl
*/


#define KTIREUT_PH_TTSNL_KTI_REUT_REG 0x091201CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lpbkpath : 24;

                            /* Bits[23:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Lane selects for loopback path from Rx Bit 0 -
                               Controls Lane 0 Bit 1 - Controls Lane 1 ..., and
                               so on. 0 - Corresponding lane will use RX
                               loopback path 1 - Corresponding lane will use
                               path from pattern generator
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TTSNL_KTI_REUT_STRUCT;

/* KTIREUT_PH_TL0PENTRYTX_KTI_REUT_REG supported on:                            */
/*      SPRA0 (0x21e0a1e0)                                                      */
/*      SPRB0 (0x21e0a1e0)                                                      */
/*      SPRHBM (0x21e0a1e0)                                                     */
/*      SPRC0 (0x21e0a1e0)                                                      */
/*      SPRMCC (0x21e0a1e0)                                                     */
/*      SPRUCC (0x21e0a1e0)                                                     */
/* Register default value on SPRA0: 0x06000664                                  */
/* Register default value on SPRB0: 0x06000664                                  */
/* Register default value on SPRHBM: 0x06000664                                 */
/* Register default value on SPRC0: 0x06000664                                  */
/* Register default value on SPRMCC: 0x06000664                                 */
/* Register default value on SPRUCC: 0x06000664                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_tl0pentrytx
*/


#define KTIREUT_PH_TL0PENTRYTX_KTI_REUT_REG 0x091201E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_l0p_tx_tran2done : 8;

                            /* Bits[7:0], Access Type=RW/P/L, default=0x00000064*/

                            /* Linear time value, with granularity of 8UI */
    UINT32 t_l0p_tx_enter2tran : 8;

                            /* Bits[15:8], Access Type=RW/P/L, default=0x00000006*/

                            /*
                               Linear time value, with granularity of 8UI; due
                               to an implementation-specific restriction, the
                               value of this register field must be greater
                               than or equal to 6.
                            */
    UINT32 t_l0p_tx_nullstart2nullstop : 8;

                            /* Bits[23:16], Access Type=RW/P/L, default=0x00000000*/

                            /* Linear time value, with granularity of 8UI */
    UINT32 t_l0p_tx_enter2null : 8;

                            /* Bits[31:24], Access Type=RW/P/L, default=0x00000006*/

                            /*
                               Linear time value, with granularity of 8UI; due
                               to an implementation-specific restriction, the
                               value of this register field must be greater
                               than or equal to 6.
                            */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TL0PENTRYTX_KTI_REUT_STRUCT;

/* KTIREUT_PH_L0PENTRYRX_KTI_REUT_REG supported on:                             */
/*      SPRA0 (0x21e0a1e4)                                                      */
/*      SPRB0 (0x21e0a1e4)                                                      */
/*      SPRHBM (0x21e0a1e4)                                                     */
/*      SPRC0 (0x21e0a1e4)                                                      */
/*      SPRMCC (0x21e0a1e4)                                                     */
/*      SPRUCC (0x21e0a1e4)                                                     */
/* Register default value on SPRA0: 0x06000664                                  */
/* Register default value on SPRB0: 0x06000664                                  */
/* Register default value on SPRHBM: 0x06000664                                 */
/* Register default value on SPRC0: 0x06000664                                  */
/* Register default value on SPRMCC: 0x06000664                                 */
/* Register default value on SPRUCC: 0x06000664                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_l0pentryrx
*/


#define KTIREUT_PH_L0PENTRYRX_KTI_REUT_REG 0x091201E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_l0p_rx_tran2done : 8;

                            /* Bits[7:0], Access Type=RW/P/L, default=0x00000064*/

                            /* Linear time value, with Granularity is 8UI */
    UINT32 t_l0p_rx_enter2tran : 8;

                            /* Bits[15:8], Access Type=RW/P/L, default=0x00000006*/

                            /*
                               Linear time value, with Granularity is 8UI; due
                               to an implementation-specific restriction, the
                               value of this register field must be greater
                               than or equal to 6.
                            */
    UINT32 t_l0p_rx_tran2unblock : 8;

                            /* Bits[23:16], Access Type=RW/P/L, default=0x00000000*/

                            /* Linear time value, with Granularity is 8UI */
    UINT32 t_l0p_rx_enter2block : 8;

                            /* Bits[31:24], Access Type=RW/P/L, default=0x00000006*/

                            /*
                               Linear time value, with Granularity is 8UI; due
                               to an implementation-specific restriction, the
                               value of this register field must be greater
                               than or equal to 6.
                            */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_L0PENTRYRX_KTI_REUT_STRUCT;

/* KTIREUT_PH_TL0PEXITTXACT_KTI_REUT_REG supported on:                          */
/*      SPRA0 (0x21e0a1e8)                                                      */
/*      SPRB0 (0x21e0a1e8)                                                      */
/*      SPRHBM (0x21e0a1e8)                                                     */
/*      SPRC0 (0x21e0a1e8)                                                      */
/*      SPRMCC (0x21e0a1e8)                                                     */
/*      SPRUCC (0x21e0a1e8)                                                     */
/* Register default value on SPRA0: 0x0000B500                                  */
/* Register default value on SPRB0: 0x0000B500                                  */
/* Register default value on SPRHBM: 0x0000B500                                 */
/* Register default value on SPRC0: 0x0000B500                                  */
/* Register default value on SPRMCC: 0x0000B500                                 */
/* Register default value on SPRUCC: 0x0000B500                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_tl0pexittxact
*/


#define KTIREUT_PH_TL0PEXITTXACT_KTI_REUT_REG 0x091201E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_l0p_tx_nullsexit2tran : 8;

                            /* Bits[7:0], Access Type=RW/P/L, default=0x00000000*/

                            /* Linear time value, with granularity of 8UI */
    UINT32 t_l0p_tx_sqend2sds : 8;

                            /* Bits[15:8], Access Type=RW/P/L, default=0x000000B5*/

                            /* Linear time value, with granularity of 128UI */
    UINT32 t_l0p_tx_nullsexit_sq : 8;

                            /* Bits[23:16], Access Type=RW/P/L, default=0x00000000*/

                            /* Linear time value, with granularity of 8UI */
    UINT32 t_l0p_tx_exitend2l0c : 8;

                            /* Bits[31:24], Access Type=RW/P/L, default=0x00000000*/

                            /* Linear time value, with granularity of 8UI */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TL0PEXITTXACT_KTI_REUT_STRUCT;

/* KTIREUT_PH_TL0PEXITTXIDL_KTI_REUT_REG supported on:                          */
/*      SPRA0 (0x21e0a1ec)                                                      */
/*      SPRB0 (0x21e0a1ec)                                                      */
/*      SPRHBM (0x21e0a1ec)                                                     */
/*      SPRC0 (0x21e0a1ec)                                                      */
/*      SPRMCC (0x21e0a1ec)                                                     */
/*      SPRUCC (0x21e0a1ec)                                                     */
/* Register default value on SPRA0: 0x06F50620                                  */
/* Register default value on SPRB0: 0x06F50620                                  */
/* Register default value on SPRHBM: 0x06F50620                                 */
/* Register default value on SPRC0: 0x060D0620                                  */
/* Register default value on SPRMCC: 0x060D0620                                 */
/* Register default value on SPRUCC: 0x060D0620                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_tl0pexittxidl
*/


#define KTIREUT_PH_TL0PEXITTXIDL_KTI_REUT_REG 0x091201EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_l0p_tx_exit2next : 8;

                            /* Bits[7:0], Access Type=RW/P/L, default=0x00000020*/

                            /* Linear time value, with granularity of 128UI */
    UINT32 t_l0p_tx_sds2widthchg : 8;

                            /* Bits[15:8], Access Type=RW/P/L, default=0x00000006*/

                            /* Linear time value, with granularity of 8UI */
    UINT32 t_l0p_tx_sds : 2;

                            /* Bits[17:16], Access Type=RO, default=0x00000001*/

                            /* Linear time value, with granularity of 128UI */
    UINT32 t_l0p_tx_pat : 8;

                            /* Bits[25:18], Access Type=RW/P/L, default=0x000000BD*/

                            /* Linear time value, with granularity of 128UI */
    UINT32 t_l0p_tx_sbpa : 2;

                            /* Bits[27:26], Access Type=RW/P/L, default=0x00000001*/

                            /* Linear time value, with granularity of 128UI */
    UINT32 t_l0p_tx_null2firston : 4;

                            /* Bits[31:28], Access Type=RW/P/L, default=0x00000000*/

                            /* Linear time value, with granularity of 8UI */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TL0PEXITTXIDL_KTI_REUT_STRUCT;

/* KTIREUT_PH_TL0PEXITRX_KTI_REUT_REG supported on:                             */
/*      SPRA0 (0x21e0a1f4)                                                      */
/*      SPRB0 (0x21e0a1f4)                                                      */
/*      SPRHBM (0x21e0a1f4)                                                     */
/*      SPRC0 (0x21e0a1f4)                                                      */
/*      SPRMCC (0x21e0a1f4)                                                     */
/*      SPRUCC (0x21e0a1f4)                                                     */
/* Register default value on SPRA0: 0x00BE0600                                  */
/* Register default value on SPRB0: 0x00BE0600                                  */
/* Register default value on SPRHBM: 0x00BE0600                                 */
/* Register default value on SPRC0: 0x00840600                                  */
/* Register default value on SPRMCC: 0x00840600                                 */
/* Register default value on SPRUCC: 0x00840600                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_tl0pexitrx
*/


#define KTIREUT_PH_TL0PEXITRX_KTI_REUT_REG 0x091201F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_l0p_rx_nulls_widthchg : 8;

                            /* Bits[7:0], Access Type=RW/P/L, default=0x00000000*/

                            /* Linear time value, with granularity of 8UI */
    UINT32 t_l0p_rx_sds2widthchg : 8;

                            /* Bits[15:8], Access Type=RW/P/L, default=0x00000006*/

                            /* Linear time value, with granularity of 8UI */
    UINT32 t_l0p_est : 8;

                            /* Bits[23:16], Access Type=RW/P/L, default=0x000000BE*/

                            /* Linear time value, with granularity of 128UI */
    UINT32 t_l0p_rx_nullsexit_sq : 8;

                            /* Bits[31:24], Access Type=RW/P/L, default=0x00000000*/

                            /* Linear time value, with granularity of 8UI */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TL0PEXITRX_KTI_REUT_STRUCT;

/* KTIREUT_PH_TL1ENTRY_KTI_REUT_REG supported on:                               */
/*      SPRA0 (0x21e0a1f8)                                                      */
/*      SPRB0 (0x21e0a1f8)                                                      */
/*      SPRHBM (0x21e0a1f8)                                                     */
/*      SPRC0 (0x21e0a1f8)                                                      */
/*      SPRMCC (0x21e0a1f8)                                                     */
/*      SPRUCC (0x21e0a1f8)                                                     */
/* Register default value on SPRA0: 0x0F00004A                                  */
/* Register default value on SPRB0: 0x0F00004A                                  */
/* Register default value on SPRHBM: 0x0F00004A                                 */
/* Register default value on SPRC0: 0x0F00004A                                  */
/* Register default value on SPRMCC: 0x0F00004A                                 */
/* Register default value on SPRUCC: 0x0F00004A                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_tl1entry
*/


#define KTIREUT_PH_TL1ENTRY_KTI_REUT_REG 0x091201F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_l1_minres : 12;

                            /* Bits[11:0], Access Type=RW/P/L, default=0x0000004A*/

                            /* Linear time value, with granularity of 128UI */
    UINT32 rsvd : 8;

                            /* Bits[19:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 t_l1_tx_firstoff2lastoff : 4;

                            /* Bits[23:20], Access Type=RW/P/L, default=0x00000000*/

                            /* Linear time value, with granularity of 128UI */
    UINT32 t_l1_tx_enter2firstoff : 8;

                            /* Bits[31:24], Access Type=RW/P/L, default=0x0000000F*/

                            /* Linear time value, with granularity of 8UI */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TL1ENTRY_KTI_REUT_STRUCT;

/* KTIREUT_PH_TPRI_SL_KTI_REUT_REG supported on:                                */
/*      SPRA0 (0x21e0a200)                                                      */
/*      SPRB0 (0x21e0a200)                                                      */
/*      SPRHBM (0x21e0a200)                                                     */
/*      SPRC0 (0x21e0a200)                                                      */
/*      SPRMCC (0x21e0a200)                                                     */
/*      SPRUCC (0x21e0a200)                                                     */
/* Register default value on SPRA0: 0x00C13344                                  */
/* Register default value on SPRB0: 0x00C13344                                  */
/* Register default value on SPRHBM: 0x00C13344                                 */
/* Register default value on SPRC0: 0x00C13344                                  */
/* Register default value on SPRMCC: 0x00C13344                                 */
/* Register default value on SPRUCC: 0x00C13344                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_tpri_sl
*/


#define KTIREUT_PH_TPRI_SL_KTI_REUT_REG 0x09120200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_tc : 4;

                            /* Bits[3:0], Access Type=RW/P/L, default=0x00000004*/

                            /*
                               Tc calib min time; for stagger on; exponential
                               time value, with granularity of 128UI
                            */
    UINT32 t_rc : 4;

                            /* Bits[7:4], Access Type=RW/P/L, default=0x00000004*/

                            /*
                               Rx calib min time; for stagger Tx off;
                               exponential time value, with granularity of
                               128UI
                            */
    UINT32 t_rs : 4;

                            /* Bits[11:8], Access Type=RW/P/L, default=0x00000003*/

                            /*
                               ResetS residency for driving EIOS; exponential
                               time value, with granularity of 128UI
                            */
    UINT32 t_rdb : 4;

                            /* Bits[15:12], Access Type=RW/P/L, default=0x00000003*/

                            /*
                               RxDetect debounce time per leg. Should be
                               programmed to same value on both sides of link.
                               Exponential time value, with granularity of
                               128UI
                            */
    UINT32 n_tdr : 4;

                            /* Bits[19:16], Access Type=RW/P/L, default=0x00000001*/

                            /*
                               TxDetect Rx wake occurance window count for
                               handshake; value*granularity (1) = number of
                               consecutive 4K UI windows during which wake must
                               be detected for debounce success in Tx Detect
                            */
    UINT32 n_tdt : 8;

                            /* Bits[27:20], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               TxDetect wake supersequence (PatDetect) count
                               for handshake; value*granularity (1) = number of
                               TxDetect wake supersequence (PatDetect) count
                               for handshake
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TPRI_SL_KTI_REUT_STRUCT;

/* KTIREUT_PH_TSEC_SL_KTI_REUT_REG supported on:                                */
/*      SPRA0 (0x21e0a204)                                                      */
/*      SPRB0 (0x21e0a204)                                                      */
/*      SPRHBM (0x21e0a204)                                                     */
/*      SPRC0 (0x21e0a204)                                                      */
/*      SPRMCC (0x21e0a204)                                                     */
/*      SPRUCC (0x21e0a204)                                                     */
/* Register default value on SPRA0: 0xA5C09640                                  */
/* Register default value on SPRB0: 0xA5C09640                                  */
/* Register default value on SPRHBM: 0xA5C09640                                 */
/* Register default value on SPRC0: 0xA5C09640                                  */
/* Register default value on SPRMCC: 0xA5C09640                                 */
/* Register default value on SPRUCC: 0xA5C09640                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_tsec_sl
*/


#define KTIREUT_PH_TSEC_SL_KTI_REUT_REG 0x09120204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_lmst : 4;

                            /* Bits[3:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               For Tx adaptation in loopback master-master mode
                               non-zero value should be used for fast speed
                               (e.g., value used for Tclst). Tlmst is typically
                               0. For debug, CSingleStep and CFIA (with non-
                               zero Tlmst) can be used. Exponential time value,
                               with granularity of 4096UI
                            */
    UINT32 t_cfst : 4;

                            /* Bits[7:4], Access Type=RW/P/L, default=0x00000004*/

                            /*
                               Wait for planetary aligned clean flit boundary.
                               If determinism is enabled, Tcfst must be greater
                               than EPhyL0Sync period. Exponential time value,
                               with granularity of 4096UI
                            */
    UINT32 t_clst : 4;

                            /* Bits[11:8], Access Type=RW/P/L, default=0x00000006*/

                            /*
                               Configure link widths; exponential time value,
                               with granularity of 4096UI
                            */
    UINT32 t_plst : 4;

                            /* Bits[15:12], Access Type=RW/P/L, default=0x00000009*/

                            /*
                               Adapt+bitlock/bytelock/deskew; exponential time
                               value, with granularity of 4096UI
                            */
    UINT32 t_rdst : 4;

                            /* Bits[19:16], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Remote Rx detect/debounce; 0 or no timeout for
                               hot plug; non-0 value to debug hangs exponential
                               time value, with granularity of 4096UI
                            */
    UINT32 t_tdst : 4;

                            /* Bits[23:20], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               Squelch exit detect/debounce. This value (in UI)
                               should be larger than Tl0c. Exponential time
                               value, with granularity of 4096UI
                            */
    UINT32 t_tcst : 4;

                            /* Bits[27:24], Access Type=RW/P/L, default=0x00000005*/

                            /*
                               Timed Tx calib, e.g. icomp; exponential time
                               value, with granularity of 4096UI
                            */
    UINT32 t_rcst : 4;

                            /* Bits[31:28], Access Type=RW/P/L, default=0x0000000A*/

                            /*
                               Timed Rx calibration, e.g. oc; exponential time
                               value, with granularity of 4096UI
                            */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TSEC_SL_KTI_REUT_STRUCT;

/* KTIREUT_PH_TL0CENTRY_SL_KTI_REUT_REG supported on:                           */
/*      SPRA0 (0x21e0a20c)                                                      */
/*      SPRB0 (0x21e0a20c)                                                      */
/*      SPRHBM (0x21e0a20c)                                                     */
/*      SPRC0 (0x21e0a20c)                                                      */
/*      SPRMCC (0x21e0a20c)                                                     */
/*      SPRUCC (0x21e0a20c)                                                     */
/* Register default value on SPRA0: 0x00000020                                  */
/* Register default value on SPRB0: 0x00000020                                  */
/* Register default value on SPRHBM: 0x00000020                                 */
/* Register default value on SPRC0: 0x00000020                                  */
/* Register default value on SPRMCC: 0x00000020                                 */
/* Register default value on SPRUCC: 0x00000020                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_tl0centry_sl
*/


#define KTIREUT_PH_TL0CENTRY_SL_KTI_REUT_REG 0x0912020C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_ieo : 12;

                            /* Bits[11:0], Access Type=RW/P/L, default=0x00000020*/

                            /*
                               Init exit delay; used in debug to delay exit
                               from init; linear time value, with granularity
                               of 8UI
                            */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TL0CENTRY_SL_KTI_REUT_STRUCT;

/* KTIREUT_PH_TL0C_SL_KTI_REUT_REG supported on:                                */
/*      SPRA0 (0x21e0a210)                                                      */
/*      SPRB0 (0x21e0a210)                                                      */
/*      SPRHBM (0x21e0a210)                                                     */
/*      SPRC0 (0x21e0a210)                                                      */
/*      SPRMCC (0x21e0a210)                                                     */
/*      SPRUCC (0x21e0a210)                                                     */
/* Register default value on SPRA0: 0x00010021                                  */
/* Register default value on SPRB0: 0x00010021                                  */
/* Register default value on SPRHBM: 0x00010021                                 */
/* Register default value on SPRC0: 0x00010021                                  */
/* Register default value on SPRMCC: 0x00010021                                 */
/* Register default value on SPRUCC: 0x00010021                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_tl0c_sl
*/


#define KTIREUT_PH_TL0C_SL_KTI_REUT_REG 0x09120210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_l0c : 8;

                            /* Bits[7:0], Access Type=RW/P/L, default=0x00000021*/

                            /*
                               L0c clean flit interval. This value is loaded
                               into the local TL0ct and TL0cr timers under PHY
                               LTSSM control. Timeout is disabled if value is
                               0. Linear time value, with granularity of 128UI
                            */
    UINT32 t_l1_reissue_delay : 8;

                            /* Bits[15:8], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               L1 reissue delayed until after timeout (512UI
                               granularity)
                            */
    UINT32 t_l0cq : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               L0c duration. Value must be 1. Linear time
                               value, with granularity of 8UI. Errata: SPEC to
                               RTL Mismatch. Unused register
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TL0C_SL_KTI_REUT_STRUCT;

/* KTIREUT_PH_TPRI_OP_KTI_REUT_REG supported on:                                */
/*      SPRA0 (0x21e0a214)                                                      */
/*      SPRB0 (0x21e0a214)                                                      */
/*      SPRHBM (0x21e0a214)                                                     */
/*      SPRC0 (0x21e0a214)                                                      */
/*      SPRMCC (0x21e0a214)                                                     */
/*      SPRUCC (0x21e0a214)                                                     */
/* Register default value on SPRA0: 0x00C13344                                  */
/* Register default value on SPRB0: 0x00C13344                                  */
/* Register default value on SPRHBM: 0x00C13344                                 */
/* Register default value on SPRC0: 0x00C13344                                  */
/* Register default value on SPRMCC: 0x00C13344                                 */
/* Register default value on SPRUCC: 0x00C13344                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_tpri_op
*/


#define KTIREUT_PH_TPRI_OP_KTI_REUT_REG 0x09120214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_tc : 4;

                            /* Bits[3:0], Access Type=RW/P/L, default=0x00000004*/

                            /*
                               Tc calib min time; for stagger on; exponential
                               time value, with granularity of 128UI
                            */
    UINT32 t_rc : 4;

                            /* Bits[7:4], Access Type=RW/P/L, default=0x00000004*/

                            /*
                               Rx calib min time; for stagger Tx off;
                               exponential time value, with granularity of
                               128UI
                            */
    UINT32 t_rs : 4;

                            /* Bits[11:8], Access Type=RW/P/L, default=0x00000003*/

                            /*
                               ResetS residency for driving EIOS; exponential
                               time value, with granularity of 128UI
                            */
    UINT32 t_rdb : 4;

                            /* Bits[15:12], Access Type=RW/P/L, default=0x00000003*/

                            /*
                               RxDetect debounce time per leg. Should be
                               programmed to same value on both sides of link.
                               Exponential time value, with granularity of
                               128UI
                            */
    UINT32 n_tdr : 4;

                            /* Bits[19:16], Access Type=RW/P/L, default=0x00000001*/

                            /*
                               TxDetect Rx wake occurance window count for
                               handshake; value*granluarity (1) = number of
                               consecutive 4K UI windows during which wake must
                               be detected for debounce success in Tx Detect
                            */
    UINT32 n_tdt : 8;

                            /* Bits[27:20], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               TxDetect wake supersequence (PatDetect) count
                               for handshake; value*granularity (1) = number of
                               TxDetect wake supersequence (PatDetect) count
                               for handshake
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TPRI_OP_KTI_REUT_STRUCT;

/* KTIREUT_PH_TSEC_OP_KTI_REUT_REG supported on:                                */
/*      SPRA0 (0x21e0a218)                                                      */
/*      SPRB0 (0x21e0a218)                                                      */
/*      SPRHBM (0x21e0a218)                                                     */
/*      SPRC0 (0x21e0a218)                                                      */
/*      SPRMCC (0x21e0a218)                                                     */
/*      SPRUCC (0x21e0a218)                                                     */
/* Register default value on SPRA0: 0xA9F0C640                                  */
/* Register default value on SPRB0: 0xA9F0C640                                  */
/* Register default value on SPRHBM: 0xA9F0C640                                 */
/* Register default value on SPRC0: 0xA9F0C640                                  */
/* Register default value on SPRMCC: 0xA9F0C640                                 */
/* Register default value on SPRUCC: 0xA9F0C640                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_tsec_op
*/


#define KTIREUT_PH_TSEC_OP_KTI_REUT_REG 0x09120218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_lmst : 4;

                            /* Bits[3:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               For Tx adaptation in loopback master-master mode
                               non-zero value should be used for fast speed
                               (e.g., value used for Tclst). Tlmst is typically
                               0. For debug, CSingleStep and CFIA (with non-
                               zero Tlmst) can be used. Exponential time value,
                               with granularity of 4096UI
                            */
    UINT32 t_cfst : 4;

                            /* Bits[7:4], Access Type=RW/P/L, default=0x00000004*/

                            /*
                               Wait for planetary aligned clean flit boundary.
                               If determinism is enabled, Tcfst must be greater
                               than EPhyL0Sync period. Exponential time value,
                               with granularity of 4096UI
                            */
    UINT32 t_clst : 4;

                            /* Bits[11:8], Access Type=RW/P/L, default=0x00000006*/

                            /*
                               Configure link widths; exponential time value,
                               with granularity of 4096UI
                            */
    UINT32 t_plst : 4;

                            /* Bits[15:12], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               Adapt+bitlock/bytelock/deskew; exponential time
                               value, with granularity of 4096UI
                            */
    UINT32 t_rdst : 4;

                            /* Bits[19:16], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Remote Rx detect/debounce; 0 or no timeout for
                               hot plug; non-0 value to debug hangs exponential
                               time value, with granularity of 4096UI
                            */
    UINT32 t_tdst : 4;

                            /* Bits[23:20], Access Type=RW/P/L, default=0x0000000F*/

                            /*
                               Squelch exit detect/debounce. This value (in UI)
                               should be larger than Tl0c. Exponential time
                               value, with granularity of 4096UI
                            */
    UINT32 t_tcst : 4;

                            /* Bits[27:24], Access Type=RW/P/L, default=0x00000009*/

                            /*
                               Timed Tx calib, e.g. icomp; exponential time
                               value, with granularity of 4096UI
                            */
    UINT32 t_rcst : 4;

                            /* Bits[31:28], Access Type=RW/P/L, default=0x0000000A*/

                            /*
                               Timed Rx calibration, e.g. oc; exponential time
                               value, with granularity of 4096UI
                            */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TSEC_OP_KTI_REUT_STRUCT;

/* KTIREUT_PH_TL0CENTRY_OP_KTI_REUT_REG supported on:                           */
/*      SPRA0 (0x21e0a220)                                                      */
/*      SPRB0 (0x21e0a220)                                                      */
/*      SPRHBM (0x21e0a220)                                                     */
/*      SPRC0 (0x21e0a220)                                                      */
/*      SPRMCC (0x21e0a220)                                                     */
/*      SPRUCC (0x21e0a220)                                                     */
/* Register default value on SPRA0: 0x00000020                                  */
/* Register default value on SPRB0: 0x00000020                                  */
/* Register default value on SPRHBM: 0x00000020                                 */
/* Register default value on SPRC0: 0x00000020                                  */
/* Register default value on SPRMCC: 0x00000020                                 */
/* Register default value on SPRUCC: 0x00000020                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_tl0centry_op
*/


#define KTIREUT_PH_TL0CENTRY_OP_KTI_REUT_REG 0x09120220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_ieo : 12;

                            /* Bits[11:0], Access Type=RW/P/L, default=0x00000020*/

                            /*
                               Init exit delay; used in debug to delay exit
                               from init; linear time value, with granularity
                               of 8UI
                            */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TL0CENTRY_OP_KTI_REUT_STRUCT;

/* KTIREUT_PH_TL0C_OP_KTI_REUT_REG supported on:                                */
/*      SPRA0 (0x21e0a224)                                                      */
/*      SPRB0 (0x21e0a224)                                                      */
/*      SPRHBM (0x21e0a224)                                                     */
/*      SPRC0 (0x21e0a224)                                                      */
/*      SPRMCC (0x21e0a224)                                                     */
/*      SPRUCC (0x21e0a224)                                                     */
/* Register default value on SPRA0: 0x00010021                                  */
/* Register default value on SPRB0: 0x00010021                                  */
/* Register default value on SPRHBM: 0x00010021                                 */
/* Register default value on SPRC0: 0x00010021                                  */
/* Register default value on SPRMCC: 0x00010021                                 */
/* Register default value on SPRUCC: 0x00010021                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* ktireut_ph_tl0c_op
*/


#define KTIREUT_PH_TL0C_OP_KTI_REUT_REG 0x09120224

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_l0c : 8;

                            /* Bits[7:0], Access Type=RW/P/L, default=0x00000021*/

                            /*
                               L0c clean flit interval. This value is loaded
                               into the local TL0ct and TL0cr timers under PHY
                               LTSSM control. Linear time value, with
                               granularity of 128UI. Can only be legally adjust
                               in steps of 3 to keep alignment to 48UI
                            */
    UINT32 t_l1_reissue_delay : 8;

                            /* Bits[15:8], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               L1 reissue delayed until after timeout (512UI
                               Granularity)
                            */
    UINT32 t_l0cq : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               L0c duration. Value must be 1. Linear time
                               value, with granularity of 8UI Errata: SPEC to
                               RTL Mismatch. Unused register
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIREUT_PH_TL0C_OP_KTI_REUT_STRUCT;

/* KTIPHYPWRCTRL_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a228)                                                      */
/*      SPRB0 (0x21e0a228)                                                      */
/*      SPRHBM (0x21e0a228)                                                     */
/*      SPRC0 (0x21e0a228)                                                      */
/*      SPRMCC (0x21e0a228)                                                     */
/*      SPRUCC (0x21e0a228)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* ktiphypwrctrl
*/


#define KTIPHYPWRCTRL_KTI_REUT_REG 0x09120228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fake_l1 : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Overrides the signal the tells the PCU the link
                               is eligible to enter Pkg-C.
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPHYPWRCTRL_KTI_REUT_STRUCT;

/* UPIPH_OS_W_CP_LO_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a22c)                                                      */
/*      SPRB0 (0x21e0a22c)                                                      */
/*      SPRHBM (0x21e0a22c)                                                     */
/*      SPRC0 (0x21e0a22c)                                                      */
/*      SPRMCC (0x21e0a22c)                                                     */
/*      SPRUCC (0x21e0a22c)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* UPIPH_OS_W_CP LOWER 32bits
*/


#define UPIPH_OS_W_CP_LO_KTI_REUT_REG 0x0912022C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIPH_OS_W_CP_LO_KTI_REUT_STRUCT;

/* UPIPH_OS_W_CP_HI_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a230)                                                      */
/*      SPRB0 (0x21e0a230)                                                      */
/*      SPRHBM (0x21e0a230)                                                     */
/*      SPRC0 (0x21e0a230)                                                      */
/*      SPRMCC (0x21e0a230)                                                     */
/*      SPRUCC (0x21e0a230)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* UPIPH_OS_W_CP UPPER 32bits
*/


#define UPIPH_OS_W_CP_HI_KTI_REUT_REG 0x09120230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIPH_OS_W_CP_HI_KTI_REUT_STRUCT;

/* UPIPH_OS_W_RAC_LO_KTI_REUT_REG supported on:                                 */
/*      SPRA0 (0x21e0a234)                                                      */
/*      SPRB0 (0x21e0a234)                                                      */
/*      SPRHBM (0x21e0a234)                                                     */
/*      SPRC0 (0x21e0a234)                                                      */
/*      SPRMCC (0x21e0a234)                                                     */
/*      SPRUCC (0x21e0a234)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* UPIPH_OS_W_RAC LOWER 32bits
*/


#define UPIPH_OS_W_RAC_LO_KTI_REUT_REG 0x09120234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIPH_OS_W_RAC_LO_KTI_REUT_STRUCT;

/* UPIPH_OS_W_RAC_HI_KTI_REUT_REG supported on:                                 */
/*      SPRA0 (0x21e0a238)                                                      */
/*      SPRB0 (0x21e0a238)                                                      */
/*      SPRHBM (0x21e0a238)                                                     */
/*      SPRC0 (0x21e0a238)                                                      */
/*      SPRMCC (0x21e0a238)                                                     */
/*      SPRUCC (0x21e0a238)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* UPIPH_OS_W_RAC UPPER 32bits
*/


#define UPIPH_OS_W_RAC_HI_KTI_REUT_REG 0x09120238

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIPH_OS_W_RAC_HI_KTI_REUT_STRUCT;

/* UPIPH_OS_W_WAC_LO_KTI_REUT_REG supported on:                                 */
/*      SPRA0 (0x21e0a23c)                                                      */
/*      SPRB0 (0x21e0a23c)                                                      */
/*      SPRHBM (0x21e0a23c)                                                     */
/*      SPRC0 (0x21e0a23c)                                                      */
/*      SPRMCC (0x21e0a23c)                                                     */
/*      SPRUCC (0x21e0a23c)                                                     */
/* Register default value on SPRA0: 0x0300021F                                  */
/* Register default value on SPRB0: 0x0300021F                                  */
/* Register default value on SPRHBM: 0x0300021F                                 */
/* Register default value on SPRC0: 0x0300021F                                  */
/* Register default value on SPRMCC: 0x0300021F                                 */
/* Register default value on SPRUCC: 0x0300021F                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* UPIPH_OS_W_WAC LOWER 32bits
*/


#define UPIPH_OS_W_WAC_LO_KTI_REUT_REG 0x0912023C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIPH_OS_W_WAC_LO_KTI_REUT_STRUCT;

/* UPIPH_OS_W_WAC_HI_KTI_REUT_REG supported on:                                 */
/*      SPRA0 (0x21e0a240)                                                      */
/*      SPRB0 (0x21e0a240)                                                      */
/*      SPRHBM (0x21e0a240)                                                     */
/*      SPRC0 (0x21e0a240)                                                      */
/*      SPRMCC (0x21e0a240)                                                     */
/*      SPRUCC (0x21e0a240)                                                     */
/* Register default value on SPRA0: 0x20000C00                                  */
/* Register default value on SPRB0: 0x20000C00                                  */
/* Register default value on SPRHBM: 0x20000C00                                 */
/* Register default value on SPRC0: 0x20000C00                                  */
/* Register default value on SPRMCC: 0x20000C00                                 */
/* Register default value on SPRUCC: 0x20000C00                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* UPIPH_OS_W_WAC UPPER 32bits
*/


#define UPIPH_OS_W_WAC_HI_KTI_REUT_REG 0x09120240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sai_mask_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_9 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_10 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_24 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */
    UINT32 sai_mask_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Used to determine which agents are allowed write
                               access to OS_W WAC, RAC and CP registers, based
                               on the value from each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} UPIPH_OS_W_WAC_HI_KTI_REUT_STRUCT;













/* UPIPH_DFX_CP_LO_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a28c)                                                      */
/*      SPRB0 (0x21e0a28c)                                                      */
/*      SPRHBM (0x21e0a28c)                                                     */
/*      SPRC0 (0x21e0a28c)                                                      */
/*      SPRMCC (0x21e0a28c)                                                     */
/*      SPRUCC (0x21e0a28c)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* UPIPH_DFX_CP LOWER 32bits
*/


#define UPIPH_DFX_CP_LO_KTI_REUT_REG 0x0912028C

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

} UPIPH_DFX_CP_LO_KTI_REUT_STRUCT;

/* UPIPH_DFX_CP_HI_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a290)                                                      */
/*      SPRB0 (0x21e0a290)                                                      */
/*      SPRHBM (0x21e0a290)                                                     */
/*      SPRC0 (0x21e0a290)                                                      */
/*      SPRMCC (0x21e0a290)                                                     */
/*      SPRUCC (0x21e0a290)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* UPIPH_DFX_CP UPPER 32bits
*/


#define UPIPH_DFX_CP_HI_KTI_REUT_REG 0x09120290

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

} UPIPH_DFX_CP_HI_KTI_REUT_STRUCT;

/* UPIPH_DFX_RAC_LO_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a294)                                                      */
/*      SPRB0 (0x21e0a294)                                                      */
/*      SPRHBM (0x21e0a294)                                                     */
/*      SPRC0 (0x21e0a294)                                                      */
/*      SPRMCC (0x21e0a294)                                                     */
/*      SPRUCC (0x21e0a294)                                                     */
/* Register default value on SPRA0: 0x0100020A                                  */
/* Register default value on SPRB0: 0x0100020A                                  */
/* Register default value on SPRHBM: 0x0100020A                                 */
/* Register default value on SPRC0: 0x0100020A                                  */
/* Register default value on SPRMCC: 0x0100020A                                 */
/* Register default value on SPRUCC: 0x0100020A                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* UPIPH_DFX_RAC LOWER 32bits
*/


#define UPIPH_DFX_RAC_LO_KTI_REUT_REG 0x09120294

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

} UPIPH_DFX_RAC_LO_KTI_REUT_STRUCT;

/* UPIPH_DFX_RAC_HI_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a298)                                                      */
/*      SPRB0 (0x21e0a298)                                                      */
/*      SPRHBM (0x21e0a298)                                                     */
/*      SPRC0 (0x21e0a298)                                                      */
/*      SPRMCC (0x21e0a298)                                                     */
/*      SPRUCC (0x21e0a298)                                                     */
/* Register default value on SPRA0: 0x20000500                                  */
/* Register default value on SPRB0: 0x20000500                                  */
/* Register default value on SPRHBM: 0x20000500                                 */
/* Register default value on SPRC0: 0x20000500                                  */
/* Register default value on SPRMCC: 0x20000500                                 */
/* Register default value on SPRUCC: 0x20000500                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* UPIPH_DFX_RAC UPPER 32bits
*/


#define UPIPH_DFX_RAC_HI_KTI_REUT_REG 0x09120298

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

} UPIPH_DFX_RAC_HI_KTI_REUT_STRUCT;

/* UPIPH_DFX_WAC_LO_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a29c)                                                      */
/*      SPRB0 (0x21e0a29c)                                                      */
/*      SPRHBM (0x21e0a29c)                                                     */
/*      SPRC0 (0x21e0a29c)                                                      */
/*      SPRMCC (0x21e0a29c)                                                     */
/*      SPRUCC (0x21e0a29c)                                                     */
/* Register default value on SPRA0: 0x0100020A                                  */
/* Register default value on SPRB0: 0x0100020A                                  */
/* Register default value on SPRHBM: 0x0100020A                                 */
/* Register default value on SPRC0: 0x0100020A                                  */
/* Register default value on SPRMCC: 0x0100020A                                 */
/* Register default value on SPRUCC: 0x0100020A                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* UPIPH_DFX_WAC LOWER 32bits
*/


#define UPIPH_DFX_WAC_LO_KTI_REUT_REG 0x0912029C

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

} UPIPH_DFX_WAC_LO_KTI_REUT_STRUCT;

/* UPIPH_DFX_WAC_HI_KTI_REUT_REG supported on:                                  */
/*      SPRA0 (0x21e0a2a0)                                                      */
/*      SPRB0 (0x21e0a2a0)                                                      */
/*      SPRHBM (0x21e0a2a0)                                                     */
/*      SPRC0 (0x21e0a2a0)                                                      */
/*      SPRMCC (0x21e0a2a0)                                                     */
/*      SPRUCC (0x21e0a2a0)                                                     */
/* Register default value on SPRA0: 0x20000400                                  */
/* Register default value on SPRB0: 0x20000400                                  */
/* Register default value on SPRHBM: 0x20000400                                 */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRMCC: 0x20000400                                 */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* UPIPH_DFX_WAC UPPER 32bits
*/


#define UPIPH_DFX_WAC_HI_KTI_REUT_REG 0x091202A0

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

} UPIPH_DFX_WAC_HI_KTI_REUT_STRUCT;

/* UPIPH_LB_MCHECK_W_CP_LO_KTI_REUT_REG supported on:                           */
/*      SPRA0 (0x21e0a2a4)                                                      */
/*      SPRB0 (0x21e0a2a4)                                                      */
/*      SPRHBM (0x21e0a2a4)                                                     */
/*      SPRC0 (0x21e0a2a4)                                                      */
/*      SPRMCC (0x21e0a2a4)                                                     */
/*      SPRUCC (0x21e0a2a4)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* UPIPH_LB_MCHECK_W_CP LOWER 32bits
*/


#define UPIPH_LB_MCHECK_W_CP_LO_KTI_REUT_REG 0x091202A4

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

} UPIPH_LB_MCHECK_W_CP_LO_KTI_REUT_STRUCT;

/* UPIPH_LB_MCHECK_W_CP_HI_KTI_REUT_REG supported on:                           */
/*      SPRA0 (0x21e0a2a8)                                                      */
/*      SPRB0 (0x21e0a2a8)                                                      */
/*      SPRHBM (0x21e0a2a8)                                                     */
/*      SPRC0 (0x21e0a2a8)                                                      */
/*      SPRMCC (0x21e0a2a8)                                                     */
/*      SPRUCC (0x21e0a2a8)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* UPIPH_LB_MCHECK_W_CP UPPER 32bits
*/


#define UPIPH_LB_MCHECK_W_CP_HI_KTI_REUT_REG 0x091202A8

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

} UPIPH_LB_MCHECK_W_CP_HI_KTI_REUT_STRUCT;

/* UPIPH_LB_MCHECK_W_RAC_LO_KTI_REUT_REG supported on:                          */
/*      SPRA0 (0x21e0a2ac)                                                      */
/*      SPRB0 (0x21e0a2ac)                                                      */
/*      SPRHBM (0x21e0a2ac)                                                     */
/*      SPRC0 (0x21e0a2ac)                                                      */
/*      SPRMCC (0x21e0a2ac)                                                     */
/*      SPRUCC (0x21e0a2ac)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* UPIPH_LB_MCHECK_W_RAC LOWER 32bits
*/


#define UPIPH_LB_MCHECK_W_RAC_LO_KTI_REUT_REG 0x091202AC

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

} UPIPH_LB_MCHECK_W_RAC_LO_KTI_REUT_STRUCT;

/* UPIPH_LB_MCHECK_W_RAC_HI_KTI_REUT_REG supported on:                          */
/*      SPRA0 (0x21e0a2b0)                                                      */
/*      SPRB0 (0x21e0a2b0)                                                      */
/*      SPRHBM (0x21e0a2b0)                                                     */
/*      SPRC0 (0x21e0a2b0)                                                      */
/*      SPRMCC (0x21e0a2b0)                                                     */
/*      SPRUCC (0x21e0a2b0)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* UPIPH_LB_MCHECK_W_RAC UPPER 32bits
*/


#define UPIPH_LB_MCHECK_W_RAC_HI_KTI_REUT_REG 0x091202B0

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

} UPIPH_LB_MCHECK_W_RAC_HI_KTI_REUT_STRUCT;

/* UPIPH_LB_MCHECK_W_WAC_LO_KTI_REUT_REG supported on:                          */
/*      SPRA0 (0x21e0a2b4)                                                      */
/*      SPRB0 (0x21e0a2b4)                                                      */
/*      SPRHBM (0x21e0a2b4)                                                     */
/*      SPRC0 (0x21e0a2b4)                                                      */
/*      SPRMCC (0x21e0a2b4)                                                     */
/*      SPRUCC (0x21e0a2b4)                                                     */
/* Register default value on SPRA0: 0x0100021E                                  */
/* Register default value on SPRB0: 0x0100021E                                  */
/* Register default value on SPRHBM: 0x0100021E                                 */
/* Register default value on SPRC0: 0x0100021E                                  */
/* Register default value on SPRMCC: 0x0100021E                                 */
/* Register default value on SPRUCC: 0x0100021E                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* UPIPH_LB_MCHECK_W_WAC LOWER 32bits
*/


#define UPIPH_LB_MCHECK_W_WAC_LO_KTI_REUT_REG 0x091202B4

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

} UPIPH_LB_MCHECK_W_WAC_LO_KTI_REUT_STRUCT;

/* UPIPH_LB_MCHECK_W_WAC_HI_KTI_REUT_REG supported on:                          */
/*      SPRA0 (0x21e0a2b8)                                                      */
/*      SPRB0 (0x21e0a2b8)                                                      */
/*      SPRHBM (0x21e0a2b8)                                                     */
/*      SPRC0 (0x21e0a2b8)                                                      */
/*      SPRMCC (0x21e0a2b8)                                                     */
/*      SPRUCC (0x21e0a2b8)                                                     */
/* Register default value on SPRA0: 0x20000400                                  */
/* Register default value on SPRB0: 0x20000400                                  */
/* Register default value on SPRHBM: 0x20000400                                 */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRMCC: 0x20000400                                 */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* UPIPH_LB_MCHECK_W_WAC UPPER 32bits
*/


#define UPIPH_LB_MCHECK_W_WAC_HI_KTI_REUT_REG 0x091202B8

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

} UPIPH_LB_MCHECK_W_WAC_HI_KTI_REUT_STRUCT;









/* PIPE_CLK_RATE_CTRL_KTI_REUT_REG supported on:                                */
/*      SPRA0 (0x21e0a2e0)                                                      */
/*      SPRB0 (0x21e0a2e0)                                                      */
/*      SPRHBM (0x21e0a2e0)                                                     */
/*      SPRC0 (0x21e0a2e0)                                                      */
/*      SPRMCC (0x21e0a2e0)                                                     */
/*      SPRUCC (0x21e0a2e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* reg_agent_slow_mode_1
*/


#define PIPE_CLK_RATE_CTRL_KTI_REUT_REG 0x091202E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cri_freq_select : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               This CSR is used to select "pipe_rate" and
                               "pipe_pclkrate" encoding based on frequency of
                               operation of link. Its encoding is as below
                               3'b000: slow mode i.e. Link speed 2.5 GT/s.
                               logphy freq = 125 Mhz 3'b001: Link speed 12.8
                               GT/s. logphy freq = 800 Mhz 3'b010: Link speed
                               14.4 GT/s logphy freq = 900 Mhz 3'b011: Link
                               speed 16 GT/s logphy freq = 1 Ghz
                            */
    UINT32 cri_pclkchangeok_flag : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit will be set and reset by HW and FW will
                               use it for GV flow during speed change 1 HW will
                               set this bit when pclkchangeok asserted by HSPHY
                               for all lanes 2 HW will clear this bit when FW
                               assert PhyInit Begin bit
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PIPE_CLK_RATE_CTRL_KTI_REUT_STRUCT;

/* PH_FOM_LANE0_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a2e4)                                                      */
/*      SPRB0 (0x21e0a2e4)                                                      */
/*      SPRHBM (0x21e0a2e4)                                                     */
/*      SPRC0 (0x21e0a2e4)                                                      */
/*      SPRMCC (0x21e0a2e4)                                                     */
/*      SPRUCC (0x21e0a2e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane0
*/


#define PH_FOM_LANE0_KTI_REUT_REG 0x091202E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE0_KTI_REUT_STRUCT;

/* PH_FOM_LANE1_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a2e8)                                                      */
/*      SPRB0 (0x21e0a2e8)                                                      */
/*      SPRHBM (0x21e0a2e8)                                                     */
/*      SPRC0 (0x21e0a2e8)                                                      */
/*      SPRMCC (0x21e0a2e8)                                                     */
/*      SPRUCC (0x21e0a2e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane1
*/


#define PH_FOM_LANE1_KTI_REUT_REG 0x091202E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE1_KTI_REUT_STRUCT;

/* PH_FOM_LANE2_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a2ec)                                                      */
/*      SPRB0 (0x21e0a2ec)                                                      */
/*      SPRHBM (0x21e0a2ec)                                                     */
/*      SPRC0 (0x21e0a2ec)                                                      */
/*      SPRMCC (0x21e0a2ec)                                                     */
/*      SPRUCC (0x21e0a2ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane2
*/


#define PH_FOM_LANE2_KTI_REUT_REG 0x091202EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE2_KTI_REUT_STRUCT;

/* PH_FOM_LANE3_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a2f0)                                                      */
/*      SPRB0 (0x21e0a2f0)                                                      */
/*      SPRHBM (0x21e0a2f0)                                                     */
/*      SPRC0 (0x21e0a2f0)                                                      */
/*      SPRMCC (0x21e0a2f0)                                                     */
/*      SPRUCC (0x21e0a2f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane3
*/


#define PH_FOM_LANE3_KTI_REUT_REG 0x091202F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE3_KTI_REUT_STRUCT;

/* PH_FOM_LANE4_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a2f4)                                                      */
/*      SPRB0 (0x21e0a2f4)                                                      */
/*      SPRHBM (0x21e0a2f4)                                                     */
/*      SPRC0 (0x21e0a2f4)                                                      */
/*      SPRMCC (0x21e0a2f4)                                                     */
/*      SPRUCC (0x21e0a2f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane4
*/


#define PH_FOM_LANE4_KTI_REUT_REG 0x091202F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE4_KTI_REUT_STRUCT;

/* PH_FOM_LANE5_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a2f8)                                                      */
/*      SPRB0 (0x21e0a2f8)                                                      */
/*      SPRHBM (0x21e0a2f8)                                                     */
/*      SPRC0 (0x21e0a2f8)                                                      */
/*      SPRMCC (0x21e0a2f8)                                                     */
/*      SPRUCC (0x21e0a2f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane5
*/


#define PH_FOM_LANE5_KTI_REUT_REG 0x091202F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE5_KTI_REUT_STRUCT;

/* PH_FOM_LANE6_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a2fc)                                                      */
/*      SPRB0 (0x21e0a2fc)                                                      */
/*      SPRHBM (0x21e0a2fc)                                                     */
/*      SPRC0 (0x21e0a2fc)                                                      */
/*      SPRMCC (0x21e0a2fc)                                                     */
/*      SPRUCC (0x21e0a2fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane6
*/


#define PH_FOM_LANE6_KTI_REUT_REG 0x091202FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE6_KTI_REUT_STRUCT;

/* PH_FOM_LANE7_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a300)                                                      */
/*      SPRB0 (0x21e0a300)                                                      */
/*      SPRHBM (0x21e0a300)                                                     */
/*      SPRC0 (0x21e0a300)                                                      */
/*      SPRMCC (0x21e0a300)                                                     */
/*      SPRUCC (0x21e0a300)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane7
*/


#define PH_FOM_LANE7_KTI_REUT_REG 0x09120300

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE7_KTI_REUT_STRUCT;

/* PH_FOM_LANE8_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a304)                                                      */
/*      SPRB0 (0x21e0a304)                                                      */
/*      SPRHBM (0x21e0a304)                                                     */
/*      SPRC0 (0x21e0a304)                                                      */
/*      SPRMCC (0x21e0a304)                                                     */
/*      SPRUCC (0x21e0a304)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane8
*/


#define PH_FOM_LANE8_KTI_REUT_REG 0x09120304

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE8_KTI_REUT_STRUCT;

/* PH_FOM_LANE9_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a308)                                                      */
/*      SPRB0 (0x21e0a308)                                                      */
/*      SPRHBM (0x21e0a308)                                                     */
/*      SPRC0 (0x21e0a308)                                                      */
/*      SPRMCC (0x21e0a308)                                                     */
/*      SPRUCC (0x21e0a308)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane9
*/


#define PH_FOM_LANE9_KTI_REUT_REG 0x09120308

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE9_KTI_REUT_STRUCT;

/* PH_FOM_LANE10_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a30c)                                                      */
/*      SPRB0 (0x21e0a30c)                                                      */
/*      SPRHBM (0x21e0a30c)                                                     */
/*      SPRC0 (0x21e0a30c)                                                      */
/*      SPRMCC (0x21e0a30c)                                                     */
/*      SPRUCC (0x21e0a30c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane10
*/


#define PH_FOM_LANE10_KTI_REUT_REG 0x0912030C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE10_KTI_REUT_STRUCT;

/* PH_FOM_LANE11_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a310)                                                      */
/*      SPRB0 (0x21e0a310)                                                      */
/*      SPRHBM (0x21e0a310)                                                     */
/*      SPRC0 (0x21e0a310)                                                      */
/*      SPRMCC (0x21e0a310)                                                     */
/*      SPRUCC (0x21e0a310)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane11
*/


#define PH_FOM_LANE11_KTI_REUT_REG 0x09120310

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE11_KTI_REUT_STRUCT;

/* PH_FOM_LANE12_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a314)                                                      */
/*      SPRB0 (0x21e0a314)                                                      */
/*      SPRHBM (0x21e0a314)                                                     */
/*      SPRC0 (0x21e0a314)                                                      */
/*      SPRMCC (0x21e0a314)                                                     */
/*      SPRUCC (0x21e0a314)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane12
*/


#define PH_FOM_LANE12_KTI_REUT_REG 0x09120314

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE12_KTI_REUT_STRUCT;

/* PH_FOM_LANE13_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a318)                                                      */
/*      SPRB0 (0x21e0a318)                                                      */
/*      SPRHBM (0x21e0a318)                                                     */
/*      SPRC0 (0x21e0a318)                                                      */
/*      SPRMCC (0x21e0a318)                                                     */
/*      SPRUCC (0x21e0a318)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane13
*/


#define PH_FOM_LANE13_KTI_REUT_REG 0x09120318

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE13_KTI_REUT_STRUCT;

/* PH_FOM_LANE14_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a31c)                                                      */
/*      SPRB0 (0x21e0a31c)                                                      */
/*      SPRHBM (0x21e0a31c)                                                     */
/*      SPRC0 (0x21e0a31c)                                                      */
/*      SPRMCC (0x21e0a31c)                                                     */
/*      SPRUCC (0x21e0a31c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane14
*/


#define PH_FOM_LANE14_KTI_REUT_REG 0x0912031C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE14_KTI_REUT_STRUCT;

/* PH_FOM_LANE15_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a320)                                                      */
/*      SPRB0 (0x21e0a320)                                                      */
/*      SPRHBM (0x21e0a320)                                                     */
/*      SPRC0 (0x21e0a320)                                                      */
/*      SPRMCC (0x21e0a320)                                                     */
/*      SPRUCC (0x21e0a320)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane15
*/


#define PH_FOM_LANE15_KTI_REUT_REG 0x09120320

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE15_KTI_REUT_STRUCT;

/* PH_FOM_LANE16_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a324)                                                      */
/*      SPRB0 (0x21e0a324)                                                      */
/*      SPRHBM (0x21e0a324)                                                     */
/*      SPRC0 (0x21e0a324)                                                      */
/*      SPRMCC (0x21e0a324)                                                     */
/*      SPRUCC (0x21e0a324)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane16
*/


#define PH_FOM_LANE16_KTI_REUT_REG 0x09120324

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE16_KTI_REUT_STRUCT;

/* PH_FOM_LANE17_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a328)                                                      */
/*      SPRB0 (0x21e0a328)                                                      */
/*      SPRHBM (0x21e0a328)                                                     */
/*      SPRC0 (0x21e0a328)                                                      */
/*      SPRMCC (0x21e0a328)                                                     */
/*      SPRUCC (0x21e0a328)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane17
*/


#define PH_FOM_LANE17_KTI_REUT_REG 0x09120328

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE17_KTI_REUT_STRUCT;

/* PH_FOM_LANE18_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a32c)                                                      */
/*      SPRB0 (0x21e0a32c)                                                      */
/*      SPRHBM (0x21e0a32c)                                                     */
/*      SPRC0 (0x21e0a32c)                                                      */
/*      SPRMCC (0x21e0a32c)                                                     */
/*      SPRUCC (0x21e0a32c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane0
*/


#define PH_FOM_LANE18_KTI_REUT_REG 0x0912032C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE18_KTI_REUT_STRUCT;

/* PH_FOM_LANE19_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a330)                                                      */
/*      SPRB0 (0x21e0a330)                                                      */
/*      SPRHBM (0x21e0a330)                                                     */
/*      SPRC0 (0x21e0a330)                                                      */
/*      SPRMCC (0x21e0a330)                                                     */
/*      SPRUCC (0x21e0a330)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane19
*/


#define PH_FOM_LANE19_KTI_REUT_REG 0x09120330

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE19_KTI_REUT_STRUCT;

/* PH_FOM_LANE20_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a334)                                                      */
/*      SPRB0 (0x21e0a334)                                                      */
/*      SPRHBM (0x21e0a334)                                                     */
/*      SPRC0 (0x21e0a334)                                                      */
/*      SPRMCC (0x21e0a334)                                                     */
/*      SPRUCC (0x21e0a334)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane20
*/


#define PH_FOM_LANE20_KTI_REUT_REG 0x09120334

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE20_KTI_REUT_STRUCT;

/* PH_FOM_LANE21_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a338)                                                      */
/*      SPRB0 (0x21e0a338)                                                      */
/*      SPRHBM (0x21e0a338)                                                     */
/*      SPRC0 (0x21e0a338)                                                      */
/*      SPRMCC (0x21e0a338)                                                     */
/*      SPRUCC (0x21e0a338)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane0
*/


#define PH_FOM_LANE21_KTI_REUT_REG 0x09120338

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE21_KTI_REUT_STRUCT;

/* PH_FOM_LANE22_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a33c)                                                      */
/*      SPRB0 (0x21e0a33c)                                                      */
/*      SPRHBM (0x21e0a33c)                                                     */
/*      SPRC0 (0x21e0a33c)                                                      */
/*      SPRMCC (0x21e0a33c)                                                     */
/*      SPRUCC (0x21e0a33c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane22
*/


#define PH_FOM_LANE22_KTI_REUT_REG 0x0912033C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE22_KTI_REUT_STRUCT;

/* PH_FOM_LANE23_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a340)                                                      */
/*      SPRB0 (0x21e0a340)                                                      */
/*      SPRHBM (0x21e0a340)                                                     */
/*      SPRC0 (0x21e0a340)                                                      */
/*      SPRMCC (0x21e0a340)                                                     */
/*      SPRUCC (0x21e0a340)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_fom_lane23
*/


#define PH_FOM_LANE23_KTI_REUT_REG 0x09120340

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sleffm : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               sleffm Link Evaluation Feedback Figure of Merit.
                               This 8 bit register is as defined in PIPE spec
                               RX Link Evaluation Status0
                            */
    UINT32 rsvd : 23;

                            /* Bits[30:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sleffmvalid : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               When set indicates that sleffm field is valid
                               and stores the Figure of Merit Value returned by
                               the physical layer. Cleared by hardware.
                            */

  } Bits;
  UINT32 Data;

} PH_FOM_LANE23_KTI_REUT_STRUCT;

/* PH_TCL_LANE0_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a344)                                                      */
/*      SPRB0 (0x21e0a344)                                                      */
/*      SPRHBM (0x21e0a344)                                                     */
/*      SPRC0 (0x21e0a344)                                                      */
/*      SPRMCC (0x21e0a344)                                                     */
/*      SPRUCC (0x21e0a344)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE0_KTI_REUT_REG 0x09120344

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE0_KTI_REUT_STRUCT;

/* PH_TCL_LANE1_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a348)                                                      */
/*      SPRB0 (0x21e0a348)                                                      */
/*      SPRHBM (0x21e0a348)                                                     */
/*      SPRC0 (0x21e0a348)                                                      */
/*      SPRMCC (0x21e0a348)                                                     */
/*      SPRUCC (0x21e0a348)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE1_KTI_REUT_REG 0x09120348

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE1_KTI_REUT_STRUCT;

/* PH_TCL_LANE2_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a34c)                                                      */
/*      SPRB0 (0x21e0a34c)                                                      */
/*      SPRHBM (0x21e0a34c)                                                     */
/*      SPRC0 (0x21e0a34c)                                                      */
/*      SPRMCC (0x21e0a34c)                                                     */
/*      SPRUCC (0x21e0a34c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE2_KTI_REUT_REG 0x0912034C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE2_KTI_REUT_STRUCT;

/* PH_TCL_LANE3_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a350)                                                      */
/*      SPRB0 (0x21e0a350)                                                      */
/*      SPRHBM (0x21e0a350)                                                     */
/*      SPRC0 (0x21e0a350)                                                      */
/*      SPRMCC (0x21e0a350)                                                     */
/*      SPRUCC (0x21e0a350)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE3_KTI_REUT_REG 0x09120350

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE3_KTI_REUT_STRUCT;

/* PH_TCL_LANE5_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a354)                                                      */
/*      SPRB0 (0x21e0a354)                                                      */
/*      SPRHBM (0x21e0a354)                                                     */
/*      SPRC0 (0x21e0a354)                                                      */
/*      SPRMCC (0x21e0a354)                                                     */
/*      SPRUCC (0x21e0a354)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE5_KTI_REUT_REG 0x09120354

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE5_KTI_REUT_STRUCT;

/* PH_TCL_LANE6_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a358)                                                      */
/*      SPRB0 (0x21e0a358)                                                      */
/*      SPRHBM (0x21e0a358)                                                     */
/*      SPRC0 (0x21e0a358)                                                      */
/*      SPRMCC (0x21e0a358)                                                     */
/*      SPRUCC (0x21e0a358)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE6_KTI_REUT_REG 0x09120358

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE6_KTI_REUT_STRUCT;

/* PH_TCL_LANE7_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a35c)                                                      */
/*      SPRB0 (0x21e0a35c)                                                      */
/*      SPRHBM (0x21e0a35c)                                                     */
/*      SPRC0 (0x21e0a35c)                                                      */
/*      SPRMCC (0x21e0a35c)                                                     */
/*      SPRUCC (0x21e0a35c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE7_KTI_REUT_REG 0x0912035C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE7_KTI_REUT_STRUCT;

/* PH_TCL_LANE8_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a360)                                                      */
/*      SPRB0 (0x21e0a360)                                                      */
/*      SPRHBM (0x21e0a360)                                                     */
/*      SPRC0 (0x21e0a360)                                                      */
/*      SPRMCC (0x21e0a360)                                                     */
/*      SPRUCC (0x21e0a360)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE8_KTI_REUT_REG 0x09120360

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE8_KTI_REUT_STRUCT;

/* PH_TCL_LANE9_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a364)                                                      */
/*      SPRB0 (0x21e0a364)                                                      */
/*      SPRHBM (0x21e0a364)                                                     */
/*      SPRC0 (0x21e0a364)                                                      */
/*      SPRMCC (0x21e0a364)                                                     */
/*      SPRUCC (0x21e0a364)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE9_KTI_REUT_REG 0x09120364

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE9_KTI_REUT_STRUCT;

/* PH_TCL_LANE10_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a368)                                                      */
/*      SPRB0 (0x21e0a368)                                                      */
/*      SPRHBM (0x21e0a368)                                                     */
/*      SPRC0 (0x21e0a368)                                                      */
/*      SPRMCC (0x21e0a368)                                                     */
/*      SPRUCC (0x21e0a368)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE10_KTI_REUT_REG 0x09120368

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE10_KTI_REUT_STRUCT;

/* PH_TCL_LANE11_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a36c)                                                      */
/*      SPRB0 (0x21e0a36c)                                                      */
/*      SPRHBM (0x21e0a36c)                                                     */
/*      SPRC0 (0x21e0a36c)                                                      */
/*      SPRMCC (0x21e0a36c)                                                     */
/*      SPRUCC (0x21e0a36c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE11_KTI_REUT_REG 0x0912036C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE11_KTI_REUT_STRUCT;

/* PH_TCL_LANE12_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a370)                                                      */
/*      SPRB0 (0x21e0a370)                                                      */
/*      SPRHBM (0x21e0a370)                                                     */
/*      SPRC0 (0x21e0a370)                                                      */
/*      SPRMCC (0x21e0a370)                                                     */
/*      SPRUCC (0x21e0a370)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE12_KTI_REUT_REG 0x09120370

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE12_KTI_REUT_STRUCT;

/* PH_TCL_LANE13_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a374)                                                      */
/*      SPRB0 (0x21e0a374)                                                      */
/*      SPRHBM (0x21e0a374)                                                     */
/*      SPRC0 (0x21e0a374)                                                      */
/*      SPRMCC (0x21e0a374)                                                     */
/*      SPRUCC (0x21e0a374)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE13_KTI_REUT_REG 0x09120374

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE13_KTI_REUT_STRUCT;

/* PH_TCL_LANE14_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a378)                                                      */
/*      SPRB0 (0x21e0a378)                                                      */
/*      SPRHBM (0x21e0a378)                                                     */
/*      SPRC0 (0x21e0a378)                                                      */
/*      SPRMCC (0x21e0a378)                                                     */
/*      SPRUCC (0x21e0a378)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE14_KTI_REUT_REG 0x09120378

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE14_KTI_REUT_STRUCT;

/* PH_TCL_LANE15_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a37c)                                                      */
/*      SPRB0 (0x21e0a37c)                                                      */
/*      SPRHBM (0x21e0a37c)                                                     */
/*      SPRC0 (0x21e0a37c)                                                      */
/*      SPRMCC (0x21e0a37c)                                                     */
/*      SPRUCC (0x21e0a37c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE15_KTI_REUT_REG 0x0912037C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE15_KTI_REUT_STRUCT;

/* PH_TCL_LANE16_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a380)                                                      */
/*      SPRB0 (0x21e0a380)                                                      */
/*      SPRHBM (0x21e0a380)                                                     */
/*      SPRC0 (0x21e0a380)                                                      */
/*      SPRMCC (0x21e0a380)                                                     */
/*      SPRUCC (0x21e0a380)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE16_KTI_REUT_REG 0x09120380

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE16_KTI_REUT_STRUCT;

/* PH_TCL_LANE17_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a384)                                                      */
/*      SPRB0 (0x21e0a384)                                                      */
/*      SPRHBM (0x21e0a384)                                                     */
/*      SPRC0 (0x21e0a384)                                                      */
/*      SPRMCC (0x21e0a384)                                                     */
/*      SPRUCC (0x21e0a384)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE17_KTI_REUT_REG 0x09120384

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE17_KTI_REUT_STRUCT;

/* PH_TCL_LANE4_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a388)                                                      */
/*      SPRB0 (0x21e0a388)                                                      */
/*      SPRHBM (0x21e0a388)                                                     */
/*      SPRC0 (0x21e0a388)                                                      */
/*      SPRMCC (0x21e0a388)                                                     */
/*      SPRUCC (0x21e0a388)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE4_KTI_REUT_REG 0x09120388

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE4_KTI_REUT_STRUCT;

/* PH_TCL_LANE18_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a38c)                                                      */
/*      SPRB0 (0x21e0a38c)                                                      */
/*      SPRHBM (0x21e0a38c)                                                     */
/*      SPRC0 (0x21e0a38c)                                                      */
/*      SPRMCC (0x21e0a38c)                                                     */
/*      SPRUCC (0x21e0a38c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE18_KTI_REUT_REG 0x0912038C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE18_KTI_REUT_STRUCT;

/* PH_TCL_LANE19_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a390)                                                      */
/*      SPRB0 (0x21e0a390)                                                      */
/*      SPRHBM (0x21e0a390)                                                     */
/*      SPRC0 (0x21e0a390)                                                      */
/*      SPRMCC (0x21e0a390)                                                     */
/*      SPRUCC (0x21e0a390)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE19_KTI_REUT_REG 0x09120390

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE19_KTI_REUT_STRUCT;

/* PH_TCL_LANE20_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a394)                                                      */
/*      SPRB0 (0x21e0a394)                                                      */
/*      SPRHBM (0x21e0a394)                                                     */
/*      SPRC0 (0x21e0a394)                                                      */
/*      SPRMCC (0x21e0a394)                                                     */
/*      SPRUCC (0x21e0a394)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE20_KTI_REUT_REG 0x09120394

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE20_KTI_REUT_STRUCT;

/* PH_TCL_LANE21_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a398)                                                      */
/*      SPRB0 (0x21e0a398)                                                      */
/*      SPRHBM (0x21e0a398)                                                     */
/*      SPRC0 (0x21e0a398)                                                      */
/*      SPRMCC (0x21e0a398)                                                     */
/*      SPRUCC (0x21e0a398)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE21_KTI_REUT_REG 0x09120398

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE21_KTI_REUT_STRUCT;

/* PH_TCL_LANE22_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a39c)                                                      */
/*      SPRB0 (0x21e0a39c)                                                      */
/*      SPRHBM (0x21e0a39c)                                                     */
/*      SPRC0 (0x21e0a39c)                                                      */
/*      SPRMCC (0x21e0a39c)                                                     */
/*      SPRUCC (0x21e0a39c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE22_KTI_REUT_REG 0x0912039C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE22_KTI_REUT_STRUCT;

/* PH_TCL_LANE23_KTI_REUT_REG supported on:                                     */
/*      SPRA0 (0x21e0a3a0)                                                      */
/*      SPRB0 (0x21e0a3a0)                                                      */
/*      SPRHBM (0x21e0a3a0)                                                     */
/*      SPRC0 (0x21e0a3a0)                                                      */
/*      SPRMCC (0x21e0a3a0)                                                     */
/*      SPRUCC (0x21e0a3a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient  per lane
*/


#define PH_TCL_LANE23_KTI_REUT_REG 0x091203A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficientperlane : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use on lanes
                               addressed by CLaneAddress.When CTCL Valid is
                               asserted the TxCoefficient values are applied
                               and the values are maintained for that lane
                               until further updated
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctclvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that the Tx coefficient value
                               must be applied to the lane. Cleared by hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCL_LANE23_KTI_REUT_STRUCT;

/* PH_TCA_KTI_REUT_REG supported on:                                            */
/*      SPRA0 (0x21e0a3a4)                                                      */
/*      SPRB0 (0x21e0a3a4)                                                      */
/*      SPRHBM (0x21e0a3a4)                                                     */
/*      SPRC0 (0x21e0a3a4)                                                      */
/*      SPRMCC (0x21e0a3a4)                                                     */
/*      SPRUCC (0x21e0a3a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient common adaptation
*/


#define PH_TCA_KTI_REUT_REG 0x091203A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficients : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use across all
                               lanes. When CTXCCValid is asserted the
                               TxCoefficient values are applied to all lanes
                               and the values are maintained for all lanes
                               unless specified by per lane coefficient
                               register.
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctxcavalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that Tx Coefficient values
                               programmed in CTxCoeffVal are valid. Cleared by
                               hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCA_KTI_REUT_STRUCT;

/* PH_TCC_KTI_REUT_REG supported on:                                            */
/*      SPRA0 (0x21e0a3a8)                                                      */
/*      SPRB0 (0x21e0a3a8)                                                      */
/*      SPRHBM (0x21e0a3a8)                                                     */
/*      SPRC0 (0x21e0a3a8)                                                      */
/*      SPRMCC (0x21e0a3a8)                                                     */
/*      SPRUCC (0x21e0a3a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Tx Coefficient common
*/


#define PH_TCC_KTI_REUT_REG 0x091203A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctxcoefficients : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Preset Coefficient values to use across all
                               lanes. When CTXCCValid is asserted the
                               TxCoefficient values are applied to all lanes
                               and the values are maintained for all lanes
                               unless specified by per lane coefficient
                               register.
                            */
    UINT32 rsvd : 13;

                            /* Bits[30:18], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 ctxccvalid : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set indicates that Tx Coefficient values
                               programmed in CTxCoeffVal are valid. Cleared by
                               hardware
                            */

  } Bits;
  UINT32 Data;

} PH_TCC_KTI_REUT_STRUCT;

/* PH_LFFS_KTI_REUT_REG supported on:                                           */
/*      SPRA0 (0x21e0a3ac)                                                      */
/*      SPRB0 (0x21e0a3ac)                                                      */
/*      SPRHBM (0x21e0a3ac)                                                     */
/*      SPRC0 (0x21e0a3ac)                                                      */
/*      SPRMCC (0x21e0a3ac)                                                     */
/*      SPRUCC (0x21e0a3ac)                                                     */
/* Register default value on SPRA0: 0x00003010                                  */
/* Register default value on SPRB0: 0x00003010                                  */
/* Register default value on SPRHBM: 0x00003010                                 */
/* Register default value on SPRC0: 0x00003010                                  */
/* Register default value on SPRMCC: 0x00003010                                 */
/* Register default value on SPRUCC: 0x00003010                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* LF, FS and Remote LF, FS register
*/


#define PH_LFFS_KTI_REUT_REG 0x091203AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cllf : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000010*/

                            /*
                               Reports the local LF Value for the PHY. Refer to
                               the PIPE specification for register details
                            */
    UINT32 clfs : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000030*/

                            /*
                               Reports the local FS Value for the PHY. Refer to
                               the PIPE specification for register details
                            */
    UINT32 crlf : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               Reports the remote LF Value for the PHY. Refer
                               to the PIPE specification for register details
                            */
    UINT32 crfs : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               Reports the remote FS Value for the PHY. Refer
                               to the PIPE specification for register details
                            */

  } Bits;
  UINT32 Data;

} PH_LFFS_KTI_REUT_STRUCT;

/* PH_LTPC_KTI_REUT_REG supported on:                                           */
/*      SPRA0 (0x21e0a3b0)                                                      */
/*      SPRB0 (0x21e0a3b0)                                                      */
/*      SPRHBM (0x21e0a3b0)                                                     */
/*      SPRC0 (0x21e0a3b0)                                                      */
/*      SPRMCC (0x21e0a3b0)                                                     */
/*      SPRUCC (0x21e0a3b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Local Tx Preset Coefficient register
*/


#define PH_LTPC_KTI_REUT_REG 0x091203B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sltpc : 18;

                            /* Bits[17:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Reports the Local Tx Preset Coefficient value
                               for PHY
                            */
    UINT32 sltpcvalid : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Local Tx Preset Coefficients are Valid */
    UINT32 sltpcrdreq : 1;

                            /* Bits[19:19], Access Type=RW/V, default=0x00000000*/

                            /*
                               Read Request Command for local Tx Preset
                               Coefficients
                            */
    UINT32 sltpcpreset : 6;

                            /* Bits[25:20], Access Type=RW/P, default=0x00000000*/

                            /* Preset Value for sltpc Read Requset Command */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_LTPC_KTI_REUT_STRUCT;

/* PH_L1BRES_SL_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a3b4)                                                      */
/*      SPRB0 (0x21e0a3b4)                                                      */
/*      SPRHBM (0x21e0a3b4)                                                     */
/*      SPRC0 (0x21e0a3b4)                                                      */
/*      SPRMCC (0x21e0a3b4)                                                     */
/*      SPRUCC (0x21e0a3b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* residency exponential timer  for SL
*/


#define PH_L1BRES_SL_KTI_REUT_REG 0x091203B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_tl1bres_sl : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               L1b Residency exponential timer value for slow
                               mode
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_L1BRES_SL_KTI_REUT_STRUCT;

/* UPI_PHY_MISC_CTRL_KTI_REUT_REG supported on:                                 */
/*      SPRA0 (0x21e0a3b8)                                                      */
/*      SPRB0 (0x21e0a3b8)                                                      */
/*      SPRHBM (0x21e0a3b8)                                                     */
/*      SPRC0 (0x21e0a3b8)                                                      */
/*      SPRMCC (0x21e0a3b8)                                                     */
/*      SPRUCC (0x21e0a3b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Misc PHY control register
*/


#define UPI_PHY_MISC_CTRL_KTI_REUT_REG 0x091203B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 x8_lane_select : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               This CSR is used to select x8 lane for a
                               failover scenario. The encoding is as below: 0 :
                               Use lanes 0-7 1 : Use lanes 23:16
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UPI_PHY_MISC_CTRL_KTI_REUT_STRUCT;

/* PH_L1BRES_OP_KTI_REUT_REG supported on:                                      */
/*      SPRA0 (0x21e0a3bc)                                                      */
/*      SPRB0 (0x21e0a3bc)                                                      */
/*      SPRHBM (0x21e0a3bc)                                                     */
/*      SPRC0 (0x21e0a3bc)                                                      */
/*      SPRMCC (0x21e0a3bc)                                                     */
/*      SPRUCC (0x21e0a3bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* residency exponential timer value for OP
*/


#define PH_L1BRES_OP_KTI_REUT_REG 0x091203BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_tl1bres_op : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* L1b Residency exponential timer value fro OP */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_L1BRES_OP_KTI_REUT_STRUCT;

/* PH_LTSSM_LOGGER_CTRL_KTI_REUT_REG supported on:                              */
/*      SPRA0 (0x21e0a3c0)                                                      */
/*      SPRB0 (0x21e0a3c0)                                                      */
/*      SPRHBM (0x21e0a3c0)                                                     */
/*      SPRC0 (0x21e0a3c0)                                                      */
/*      SPRMCC (0x21e0a3c0)                                                     */
/*      SPRUCC (0x21e0a3c0)                                                     */
/* Register default value on SPRA0: 0x28000000                                  */
/* Register default value on SPRB0: 0x28000000                                  */
/* Register default value on SPRHBM: 0x28000000                                 */
/* Register default value on SPRC0: 0x28000000                                  */
/* Register default value on SPRMCC: 0x28000000                                 */
/* Register default value on SPRUCC: 0x28000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Ltssm logger control signals
[2:0] State select
[8:3] Arc Select
[9:9] Start logging
[10:10] Divite the clock by 2
[16:11] Read Index
[24:17] Mask L1 State
[25:25] Bypass the stop logic for state transition
[26:26] Indicates that the logger has stopped
*/


#define PH_LTSSM_LOGGER_CTRL_KTI_REUT_REG 0x091203C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 csr_ltssm_state_sel : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /* state selector for ltssm */
    UINT32 csr_ltssm_arc_sel : 6;

                            /* Bits[10:5], Access Type=RW/P, default=0x00000000*/

                            /* arc selector for ltssm */
    UINT32 csr_ltssm_start : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /* Start Logging */
    UINT32 csr_ltssm_enable_clkdiv2 : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* Enable clock division by 2 */
    UINT32 csr_ltssm_rd_index : 6;

                            /* Bits[18:13], Access Type=RW/P, default=0x00000000*/

                            /* set the read index */
    UINT32 csr_ltssm_mask_lt_states : 8;

                            /* Bits[26:19], Access Type=RW/P, default=0x00000000*/

                            /* program it to mask the states for start logic */
    UINT32 csr_ltssm_stoplog_bypass : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000001*/

                            /*
                               Bypass the state transition logic to stop
                               logging, by default this is high
                            */
    UINT32 csr_ltssm_cto_event : 1;

                            /* Bits[28:28], Access Type=RO/V/P, default=0x00000000*/

                            /* Denotes that logger has stopped logging */
    UINT32 csr_ltssm_logger_en : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000001*/

                            /* Disable the logging feature */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_LTSSM_LOGGER_CTRL_KTI_REUT_STRUCT;

/* PH_LTSSM_LOGGER_STATE_SEL_MASK_KTI_REUT_REG supported on:                    */
/*      SPRA0 (0x21e0a3c4)                                                      */
/*      SPRB0 (0x21e0a3c4)                                                      */
/*      SPRHBM (0x21e0a3c4)                                                     */
/*      SPRC0 (0x21e0a3c4)                                                      */
/*      SPRMCC (0x21e0a3c4)                                                     */
/*      SPRUCC (0x21e0a3c4)                                                     */
/* Register default value on SPRA0: 0x00003FFF                                  */
/* Register default value on SPRB0: 0x00003FFF                                  */
/* Register default value on SPRHBM: 0x00003FFF                                 */
/* Register default value on SPRC0: 0x00003FFF                                  */
/* Register default value on SPRMCC: 0x00003FFF                                 */
/* Register default value on SPRUCC: 0x00003FFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Mask for the ltssm state sel sig
*/


#define PH_LTSSM_LOGGER_STATE_SEL_MASK_KTI_REUT_REG 0x091203C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 csr_ltssm_mask : 14;

                            /* Bits[13:0], Access Type=RW, default=0x00003FFF*/

                            /*
                               Mask the ltssm select signal, by default is does
                               not mask.
                            */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_LTSSM_LOGGER_STATE_SEL_MASK_KTI_REUT_STRUCT;

/* PH_LTSSM_LOGGER_STATE_TRANSITION_KTI_REUT_REG supported on:                  */
/*      SPRA0 (0x21e0a3c8)                                                      */
/*      SPRB0 (0x21e0a3c8)                                                      */
/*      SPRHBM (0x21e0a3c8)                                                     */
/*      SPRC0 (0x21e0a3c8)                                                      */
/*      SPRMCC (0x21e0a3c8)                                                     */
/*      SPRUCC (0x21e0a3c8)                                                     */
/* Register default value on SPRA0: 0x00038000                                  */
/* Register default value on SPRB0: 0x00038000                                  */
/* Register default value on SPRHBM: 0x00038000                                 */
/* Register default value on SPRC0: 0x00038000                                  */
/* Register default value on SPRMCC: 0x00038000                                 */
/* Register default value on SPRUCC: 0x00038000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Program the current state and next state. Whenever ltssm transitions from current state to next state, logging stops
Current state: csr_ltssm_cur_state
Next state: csr_ltssm_nxt_state
*/


#define PH_LTSSM_LOGGER_STATE_TRANSITION_KTI_REUT_REG 0x091203C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 csr_ltssm_cur_state : 14;

                            /* Bits[13:0], Access Type=RW/P, default=0x00000000*/

                            /* Program the current state */
    UINT32 csr_ltssm_nxt_state : 14;

                            /* Bits[27:14], Access Type=RW/P, default=0x0000000E*/

                            /* Program the next state */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_LTSSM_LOGGER_STATE_TRANSITION_KTI_REUT_STRUCT;

/* PH_LTSSM_LOGGER_ENTRY_LO_KTI_REUT_REG supported on:                          */
/*      SPRA0 (0x21e0a3cc)                                                      */
/*      SPRB0 (0x21e0a3cc)                                                      */
/*      SPRHBM (0x21e0a3cc)                                                     */
/*      SPRC0 (0x21e0a3cc)                                                      */
/*      SPRMCC (0x21e0a3cc)                                                     */
/*      SPRUCC (0x21e0a3cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Contains the Time and State Info
[7:0] LTSSM STATE
[31:8] Time spent in the given state
*/


#define PH_LTSSM_LOGGER_ENTRY_LO_KTI_REUT_REG 0x091203CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 csr_ltssm_state : 8;

                            /* Bits[7:0], Access Type=RO/V/P, default=0x00000000*/

                            /* ltssm state */
    UINT32 csr_ltssm_time : 24;

                            /* Bits[31:8], Access Type=RO/V/P, default=0x00000000*/

                            /* Time spent in the ltssm state */

  } Bits;
  UINT32 Data;

} PH_LTSSM_LOGGER_ENTRY_LO_KTI_REUT_STRUCT;

/* PH_LTSSM_LOGGER_ENTRY_HI_KTI_REUT_REG supported on:                          */
/*      SPRA0 (0x21e0a3d0)                                                      */
/*      SPRB0 (0x21e0a3d0)                                                      */
/*      SPRHBM (0x21e0a3d0)                                                     */
/*      SPRC0 (0x21e0a3d0)                                                      */
/*      SPRMCC (0x21e0a3d0)                                                     */
/*      SPRUCC (0x21e0a3d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Stores the rate and arc info
[5:0] Arc
[6:6] Slow mode indicator
[7:7] Valid entry indicator
*/


#define PH_LTSSM_LOGGER_ENTRY_HI_KTI_REUT_REG 0x091203D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 csr_ltssm_arc : 6;

                            /* Bits[5:0], Access Type=RO/V/P, default=0x00000000*/

                            /* stores the arc info */
    UINT32 csr_ltssm_sso_en : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /* indicates if the entry is valid */
    UINT32 csr_ltssm_valid : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /* indicates if the entry is valid */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_LTSSM_LOGGER_ENTRY_HI_KTI_REUT_STRUCT;

/* PH_LTSSM_LOGGER_CTRL2_KTI_REUT_REG supported on:                             */
/*      SPRA0 (0x21e0a3d4)                                                      */
/*      SPRB0 (0x21e0a3d4)                                                      */
/*      SPRHBM (0x21e0a3d4)                                                     */
/*      SPRC0 (0x21e0a3d4)                                                      */
/*      SPRMCC (0x21e0a3d4)                                                     */
/*      SPRUCC (0x21e0a3d4)                                                     */
/* Register default value on SPRA0: 0x003FC023                                  */
/* Register default value on SPRB0: 0x003FC023                                  */
/* Register default value on SPRHBM: 0x003FC023                                 */
/* Register default value on SPRC0: 0x003FC023                                  */
/* Register default value on SPRMCC: 0x003FC023                                 */
/* Register default value on SPRUCC: 0x003FC023                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Ltssm Logger control signals
*/


#define PH_LTSSM_LOGGER_CTRL2_KTI_REUT_REG 0x091203D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 csr_ltslog_startonstate_bypass : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /* Start-on-a-particular-state logic bypass */
    UINT32 csr_ltslog_startonstatetran_bypass : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /*
                               Start on transition from one particular state to
                               another logic bypass
                            */
    UINT32 csr_ltslog_startonl0pexit_bypass : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* Start-on-l0p-exit logic bypas */
    UINT32 csr_ltslog_stoponl0pexit_bypass : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* Stop-on-l0p-exit logic bypass */
    UINT32 csr_ltslog_startondatalanefail_bypass : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* start-on-datalane-failover logic bypass */
    UINT32 csr_ltslog_stopondatalanefail_bypass : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* stop-on-datalane-failover logic bypass */
    UINT32 csr_ltssm_mask_cur_lt_states : 8;

                            /* Bits[13:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               program current state to state logging on
                               transition
                            */
    UINT32 csr_ltssm_state_mask : 8;

                            /* Bits[21:14], Access Type=RW/P, default=0x000000FF*/

                            /* Mask the logging of a particular state */
    UINT32 csr_ltslog_trigger_en : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* Enable the trigger mode of operation */
    UINT32 csr_ltslog_stop : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* Manual stop */
    UINT32 csr_l0_multilogging_enable : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* Manual stop */
    UINT32 rsvd : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_LTSSM_LOGGER_CTRL2_KTI_REUT_STRUCT;

/* PH_POL_INV_CTRL_KTI_REUT_REG supported on:                                   */
/*      SPRA0 (0x21e0a3d8)                                                      */
/*      SPRB0 (0x21e0a3d8)                                                      */
/*      SPRHBM (0x21e0a3d8)                                                     */
/*      SPRC0 (0x21e0a3d8)                                                      */
/*      SPRMCC (0x21e0a3d8)                                                     */
/*      SPRUCC (0x21e0a3d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Polarity inversion selection bits.
*/


#define PH_POL_INV_CTRL_KTI_REUT_REG 0x091203D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pol_inv_sel_bit : 24;

                            /* Bits[23:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Polarity inversion selection bit for slow mode
                               RX data.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_POL_INV_CTRL_KTI_REUT_STRUCT;



/* PH_IKS_KTI_REUT_REG supported on:                                            */
/*      SPRA0 (0x21e0a3e8)                                                      */
/*      SPRB0 (0x21e0a3e8)                                                      */
/*      SPRHBM (0x21e0a3e8)                                                     */
/*      SPRC0 (0x21e0a3e8)                                                      */
/*      SPRMCC (0x21e0a3e8)                                                     */
/*      SPRUCC (0x21e0a3e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_iks
*/


#define PH_IKS_KTI_REUT_REG 0x091203E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sp_ts8b10berror_0 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_1 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_2 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_3 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_4 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_5 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_6 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_7 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_8 : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_9 : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_10 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_11 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_12 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_13 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_14 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_15 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_16 : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_17 : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_18 : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_19 : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_20 : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_21 : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_22 : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 sp_ts8b10berror_23 : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B error detected in
                               TS.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_IKS_KTI_REUT_STRUCT;

/* PH_AKS_KTI_REUT_REG supported on:                                            */
/*      SPRA0 (0x21e0a3f0)                                                      */
/*      SPRB0 (0x21e0a3f0)                                                      */
/*      SPRHBM (0x21e0a3f0)                                                     */
/*      SPRC0 (0x21e0a3f0)                                                      */
/*      SPRMCC (0x21e0a3f0)                                                     */
/*      SPRUCC (0x21e0a3f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* SPRUCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_1           */
/* Struct generated from SPRA0 BDF: 30_1_2                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ph_aks
*/


#define PH_AKS_KTI_REUT_REG 0x091203F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 spactive8b10berror_0 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_1 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_2 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_3 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_4 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_5 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_6 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_7 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_8 : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_9 : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_10 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_11 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_12 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_13 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_14 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_15 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_16 : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_17 : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_18 : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_19 : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_20 : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_21 : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_22 : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 spactive8b10berror_23 : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicates 8B10B encoding error in
                               active state.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PH_AKS_KTI_REUT_STRUCT;
#endif /* _KTI_REUT_h */
