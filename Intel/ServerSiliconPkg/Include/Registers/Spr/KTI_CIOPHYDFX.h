
/** @file
  KTI_CIOPHYDFX.h

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
/* COR_CNT                                                                      */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
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
/* COR_CNT                                                                      */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
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
/* COR_CNT                                                                      */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
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
/* COR_CNT                                                                      */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
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
/* COR_CNT                                                                      */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_UCODE_SAI |             */
/*    HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI | OOB_MSM_SAI | PM_PCS_SAI |             */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI                                    */
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


#ifndef _KTI_CIOPHYDFX_h
#define _KTI_CIOPHYDFX_h
#include <Base.h>

/* VID_2_KTI_CIOPHYDFX_REG supported on:                                        */
/*      SPRA0 (0x11e0b000)                                                      */
/*      SPRB0 (0x11e0b000)                                                      */
/*      SPRHBM (0x11e0b000)                                                     */
/*      SPRC0 (0x11e0b000)                                                      */
/*      SPRMCC (0x11e0b000)                                                     */
/*      SPRUCC (0x11e49000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Vendor ID Register
*/


#define VID_2_KTI_CIOPHYDFX_REG 0x09210000

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

} VID_2_KTI_CIOPHYDFX_STRUCT;

/* DID_2_KTI_CIOPHYDFX_REG supported on:                                        */
/*      SPRA0 (0x11e0b002)                                                      */
/*      SPRB0 (0x11e0b002)                                                      */
/*      SPRHBM (0x11e0b002)                                                     */
/*      SPRC0 (0x11e0b002)                                                      */
/*      SPRMCC (0x11e0b002)                                                     */
/*      SPRUCC (0x11e49002)                                                     */
/* Register default value on SPRA0: 0x00003243                                  */
/* Register default value on SPRB0: 0x00003243                                  */
/* Register default value on SPRHBM: 0x00003243                                 */
/* Register default value on SPRC0: 0x00003243                                  */
/* Register default value on SPRMCC: 0x00003243                                 */
/* Register default value on SPRUCC: 0x00003243                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Device Identification Number
*/


#define DID_2_KTI_CIOPHYDFX_REG 0x09210002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00003243*/

                            /*
                               The value is assigned by each IP/function owner
                               as a unique identifier.
                            */

  } Bits;
  UINT16 Data;

} DID_2_KTI_CIOPHYDFX_STRUCT;

/* PCICMD_2_KTI_CIOPHYDFX_REG supported on:                                     */
/*      SPRA0 (0x11e0b004)                                                      */
/*      SPRB0 (0x11e0b004)                                                      */
/*      SPRHBM (0x11e0b004)                                                     */
/*      SPRC0 (0x11e0b004)                                                      */
/*      SPRMCC (0x11e0b004)                                                     */
/*      SPRUCC (0x11e49004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command Register
*/


#define PCICMD_2_KTI_CIOPHYDFX_REG 0x09210004

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

} PCICMD_2_KTI_CIOPHYDFX_STRUCT;

/* PCISTS_2_KTI_CIOPHYDFX_REG supported on:                                     */
/*      SPRA0 (0x11e0b006)                                                      */
/*      SPRB0 (0x11e0b006)                                                      */
/*      SPRHBM (0x11e0b006)                                                     */
/*      SPRC0 (0x11e0b006)                                                      */
/*      SPRMCC (0x11e0b006)                                                     */
/*      SPRUCC (0x11e49006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status Register
*/


#define PCISTS_2_KTI_CIOPHYDFX_REG 0x09210006

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

} PCISTS_2_KTI_CIOPHYDFX_STRUCT;

/* RID_CCR_2_KTI_CIOPHYDFX_REG supported on:                                    */
/*      SPRA0 (0x21e0b008)                                                      */
/*      SPRB0 (0x21e0b008)                                                      */
/*      SPRHBM (0x21e0b008)                                                     */
/*      SPRC0 (0x21e0b008)                                                      */
/*      SPRMCC (0x21e0b008)                                                     */
/*      SPRUCC (0x21e49008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_2_KTI_CIOPHYDFX_REG 0x09220008

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

} RID_CCR_2_KTI_CIOPHYDFX_STRUCT;

/* CLSR_2_KTI_CIOPHYDFX_REG supported on:                                       */
/*      SPRA0 (0x1e0b00c)                                                       */
/*      SPRB0 (0x1e0b00c)                                                       */
/*      SPRHBM (0x1e0b00c)                                                      */
/*      SPRC0 (0x1e0b00c)                                                       */
/*      SPRMCC (0x1e0b00c)                                                      */
/*      SPRUCC (0x1e4900c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Cache Line Size Register
*/


#define CLSR_2_KTI_CIOPHYDFX_REG 0x0920000C

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

} CLSR_2_KTI_CIOPHYDFX_STRUCT;

/* PLAT_2_KTI_CIOPHYDFX_REG supported on:                                       */
/*      SPRA0 (0x1e0b00d)                                                       */
/*      SPRB0 (0x1e0b00d)                                                       */
/*      SPRHBM (0x1e0b00d)                                                      */
/*      SPRC0 (0x1e0b00d)                                                       */
/*      SPRMCC (0x1e0b00d)                                                      */
/*      SPRUCC (0x1e4900d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Latency Timer
*/


#define PLAT_2_KTI_CIOPHYDFX_REG 0x0920000D

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

} PLAT_2_KTI_CIOPHYDFX_STRUCT;

/* HDR_2_KTI_CIOPHYDFX_REG supported on:                                        */
/*      SPRA0 (0x1e0b00e)                                                       */
/*      SPRB0 (0x1e0b00e)                                                       */
/*      SPRHBM (0x1e0b00e)                                                      */
/*      SPRC0 (0x1e0b00e)                                                       */
/*      SPRMCC (0x1e0b00e)                                                      */
/*      SPRUCC (0x1e4900e)                                                      */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Header Type
*/


#define HDR_2_KTI_CIOPHYDFX_REG 0x0920000E

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

} HDR_2_KTI_CIOPHYDFX_STRUCT;

/* BIST_2_KTI_CIOPHYDFX_REG supported on:                                       */
/*      SPRA0 (0x1e0b00f)                                                       */
/*      SPRB0 (0x1e0b00f)                                                       */
/*      SPRHBM (0x1e0b00f)                                                      */
/*      SPRC0 (0x1e0b00f)                                                       */
/*      SPRMCC (0x1e0b00f)                                                      */
/*      SPRUCC (0x1e4900f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI BIST Register
*/


#define BIST_2_KTI_CIOPHYDFX_REG 0x0920000F

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

} BIST_2_KTI_CIOPHYDFX_STRUCT;

/* SVID_2_KTI_CIOPHYDFX_REG supported on:                                       */
/*      SPRA0 (0x11e0b02c)                                                      */
/*      SPRB0 (0x11e0b02c)                                                      */
/*      SPRHBM (0x11e0b02c)                                                     */
/*      SPRC0 (0x11e0b02c)                                                      */
/*      SPRMCC (0x11e0b02c)                                                     */
/*      SPRUCC (0x11e4902c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID Register
*/


#define SVID_2_KTI_CIOPHYDFX_REG 0x0921002C

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

} SVID_2_KTI_CIOPHYDFX_STRUCT;

/* SDID_2_KTI_CIOPHYDFX_REG supported on:                                       */
/*      SPRA0 (0x11e0b02e)                                                      */
/*      SPRB0 (0x11e0b02e)                                                      */
/*      SPRHBM (0x11e0b02e)                                                     */
/*      SPRC0 (0x11e0b02e)                                                      */
/*      SPRMCC (0x11e0b02e)                                                     */
/*      SPRUCC (0x11e4902e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID Register
*/


#define SDID_2_KTI_CIOPHYDFX_REG 0x0921002E

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

} SDID_2_KTI_CIOPHYDFX_STRUCT;

/* CAPPTR_2_KTI_CIOPHYDFX_REG supported on:                                     */
/*      SPRA0 (0x1e0b034)                                                       */
/*      SPRB0 (0x1e0b034)                                                       */
/*      SPRHBM (0x1e0b034)                                                      */
/*      SPRC0 (0x1e0b034)                                                       */
/*      SPRMCC (0x1e0b034)                                                      */
/*      SPRUCC (0x1e49034)                                                      */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Capability Pointer Register
*/


#define CAPPTR_2_KTI_CIOPHYDFX_REG 0x09200034

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

} CAPPTR_2_KTI_CIOPHYDFX_STRUCT;

/* INTL_2_KTI_CIOPHYDFX_REG supported on:                                       */
/*      SPRA0 (0x1e0b03c)                                                       */
/*      SPRB0 (0x1e0b03c)                                                       */
/*      SPRHBM (0x1e0b03c)                                                      */
/*      SPRC0 (0x1e0b03c)                                                       */
/*      SPRMCC (0x1e0b03c)                                                      */
/*      SPRUCC (0x1e4903c)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Line Register
*/


#define INTL_2_KTI_CIOPHYDFX_REG 0x0920003C

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

} INTL_2_KTI_CIOPHYDFX_STRUCT;

/* INTPIN_2_KTI_CIOPHYDFX_REG supported on:                                     */
/*      SPRA0 (0x1e0b03d)                                                       */
/*      SPRB0 (0x1e0b03d)                                                       */
/*      SPRHBM (0x1e0b03d)                                                      */
/*      SPRC0 (0x1e0b03d)                                                       */
/*      SPRMCC (0x1e0b03d)                                                      */
/*      SPRUCC (0x1e4903d)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Interrupt Pin Register
*/


#define INTPIN_2_KTI_CIOPHYDFX_REG 0x0920003D

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

} INTPIN_2_KTI_CIOPHYDFX_STRUCT;

/* MINGNT_2_KTI_CIOPHYDFX_REG supported on:                                     */
/*      SPRA0 (0x1e0b03e)                                                       */
/*      SPRB0 (0x1e0b03e)                                                       */
/*      SPRHBM (0x1e0b03e)                                                      */
/*      SPRC0 (0x1e0b03e)                                                       */
/*      SPRMCC (0x1e0b03e)                                                      */
/*      SPRUCC (0x1e4903e)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Min Grant Register
*/


#define MINGNT_2_KTI_CIOPHYDFX_REG 0x0920003E

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

} MINGNT_2_KTI_CIOPHYDFX_STRUCT;

/* MAXLAT_2_KTI_CIOPHYDFX_REG supported on:                                     */
/*      SPRA0 (0x1e0b03f)                                                       */
/*      SPRB0 (0x1e0b03f)                                                       */
/*      SPRHBM (0x1e0b03f)                                                      */
/*      SPRC0 (0x1e0b03f)                                                       */
/*      SPRMCC (0x1e0b03f)                                                      */
/*      SPRUCC (0x1e4903f)                                                      */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Max Latency Register
*/


#define MAXLAT_2_KTI_CIOPHYDFX_REG 0x0920003F

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

} MAXLAT_2_KTI_CIOPHYDFX_STRUCT;

/* PXPCAP_2_KTI_CIOPHYDFX_REG supported on:                                     */
/*      SPRA0 (0x21e0b040)                                                      */
/*      SPRB0 (0x21e0b040)                                                      */
/*      SPRHBM (0x21e0b040)                                                     */
/*      SPRC0 (0x21e0b040)                                                      */
/*      SPRMCC (0x21e0b040)                                                     */
/*      SPRUCC (0x21e49040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_2_KTI_CIOPHYDFX_REG 0x09220040

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

} PXPCAP_2_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD0_2_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b044)                                                      */
/*      SPRB0 (0x21e0b044)                                                      */
/*      SPRHBM (0x21e0b044)                                                     */
/*      SPRC0 (0x21e0b044)                                                      */
/*      SPRMCC (0x21e0b044)                                                     */
/*      SPRUCC (0x21e49044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD0_2_KTI_CIOPHYDFX_REG 0x09220044

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

} PCIE_RSVD0_2_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD1_2_N0_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b048)                                                      */
/*      SPRB0 (0x21e0b048)                                                      */
/*      SPRHBM (0x21e0b048)                                                     */
/*      SPRC0 (0x21e0b048)                                                      */
/*      SPRMCC (0x21e0b048)                                                     */
/*      SPRUCC (0x21e49048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_2_N0_KTI_CIOPHYDFX_REG 0x09220048

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

} PCIE_RSVD1_2_N0_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD1_2_N1_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b04c)                                                      */
/*      SPRB0 (0x21e0b04c)                                                      */
/*      SPRHBM (0x21e0b04c)                                                     */
/*      SPRC0 (0x21e0b04c)                                                      */
/*      SPRMCC (0x21e0b04c)                                                     */
/*      SPRUCC (0x21e4904c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD1_2_N1_KTI_CIOPHYDFX_REG 0x0922004C

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

} PCIE_RSVD1_2_N1_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD2_2_N0_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b050)                                                      */
/*      SPRB0 (0x21e0b050)                                                      */
/*      SPRHBM (0x21e0b050)                                                     */
/*      SPRC0 (0x21e0b050)                                                      */
/*      SPRMCC (0x21e0b050)                                                     */
/*      SPRUCC (0x21e49050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_2_N0_KTI_CIOPHYDFX_REG 0x09220050

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

} PCIE_RSVD2_2_N0_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD2_2_N1_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b054)                                                      */
/*      SPRB0 (0x21e0b054)                                                      */
/*      SPRHBM (0x21e0b054)                                                     */
/*      SPRC0 (0x21e0b054)                                                      */
/*      SPRMCC (0x21e0b054)                                                     */
/*      SPRUCC (0x21e49054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD2_2_N1_KTI_CIOPHYDFX_REG 0x09220054

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

} PCIE_RSVD2_2_N1_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD3_2_N0_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b058)                                                      */
/*      SPRB0 (0x21e0b058)                                                      */
/*      SPRHBM (0x21e0b058)                                                     */
/*      SPRC0 (0x21e0b058)                                                      */
/*      SPRMCC (0x21e0b058)                                                     */
/*      SPRUCC (0x21e49058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_2_N0_KTI_CIOPHYDFX_REG 0x09220058

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

} PCIE_RSVD3_2_N0_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD3_2_N1_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b05c)                                                      */
/*      SPRB0 (0x21e0b05c)                                                      */
/*      SPRHBM (0x21e0b05c)                                                     */
/*      SPRC0 (0x21e0b05c)                                                      */
/*      SPRMCC (0x21e0b05c)                                                     */
/*      SPRUCC (0x21e4905c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD3_2_N1_KTI_CIOPHYDFX_REG 0x0922005C

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

} PCIE_RSVD3_2_N1_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD4_2_N0_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b060)                                                      */
/*      SPRB0 (0x21e0b060)                                                      */
/*      SPRHBM (0x21e0b060)                                                     */
/*      SPRC0 (0x21e0b060)                                                      */
/*      SPRMCC (0x21e0b060)                                                     */
/*      SPRUCC (0x21e49060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_2_N0_KTI_CIOPHYDFX_REG 0x09220060

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

} PCIE_RSVD4_2_N0_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD4_2_N1_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b064)                                                      */
/*      SPRB0 (0x21e0b064)                                                      */
/*      SPRHBM (0x21e0b064)                                                     */
/*      SPRC0 (0x21e0b064)                                                      */
/*      SPRMCC (0x21e0b064)                                                     */
/*      SPRUCC (0x21e49064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD4_2_N1_KTI_CIOPHYDFX_REG 0x09220064

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

} PCIE_RSVD4_2_N1_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD5_2_N0_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b068)                                                      */
/*      SPRB0 (0x21e0b068)                                                      */
/*      SPRHBM (0x21e0b068)                                                     */
/*      SPRC0 (0x21e0b068)                                                      */
/*      SPRMCC (0x21e0b068)                                                     */
/*      SPRUCC (0x21e49068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_2_N0_KTI_CIOPHYDFX_REG 0x09220068

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

} PCIE_RSVD5_2_N0_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD5_2_N1_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b06c)                                                      */
/*      SPRB0 (0x21e0b06c)                                                      */
/*      SPRHBM (0x21e0b06c)                                                     */
/*      SPRC0 (0x21e0b06c)                                                      */
/*      SPRMCC (0x21e0b06c)                                                     */
/*      SPRUCC (0x21e4906c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD5_2_N1_KTI_CIOPHYDFX_REG 0x0922006C

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

} PCIE_RSVD5_2_N1_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD6_2_N0_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b070)                                                      */
/*      SPRB0 (0x21e0b070)                                                      */
/*      SPRHBM (0x21e0b070)                                                     */
/*      SPRC0 (0x21e0b070)                                                      */
/*      SPRMCC (0x21e0b070)                                                     */
/*      SPRUCC (0x21e49070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_2_N0_KTI_CIOPHYDFX_REG 0x09220070

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

} PCIE_RSVD6_2_N0_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD6_2_N1_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b074)                                                      */
/*      SPRB0 (0x21e0b074)                                                      */
/*      SPRHBM (0x21e0b074)                                                     */
/*      SPRC0 (0x21e0b074)                                                      */
/*      SPRMCC (0x21e0b074)                                                     */
/*      SPRUCC (0x21e49074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD6_2_N1_KTI_CIOPHYDFX_REG 0x09220074

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

} PCIE_RSVD6_2_N1_KTI_CIOPHYDFX_STRUCT;

/* PCIE_RSVD7_2_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b078)                                                      */
/*      SPRB0 (0x21e0b078)                                                      */
/*      SPRHBM (0x21e0b078)                                                     */
/*      SPRC0 (0x21e0b078)                                                      */
/*      SPRMCC (0x21e0b078)                                                     */
/*      SPRUCC (0x21e49078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reserved space for PCIE header registers
*/


#define PCIE_RSVD7_2_KTI_CIOPHYDFX_REG 0x09220078

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

} PCIE_RSVD7_2_KTI_CIOPHYDFX_STRUCT;

/* PXPENHCAP_2_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b100)                                                      */
/*      SPRB0 (0x21e0b100)                                                      */
/*      SPRHBM (0x21e0b100)                                                     */
/*      SPRC0 (0x21e0b100)                                                      */
/*      SPRMCC (0x21e0b100)                                                     */
/*      SPRUCC (0x21e49100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_2_KTI_CIOPHYDFX_REG 0x09220100

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

} PXPENHCAP_2_KTI_CIOPHYDFX_STRUCT;

/* KTILKSCRATCHCFG32_KTI_CIOPHYDFX_REG supported on:                            */
/*      SPRA0 (0x21e0b1f0)                                                      */
/*      SPRB0 (0x21e0b1f0)                                                      */
/*      SPRHBM (0x21e0b1f0)                                                     */
/*      SPRC0 (0x21e0b1f0)                                                      */
/*      SPRMCC (0x21e0b1f0)                                                     */
/*      SPRUCC (0x21e491f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktilkscratchcfg32
*/


#define KTILKSCRATCHCFG32_KTI_CIOPHYDFX_REG 0x092201F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scratch_dn : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Scratch CFG Lower 16-bits (sticky) register
                               field.
                            */
    UINT32 scratch_up : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /*
                               Scratch CFG Upper 16-bits (non-sticky) register
                               field.
                            */

  } Bits;
  UINT32 Data;

} KTILKSCRATCHCFG32_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMAT0_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b200)                                                      */
/*      SPRB0 (0x21e0b200)                                                      */
/*      SPRHBM (0x21e0b200)                                                     */
/*      SPRC0 (0x21e0b200)                                                      */
/*      SPRMCC (0x21e0b200)                                                     */
/*      SPRUCC (0x21e49200)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmat0
*/


#define KTIHMTXMAT0_KTI_CIOPHYDFX_REG 0x09220200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match_s0_opcode_3_0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0 Opcode */
    UINT32 match_s0_msgclass_2_0 : 3;

                            /* Bits[6:4], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0 MsgClass */
    UINT32 match_s0_dnid_2_0 : 3;

                            /* Bits[9:7], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0 DNID */
    UINT32 match_s0_rhtid_11_0 : 12;

                            /* Bits[21:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               S0 Match: Slot 0 RHTID (note: does not match on
                               headerless RTID)
                            */
    UINT32 rsvd : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 match_mode_and : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mode: 0 means OR the final results of the FC,
                               S0, S1 and S2 match/masks. 1 means AND.
                            */
    UINT32 hit_s0_invert : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Invert the Slot0 hit before ANDing/Oring down to
                               the final hit signal
                            */
    UINT32 dis_filter_s0_slot_null : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable Filtering of Slot0 Slot_NULLs 0 - Filter
                               out Slot0 Slot_NULLs from generating a Slot0
                               Match Hit even if Slot0 match would otherwise be
                               a hit. Note: if filtering is enabled in
                               conjunction with hit_s0_invert=1, then the Match
                               Hit will occur on either a non-slot0-match or on
                               an Slot0 Slot_NULL. In other words, the
                               inversion will cause filtering IN (not out)
                               Slot0 Slot_NULLs. 1 - Do not filter out Slot0
                               Slot_NULLs from Slot0 Match Hit.
                            */
    UINT32 dis_filter_s0_llctrl : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable Filtering of Slot0 LLCTRLs 0 - Filter
                               out Slot0 LLCTRLs from generating a Slot0 Match
                               Hit even if Slot0 match would otherwise be a
                               hit. Note: if filtering is enabled in
                               conjunction with hit_s0_invert=1, then the Match
                               Hit will occur on either a non-slot0-match or on
                               a Slot0 LLCTRLs. In other words, the inversion
                               will cause filtering IN (not out) Slot0 LLCTRLs.
                               1 - Do not filter out Slot0 LLCTRLsfrom Slot0
                               Match Hit.
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMTXMAT0_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMAT1_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b204)                                                      */
/*      SPRB0 (0x21e0b204)                                                      */
/*      SPRHBM (0x21e0b204)                                                     */
/*      SPRC0 (0x21e0b204)                                                      */
/*      SPRMCC (0x21e0b204)                                                     */
/*      SPRUCC (0x21e49204)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmat1
*/


#define KTIHMTXMAT1_KTI_CIOPHYDFX_REG 0x09220204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match_s0_payload_31_0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0 Payload[31:0] */

  } Bits;
  UINT32 Data;

} KTIHMTXMAT1_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMAT2_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b208)                                                      */
/*      SPRB0 (0x21e0b208)                                                      */
/*      SPRHBM (0x21e0b208)                                                     */
/*      SPRC0 (0x21e0b208)                                                      */
/*      SPRMCC (0x21e0b208)                                                     */
/*      SPRUCC (0x21e49208)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmat2
*/


#define KTIHMTXMAT2_KTI_CIOPHYDFX_REG 0x09220208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match_s0_payload_49_32 : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0 Payload[49:32] */
    UINT32 match_s0_float_htid_10_0 : 11;

                            /* Bits[28:18], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0/1 Float HTID[10:0] */
    UINT32 match_s0_float_htid_11 : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0/1 Float HTID[11] */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMTXMAT2_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMAT3_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b20c)                                                      */
/*      SPRB0 (0x21e0b20c)                                                      */
/*      SPRHBM (0x21e0b20c)                                                     */
/*      SPRC0 (0x21e0b20c)                                                      */
/*      SPRMCC (0x21e0b20c)                                                     */
/*      SPRUCC (0x21e4920c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmat3
*/


#define KTIHMTXMAT3_KTI_CIOPHYDFX_REG 0x0922020C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match_s1_opcode_3_0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 1 Opcode */
    UINT32 match_s1_msgclass_0_0 : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 1 MsgClass */
    UINT32 rsvd : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 match_s1_dnid_2_0 : 3;

                            /* Bits[9:7], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 1 DNID */
    UINT32 match_s1_rhtid_11_0 : 12;

                            /* Bits[21:10], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 1 RHTID */
    UINT32 rsvd_22 : 2;

                            /* Bits[23:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 hit_s1_invert : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Invert the Slot1 hit before ANDing/Oring down to
                               the final hit signal
                            */
    UINT32 dis_filter_s1_slot_null : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable Filtering of Slot1 Slot_NULLs
                               (implicitly including Slot0 LLCTRLs) 0 - Filter
                               out Slot1 Slot_NULLs and Slot0 LLCTRLs from
                               triggering a Slot1 Match Hit even if Slot1 match
                               would otherwise be a hit. Note: if filtering is
                               enabled in conjunction with hit_s1_invert=1,
                               then the Match Hit will occur on either a non-
                               slot1-match or on an Slot1 Slot_NULL or on a
                               Slot0 LLCTRL. In other words, the inversion will
                               cause filtering IN (not out) Slot1 Slot_NULLs
                               and Slot0 LLCTRLs. 1 - Do not filter out Slot1
                               Slot_NULLs and Slot0 LLCTRLs from Slot1 Match
                               Hit.
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMTXMAT3_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMAT4_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b210)                                                      */
/*      SPRB0 (0x21e0b210)                                                      */
/*      SPRHBM (0x21e0b210)                                                     */
/*      SPRC0 (0x21e0b210)                                                      */
/*      SPRMCC (0x21e0b210)                                                     */
/*      SPRUCC (0x21e49210)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmat4
*/


#define KTIHMTXMAT4_KTI_CIOPHYDFX_REG 0x09220210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match_s1_payload_31_0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 1 Payload[31:0] */

  } Bits;
  UINT32 Data;

} KTIHMTXMAT4_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMAT5_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b214)                                                      */
/*      SPRB0 (0x21e0b214)                                                      */
/*      SPRHBM (0x21e0b214)                                                     */
/*      SPRC0 (0x21e0b214)                                                      */
/*      SPRMCC (0x21e0b214)                                                     */
/*      SPRUCC (0x21e49214)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmat5
*/


#define KTIHMTXMAT5_KTI_CIOPHYDFX_REG 0x09220214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match_s1_payload_49_32 : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 1 Payload[49:32] */
    UINT32 match_s1_float_htid_10_0 : 11;

                            /* Bits[28:18], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 0/1 Float HTID[10:0] */
    UINT32 match_s1_float_htid_11 : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 0/1 Float HTID[11] */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMTXMAT5_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMAT6_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b218)                                                      */
/*      SPRB0 (0x21e0b218)                                                      */
/*      SPRHBM (0x21e0b218)                                                     */
/*      SPRC0 (0x21e0b218)                                                      */
/*      SPRMCC (0x21e0b218)                                                     */
/*      SPRUCC (0x21e49218)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmat6
*/


#define KTIHMTXMAT6_KTI_CIOPHYDFX_REG 0x09220218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 match_fc_4_1 : 4;

                            /* Bits[4:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               FC Match on Flit Control bits [4:1] (ie. CRD,
                               ACK, VNA, VN)
                            */
    UINT32 match_s2_opcode_2_0 : 3;

                            /* Bits[7:5], Access Type=RW/P, default=0x00000000*/

                            /* S2 Match: Slot2 Opcode */
    UINT32 match_s2_dnid_2_0 : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /* S2 Match: Slot2 DNID */
    UINT32 match_s2_rhtid_11_0 : 12;

                            /* Bits[22:11], Access Type=RW/P, default=0x00000000*/

                            /* S2 Match: Slot2 RHTID */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 hit_fc_invert : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Invert the FlitControl hit before ANDing/Oring
                               downto the final hit signal
                            */
    UINT32 hit_s2_invert : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               Invert the Slot2 hit before ANDing/Oring down to
                               the final hit signal
                            */
    UINT32 dis_filter_s2_implicit_null : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable Filtering of Slot2 Implicit NULLs
                               (implicitly including Slot0 LLCTRLs) 0 - Filter
                               out Slot2 Implicit NULLs (ie. LLCRD with all
                               zero payload) and Slot0 LLCTRLs from triggering
                               a Slot2 Match Hit even if Slot 2 match would
                               otherwise be a hit. Note: if filtering is
                               enabled in conjunction with hit_s2_invert=1,
                               then the Match Hit will occur on either a non-
                               slot2-match or on an Implicit NULL or on a Slot0
                               LLCTRL. In other words, the inversion will cause
                               filtering IN (not out) Implicit NULLs and Slot0
                               LLCTRLs. 1 - Do not filter out Slot2 Implicit
                               NULLs (ie. LLCRD with all zero payload) and
                               Slot0 LLCTRLs from Slot2 Match Hit.
                            */
    UINT32 dis_filter_s2_llcrd : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable Filtering of Slot2 Non-Zero LLCRDs
                               (implicitly including Slot0 LLCTRLs) 0 - Filter
                               out Slot2 Non-Zero LLCRDs (ie. LLCRD with non-
                               zero payload) and Slot0 LLCTRLs from triggering
                               a Slot2 Match Hit even if Slot 2 match would
                               otherwise be a hit. Note: if filtering is
                               enabled in conjunction with hit_s2_invert=1,
                               then the Match Hit will occur on either a non-
                               slot2-match or on an LLCRD or on a Slot0 LLCTRL.
                               In other words, the inversion will cause
                               filtering IN (not out) LLCRDs (including Slot2
                               Implicit NULLs) and Slot0 LLCTRLs. 1 - Do not
                               filter out Slot2 LLCRDs (including Implicit
                               NULLs) and Slot0 LLCTRLs from Slot2 Match Hit.
                            */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMTXMAT6_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMSK0_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b21c)                                                      */
/*      SPRB0 (0x21e0b21c)                                                      */
/*      SPRHBM (0x21e0b21c)                                                     */
/*      SPRC0 (0x21e0b21c)                                                      */
/*      SPRMCC (0x21e0b21c)                                                     */
/*      SPRUCC (0x21e4921c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmsk0
*/


#define KTIHMTXMSK0_KTI_CIOPHYDFX_REG 0x0922021C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_s0_opcode_3_0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0 Opcode */
    UINT32 mask_s0_msgclass_2_0 : 3;

                            /* Bits[6:4], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0 MsgClass */
    UINT32 mask_s0_dnid_2_0 : 3;

                            /* Bits[9:7], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0 DNID */
    UINT32 mask_s0_rhtid_11_0 : 12;

                            /* Bits[21:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               S0 Mask: Slot 0 RHTID (note: does not match on
                               headerless RTID)
                            */
    UINT32 rsvd : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMTXMSK0_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMSK1_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b220)                                                      */
/*      SPRB0 (0x21e0b220)                                                      */
/*      SPRHBM (0x21e0b220)                                                     */
/*      SPRC0 (0x21e0b220)                                                      */
/*      SPRMCC (0x21e0b220)                                                     */
/*      SPRUCC (0x21e49220)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmsk1
*/


#define KTIHMTXMSK1_KTI_CIOPHYDFX_REG 0x09220220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_s0_payload_31_0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0 Payload[31:0] */

  } Bits;
  UINT32 Data;

} KTIHMTXMSK1_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMSK2_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b224)                                                      */
/*      SPRB0 (0x21e0b224)                                                      */
/*      SPRHBM (0x21e0b224)                                                     */
/*      SPRC0 (0x21e0b224)                                                      */
/*      SPRMCC (0x21e0b224)                                                     */
/*      SPRUCC (0x21e49224)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmsk2
*/


#define KTIHMTXMSK2_KTI_CIOPHYDFX_REG 0x09220224

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_s0_payload_49_32 : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0 Payload[49:32] */
    UINT32 mask_s0_float_htid_10_0 : 11;

                            /* Bits[28:18], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0/1 Float HTID[10:0] */
    UINT32 mask_s0_float_htid_11 : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0/1 Float HTID[11] */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMTXMSK2_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMSK3_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b228)                                                      */
/*      SPRB0 (0x21e0b228)                                                      */
/*      SPRHBM (0x21e0b228)                                                     */
/*      SPRC0 (0x21e0b228)                                                      */
/*      SPRMCC (0x21e0b228)                                                     */
/*      SPRUCC (0x21e49228)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmsk3
*/


#define KTIHMTXMSK3_KTI_CIOPHYDFX_REG 0x09220228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_s1_opcode_3_0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 1 Opcode */
    UINT32 mask_s1_msgclass_0_0 : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 1 MsgClass */
    UINT32 rsvd : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mask_s1_dnid_2_0 : 3;

                            /* Bits[9:7], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 1 DNID */
    UINT32 mask_s1_rhtid_11_0 : 12;

                            /* Bits[21:10], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 1 RHTID */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMTXMSK3_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMSK4_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b22c)                                                      */
/*      SPRB0 (0x21e0b22c)                                                      */
/*      SPRHBM (0x21e0b22c)                                                     */
/*      SPRC0 (0x21e0b22c)                                                      */
/*      SPRMCC (0x21e0b22c)                                                     */
/*      SPRUCC (0x21e4922c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmsk4
*/


#define KTIHMTXMSK4_KTI_CIOPHYDFX_REG 0x0922022C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_s1_payload_31_0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 1 Payload[31:0] */

  } Bits;
  UINT32 Data;

} KTIHMTXMSK4_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMSK5_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b230)                                                      */
/*      SPRB0 (0x21e0b230)                                                      */
/*      SPRHBM (0x21e0b230)                                                     */
/*      SPRC0 (0x21e0b230)                                                      */
/*      SPRMCC (0x21e0b230)                                                     */
/*      SPRUCC (0x21e49230)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmsk5
*/


#define KTIHMTXMSK5_KTI_CIOPHYDFX_REG 0x09220230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_s1_payload_49_32 : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 1 Payload[49:32] */
    UINT32 mask_s1_float_htid_10_0 : 11;

                            /* Bits[28:18], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 0/1 Float HTID[10:0] */
    UINT32 mask_s1_float_htid_11 : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 0/1 Float HTID[11] */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMTXMSK5_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXMSK6_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b234)                                                      */
/*      SPRB0 (0x21e0b234)                                                      */
/*      SPRHBM (0x21e0b234)                                                     */
/*      SPRC0 (0x21e0b234)                                                      */
/*      SPRMCC (0x21e0b234)                                                     */
/*      SPRUCC (0x21e49234)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxmsk6
*/


#define KTIHMTXMSK6_KTI_CIOPHYDFX_REG 0x09220234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mask_fc_4_1 : 4;

                            /* Bits[4:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               FC Mask: on Flit Control bits [4:1] (ie. CRD,
                               ACK, VNA, VN)
                            */
    UINT32 mask_s2_opcode_2_0 : 3;

                            /* Bits[7:5], Access Type=RW/P, default=0x00000000*/

                            /* S2 Mask: Slot2 Opcode */
    UINT32 mask_s2_dnid_2_0 : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /* S2 Mask: Slot2 DNID */
    UINT32 mask_s2_rhtid_11_0 : 12;

                            /* Bits[22:11], Access Type=RW/P, default=0x00000000*/

                            /* S2 Mask: Slot2 RHTID */
    UINT32 rsvd_23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMTXMSK6_KTI_CIOPHYDFX_STRUCT;

/* KTIHMTXSTAT_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b238)                                                      */
/*      SPRB0 (0x21e0b238)                                                      */
/*      SPRHBM (0x21e0b238)                                                     */
/*      SPRC0 (0x21e0b238)                                                      */
/*      SPRMCC (0x21e0b238)                                                     */
/*      SPRUCC (0x21e49238)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmtxstat
*/


#define KTIHMTXSTAT_KTI_CIOPHYDFX_REG 0x09220238

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hmts : 24;

                            /* Bits[23:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Header Match Time Stamp Capture When a match
                               occurs the value of the free running time stamp
                               counter is stored in this bit field. Only the
                               FIRST header match will cause the capture.
                               Subsequent header matches will not over-write
                               the first header match`s time stamp value. A
                               config write of FFFFFFh will clear the bit field
                               and re-arm the capture logic
                            */
    UINT32 rsvd : 7;

                            /* Bits[30:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 hit_status : 1;

                            /* Bits[31:31], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Header Match Hit Status 0 - no match hit since
                               last clearing of timestamp 1 - match hit
                               occurred since last clearing of timestamp
                            */

  } Bits;
  UINT32 Data;

} KTIHMTXSTAT_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMAT0_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b23c)                                                      */
/*      SPRB0 (0x21e0b23c)                                                      */
/*      SPRHBM (0x21e0b23c)                                                     */
/*      SPRC0 (0x21e0b23c)                                                      */
/*      SPRMCC (0x21e0b23c)                                                     */
/*      SPRUCC (0x21e4923c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmat0
*/


#define KTIHMRXMAT0_KTI_CIOPHYDFX_REG 0x0922023C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match_s0_opcode_3_0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0 Opcode */
    UINT32 match_s0_msgclass_2_0 : 3;

                            /* Bits[6:4], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0 MsgClass */
    UINT32 match_s0_dnid_2_0 : 3;

                            /* Bits[9:7], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0 DNID */
    UINT32 match_s0_rhtid_11_0 : 12;

                            /* Bits[21:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               S0 Match: Slot 0 RHTID (note: does not match on
                               headerless RTID)
                            */
    UINT32 rsvd : 1;

                            /* Bits[22:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 match_mode_and : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mode: 0 means OR the final results of the FC,
                               S0, S1 and S2 match/masks. 1 means AND.
                            */
    UINT32 hit_s0_invert : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Invert the Slot0 hit before ANDing/Oring down to
                               the final hit signal
                            */
    UINT32 dis_filter_s0_slot_null : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable Filtering of Slot0 Slot_NULLs 0 - Filter
                               out Slot0 Slot_NULLs from generating a Slot0
                               Match Hit even if Slot0 match would otherwise be
                               a hit. Note: if filtering is enabled in
                               conjunction with hit_s0_invert=1, then the Match
                               Hit will occur on either a non-slot0-match or on
                               an Slot0 Slot_NULL. In other words, the
                               inversion will cause filtering IN (not out)
                               Slot0 Slot_NULLs. 1 - Do not filter out Slot0
                               Slot_NULLs from Slot0 Match Hit.
                            */
    UINT32 dis_filter_s0_llctrl : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable Filtering of Slot0 LLCTRLs 0 - Filter
                               out Slot0 LLCTRLs from generating a Slot0 Match
                               Hit even if Slot0 match would otherwise be a
                               hit. Note: if filtering is enabled in
                               conjunction with hit_s0_invert=1, then the Match
                               Hit will occur on either a non-slot0-match or on
                               a Slot0 LLCTRLs. In other words, the inversion
                               will cause filtering IN (not out) Slot0 LLCTRLs.
                               1 - Do not filter out Slot0 LLCTRLsfrom Slot0
                               Match Hit.
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMRXMAT0_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMAT1_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b240)                                                      */
/*      SPRB0 (0x21e0b240)                                                      */
/*      SPRHBM (0x21e0b240)                                                     */
/*      SPRC0 (0x21e0b240)                                                      */
/*      SPRMCC (0x21e0b240)                                                     */
/*      SPRUCC (0x21e49240)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmat1
*/


#define KTIHMRXMAT1_KTI_CIOPHYDFX_REG 0x09220240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match_s0_payload_31_0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0 Payload[31:0] */

  } Bits;
  UINT32 Data;

} KTIHMRXMAT1_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMAT2_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b244)                                                      */
/*      SPRB0 (0x21e0b244)                                                      */
/*      SPRHBM (0x21e0b244)                                                     */
/*      SPRC0 (0x21e0b244)                                                      */
/*      SPRMCC (0x21e0b244)                                                     */
/*      SPRUCC (0x21e49244)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmat2
*/


#define KTIHMRXMAT2_KTI_CIOPHYDFX_REG 0x09220244

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match_s0_payload_49_32 : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0 Payload[49:32] */
    UINT32 match_s0_float_htid_10_0 : 11;

                            /* Bits[28:18], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0/1 Float HTID[10:0] */
    UINT32 match_s0_float_htid_11 : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* S0 Match: Slot 0/1 Float HTID[11] */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMRXMAT2_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMAT3_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b248)                                                      */
/*      SPRB0 (0x21e0b248)                                                      */
/*      SPRHBM (0x21e0b248)                                                     */
/*      SPRC0 (0x21e0b248)                                                      */
/*      SPRMCC (0x21e0b248)                                                     */
/*      SPRUCC (0x21e49248)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmat3
*/


#define KTIHMRXMAT3_KTI_CIOPHYDFX_REG 0x09220248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match_s1_opcode_3_0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 1 Opcode */
    UINT32 match_s1_msgclass_0_0 : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 1 MsgClass */
    UINT32 rsvd : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 match_s1_dnid_2_0 : 3;

                            /* Bits[9:7], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 1 DNID */
    UINT32 match_s1_rhtid_11_0 : 12;

                            /* Bits[21:10], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 1 RHTID */
    UINT32 rsvd_22 : 2;

                            /* Bits[23:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 hit_s1_invert : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Invert the Slot1 hit before ANDing/Oring down to
                               the final hit signal
                            */
    UINT32 dis_filter_s1_slot_null : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable Filtering of Slot1 Slot_NULLs
                               (implicitly including Slot0 LLCTRLs) 0 - Filter
                               out Slot1 Slot_NULLs and Slot0 LLCTRLs from
                               triggering a Slot1 Match Hit even if Slot1 match
                               would otherwise be a hit. Note: if filtering is
                               enabled in conjunction with hit_s1_invert=1,
                               then the Match Hit will occur on either a non-
                               slot1-match or on an Slot1 Slot_NULL or on a
                               Slot0 LLCTRL. In other words, the inversion will
                               cause filtering IN (not out) Slot1 Slot_NULLs
                               and Slot0 LLCTRLs. 1 - Do not filter out Slot1
                               Slot_NULLs and Slot0 LLCTRLs from Slot1 Match
                               Hit.
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMRXMAT3_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMAT4_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b24c)                                                      */
/*      SPRB0 (0x21e0b24c)                                                      */
/*      SPRHBM (0x21e0b24c)                                                     */
/*      SPRC0 (0x21e0b24c)                                                      */
/*      SPRMCC (0x21e0b24c)                                                     */
/*      SPRUCC (0x21e4924c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmat4
*/


#define KTIHMRXMAT4_KTI_CIOPHYDFX_REG 0x0922024C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match_s1_payload_31_0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 1 Payload[31:0] */

  } Bits;
  UINT32 Data;

} KTIHMRXMAT4_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMAT5_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b250)                                                      */
/*      SPRB0 (0x21e0b250)                                                      */
/*      SPRHBM (0x21e0b250)                                                     */
/*      SPRC0 (0x21e0b250)                                                      */
/*      SPRMCC (0x21e0b250)                                                     */
/*      SPRUCC (0x21e49250)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmat5
*/


#define KTIHMRXMAT5_KTI_CIOPHYDFX_REG 0x09220250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match_s1_payload_49_32 : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 1 Payload[49:32] */
    UINT32 match_s1_float_htid_10_0 : 11;

                            /* Bits[28:18], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 0/1 Float HTID[10:0] */
    UINT32 match_s1_float_htid_11 : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* S1 Match: Slot 0/1 Float HTID[11] */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMRXMAT5_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMAT6_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b254)                                                      */
/*      SPRB0 (0x21e0b254)                                                      */
/*      SPRHBM (0x21e0b254)                                                     */
/*      SPRC0 (0x21e0b254)                                                      */
/*      SPRMCC (0x21e0b254)                                                     */
/*      SPRUCC (0x21e49254)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmat6
*/


#define KTIHMRXMAT6_KTI_CIOPHYDFX_REG 0x09220254

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 match_fc_4_1 : 4;

                            /* Bits[4:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               FC Match on Flit Control bits [4:1] (ie. CRD,
                               ACK, VNA, VN)
                            */
    UINT32 match_s2_opcode_2_0 : 3;

                            /* Bits[7:5], Access Type=RW/P, default=0x00000000*/

                            /* S2 Match: Slot2 Opcode */
    UINT32 match_s2_dnid_2_0 : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /* S2 Match: Slot2 DNID */
    UINT32 match_s2_rhtid_11_0 : 12;

                            /* Bits[22:11], Access Type=RW/P, default=0x00000000*/

                            /* S2 Match: Slot2 RHTID */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 hit_fc_invert : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Invert the FlitControl hit before ANDing/Oring
                               downto the final hit signal
                            */
    UINT32 hit_s2_invert : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               Invert the Slot2 hit before ANDing/Oring down to
                               the final hit signal
                            */
    UINT32 dis_filter_s2_implicit_null : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable Filtering of Slot2 Implicit NULLs
                               (implicitly including Slot0 LLCTRLs) 0 - Filter
                               out Slot2 Implicit NULLs (ie. LLCRD with all
                               zero payload) and Slot0 LLCTRLs from triggering
                               a Slot2 Match Hit even if Slot 2 match would
                               otherwise be a hit. Note: if filtering is
                               enabled in conjunction with hit_s2_invert=1,
                               then the Match Hit will occur on either a non-
                               slot2-match or on an Implicit NULL or on a Slot0
                               LLCTRL. In other words, the inversion will cause
                               filtering IN (not out) Implicit NULLs and Slot0
                               LLCTRLs. 1 - Do not filter out Slot2 Implicit
                               NULLs (ie. LLCRD with all zero payload) and
                               Slot0 LLCTRLs from Slot2 Match Hit.
                            */
    UINT32 dis_filter_s2_llcrd : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable Filtering of Slot2 Non-Zero LLCRDs
                               (implicitly including Slot0 LLCTRLs) 0 - Filter
                               out Slot2 Non-Zero LLCRDs (ie. LLCRD with non-
                               zero payload) and Slot0 LLCTRLs from triggering
                               a Slot2 Match Hit even if Slot 2 match would
                               otherwise be a hit. Note: if filtering is
                               enabled in conjunction with hit_s2_invert=1,
                               then the Match Hit will occur on either a non-
                               slot2-match or on an LLCRD or on a Slot0 LLCTRL.
                               In other words, the inversion will cause
                               filtering IN (not out) LLCRDs (including Slot2
                               Implicit NULLs) and Slot0 LLCTRLs. 1 - Do not
                               filter out Slot2 LLCRDs (including Implicit
                               NULLs) and Slot0 LLCTRLs from Slot2 Match Hit.
                            */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMRXMAT6_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMSK0_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b258)                                                      */
/*      SPRB0 (0x21e0b258)                                                      */
/*      SPRHBM (0x21e0b258)                                                     */
/*      SPRC0 (0x21e0b258)                                                      */
/*      SPRMCC (0x21e0b258)                                                     */
/*      SPRUCC (0x21e49258)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmsk0
*/


#define KTIHMRXMSK0_KTI_CIOPHYDFX_REG 0x09220258

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_s0_opcode_3_0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0 Opcode */
    UINT32 mask_s0_msgclass_2_0 : 3;

                            /* Bits[6:4], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0 MsgClass */
    UINT32 mask_s0_dnid_2_0 : 3;

                            /* Bits[9:7], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0 DNID */
    UINT32 mask_s0_rhtid_11_0 : 12;

                            /* Bits[21:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               S0 Mask: Slot 0 RHTID (note: does not match on
                               headerless RTID)
                            */
    UINT32 rsvd : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMRXMSK0_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMSK1_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b25c)                                                      */
/*      SPRB0 (0x21e0b25c)                                                      */
/*      SPRHBM (0x21e0b25c)                                                     */
/*      SPRC0 (0x21e0b25c)                                                      */
/*      SPRMCC (0x21e0b25c)                                                     */
/*      SPRUCC (0x21e4925c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmsk1
*/


#define KTIHMRXMSK1_KTI_CIOPHYDFX_REG 0x0922025C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_s0_payload_31_0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0 Payload[31:0] */

  } Bits;
  UINT32 Data;

} KTIHMRXMSK1_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMSK2_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b260)                                                      */
/*      SPRB0 (0x21e0b260)                                                      */
/*      SPRHBM (0x21e0b260)                                                     */
/*      SPRC0 (0x21e0b260)                                                      */
/*      SPRMCC (0x21e0b260)                                                     */
/*      SPRUCC (0x21e49260)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmsk2
*/


#define KTIHMRXMSK2_KTI_CIOPHYDFX_REG 0x09220260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_s0_payload_49_32 : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0 Payload[49:32] */
    UINT32 mask_s0_float_htid_10_0 : 11;

                            /* Bits[28:18], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0/1 Float HTID[10:0] */
    UINT32 mask_s0_float_htid_11 : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* S0 Mask: Slot 0/1 Float HTID[11] */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMRXMSK2_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMSK3_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b264)                                                      */
/*      SPRB0 (0x21e0b264)                                                      */
/*      SPRHBM (0x21e0b264)                                                     */
/*      SPRC0 (0x21e0b264)                                                      */
/*      SPRMCC (0x21e0b264)                                                     */
/*      SPRUCC (0x21e49264)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmsk3
*/


#define KTIHMRXMSK3_KTI_CIOPHYDFX_REG 0x09220264

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_s1_opcode_3_0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 1 Opcode */
    UINT32 mask_s1_msgclass_0_0 : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 1 MsgClass */
    UINT32 rsvd : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mask_s1_dnid_2_0 : 3;

                            /* Bits[9:7], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 1 DNID */
    UINT32 mask_s1_rhtid_11_0 : 12;

                            /* Bits[21:10], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 1 RHTID */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMRXMSK3_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMSK4_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b268)                                                      */
/*      SPRB0 (0x21e0b268)                                                      */
/*      SPRHBM (0x21e0b268)                                                     */
/*      SPRC0 (0x21e0b268)                                                      */
/*      SPRMCC (0x21e0b268)                                                     */
/*      SPRUCC (0x21e49268)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmsk4
*/


#define KTIHMRXMSK4_KTI_CIOPHYDFX_REG 0x09220268

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_s1_payload_31_0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 1 Payload[31:0] */

  } Bits;
  UINT32 Data;

} KTIHMRXMSK4_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMSK5_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b26c)                                                      */
/*      SPRB0 (0x21e0b26c)                                                      */
/*      SPRHBM (0x21e0b26c)                                                     */
/*      SPRC0 (0x21e0b26c)                                                      */
/*      SPRMCC (0x21e0b26c)                                                     */
/*      SPRUCC (0x21e4926c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmsk5
*/


#define KTIHMRXMSK5_KTI_CIOPHYDFX_REG 0x0922026C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask_s1_payload_49_32 : 18;

                            /* Bits[17:0], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 1 Payload[49:32] */
    UINT32 mask_s1_float_htid_10_0 : 11;

                            /* Bits[28:18], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 0/1 Float HTID[10:0] */
    UINT32 mask_s1_float_htid_11 : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* S1 Mask: Slot 0/1 Float HTID[11] */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMRXMSK5_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXMSK6_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b270)                                                      */
/*      SPRB0 (0x21e0b270)                                                      */
/*      SPRHBM (0x21e0b270)                                                     */
/*      SPRC0 (0x21e0b270)                                                      */
/*      SPRMCC (0x21e0b270)                                                     */
/*      SPRUCC (0x21e49270)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxmsk6
*/


#define KTIHMRXMSK6_KTI_CIOPHYDFX_REG 0x09220270

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mask_fc_4_1 : 4;

                            /* Bits[4:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               FC Mask: on Flit Control bits [4:1] (ie. CRD,
                               ACK, VNA, VN)
                            */
    UINT32 mask_s2_opcode_2_0 : 3;

                            /* Bits[7:5], Access Type=RW/P, default=0x00000000*/

                            /* S2 Mask: Slot2 Opcode */
    UINT32 mask_s2_dnid_2_0 : 3;

                            /* Bits[10:8], Access Type=RW/P, default=0x00000000*/

                            /* S2 Mask: Slot2 DNID */
    UINT32 mask_s2_rhtid_11_0 : 12;

                            /* Bits[22:11], Access Type=RW/P, default=0x00000000*/

                            /* S2 Mask: Slot2 RHTID */
    UINT32 rsvd_23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIHMRXMSK6_KTI_CIOPHYDFX_STRUCT;

/* KTIHMRXSTAT_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b274)                                                      */
/*      SPRB0 (0x21e0b274)                                                      */
/*      SPRHBM (0x21e0b274)                                                     */
/*      SPRC0 (0x21e0b274)                                                      */
/*      SPRMCC (0x21e0b274)                                                     */
/*      SPRUCC (0x21e49274)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktihmrxstat
*/


#define KTIHMRXSTAT_KTI_CIOPHYDFX_REG 0x09220274

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hmts : 24;

                            /* Bits[23:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Header Match Time Stamp Capture When a match
                               occurs the value of the free running time stamp
                               counter is stored in this bit field. Only the
                               FIRST header match will cause the capture.
                               Subsequent header matches will not over-write
                               the first header match`s time stamp value. A
                               config write of FFFFFFh will clear the bit field
                               and re-arm the capture logic
                            */
    UINT32 rsvd : 7;

                            /* Bits[30:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 hit_status : 1;

                            /* Bits[31:31], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Header Match Hit Status 0 - no match hit since
                               last clearing of timestamp 1 - match hit
                               occurred since last clearing of timestamp
                            */

  } Bits;
  UINT32 Data;

} KTIHMRXSTAT_KTI_CIOPHYDFX_STRUCT;

/* KTIINBDPCTL_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b278)                                                      */
/*      SPRB0 (0x21e0b278)                                                      */
/*      SPRHBM (0x21e0b278)                                                     */
/*      SPRC0 (0x21e0b278)                                                      */
/*      SPRMCC (0x21e0b278)                                                     */
/*      SPRUCC (0x21e49278)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktiinbdpctl
*/


#define KTIINBDPCTL_KTI_CIOPHYDFX_REG 0x09220278

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ibdevten : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               enables transmission of Inband Debug Event flits
                               when a debug event occurs. The set of debug
                               events is programmable (KTIINBDSEL).
                            */
    UINT32 ibdrtpen : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* enables transmission of RTP Debug flits. */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 evtfhigh : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               forces Inband Debug Event flits to be sent as
                               high priority (will pre-empt protocol traffic -
                               otherwise these flits will be sent only
                               opportunistically)
                            */
    UINT32 evtwaken : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               enables Tx PM wakeup due to Inband Debug Event
                               flits
                            */
    UINT32 rsvd_5 : 2;

                            /* Bits[6:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rtpfhigh : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               forces RTP Debug flits to be sent as high
                               priority (will pre-empt protocol traffic -
                               otherwise these flits will be sent only
                               opportunistically)
                            */
    UINT32 rtpwaken : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* enables Tx PM wakeup due to RTP Debug flits */
    UINT32 forcetimer : 3;

                            /* Bits[11:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               amount of time before an opportunistic Debug
                               flit is converted to a high priority request 000
                               - 64 flit clocks 001 - 128 flit clocks 010 - 512
                               flit clocks 011 - 2048 flit clocks 100 - 4096
                               flit clocks 101 - 16384 flit clocks 110 - 65536
                               flit clocks 111 - never
                            */
    UINT32 rtpfreq : 3;

                            /* Bits[14:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               configures the frequency of transmission for
                               master timer RTP Debug flits: 000 - 32 flit
                               clocks 001 - 64 flit clocks 010 - 256 flit
                               clocks 011 - 1024 flit clocks 100 - 2048 flit
                               clocks 101 - 4096 flit clocks 110 - 32768 flit
                               clocks 111 - master timer RTP Debug flits
                               disabled
                            */
    UINT32 rtpslave : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               Selects slave mode generation of RTP Debug
                               flits: 1 - Slave mode - generate RTP Debug flit
                               upon reception of RTP Debug flit 0 - Master mode
                               - generate RTP Debug flit upon expiration of
                               'master timer'
                            */
    UINT32 rsvd_16 : 6;

                            /* Bits[21:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rtp_on_masterslave_en : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               higher level enable for master/slave generation
                               of RTP Debug flits: master: master timer
                               expiration slave: reception of RTP flits
                            */
    UINT32 rtp_on_phy_rst_en : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               enables generation of RTP Debug flit upon PHY
                               exit from Reset
                            */
    UINT32 rtp_on_ll_rst_en : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               enables generation of RTP Debug flit upon solid
                               entry into Normal Op by the Link Layer Init FSM
                            */
    UINT32 rtp_on_localevent_en : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               enables generation of RTP Debug flits upon
                               occurrence of a 'local event' (CTO[1] -
                               programmed in KTICTOSEL)
                            */
    UINT32 rtp_on_csr_write : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               when written to 1 - immediately generates an RTP
                               Debug flit
                            */
    UINT32 clear_force_timer : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               when written to a 1, it clears the force timer.
                               It must then be written to 0 to release the
                               force timer.
                            */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIINBDPCTL_KTI_CIOPHYDFX_STRUCT;

/* KTIINBDSEL_KTI_CIOPHYDFX_REG supported on:                                   */
/*      SPRA0 (0x21e0b27c)                                                      */
/*      SPRB0 (0x21e0b27c)                                                      */
/*      SPRHBM (0x21e0b27c)                                                     */
/*      SPRC0 (0x21e0b27c)                                                      */
/*      SPRMCC (0x21e0b27c)                                                     */
/*      SPRUCC (0x21e4927c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktiinbdsel
*/


#define KTIINBDSEL_KTI_CIOPHYDFX_REG 0x0922027C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ena : 11;

                            /* Bits[10:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Each bit enables event. Per bit enable
                               corresponds to the following events: [0] -
                               mBP[0] [1] - mBP[1] [2] - Cluster Trigger Out
                               [0] [3] - Cluster Trigger Out [1] [4] - PerfMon
                               Overflow Counter 3 [5] - PerfMon Overflow
                               Counter 4 [6] - UNC error [7] - COR error [8] -
                               IOT Trace En Start [9] - IOT Trace En Stop [10]
                               - IOT Hub Drain Done
                            */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIINBDSEL_KTI_CIOPHYDFX_STRUCT;

/* KTICTOSEL_KTI_CIOPHYDFX_REG supported on:                                    */
/*      SPRA0 (0x21e0b280)                                                      */
/*      SPRB0 (0x21e0b280)                                                      */
/*      SPRHBM (0x21e0b280)                                                     */
/*      SPRC0 (0x21e0b280)                                                      */
/*      SPRMCC (0x21e0b280)                                                     */
/*      SPRUCC (0x21e49280)                                                     */
/* Register default value on SPRA0: 0x00001F1F                                  */
/* Register default value on SPRB0: 0x00001F1F                                  */
/* Register default value on SPRHBM: 0x00001F1F                                 */
/* Register default value on SPRC0: 0x00001F1F                                  */
/* Register default value on SPRMCC: 0x00001F1F                                 */
/* Register default value on SPRUCC: 0x00001F1F                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktictosel
*/


#define KTICTOSEL_KTI_CIOPHYDFX_REG 0x09220280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cto0sel : 5;

                            /* Bits[4:0], Access Type=RW/P/L, default=0x0000001F*/

                            /*
                               0x0 - Rx Header Match 0x1 - Tx Header Match 0x2
                               - mBP[0] 0x3 - mBP[1] 0x4 - RX DBG.EVT (Inband
                               Debug Event) DebugEvent mBP[0] 0x5 - RX DBG.EVT
                               (Inband Debug Event) DebugEvent mBP[1] 0x6 - RX
                               DBG.EVT (Inband Debug Event) DebugEvent CTO[0]
                               0x7 - RX DBG.EVT (Inband Debug Event) DebugEvent
                               CTO[1] 0x8 - RX DBG.EVT (Inband Debug Event)
                               DebugEvent PTO[0] 0x9 - RX DBG.EVT (Inband Debug
                               Event) DebugEvent PTO[1] 0xA - RX DBG.EVT
                               (Inband Debug Event) DebugEvent UNCERR 0xB - RX
                               DBG.EVT (Inband Debug Event) DebugEvent CORERR
                               0xC - RX DBG.EVT (Inband Debug Event) DebugEvent
                               IOT_TRACE_START 0xD - RX DBG.EVT (Inband Debug
                               Event) DebugEvent IOT_TRACE_STOP 0xE - UNC error
                               0xF - COR error 0x10 - PerfMon Overflow (Rising
                               Edge) - see KTIPmonDbgMbpSel for details 0x11 -
                               Perfmon Trigger Out[0] 0x12 - CTS Trigger Out
                               0x13 - CTS Event [0] 0x14 - CTS Event [1] 0x15 -
                               ASC[0] 0x16-0x1E - Reserved 0x1F - Off
                            */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cto1sel : 5;

                            /* Bits[12:8], Access Type=RW/P/L, default=0x0000001F*/

                            /*
                               0x0 - Rx Header Match 0x1 - Tx Header Match 0x2
                               - mBP[0] 0x3 - mBP[1] 0x4 - RX DBG.EVT (Inband
                               Debug Event) DebugEvent mBP[0] 0x5 - RX DBG.EVT
                               (Inband Debug Event) DebugEvent mBP[1] 0x6 - RX
                               DBG.EVT (Inband Debug Event) DebugEvent CTO[0]
                               0x7 - RX DBG.EVT (Inband Debug Event) DebugEvent
                               CTO[1] 0x8 - RX DBG.EVT (Inband Debug Event)
                               DebugEvent PTO[0] 0x9 - RX DBG.EVT (Inband Debug
                               Event) DebugEvent PTO[1] 0xA - RX DBG.EVT
                               (Inband Debug Event) DebugEvent UNCERR 0xB - RX
                               DBG.EVT (Inband Debug Event) DebugEvent CORERR
                               0xC - RX DBG.EVT (Inband Debug Event) DebugEvent
                               IOT_TRACE_START 0xD - RX DBG.EVT (Inband Debug
                               Event) DebugEvent IOT_TRACE_STOP 0xE - UNC error
                               0xF - COR error 0x10 - PerfMon Overflow (Rising
                               Edge) - see KTIPmonDbgMbpSel for details 0x11 -
                               Perfmon Trigger Out[0] 0x12 - CTS Trigger Out
                               0x13 - CTS Event [0] 0x14 - CTS Event [1] 0x15 -
                               ASC[0] 0x16-0x1E - Reserved 0x1F - Off
                            */
    UINT32 rsvd_13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTICTOSEL_KTI_CIOPHYDFX_STRUCT;

/* KTIRXCRD2_KTI_CIOPHYDFX_REG supported on:                                    */
/*      SPRA0 (0x21e0b288)                                                      */
/*      SPRB0 (0x21e0b288)                                                      */
/*      SPRHBM (0x21e0b288)                                                     */
/*      SPRC0 (0x21e0b288)                                                      */
/*      SPRMCC (0x21e0b288)                                                     */
/*      SPRUCC (0x21e49288)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktirxcrd2
*/


#define KTIRXCRD2_KTI_CIOPHYDFX_REG 0x09220288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 8;

                            /* Bits[7:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vn1reqcrdrem : 3;

                            /* Bits[10:8], Access Type=RO/V/P, default=0x00000000*/

                            /* VN1 REQ credit remaining counter[2:0] */
    UINT32 rsvd_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vn1snpcrdrem : 3;

                            /* Bits[14:12], Access Type=RO/V/P, default=0x00000000*/

                            /* VN1 SNP credit remaining counter[2:0] */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vn1rspcrdrem : 3;

                            /* Bits[18:16], Access Type=RO/V/P, default=0x00000000*/

                            /* VN1 RSP credit remaining counter[2:0] */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vn1wbcrdrem : 3;

                            /* Bits[22:20], Access Type=RO/V/P, default=0x00000000*/

                            /* VN1 WB credit remaining counter[2:0] */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vn1ncbcrdrem : 3;

                            /* Bits[26:24], Access Type=RO/V/P, default=0x00000000*/

                            /* VN1 NCB credit remaining counter[2:0] */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vn1ncscrdrem : 3;

                            /* Bits[30:28], Access Type=RO/V/P, default=0x00000000*/

                            /* VN1 NCS credit remaining counter[2:0] */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIRXCRD2_KTI_CIOPHYDFX_STRUCT;

/* KTIRXCRD_KTI_CIOPHYDFX_REG supported on:                                     */
/*      SPRA0 (0x21e0b28c)                                                      */
/*      SPRB0 (0x21e0b28c)                                                      */
/*      SPRHBM (0x21e0b28c)                                                     */
/*      SPRC0 (0x21e0b28c)                                                      */
/*      SPRMCC (0x21e0b28c)                                                     */
/*      SPRUCC (0x21e4928c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktirxcrd
*/


#define KTIRXCRD_KTI_CIOPHYDFX_REG 0x0922028C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vnacrdrem : 8;

                            /* Bits[7:0], Access Type=RO/V/P, default=0x00000000*/

                            /* VNA credit remaining counter[7:0] */
    UINT32 vn0reqcrdrem : 3;

                            /* Bits[10:8], Access Type=RO/V/P, default=0x00000000*/

                            /* VN0 REQ credit remaining counter[2:0] */
    UINT32 rsvd : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vn0snpcrdrem : 3;

                            /* Bits[14:12], Access Type=RO/V/P, default=0x00000000*/

                            /* VN0 SNP credit remaining counter[2:0] */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vn0rspcrdrem : 3;

                            /* Bits[18:16], Access Type=RO/V/P, default=0x00000000*/

                            /* VN0 RSP credit remaining counter[2:0] */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vn0wbcrdrem : 3;

                            /* Bits[22:20], Access Type=RO/V/P, default=0x00000000*/

                            /* VN0 WB credit remaining counter[2:0] */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vn0ncbcrdrem : 3;

                            /* Bits[26:24], Access Type=RO/V/P, default=0x00000000*/

                            /* VN0 NCB credit remaining counter[2:0] */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vn0ncscrdrem : 3;

                            /* Bits[30:28], Access Type=RO/V/P, default=0x00000000*/

                            /* VN0 NCS credit remaining counter[2:0] */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIRXCRD_KTI_CIOPHYDFX_STRUCT;

/* KTIACKCNT_KTI_CIOPHYDFX_REG supported on:                                    */
/*      SPRA0 (0x21e0b290)                                                      */
/*      SPRB0 (0x21e0b290)                                                      */
/*      SPRHBM (0x21e0b290)                                                     */
/*      SPRC0 (0x21e0b290)                                                      */
/*      SPRMCC (0x21e0b290)                                                     */
/*      SPRUCC (0x21e49290)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktiackcnt
*/


#define KTIACKCNT_KTI_CIOPHYDFX_REG 0x09220290

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ackcnt : 8;

                            /* Bits[7:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Acknowledge count[7:0] */
    UINT32 llrptr : 8;

                            /* Bits[15:8], Access Type=RO/V/P, default=0x00000000*/

                            /* Link layer retry read pointer[7:0] */
    UINT32 llwptr : 8;

                            /* Bits[23:16], Access Type=RO/V/P, default=0x00000000*/

                            /* Link layer retry write pointer[7:0] */
    UINT32 crdcptmd : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 - then loads continuously (ie. every cycle). 1
                               - then loads based on DFX cluster trigger out
                               bit1 (which itself is a selectable field that
                               can use mBPs, response functions, etc.)
                            */
    UINT32 rsvd : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIACKCNT_KTI_CIOPHYDFX_STRUCT;




/* KTIPARERRLOG_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b2a0)                                                      */
/*      SPRB0 (0x21e0b2a0)                                                      */
/*      SPRHBM (0x21e0b2a0)                                                     */
/*      SPRC0 (0x21e0b2a0)                                                      */
/*      SPRMCC (0x21e0b2a0)                                                     */
/*      SPRUCC (0x21e492a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ktiparerrlog
*/


#define KTIPARERRLOG_KTI_CIOPHYDFX_REG 0x092202A0

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pelset0 : 8;

                            /* Bits[7:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               indicates which (if any) parity bit failed
                               parity checkon a read from the RxQ
                            */
    UINT32 pelset1 : 8;

                            /* Bits[15:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               indicates which (if any) parity bit failed
                               parity check on a read from the LLRB
                            */
    UINT32 pelset2 : 8;

                            /* Bits[23:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               indicates which (if any) parity bit failed
                               parity check on a read from the TxQ
                            */
    UINT32 pelset3 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               indicates parity check failed on a read from the
                               RBT
                            */
    UINT32 rsvd : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPARERRLOG_KTI_CIOPHYDFX_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
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
    UINT32 pelset3 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               indicates parity check failed on a read from the
                               RBT
                            */
    UINT32 rsvd_25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPARERRLOG_KTI_CIOPHYDFX_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 24;

                            /* Bits[23:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pelset3 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               indicates parity check failed on a read from the
                               RBT
                            */
    UINT32 rsvd_25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPARERRLOG_KTI_CIOPHYDFX_STRUCT;

/* KTIRXEINJCTL0_KTI_CIOPHYDFX_REG supported on:                                */
/*      SPRA0 (0x21e0b2b0)                                                      */
/*      SPRB0 (0x21e0b2b0)                                                      */
/*      SPRHBM (0x21e0b2b0)                                                     */
/*      SPRC0 (0x21e0b2b0)                                                      */
/*      SPRMCC (0x21e0b2b0)                                                     */
/*      SPRUCC (0x21e492b0)                                                     */
/* Register default value on SPRA0: 0xCACA0000                                  */
/* Register default value on SPRB0: 0xCACA0000                                  */
/* Register default value on SPRHBM: 0xCACA0000                                 */
/* Register default value on SPRC0: 0xCACA0000                                  */
/* Register default value on SPRMCC: 0xCACA0000                                 */
/* Register default value on SPRUCC: 0xCACA0000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ktirxeinjctl0
*/


#define KTIRXEINJCTL0_KTI_CIOPHYDFX_REG 0x092202B0

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 superr_en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 - Do not suppress error injection in replay.
                               Warning: using this setting makes it more likely
                               to hit the abort state. 1 - Suppress errors on
                               the replay that reached the escalation counters
                               according to the bit settings in
                               EinjRetryActiveMode, EinjNumRetry, and
                               EinjNumPhyReInit
                            */
    UINT32 superr_hold_on_respfcn : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 - allow suppression to stop when LRSM goes
                               inactive (on successful retry with Empty=1 or
                               retriable flit) 1 - hold suppression even after
                               LRSM goes inactive (on successful retry with
                               Empty=1 or retriable flit) and only stop
                               suppression once the response function deasserts
                               injection (or when superr_en=0).
                            */
    UINT32 maten : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 - No Header match, inject error on any header
                               (IIB==1), except exclude NULLs if bit[5]
                               EXCLUDE_NULL==1. 1 - Inject error based on
                               Header match MATF1 hit signal (ignores MATF2 and
                               HDmatOut hit signals)
                            */
    UINT32 crcdis : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* 0 - Enable CRC checking 1 - Disable CRC checking */
    UINT32 suppress_st : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error Injection suppression status 0 - No
                               suppression of injection has occurred. 1 -
                               Suppression of injection has occurred
                            */
    UINT32 errinjst : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               0 - No error injection occurred. 1 - An error
                               was injected
                            */
    UINT32 einjretryactivemode : 2;

                            /* Bits[7:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               This determines how the link will respond to
                               error injection during replay. 00 - Retry mode
                               01 - Phy init mode 10 - Retry and Phy init mode
                               11 - Abort mode
                            */
    UINT32 einjnumretry : 5;

                            /* Bits[12:8], Access Type=RW/P, default=0x00000000*/

                            /* Number of retries [4:0] */
    UINT32 einjnumphyreinit : 3;

                            /* Bits[15:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               Number of phy retries before re-initialization
                               [2:0]
                            */
    UINT32 openloop_str_sel : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Start selection for injecting in open-loop
                               response function. Open-loop means that the
                               response function will only watch for the stop
                               condition and ignores the response from the
                               affected logic. 0000 - Micro Break Point 0
                               (MBP[0]) 0001 - Micro Break Point 1 (MBP[1])
                               0010 - Cluster Trigger Out 0 (CTO[0]) 0011 -
                               Cluster Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Response never occurs
                               1011 - Response occurs immediately 1100-1111 -
                               Reserved
                            */
    UINT32 openloop_stp_sel : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Stop selection for injecting in open-loop
                               response function. Open-loop means that the
                               response function will only watch for the stop
                               condition and ignores the response from the
                               affected logic. 0000 - Micro Break Point 0
                               (MBP[0]) 0001 - Micro Break Point 1 (MBP[1])
                               0010 - Cluster Trigger Out 0 (CTO[0]) 0011 -
                               Cluster Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Deactivate one clock
                               after activation. Error injection on one
                               transaction will occur on the interface that is
                               enabled. 1011 - Response is never deactivated.
                               1100 - Deactivate the response when the
                               activating signal becomes false. 1101-1111 -
                               Reserved
                            */
    UINT32 once_str_sel : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Start selection for injecting in standard
                               closed-loop response function. Closed-loop means
                               that once activated, the response function will
                               not stop until both the stop condition has been
                               met *and* the logic affected by the response
                               function has been able to do the
                               throttling/injecting on actual traffic. Closed-
                               loop is the most common response function. 0000
                               - Micro Break Point 0 (MBP[0]) 0001 - Micro
                               Break Point 1 (MBP[1]) 0010 - Cluster Trigger
                               Out 0 (CTO[0]) 0011 - Cluster Trigger Out 1
                               (CTO[1]) 0100 - Artificial Starvation Counter 0
                               (ASC[0]) 0101 - Artificial Starvation Counter 1
                               (ASC[1]) 0110 - Common Trigger Sequencer TrigOut
                               (CTS_Trig_Out) 0111 - Reserved 1000 - MCA
                               Correctable Error 1001 - MCA Uncorrectable Error
                               1010 - Response never occurs 1011 - Response
                               occurs immediately 1100-1111 - Reserved
                            */
    UINT32 once_stp_sel : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Stop selection for injecting in standard closed-
                               loop response function. Closed-loop means that
                               once activated, the response function will not
                               stop until both the stop condition has been met
                               *and* the logic affected by the response
                               function has been able to do the
                               throttling/injecting on actual traffic. Closed-
                               loop is the most common response function. 0000
                               - Micro Break Point 0 (MBP[0]) 0001 - Micro
                               Break Point 1 (MBP[1]) 0010 - Cluster Trigger
                               Out 0 (CTO[0]) 0011 - Cluster Trigger Out 1
                               (CTO[1]) 0100 - Artificial Starvation Counter 0
                               (ASC[0]) 0101 - Artificial Starvation Counter 1
                               (ASC[1]) 0110 - Common Trigger Sequencer TrigOut
                               (CTS_Trig_Out) 0111 - Reserved 1000 - MCA
                               Correctable Error 1001 - MCA Uncorrectable Error
                               1010 - Deactivate one clock after activation.
                               Error injection on one transaction will occur on
                               the interface that is enabled. 1011 - Response
                               is never deactivated. 1100 - Deactivate the
                               response when the activating signal becomes
                               false. 1101-1111 - Reserved
                            */

  } Bits;
  UINT32 Data;

} KTIRXEINJCTL0_KTI_CIOPHYDFX_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
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
    UINT32 maten : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 - No Header match, inject error on any header
                               (IIB==1), except exclude NULLs if bit[5]
                               EXCLUDE_NULL==1. 1 - Inject error based on
                               Header match MATF1 hit signal (ignores MATF2 and
                               HDmatOut hit signals)
                            */
    UINT32 rsvd_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 suppress_st : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error Injection suppression status 0 - No
                               suppression of injection has occurred. 1 -
                               Suppression of injection has occurred
                            */
    UINT32 errinjst : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               0 - No error injection occurred. 1 - An error
                               was injected
                            */
    UINT32 rsvd_6 : 10;

                            /* Bits[15:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 openloop_str_sel : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Start selection for injecting in open-loop
                               response function. Open-loop means that the
                               response function will only watch for the stop
                               condition and ignores the response from the
                               affected logic. 0000 - Micro Break Point 0
                               (MBP[0]) 0001 - Micro Break Point 1 (MBP[1])
                               0010 - Cluster Trigger Out 0 (CTO[0]) 0011 -
                               Cluster Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Response never occurs
                               1011 - Response occurs immediately 1100-1111 -
                               Reserved
                            */
    UINT32 openloop_stp_sel : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Stop selection for injecting in open-loop
                               response function. Open-loop means that the
                               response function will only watch for the stop
                               condition and ignores the response from the
                               affected logic. 0000 - Micro Break Point 0
                               (MBP[0]) 0001 - Micro Break Point 1 (MBP[1])
                               0010 - Cluster Trigger Out 0 (CTO[0]) 0011 -
                               Cluster Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Deactivate one clock
                               after activation. Error injection on one
                               transaction will occur on the interface that is
                               enabled. 1011 - Response is never deactivated.
                               1100 - Deactivate the response when the
                               activating signal becomes false. 1101-1111 -
                               Reserved
                            */
    UINT32 once_str_sel : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Start selection for injecting in standard
                               closed-loop response function. Closed-loop means
                               that once activated, the response function will
                               not stop until both the stop condition has been
                               met *and* the logic affected by the response
                               function has been able to do the
                               throttling/injecting on actual traffic. Closed-
                               loop is the most common response function. 0000
                               - Micro Break Point 0 (MBP[0]) 0001 - Micro
                               Break Point 1 (MBP[1]) 0010 - Cluster Trigger
                               Out 0 (CTO[0]) 0011 - Cluster Trigger Out 1
                               (CTO[1]) 0100 - Artificial Starvation Counter 0
                               (ASC[0]) 0101 - Artificial Starvation Counter 1
                               (ASC[1]) 0110 - Common Trigger Sequencer TrigOut
                               (CTS_Trig_Out) 0111 - Reserved 1000 - MCA
                               Correctable Error 1001 - MCA Uncorrectable Error
                               1010 - Response never occurs 1011 - Response
                               occurs immediately 1100-1111 - Reserved
                            */
    UINT32 once_stp_sel : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Stop selection for injecting in standard closed-
                               loop response function. Closed-loop means that
                               once activated, the response function will not
                               stop until both the stop condition has been met
                               *and* the logic affected by the response
                               function has been able to do the
                               throttling/injecting on actual traffic. Closed-
                               loop is the most common response function. 0000
                               - Micro Break Point 0 (MBP[0]) 0001 - Micro
                               Break Point 1 (MBP[1]) 0010 - Cluster Trigger
                               Out 0 (CTO[0]) 0011 - Cluster Trigger Out 1
                               (CTO[1]) 0100 - Artificial Starvation Counter 0
                               (ASC[0]) 0101 - Artificial Starvation Counter 1
                               (ASC[1]) 0110 - Common Trigger Sequencer TrigOut
                               (CTS_Trig_Out) 0111 - Reserved 1000 - MCA
                               Correctable Error 1001 - MCA Uncorrectable Error
                               1010 - Deactivate one clock after activation.
                               Error injection on one transaction will occur on
                               the interface that is enabled. 1011 - Response
                               is never deactivated. 1100 - Deactivate the
                               response when the activating signal becomes
                               false. 1101-1111 - Reserved
                            */

  } Bits;
  UINT32 Data;

} KTIRXEINJCTL0_KTI_CIOPHYDFX_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 maten : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               0 - No Header match, inject error on any header
                               (IIB==1), except exclude NULLs if bit[5]
                               EXCLUDE_NULL==1. 1 - Inject error based on
                               Header match MATF1 hit signal (ignores MATF2 and
                               HDmatOut hit signals)
                            */
    UINT32 rsvd_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 suppress_st : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error Injection suppression status 0 - No
                               suppression of injection has occurred. 1 -
                               Suppression of injection has occurred
                            */
    UINT32 errinjst : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               0 - No error injection occurred. 1 - An error
                               was injected
                            */
    UINT32 rsvd_6 : 10;

                            /* Bits[15:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 openloop_str_sel : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Start selection for injecting in open-loop
                               response function. Open-loop means that the
                               response function will only watch for the stop
                               condition and ignores the response from the
                               affected logic. 0000 - Micro Break Point 0
                               (MBP[0]) 0001 - Micro Break Point 1 (MBP[1])
                               0010 - Cluster Trigger Out 0 (CTO[0]) 0011 -
                               Cluster Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Response never occurs
                               1011 - Response occurs immediately 1100-1111 -
                               Reserved
                            */
    UINT32 openloop_stp_sel : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Stop selection for injecting in open-loop
                               response function. Open-loop means that the
                               response function will only watch for the stop
                               condition and ignores the response from the
                               affected logic. 0000 - Micro Break Point 0
                               (MBP[0]) 0001 - Micro Break Point 1 (MBP[1])
                               0010 - Cluster Trigger Out 0 (CTO[0]) 0011 -
                               Cluster Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Deactivate one clock
                               after activation. Error injection on one
                               transaction will occur on the interface that is
                               enabled. 1011 - Response is never deactivated.
                               1100 - Deactivate the response when the
                               activating signal becomes false. 1101-1111 -
                               Reserved
                            */
    UINT32 once_str_sel : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Start selection for injecting in standard
                               closed-loop response function. Closed-loop means
                               that once activated, the response function will
                               not stop until both the stop condition has been
                               met *and* the logic affected by the response
                               function has been able to do the
                               throttling/injecting on actual traffic. Closed-
                               loop is the most common response function. 0000
                               - Micro Break Point 0 (MBP[0]) 0001 - Micro
                               Break Point 1 (MBP[1]) 0010 - Cluster Trigger
                               Out 0 (CTO[0]) 0011 - Cluster Trigger Out 1
                               (CTO[1]) 0100 - Artificial Starvation Counter 0
                               (ASC[0]) 0101 - Artificial Starvation Counter 1
                               (ASC[1]) 0110 - Common Trigger Sequencer TrigOut
                               (CTS_Trig_Out) 0111 - Reserved 1000 - MCA
                               Correctable Error 1001 - MCA Uncorrectable Error
                               1010 - Response never occurs 1011 - Response
                               occurs immediately 1100-1111 - Reserved
                            */
    UINT32 once_stp_sel : 4;

                            /* Bits[31:28], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Stop selection for injecting in standard closed-
                               loop response function. Closed-loop means that
                               once activated, the response function will not
                               stop until both the stop condition has been met
                               *and* the logic affected by the response
                               function has been able to do the
                               throttling/injecting on actual traffic. Closed-
                               loop is the most common response function. 0000
                               - Micro Break Point 0 (MBP[0]) 0001 - Micro
                               Break Point 1 (MBP[1]) 0010 - Cluster Trigger
                               Out 0 (CTO[0]) 0011 - Cluster Trigger Out 1
                               (CTO[1]) 0100 - Artificial Starvation Counter 0
                               (ASC[0]) 0101 - Artificial Starvation Counter 1
                               (ASC[1]) 0110 - Common Trigger Sequencer TrigOut
                               (CTS_Trig_Out) 0111 - Reserved 1000 - MCA
                               Correctable Error 1001 - MCA Uncorrectable Error
                               1010 - Deactivate one clock after activation.
                               Error injection on one transaction will occur on
                               the interface that is enabled. 1011 - Response
                               is never deactivated. 1100 - Deactivate the
                               response when the activating signal becomes
                               false. 1101-1111 - Reserved
                            */

  } Bits;
  UINT32 Data;

} KTIRXEINJCTL0_KTI_CIOPHYDFX_STRUCT;

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIRXEINJCTL1_KTI_CIOPHYDFX_REG supported on:                                */
/*      SPRA0 (0x21e0b2b4)                                                      */
/*      SPRB0 (0x21e0b2b4)                                                      */
/*      SPRHBM (0x21e0b2b4)                                                     */
/*      SPRC0 (0x21e0b2b4)                                                      */
/*      SPRMCC (0x21e0b2b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* ktirxeinjctl1
*/


#define KTIRXEINJCTL1_KTI_CIOPHYDFX_REG 0x092202B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 exclude_data : 1;

                            /* Bits[2:2], Access Type=RW/P/L, default=0x00000000*/

                            /* Exclude Data Flits */
    UINT32 exclude_llctrl_null : 1;

                            /* Bits[3:3], Access Type=RW/P/L, default=0x00000000*/

                            /* Exclude LLCTRL.NULL */
    UINT32 rsvd_4 : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 exclude_llctrl_all : 1;

                            /* Bits[5:5], Access Type=RW/P/L, default=0x00000000*/

                            /* Exclude LLCTRL All */
    UINT32 exclude_llcrd : 1;

                            /* Bits[6:6], Access Type=RW/P/L, default=0x00000000*/

                            /* Exclude LLCRD */
    UINT32 exclude_llctrl_init_all : 1;

                            /* Bits[7:7], Access Type=RW/P/L, default=0x00000000*/

                            /* Exclude LLCTRL.INIT All */
    UINT32 exclude_llctrl_retry_req : 1;

                            /* Bits[8:8], Access Type=RW/P/L, default=0x00000000*/

                            /* Exclude LLCTRL.RETRY_REQ */
    UINT32 exclude_llctrl_retry_ack : 1;

                            /* Bits[9:9], Access Type=RW/P/L, default=0x00000000*/

                            /* Exclude LLCTRL.RETRY_ACK */
    UINT32 spare13_10 : 4;

                            /* Bits[13:10], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 rsvd_14 : 6;

                            /* Bits[19:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 include_llcrd : 1;

                            /* Bits[20:20], Access Type=RW/P/L, default=0x00000000*/

                            /* Include LLCRD */
    UINT32 rsvd_21 : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 include_llctrl_retry_req : 1;

                            /* Bits[24:24], Access Type=RW/P/L, default=0x00000000*/

                            /* Include LLCTRL.RETRY_REQ */
    UINT32 include_llctrl_retry_ack : 1;

                            /* Bits[25:25], Access Type=RW/P/L, default=0x00000000*/

                            /* Include LLCTRL.RETRY_ACK */
    UINT32 spare31_26 : 6;

                            /* Bits[31:26], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */

  } Bits;
  UINT32 Data;

} KTIRXEINJCTL1_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

/* KTILKQEINJST_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b2b8)                                                      */
/*      SPRB0 (0x21e0b2b8)                                                      */
/*      SPRHBM (0x21e0b2b8)                                                     */
/*      SPRC0 (0x21e0b2b8)                                                      */
/*      SPRMCC (0x21e0b2b8)                                                     */
/*      SPRUCC (0x21e492b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ktilkqeinjst
*/


#define KTILKQEINJST_KTI_CIOPHYDFX_REG 0x092202B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxeinj_once_st : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* RX error injection once status */
    UINT32 rxeinj_openloop_st : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* RX error injection continuous status */
    UINT32 pvaeinj_st : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Poison/Viral/Address injection status */
    UINT32 pareinj_st : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error injection status */
    UINT32 rsvd_5 : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTILKQEINJST_KTI_CIOPHYDFX_STRUCT;

/* KTILKPRIVS_KTI_CIOPHYDFX_REG supported on:                                   */
/*      SPRA0 (0x21e0b2c8)                                                      */
/*      SPRB0 (0x21e0b2c8)                                                      */
/*      SPRHBM (0x21e0b2c8)                                                     */
/*      SPRC0 (0x21e0b2c8)                                                      */
/*      SPRMCC (0x21e0b2c8)                                                     */
/*      SPRUCC (0x21e492c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ktilkprivs
*/


#define KTILKPRIVS_KTI_CIOPHYDFX_REG 0x092202C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 crc_err_phy_rst : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates that a PHY reset was requested to
                               recover from CRC errors
                            */
    UINT32 spare1 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* spare */
    UINT32 spare2 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* spare */
    UINT32 spare3 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* spare */
    UINT32 spare4 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* spare */
    UINT32 spare5 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* spare */
    UINT32 spare6 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* spare */
    UINT32 spare7 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* spare */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTILKPRIVS_KTI_CIOPHYDFX_STRUCT;

/* KTILKPRIVC0_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b2d8)                                                      */
/*      SPRB0 (0x21e0b2d8)                                                      */
/*      SPRHBM (0x21e0b2d8)                                                     */
/*      SPRC0 (0x21e0b2d8)                                                      */
/*      SPRMCC (0x21e0b2d8)                                                     */
/*      SPRUCC (0x21e492d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* ktilkprivc0
*/


#define KTILKPRIVC0_KTI_CIOPHYDFX_REG 0x092202D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxq_ck_gate_dis : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Disable clock gating on selected LL Rxq flops */
    UINT32 ctl_ck_gate_dis : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable clock gating on selected LL Control
                               flops
                            */
    UINT32 rx_ck_gate_dis : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* Disable clock gating on selected LL Rx flops */
    UINT32 tx_ck_gate_dis : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* Disable clock gating on selected LL Tx flops */
    UINT32 cfg_ck_gate_dis : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable clock gating on downstream (of MsgCh
                               Endpoint) config flops
                            */
    UINT32 msgch_ck_gate_dis : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* Disable clock gating on Message Channel Endpoint */
    UINT32 rxbgf_ck_gate_dis : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable clock gating on the credit trackers for
                               RxHdrDataBgf and RxCrdBgf.
                            */
    UINT32 rcdt_ck_gate_dis : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable clock gating on selected LL Remote
                               credit return flops
                            */
    UINT32 rfp_dis_rxchk_invalid_pkt : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable LL Rx Invalid Packet check. The 'Invalid
                               Packet Check' looks for any Message Class +
                               Opcode combination that is RSVD in the UPI spec.
                               If a such an invalid packet is seen, it is
                               logged as part of the 'RXUUU Unsupported Packet'
                               error
                            */
    UINT32 rfp_dis_rxchk_invalid_fields : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable LL Rx Invalid Fields check. The Invalid
                               Fields Check looks for any of the following in
                               received flits: - any RSVD bits of a field
                               (Addr[51:46], RHTID[10]) with non-zero value -
                               VN=1 while KTILCCL.VN1_Enable=0 If such an
                               invalid field is seen, it is logged as part of
                               the RXUUU Unsupported Packet error
                            */
    UINT32 dis_check_llctrl_fixed_field_for_ktispec1p0 : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disables checking of 'fixed fields' for LLCTRL
                               flits. This disable squashes all Fixed Field
                               Error actions (CRC injection and error logging).
                               Note: there are individual disables for Fixed
                               Field CRC injection (KTILKPRIVC0.rfp_dis_rx_llct
                               rl_fixed_field_err_retryinj) and Fixed Field
                               error logging
                               (KTIDBGERRSTDIS0.disable_rxuuu[10]), as well as
                               a BIOS-controllable Fixed Field error logging
                               enable (KTIAGCTRL.fixed_field_reporting).
                            */
    UINT32 tr_infinite_llr_timeout : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               Allows an infinite number of CRC-errored flits
                               (LRSM never times out)
                            */
    UINT32 tr_infinite_llr_numretry : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               Allows an infinite number of retries (ignore
                               MAX_NUM_RETRY)
                            */
    UINT32 tr_infinite_llr_numphyreinit : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               Allows an infinite number of phy reinits (ignore
                               MAX_NUM_PHYREINIT)
                            */
    UINT32 tr_txllrdal : 8;

                            /* Bits[21:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               Link Layer Retry Queue Deallocate Entries -
                               indicates how many buffers in the LLRB should be
                               treated as not useable (they are added to the
                               minimum NumFreeBuf checks). T<span
                               style=&#39;font-family: "Trebuchet MS",sans-
                               serif; font-size: 10pt; mso-fareast-font-family:
                               Calibri; mso-fareast-theme-font: minor-latin;
                               mso-bidi-font-family: "Times New Roman"; mso-
                               ansi-language: EN-US; mso-fareast-language: EN-
                               US; mso-bidi-language: AR-SA;&#39;><font
                               color="#000000">he minimum number of free
                               buffers that can be programmed needs to be at
                               the most (maximum_llrb_depth_value -
                               6)</font></span>
                            */
    UINT32 enable_adapt_inhibit : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable Phy Rx Adapt Inhibit (which puts the
                               UniPhy Rx into openloop) when LRSM activated.
                               This is a surviveability feature only, there are
                               no known issues with the UPI definition that
                               would require use of it.
                            */
    UINT32 rfp_dis_rx_llctrl_fixed_field_err_retryinj : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable LL Rx LLCTRL Fixed Field Error retry
                               injection The 'LLCTRL Fixed Field Check' looks
                               for any fixed (hard-coded 0/1) LLCTRL fields not
                               matching the UPI spec. This disable bit has the
                               following meanings: If set to 1, disable retry
                               injection action even if a Fixed Field Error
                               occurs. If set to 0, enable retry injection
                               action if a Fixed Field Error occurs. A Fixed
                               Field Error will occur if any of the following
                               are true: - slot2 payload not-equal-to 0 - slot2
                               opcode not-equal-to LLCRD - slot1 msgclass not-
                               equal-to SNP - slot1 opcode not-equal-to
                               Slot_NULL - FlitControl CRD/ACK/VNA/VN not-
                               equal-to 0
                            */
    UINT32 force_null_or_retry : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set the Tx path on the LL will behave as
                               though the Null_or_Retry wire is asserted from
                               the phy. The intended usage of this mode is for
                               debug of phy layer to allow only known NULL
                               flits to be injected and to avoid all the
                               training flits.
                            */
    UINT32 spare25 : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare26 : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare27 : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare28 : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare29 : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 dis_mixed_rxq_slot_bypass : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disabled mixed RXQ slot bypass 0 - allow mixed
                               slot bypass 1 - disallow mixed slot bypass (ie.
                               either all slots bypass or none bypass)
                            */
    UINT32 force_cfgwrltlck_nonposted : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               Force all CfgWrLTLck requests targeting UPI
                               agent to be treated as non-posted. The purpose
                               is to revert to JKT/IVT/HSX QPI behavior (SKX
                               HSD 298055).
                            */

  } Bits;
  UINT32 Data;

} KTILKPRIVC0_KTI_CIOPHYDFX_STRUCT;

/* KTILKPRIVC1_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b2dc)                                                      */
/*      SPRB0 (0x21e0b2dc)                                                      */
/*      SPRHBM (0x21e0b2dc)                                                     */
/*      SPRC0 (0x21e0b2dc)                                                      */
/*      SPRMCC (0x21e0b2dc)                                                     */
/*      SPRUCC (0x21e492dc)                                                     */
/* Register default value on SPRA0: 0x10000000                                  */
/* Register default value on SPRB0: 0x10000000                                  */
/* Register default value on SPRHBM: 0x10000000                                 */
/* Register default value on SPRC0: 0x10000000                                  */
/* Register default value on SPRMCC: 0x10000000                                 */
/* Register default value on SPRUCC: 0x10000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* ktilkprivc1
*/


#define KTILKPRIVC1_KTI_CIOPHYDFX_REG 0x092202DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmh_sapm_ovr_value : 2;

                            /* Bits[1:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               SAPM (PowerModeSelect) Override Value (2-bits).
                               This value may be changed on-the-fly (in a
                               running system). 00 - config 0 best performance
                               01 - config 1 best power savings 10-11 -
                               reserved (ignored by hardware)
                            */
    UINT32 pmh_sapm_ovr_enable : 1;

                            /* Bits[2:2], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               SAPM (PowerModeSelect) Override Enable. This may
                               be enabled on-the-fly (in a running system). It
                               overrides the PCU controlled PMA Power Mode
                               Select[1:0] field with the value specified in
                               pmh_sapm_ovr_value.
                            */
    UINT32 pmh_force_l1_entry : 1;

                            /* Bits[3:3], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Forces power management heuristic to initiate
                               entry to L1 (1=force into). If
                               pmh_force_lx_use_en =1, then entry is also gated
                               by standard uCR enable bit. If
                               pmh_force_lx_use_en = 0, then entry is forcing
                               is controlled exclusive by this bit. Note: there
                               are several L1 force entry/exit conditions
                               controlled in KTILKPRIVC1 (and KTILKPMHFRC for
                               one case). This bit is priority #2A (out of 2).
                               #2A is identical priority to 2B. See
                               KTILKPMHFRC.force_l1_* for details on #2B. Note:
                               Using this DFX feature to force L1 entry can
                               cause a CRC retry to escalate to an
                               uncorrectable error. To avoid this (if forcing
                               L1 entry while doing CRC retry testing),
                               configure the Phy layer L1 Reissue Delay timer
                               (KTIREUT_PH_TL0C_SL.t_l1_reissue_delay and
                               KTIREUT_PH_TL0CENTRY_OP.t_l1_reissue_delay) to
                               the value of 2*L0c (KTIREUT_PH_LCS.s_tl0c).
                            */
    UINT32 pmh_force_l0p_entry : 1;

                            /* Bits[4:4], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Forces power management heuristic to initiate
                               entry to Tx L0p (1=force into). If
                               pmh_force_lx_use_en =1, then entry is also gated
                               by standard uCR enable bit. If
                               pmh_force_lx_use_en = 0, then entry is forcing
                               is controlled exclusive by this bit. Note: there
                               are several TxL0p force entry/exit conditions
                               controlled in KTILKPRIVC1. This bit is priority
                               #3A (out of 4). #3A is identical priority to 3B
                               and 3C.
                            */
    UINT32 pmh_disable_l1 : 1;

                            /* Bits[5:5], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Ensures L1 is disabled (will not be requested)
                               which overrides any other enable uCR or CSR
                               control. This bit may be used to force exit from
                               L1. Note: there are several L1 force entry/exit
                               conditions controlled in KTILKPRIVC1 (and
                               KTILKPMHFRC for one case). This bit is priority
                               #1 (out of 2).
                            */
    UINT32 pmh_disable_l0p : 1;

                            /* Bits[6:6], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Ensures Tx L0p is disabled (will not be
                               requested) which overrides any other enable uCR
                               or CSR control. This bit may be used to force
                               exit from TxL0p. Note: there are several TxL0p
                               force entry/exit conditions controlled in
                               KTILKPRIVC1. This bit is priority #1 (out of 4).
                            */
    UINT32 pmh_disable_m3_heur_l0p : 1;

                            /* Bits[7:7], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable M3 heuristic mechanism for Tx L0p
                               entry/exit. 0 - Will exit TxL0p (if currently in
                               TxL0p state) when M3 TxL0p exit signal goes
                               high. 1 - Will ignore M3 TxL0p exit signal.
                               Note: there are several TxL0p force entry/exit
                               conditions controlled in KTILKPRIVC1. This bit
                               is priority #4 (out of 4).
                            */
    UINT32 pmh_disable_ll_heur_l0p : 1;

                            /* Bits[8:8], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable Link Layer heuristic mechanism for Tx
                               L0p entry/exit. 0 - TxL0p leaky bucket heuristic
                               operational. 1 - TxL0p leaky bucket disabled,
                               will force immediate entry to L0p. Note: there
                               are several TxL0p force entry/exit conditions
                               controlled in KTILKPRIVC1. This bit is priority
                               #3C (out of 4). #3C is identical priority to 3A
                               and 3B.
                            */
    UINT32 pmh_l0p_resp_func_mode : 1;

                            /* Bits[9:9], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Tx L0p heuristic response function mode. This
                               has no effect unless the KTILKPMHFRC.force_l0p_*
                               is configured appropriately. 0 - force Txl0p
                               entry. Note: there are several TxL0p force
                               entry/exit conditions controlled in KTILKPRIVC1.
                               This bit with value 0 is priority #3B (out of
                               4). #3B is identical priority to 3A and 3C. 1 -
                               force TxL0p exit. Note: there are several TxL0p
                               force entry/exit conditions controlled in
                               KTILKPRIVC1. This bit with value 1 is priority
                               #2 (out of 4).
                            */
    UINT32 pmh_disable_l1idletimer_reset_on_lrsm : 1;

                            /* Bits[10:10], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable reset of L1 Idle Timer on LRSM-non-
                               normal
                            */
    UINT32 pmh_disable_l1idletimer_reset_on_rrsm : 1;

                            /* Bits[11:11], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable reset of L1 Idle Timer on RRSM-non-
                               normal
                            */
    UINT32 pma_allow_l1_ovrd : 1;

                            /* Bits[12:12], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               When to set 1, overrides the PMA Allow L1 wire
                               to be 1 (true) to allow entry to L1.
                            */
    UINT32 pmh_disable_rx_idle_deassertion_on_lrsm : 1;

                            /* Bits[13:13], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable deassertion of Rx_Idle (ie. PkgCEmpty)
                               on LRSM-non-normal
                            */
    UINT32 pmh_disable_rx_idle_deassertion_on_rrsm : 1;

                            /* Bits[14:14], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable deassertion of Rx_Idle (ie. PkgCEmpty)
                               on RRSM-non-normal
                            */
    UINT32 ts_en_tx_active_dbg : 1;

                            /* Bits[15:15], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Added DEBUG state machine IDLE state to overall
                               LL Tx Idle condition (which feeds into SAPM
                               Complete Empty notification to PMA).
                            */
    UINT32 ts_stall_ts_hp_dbg : 1;

                            /* Bits[16:16], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Blocks sending of DEBUG high priority flits.
                               Note: By setting this bit to 1, it allows the Tx
                               Arbiter to take any downstream arcs.
                            */
    UINT32 ts_en_ts_retry_abort : 1;

                            /* Bits[17:17], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Enables sending of RETRY.IDLE flits due to LLR
                               Abort (LRSM) and blocks sending all Tx traffic
                               except NULLs or RETRY.IDLE flits. Note: this
                               would violate the UPI spec but could be used if
                               a customer desired immediate containment of Tx
                               traffic on an Rx LRSM Abort.
                            */
    UINT32 ts_stall_ts_retry_ack : 1;

                            /* Bits[18:18], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Blocks sending RETRY.ACK flits. Note this can
                               also end up blocking LLRAck due to RR
                               arbitration. Note: By setting this bit to 1, it
                               allows the Tx Arbiter to take any downstream
                               arcs.
                            */
    UINT32 ts_stall_ts_retry_req : 1;

                            /* Bits[19:19], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Blocks sending RETRY.REQ flits. Note this can
                               also end up blocking LLRAck due to RR
                               arbitration. Note: By setting this bit to 1, it
                               allows the Tx Arbiter to take any downstream
                               arcs.
                            */
    UINT32 ts_stall_ts_ackonly : 1;

                            /* Bits[20:20], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Blocks sending ACKONLY flits (ie. flits with ACK
                               or LLCRD.ACK set). Note: LLCRD can still be sent
                               if NumFreeBuf > 2 Note: By setting this bit to
                               1, it allows the Tx Arbiter to take any
                               downstream arcs.
                            */
    UINT32 ts_stall_ts_nonctrl : 1;

                            /* Bits[21:21], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Blocks sending non-LLCTRL flits. Note: only
                               affects flits from M3 (does not include LLCRD
                               flits when NumFreeBuf = 2) Note: By setting this
                               bit to 1, it allows the Tx Arbiter to take any
                               downstream arcs (in this case, that only leaves
                               NULLs).
                            */
    UINT32 la_read_modifier : 1;

                            /* Bits[22:22], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               eserved in UPI link layer because
                               KTIA2RCRCTRL0_ACTIVE and KTIA2RCRCTRL1_ACTIVE
                               available for reading. This bit is required
                               unless the component provides an alternative
                               means to read the effective CSR for debug
                               purposes. This bit should be considered Reserved
                               if component does not implement this ability. 1
                               - Return CSR Effective value on read 0 - Return
                               CSR value on read
                            */
    UINT32 dfx_rstexit_dlytimer_tsv_startval : 6;

                            /* Bits[28:23], Access Type=RW/P/L, default=0x00000020*/

                            /*
                               programmable start value for reset exit delay
                               timer
                            */
    UINT32 dfx_trigger_late_active_capture : 1;

                            /* Bits[29:29], Access Type=RW/P/L, default=0x00000000*/

                            /* manually loads late active state. */
    UINT32 pmh_rx_tx_idle_force_true : 1;

                            /* Bits[30:30], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Force rx_tx_idle condition to be true (used in
                               L1 heuristics)
                            */
    UINT32 init_done_force_true : 1;

                            /* Bits[31:31], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Force initialization (parameter exchange) done
                               state to be true (used in L0p and L1 heuristics
                               among other things)
                            */

  } Bits;
  UINT32 Data;

} KTILKPRIVC1_KTI_CIOPHYDFX_STRUCT;

/* KTILKPRIVC2_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b2f0)                                                      */
/*      SPRB0 (0x21e0b2f0)                                                      */
/*      SPRHBM (0x21e0b2f0)                                                     */
/*      SPRC0 (0x21e0b2f0)                                                      */
/*      SPRMCC (0x21e0b2f0)                                                     */
/*      SPRUCC (0x21e492f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* ktilkprivc2
*/


#define KTILKPRIVC2_KTI_CIOPHYDFX_REG 0x092202F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dfx_rxbyp_blockrx_en_allfc : 6;

                            /* Bits[5:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Blocks the following message classes from using
                               the bypass path: [0] REQ [1] SNP [2] RSP [3] WB
                               [4] NCB [5] NCS
                            */
    UINT32 dfx_rxqu_blockrx_en_allfc : 6;

                            /* Bits[11:6], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Blocks the following message classes from being
                               unloaded from the RxQ: [0] REQ [1] SNP [2] RSP
                               [3] WB [4] NCB [5] NCS
                            */
    UINT32 dfx_viral_blockrx_en : 1;

                            /* Bits[12:12], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Blocks Sending Viral indication through the
                               RxCrd BGF. Note - this bit also forces the Viral
                               indicator to zero if it was set.
                            */
    UINT32 dfx_fatal_blockrx_en : 1;

                            /* Bits[13:13], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Blocks Sending Fatal indication through the
                               RxCrd BGF. Note - this bit also forces the Fatal
                               indicator to zero if it was set.
                            */
    UINT32 dfx_l0p_blockrx_en : 1;

                            /* Bits[14:14], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Blocks Sending TxL0p indication through the
                               RxCrd BGF. Note - this bit also forces the TxL0p
                               indicator to zero if it was set.
                            */
    UINT32 dfx_ln_sync_rst_blockrx_en : 1;

                            /* Bits[15:15], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Blocks sending Sync Reset to M3UPI through the
                               RxCrd BGF
                            */
    UINT32 dfx_txq_cdt_blockrx_en : 1;

                            /* Bits[16:16], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Blocks return of TxQ credits to M3UPI through
                               the RxCrd BGF
                            */
    UINT32 dfx_remcdt_blockrx_en : 1;

                            /* Bits[17:17], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Blocks return of remote credits to M3UPI by
                               blocking writes to the Rx Crd BGF
                            */
    UINT32 dfx_rxqu_rx2r3_throt_dis_allfc : 6;

                            /* Bits[23:18], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disables Response Logic Throttling in the RxQ
                               path of the following message classes: [0] REQ
                               [1] SNP [2] RSP [3] WB [4] NCB [5] NCS
                            */
    UINT32 dfx_rxbyp_rx2r3_throt_dis_allfc : 6;

                            /* Bits[29:24], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disables Response Logic Throttling in the Bypass
                               path of the following message classes: [0] REQ
                               [1] SNP [2] RSP [3] WB [4] NCB [5] NCS
                            */
    UINT32 override_enable_debug_flits : 1;

                            /* Bits[30:30], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               1 - Enable debug flits even if
                               KTILCL.enable_debug_flits==0. 0 - Obey
                               KTILCL.enable_debug_flits control.
                            */
    UINT32 clear_rtp_timestamp_value : 1;

                            /* Bits[31:31], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Clear Relative Timing Packet Timestamp (must
                               write a 1 to clear and then write a 0 to stop
                               clearing)
                            */

  } Bits;
  UINT32 Data;

} KTILKPRIVC2_KTI_CIOPHYDFX_STRUCT;

/* KTILKPRIVC3_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b2f4)                                                      */
/*      SPRB0 (0x21e0b2f4)                                                      */
/*      SPRHBM (0x21e0b2f4)                                                     */
/*      SPRC0 (0x21e0b2f4)                                                      */
/*      SPRMCC (0x21e0b2f4)                                                     */
/*      SPRUCC (0x21e492f4)                                                     */
/* Register default value on SPRA0: 0x01400085                                  */
/* Register default value on SPRB0: 0x01400085                                  */
/* Register default value on SPRHBM: 0x01400085                                 */
/* Register default value on SPRC0: 0x01400085                                  */
/* Register default value on SPRMCC: 0x01400085                                 */
/* Register default value on SPRUCC: 0x01400085                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRB0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRHBM Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRC0 Security PolicyGroup: LB_MCHECK_W                                      */
/* SPRMCC Security PolicyGroup: LB_MCHECK_W                                     */
/* SPRUCC Security PolicyGroup: LB_MCHECK_W                                     */
/* ktilkprivc3
*/


#define KTILKPRIVC3_KTI_CIOPHYDFX_REG 0x092202F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flowq_ad_vna_threshold : 7;

                            /* Bits[6:0], Access Type=RW/P, default=0x00000005*/

                            /*
                               - if all-FlowQ-VNA check is false (ie false
                               condition of AD_VNA greater-than 2 -and- BL_VNA
                               greater-than 0 -and- optional AK_VNA pre-
                               allocated), then AD packets may still consume
                               AD_VNA FlowQ credits if AD_VNA greater-than
                               threshold. Default is 5 (aka (3+3)-1). - program
                               this field to all ones (7'h7f) to disable this
                               feature Legal range: 7'h5 to 7'h7f (any value
                               greater than the allocated AK_VNA credits in
                               KTIA2RCRCTRL will effectively disable this
                               feature)
                            */
    UINT32 flowq_bl_vna_threshold : 7;

                            /* Bits[13:7], Access Type=RW/P, default=0x00000001*/

                            /*
                               - if all-FlowQ-VNA check is false (ie false
                               condition of AD_VNA greater-than 2 -and- BL_VNA
                               greater-than 0 -and- optional AK_VNA pre-
                               allocated), then BL packets may still consume
                               BL_VNA FlowQ credits if BL_VNA greater-than
                               threshold. Default is 1 (aka (1+1)-1). - program
                               this field to all ones (7'h7f) to disable this
                               feature Legal range: 7'h1 to 7'h7f (any value
                               greater than the allocated BL_VNA credits in
                               KTIA2RCRCTRL will effectively disable this
                               feature)
                            */
    UINT32 flowq_ak_vna_prealloc_all_en : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               FlowQ AK VNA pre-allocation enable 0 - do not
                               require AK_VNA pre-allocated for AD/BL traffic
                               to flow (except for WB which require BL+AK) 1 -
                               enable optional part of all-FlowQ-VNA check (ie
                               AD_VNA greater-than 2 -and- BL_VNA greater-than
                               0 -and- optional AK_VNA pre-allocated) for AD/BL
                               traffic to flow.
                            */
    UINT32 llrq_req_outofbounds_check_disable : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               Link Layer Retry Queue Request Out-of-Bounds
                               Check Disable This check monitors if an incoming
                               LLCTRL-RETRY.Req has an ESeq that is out-of-
                               bounds with respect to the NumFreeBuf. 0 - Delta
                               check is enable 1 - Delta check is disabled
                            */
    UINT32 enable_rsp2_mircmpu : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               nable RSP-2 MirCmpU opcode 0 - MirCmpU opcode
                               not supported, will log MCA error if received 1
                               - MirCmpU opcode will be accepted on receiver
                               and passed up to the mesh stop.
                            */
    UINT32 disable_txperr_override_mcacod : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable Tx Parity Error override of MCACOD to
                               value 0x0405 0 - enable override 1 - disable
                               override
                            */
    UINT32 force_vga2defiio_if_outside_legio_range : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Force NcIORd/Wr VGA (LegacyIO) hits to Default
                               IIO destination meshid if address is outside the
                               LegacyIO range. 0 - let VGA (LegacyIO) hits
                               target the VGA destination meshid 1 - force VGA
                               (LegacyIO) hits to Default IIO
                            */
    UINT32 decouple_worstnumretry_from_worstnumphyreinit : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               Decouple Worst_NUM_RETRY from
                               Worst_NUM_PHYREINIT 0 - Worst_NUM_RETRY obeys
                               UPI spec rules by incrementing only when part of
                               the current worst NUM_PHYREINIT. 1 -
                               Worst_NUM_RETRY will increment independent of
                               Worst_NUM_PHYREINIT.
                            */
    UINT32 enable_txviral_inj_on_rxviral : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable Tx Viral Injection on receipt of Rx Viral
                               0 - Don't inject Tx Viral when receive Rx Viral
                               1 - Inject Tx Viral when receive Rx Viral
                            */
    UINT32 enable_late_action_credit_reset_on_llrst : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable Late Action Credit Reset (lncm_warm_rst)
                               on LL reset 0 - asserts only on warm reset 1 -
                               asserts on both warm reset and LL reset
                            */
    UINT32 rbt_wr_enable : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               RBT Write Enable Enables writing the RBT on the
                               Tx path. 0 - Disable writes and reads from RBT 1
                               - Enable writes to RBT. Reads can be
                               independently controlled by
                               KTIAGCTRL.rbt_rd_enable. Note: disabling writes
                               can result in stale RBT data, so it is
                               recommended to avoid re-enabling writes
                               directly. Instead, perform a warm reset which
                               will re-enable writes in a safe manner.
                            */
    UINT32 squash_slot1_rdpref_on_slot0_ad_pkt : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               Squash Slot 1 Read Prefetch if Slot 0 is AD
                               Packet on same write into Rx Header BGF. 0 -
                               Allow Slot 1 Read Prefetch to occur independent
                               of Slot 0 packet type. 1 - Squash Slot 1 Read
                               Prefetch when Slot 0 is AD Packet (regardless of
                               whether Slot 0 may or may not qualify for read
                               prefetch)
                            */
    UINT32 disable_mem_rd_prefetch_nonsnprd : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This bit defeatures Intel UPI generation of
                               memory read prefetches for NonSnpRd. The default
                               value is such that Intel UPI prefetch for
                               NonSnpRd is disabled. For platforms that require
                               UPI prefetches to be sent for NonSnpRds this bit
                               needs to be set to a 1.
                            */
    UINT32 cmc_msgch_interrupt_dis : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* CMC Message Channel Interrupt Disable */
    UINT32 umc_msgch_interrupt_dis : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* UMC Message Channel Interrupt Disable */
    UINT32 smi_msgch_interrupt_dis : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* SMI Message Channel Interrupt Disable */
    UINT32 csmi_msgch_interrupt_dis : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /* CSMI Message Channel Interrupt Disable */
    UINT32 msmi_msgch_interrupt_dis : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* MSMI Message Channel Interrupt Disable */
    UINT32 pmon_msgch_interrupt_dis : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               PerfMon Overflow Message Channel Interrupt
                               Disable
                            */
    UINT32 spare31 : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /* spare */

  } Bits;
  UINT32 Data;

} KTILKPRIVC3_KTI_CIOPHYDFX_STRUCT;

/* KTILKPRIVC4_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b2f8)                                                      */
/*      SPRB0 (0x21e0b2f8)                                                      */
/*      SPRHBM (0x21e0b2f8)                                                     */
/*      SPRC0 (0x21e0b2f8)                                                      */
/*      SPRMCC (0x21e0b2f8)                                                     */
/*      SPRUCC (0x21e492f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* ktilkprivc4
*/


#define KTILKPRIVC4_KTI_CIOPHYDFX_REG 0x092202F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 spare0 : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare1 : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare2 : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare3 : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare4 : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare5 : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare6 : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare7 : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare8 : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare9 : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare10 : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare11 : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare12 : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare13 : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare14 : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare15 : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare16 : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare17 : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare18 : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare19 : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare20 : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare21 : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare22 : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare23 : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare24 : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare25 : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare26 : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare27 : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* spare */
    UINT32 spare28_nonsticky : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /* spare */
    UINT32 spare29_nonsticky : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* spare */
    UINT32 spare30_nonsticky : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* spare */
    UINT32 spare31_nonsticky : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* spare */

  } Bits;
  UINT32 Data;

} KTILKPRIVC4_KTI_CIOPHYDFX_STRUCT;

/* KTILKPRIVC5_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b2fc)                                                      */
/*      SPRB0 (0x21e0b2fc)                                                      */
/*      SPRHBM (0x21e0b2fc)                                                     */
/*      SPRC0 (0x21e0b2fc)                                                      */
/*      SPRMCC (0x21e0b2fc)                                                     */
/*      SPRUCC (0x21e492fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ktilkprivc5
*/


#define KTILKPRIVC5_KTI_CIOPHYDFX_REG 0x092202FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 spare0 : 1;

                            /* Bits[0:0], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare1 : 1;

                            /* Bits[1:1], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare2 : 1;

                            /* Bits[2:2], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare3 : 1;

                            /* Bits[3:3], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare4 : 1;

                            /* Bits[4:4], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare5 : 1;

                            /* Bits[5:5], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare6 : 1;

                            /* Bits[6:6], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare7 : 1;

                            /* Bits[7:7], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare8 : 1;

                            /* Bits[8:8], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare9 : 1;

                            /* Bits[9:9], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare10 : 1;

                            /* Bits[10:10], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare11 : 1;

                            /* Bits[11:11], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare12 : 1;

                            /* Bits[12:12], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare13 : 1;

                            /* Bits[13:13], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare14 : 1;

                            /* Bits[14:14], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare15 : 1;

                            /* Bits[15:15], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare16 : 1;

                            /* Bits[16:16], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare17 : 1;

                            /* Bits[17:17], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare18 : 1;

                            /* Bits[18:18], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare19 : 1;

                            /* Bits[19:19], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare20 : 1;

                            /* Bits[20:20], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare21 : 1;

                            /* Bits[21:21], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare22 : 1;

                            /* Bits[22:22], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare23 : 1;

                            /* Bits[23:23], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare24 : 1;

                            /* Bits[24:24], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare25 : 1;

                            /* Bits[25:25], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare26 : 1;

                            /* Bits[26:26], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare27 : 1;

                            /* Bits[27:27], Access Type=RW/P/L, default=0x00000000*/

                            /* spare */
    UINT32 spare28_nonsticky : 1;

                            /* Bits[28:28], Access Type=RW/L, default=0x00000000*/

                            /* spare */
    UINT32 spare29_nonsticky : 1;

                            /* Bits[29:29], Access Type=RW/L, default=0x00000000*/

                            /* spare */
    UINT32 spare30_nonsticky : 1;

                            /* Bits[30:30], Access Type=RW/L, default=0x00000000*/

                            /* spare */
    UINT32 spare31_nonsticky : 1;

                            /* Bits[31:31], Access Type=RW/L, default=0x00000000*/

                            /* spare */

  } Bits;
  UINT32 Data;

} KTILKPRIVC5_KTI_CIOPHYDFX_STRUCT;

/* KTIATMCTL0_KTI_CIOPHYDFX_REG supported on:                                   */
/*      SPRA0 (0x21e0b358)                                                      */
/*      SPRB0 (0x21e0b358)                                                      */
/*      SPRHBM (0x21e0b358)                                                     */
/*      SPRC0 (0x21e0b358)                                                      */
/*      SPRMCC (0x21e0b358)                                                     */
/*      SPRUCC (0x21e49358)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ktiatmctl0
*/


#define KTIATMCTL0_KTI_CIOPHYDFX_REG 0x09220358

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ll_atmmode_enable : 1;

                            /* Bits[2:2], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Enable ATM operation. This also functions as TXQ
                               bypass disable.
                            */
    UINT32 rsvd_3 : 3;

                            /* Bits[5:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 flip_addr_31_6 : 26;

                            /* Bits[31:6], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Address bit control for ATM. A 1 in any bit
                               position will flip the corresponding address bit
                               in the packet before it is looped back inot the
                               RX path.
                            */

  } Bits;
  UINT32 Data;

} KTIATMCTL0_KTI_CIOPHYDFX_STRUCT;

/* KTIATMCTL1_KTI_CIOPHYDFX_REG supported on:                                   */
/*      SPRA0 (0x21e0b35c)                                                      */
/*      SPRB0 (0x21e0b35c)                                                      */
/*      SPRHBM (0x21e0b35c)                                                     */
/*      SPRC0 (0x21e0b35c)                                                      */
/*      SPRMCC (0x21e0b35c)                                                     */
/*      SPRUCC (0x21e4935c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ktiatmctl1
*/


#define KTIATMCTL1_KTI_CIOPHYDFX_REG 0x0922035C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 flip_addr_51_32 : 20;

                            /* Bits[19:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Address bit control for ATM. A 1 in any bit
                               position will flip the corresponding address bit
                               in the packet before it is looped back inot the
                               RX path.
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIATMCTL1_KTI_CIOPHYDFX_STRUCT;

/* KTIATMCTL2_KTI_CIOPHYDFX_REG supported on:                                   */
/*      SPRA0 (0x21e0b360)                                                      */
/*      SPRB0 (0x21e0b360)                                                      */
/*      SPRHBM (0x21e0b360)                                                     */
/*      SPRC0 (0x21e0b360)                                                      */
/*      SPRMCC (0x21e0b360)                                                     */
/*      SPRUCC (0x21e49360)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ktiatmctl2
*/


#define KTIATMCTL2_KTI_CIOPHYDFX_REG 0x09220360

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 14;

                            /* Bits[13:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 flip_dnid_3_0 : 4;

                            /* Bits[17:14], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               DNID field. It's a 3b field , MSB is unused.
                               Legal programming is 0-7
                            */
    UINT32 flip_rnid_3_0 : 4;

                            /* Bits[21:18], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               RNID field. Its a 3b field, MSB is unused. Legal
                               programming os 0-7
                            */
    UINT32 flip_snid_3_0 : 4;

                            /* Bits[25:22], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               SNID field. Its a 3b field, MSB is unused. Legal
                               programming os 0-7
                            */
    UINT32 flip_cnid_3_0 : 4;

                            /* Bits[29:26], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               CNID field. Its a 3b field, MSB is unused. Legal
                               programming os 0-7
                            */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIATMCTL2_KTI_CIOPHYDFX_STRUCT;

/* KTILKQTHR0_KTI_CIOPHYDFX_REG supported on:                                   */
/*      SPRA0 (0x21e0b36c)                                                      */
/*      SPRB0 (0x21e0b36c)                                                      */
/*      SPRHBM (0x21e0b36c)                                                     */
/*      SPRC0 (0x21e0b36c)                                                      */
/*      SPRMCC (0x21e0b36c)                                                     */
/*      SPRUCC (0x21e4936c)                                                     */
/* Register default value on SPRA0: 0x00CCCACA                                  */
/* Register default value on SPRB0: 0x00CCCACA                                  */
/* Register default value on SPRHBM: 0x00CCCACA                                 */
/* Register default value on SPRC0: 0x00CCCACA                                  */
/* Register default value on SPRMCC: 0x00CCCACA                                 */
/* Register default value on SPRUCC: 0x00CCCACA                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ktilkqthr0
*/


#define KTILKQTHR0_KTI_CIOPHYDFX_REG 0x0922036C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txackstr_sel : 4;

                            /* Bits[3:0], Access Type=RW/P/L, default=0x0000000A*/

                            /*
                               Start Select encodings: 0000 - Micro Break Point
                               0 (MBP[0]) 0001 - Micro Break Point 1 (MBP[1])
                               0010 - Cluster Trigger Out 0 (CTO[0]) 0011 -
                               Cluster Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Response never occurs
                               1011 - Response occurs immediately 1100-1111 -
                               Reserved
                            */
    UINT32 txackstp_sel : 4;

                            /* Bits[7:4], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               Stop Select encoding: 0000 - Micro Break Point 0
                               (MBP[0]) 0001 - Micro Break Point 1 (MBP[1])
                               0010 - Cluster Trigger Out 0 (CTO[0]) 0011 -
                               Cluster Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Deactivate one clock
                               after activation. Error injection on one
                               transaction will occur on the interface that is
                               enabled. 1011 - Response is never deactivated.
                               1100 - Deactivate the response when the
                               activating signal becomes false. 1101-1111 -
                               Reserved
                            */
    UINT32 txllrqstr_sel : 4;

                            /* Bits[11:8], Access Type=RW/P/L, default=0x0000000A*/

                            /*
                               Start Select encodings: 0000 - Micro Break Point
                               0 (MBP[0]) 0001 - Micro Break Point 1 (MBP[1])
                               0010 - Cluster Trigger Out 0 (CTO[0]) 0011 -
                               Cluster Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Response never occurs
                               1011 - Response occurs immediately 1100-1111 -
                               Reserved
                            */
    UINT32 txllrqstp_sel : 4;

                            /* Bits[15:12], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               Stop Select encoding: 0000 - Micro Break Point 0
                               (MBP[0]) 0001 - Micro Break Point 1 (MBP[1])
                               0010 - Cluster Trigger Out 0 (CTO[0]) 0011 -
                               Cluster Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Deactivate one clock
                               after activation. Error injection on one
                               transaction will occur on the interface that is
                               enabled. 1011 - Response is never deactivated.
                               1100 - Deactivate the response when the
                               activating signal becomes false. 1101-1111 -
                               Reserved
                            */
    UINT32 rx2r3str_sel : 4;

                            /* Bits[19:16], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               Start Select encodings: 0000 - Micro Break Point
                               0 (MBP[0]) 0001 - Micro Break Point 1 (MBP[1])
                               0010 - Cluster Trigger Out 0 (CTO[0]) 0011 -
                               Cluster Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Response never occurs
                               1011 - Response occurs immediately 1100-1111 -
                               Reserved
                            */
    UINT32 rx2r3stp_sel : 4;

                            /* Bits[23:20], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               Stop Select encoding: 0000 - Micro Break Point 0
                               (MBP[0]) 0001 - Micro Break Point 1 (MBP[1])
                               0010 - Cluster Trigger Out 0 (CTO[0]) 0011 -
                               Cluster Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Deactivate one clock
                               after activation. Error injection on one
                               transaction will occur on the interface that is
                               enabled. 1011 - Response is never deactivated.
                               1100 - Deactivate the response when the
                               activating signal becomes false. 1101-1111 -
                               Reserved
                            */
    UINT32 rx2r3_throt_rxqu_hdrdata_dis : 1;

                            /* Bits[24:24], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable response function throttling of Rx-to-M3
                               Header/Data flits via Rxq path into Rx Credit
                               BGF
                            */
    UINT32 rx2r3_throt_rxbyp_hdrdata_dis : 1;

                            /* Bits[25:25], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable response function throttling of Rx-to-M3
                               Header/Data flits via Rxq bypass path into Rx
                               Credit BGF
                            */
    UINT32 rx2r3_throt_txqcdt_dis : 1;

                            /* Bits[26:26], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable response function throttling of Rx-to-M3
                               Transmit Queue credit returns into Rx Credit BGF
                            */
    UINT32 rx2r3_throt_remcdt_dis : 1;

                            /* Bits[27:27], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable response function throttling of Rx-to-M3
                               Remote Credit Returns into Rx Credit BGF
                            */
    UINT32 rx2r3_throt_viral_dis : 1;

                            /* Bits[28:28], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable response function throttling of Rx-to-M3
                               Global Viral message
                            */
    UINT32 rx2r3_throt_fatal_dis : 1;

                            /* Bits[29:29], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable response function throttling of Rx-to-M3
                               Global Fatal message into Rx Credit BGF
                            */
    UINT32 rx2r3_throt_l0p_dis : 1;

                            /* Bits[30:30], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable response function throttling of Rx-to-M3
                               L0p message into Rx Credit BGF
                            */
    UINT32 rx2r3_throt_syncrst_dis : 1;

                            /* Bits[31:31], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Disable response function throttling of Rx-to-M3
                               Synchronous reset message into Rx Credit BGF
                            */

  } Bits;
  UINT32 Data;

} KTILKQTHR0_KTI_CIOPHYDFX_STRUCT;

/* KTILKQTHR1_KTI_CIOPHYDFX_REG supported on:                                   */
/*      SPRA0 (0x21e0b370)                                                      */
/*      SPRB0 (0x21e0b370)                                                      */
/*      SPRHBM (0x21e0b370)                                                     */
/*      SPRC0 (0x21e0b370)                                                      */
/*      SPRMCC (0x21e0b370)                                                     */
/*      SPRUCC (0x21e49370)                                                     */
/* Register default value on SPRA0: 0x000007CA                                  */
/* Register default value on SPRB0: 0x000007CA                                  */
/* Register default value on SPRHBM: 0x000007CA                                 */
/* Register default value on SPRC0: 0x000007CA                                  */
/* Register default value on SPRMCC: 0x000007CA                                 */
/* Register default value on SPRUCC: 0x000007CA                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ktilkqthr1
*/


#define KTILKQTHR1_KTI_CIOPHYDFX_REG 0x09220370

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txllr_reqack_str_sel : 4;

                            /* Bits[3:0], Access Type=RW/P/L, default=0x0000000A*/

                            /* Tx LLR RequestAck Start Select */
    UINT32 txllr_reqack_stp_sel : 4;

                            /* Bits[7:4], Access Type=RW/P/L, default=0x0000000C*/

                            /* Tx LLR RequestAck Stop Select */
    UINT32 rx2r3_slot_en_mask : 3;

                            /* Bits[10:8], Access Type=RW/P/L, default=0x00000007*/

                            /*
                               RXQ response slot mask (value 1 allows
                               backpressure, 0 means dont backpressure): [0] -
                               slot0 [1] - slot1 [2] - slot2
                            */
    UINT32 rsvd : 13;

                            /* Bits[23:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 txllr_throt_reqack_req_dis : 1;

                            /* Bits[24:24], Access Type=RW/P/L, default=0x00000000*/

                            /* Tx LLR Throttle RequestAck Request Disable */
    UINT32 txllr_throt_reqack_ack_dis : 1;

                            /* Bits[25:25], Access Type=RW/P/L, default=0x00000000*/

                            /* Tx LLR Throtthel RequestAck Ack Disable */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTILKQTHR1_KTI_CIOPHYDFX_STRUCT;

/* KTILKQTHRST_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b374)                                                      */
/*      SPRB0 (0x21e0b374)                                                      */
/*      SPRHBM (0x21e0b374)                                                     */
/*      SPRC0 (0x21e0b374)                                                      */
/*      SPRMCC (0x21e0b374)                                                     */
/*      SPRUCC (0x21e49374)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ktilkqthrst
*/


#define KTILKQTHRST_KTI_CIOPHYDFX_REG 0x09220374

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 txack_st : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* TX Ack status */
    UINT32 txllrq_st : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* TX LLQ Status */
    UINT32 rx2r3_st : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Rx 2 R3 Status */
    UINT32 txllrreqack_st : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Tx LLR RequestAck Status */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTILKQTHRST_KTI_CIOPHYDFX_STRUCT;

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTILKPMHFRC_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b378)                                                      */
/*      SPRB0 (0x21e0b378)                                                      */
/*      SPRHBM (0x21e0b378)                                                     */
/*      SPRC0 (0x21e0b378)                                                      */
/*      SPRMCC (0x21e0b378)                                                     */
/* Register default value on SPRA0: 0x00CA00CA                                  */
/* Register default value on SPRB0: 0x00CA00CA                                  */
/* Register default value on SPRHBM: 0x00CA00CA                                 */
/* Register default value on SPRC0: 0x00CA00CA                                  */
/* Register default value on SPRMCC: 0x00CA00CA                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* ktilkpmhfrc
*/


#define KTILKPMHFRC_KTI_CIOPHYDFX_REG 0x09220378

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 force_l0p_str_sel : 4;

                            /* Bits[3:0], Access Type=RW/P/L, default=0x0000000A*/

                            /*
                               Start Condition for Force L1 entry response
                               function. Note: there are several TxL0p force
                               entry/exit conditions controlled in KTILKPRIVC1
                               and KTILKPMHFRC. This bit is priority #2 and #3B
                               (out of 4) depending on the mode specified in
                               KTILKPRIVC1.pmh_l0p_resp_func_mode. Start Select
                               encodings: 0000 - Micro Break Point 0 (MBP[0])
                               0001 - Micro Break Point 1 (MBP[1]) 0010 -
                               Cluster Trigger Out 0 (CTO[0]) 0011 - Cluster
                               Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Response never occurs
                               1011 - Response occurs immediately 1100-1111 -
                               Reserved
                            */
    UINT32 force_l0p_stp_sel : 4;

                            /* Bits[7:4], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               Stop Condition for Force L1 entry response
                               function. Note: there are several TxL0p force
                               entry/exit conditions controlled in KTILKPRIVC1
                               and KTILKPMHFRC. This bit is priority #2 and #3B
                               (out of 4) depending on the mode specified in
                               KTILKPRIVC1.pmh_l0p_resp_func_mode. Stop Select
                               encoding: 0000 - Micro Break Point 0 (MBP[0])
                               0001 - Micro Break Point 1 (MBP[1]) 0010 -
                               Cluster Trigger Out 0 (CTO[0]) 0011 - Cluster
                               Trigger Out 1 (CTO[1]) 0100 - Artificial
                               Starvation Counter 0 (ASC[0]) 0101 - Artificial
                               Starvation Counter 1 (ASC[1]) 0110 - Common
                               Trigger Sequencer TrigOut (CTS_Trig_Out) 0111 -
                               Reserved 1000 - MCA Correctable Error 1001 - MCA
                               Uncorrectable Error 1010 - Deactivate one clock
                               after activation. Error injection on one
                               transaction will occur on the interface that is
                               enabled. 1011 - Response is never deactivated.
                               1100 - Deactivate the response when the
                               activating signal becomes false. 1101-1111 -
                               Reserved
                            */
    UINT32 rsvd : 8;

                            /* Bits[15:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 force_l1_str_sel : 4;

                            /* Bits[19:16], Access Type=RW/P/L, default=0x0000000A*/

                            /*
                               Start Condition for Force L1 entry response
                               function. Note: there are several L1 force
                               entry/exit conditions controlled in KTILKPRIVC1
                               and KTILKPMHFRC. This bit is priority #2B (out
                               of 2). Start Select encodings: 0000 - Micro
                               Break Point 0 (MBP[0]) 0001 - Micro Break Point
                               1 (MBP[1]) 0010 - Cluster Trigger Out 0 (CTO[0])
                               0011 - Cluster Trigger Out 1 (CTO[1]) 0100 -
                               Artificial Starvation Counter 0 (ASC[0]) 0101 -
                               Artificial Starvation Counter 1 (ASC[1]) 0110 -
                               Common Trigger Sequencer TrigOut (CTS_Trig_Out)
                               0111 - Reserved 1000 - MCA Correctable Error
                               1001 - MCA Uncorrectable Error 1010 - Response
                               never occurs 1011 - Response occurs immediately
                               1100-1111 - Reserved Note: Using this DFX
                               feature to force L1 entry can cause a CRC retry
                               to escalate to an uncorrectable error. To avoid
                               this (if forcing L1 entry while doing CRC retry
                               testing), configure the Phy layer L1 Reissue
                               Delay timer
                               (KTIREUT_PH_TL0C_SL.t_l1_reissue_delay and
                               KTIREUT_PH_TL0CENTRY_OP.t_l1_reissue_delay) to
                               the value of 2*L0c (KTIREUT_PH_LCS.s_tl0c).
                            */
    UINT32 force_l1_stp_sel : 4;

                            /* Bits[23:20], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               Stop Condition for Force L1 entry response
                               function. Note: there are several L1 force
                               entry/exit conditions controlled in KTILKPRIVC1
                               and KTILKPMHFRC. This bit is priority #2B (out
                               of 2). Stop Select encoding: 0000 - Micro Break
                               Point 0 (MBP[0]) 0001 - Micro Break Point 1
                               (MBP[1]) 0010 - Cluster Trigger Out 0 (CTO[0])
                               0011 - Cluster Trigger Out 1 (CTO[1]) 0100 -
                               Artificial Starvation Counter 0 (ASC[0]) 0101 -
                               Artificial Starvation Counter 1 (ASC[1]) 0110 -
                               Common Trigger Sequencer TrigOut (CTS_Trig_Out)
                               0111 - Reserved 1000 - MCA Correctable Error
                               1001 - MCA Uncorrectable Error 1010 - Deactivate
                               one clock after activation. Error injection on
                               one transaction will occur on the interface that
                               is enabled. 1011 - Response is never
                               deactivated. 1100 - Deactivate the response when
                               the activating signal becomes false. 1101-1111 -
                               Reserved Note: Using this DFX feature to force
                               L1 entry can cause a CRC retry to escalate to an
                               uncorrectable error. To avoid this (if forcing
                               L1 entry while doing CRC retry testing),
                               configure the Phy layer L1 Reissue Delay timer
                               (KTIREUT_PH_TL0C_SL.t_l1_reissue_delay and
                               KTIREUT_PH_TL0CENTRY_OP.t_l1_reissue_delay) to
                               the value of 2*L0c (KTIREUT_PH_LCS.s_tl0c).
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTILKPMHFRC_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

/* KTILKPMHFRCST_KTI_CIOPHYDFX_REG supported on:                                */
/*      SPRA0 (0x21e0b37c)                                                      */
/*      SPRB0 (0x21e0b37c)                                                      */
/*      SPRHBM (0x21e0b37c)                                                     */
/*      SPRC0 (0x21e0b37c)                                                      */
/*      SPRMCC (0x21e0b37c)                                                     */
/*      SPRUCC (0x21e4937c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktilkpmhfrcst
*/


#define KTILKPMHFRCST_KTI_CIOPHYDFX_REG 0x0922037C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 force_l0p_st : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates that the forcing function initiated
                               TxL0p. This does not necessarily mean that Phy
                               entered TxL0p.
                            */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 force_l1_st : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates that the forcing function initiated
                               L1. This does not necessarily mean that Phy
                               entered L1.
                            */
    UINT32 rsvd_3 : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTILKPMHFRCST_KTI_CIOPHYDFX_STRUCT;

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTILKBGFCREDITCFG_KTI_CIOPHYDFX_REG supported on:                            */
/*      SPRA0 (0x21e0b380)                                                      */
/*      SPRB0 (0x21e0b380)                                                      */
/*      SPRHBM (0x21e0b380)                                                     */
/*      SPRC0 (0x21e0b380)                                                      */
/*      SPRMCC (0x21e0b380)                                                     */
/* Register default value on SPRA0: 0x001C031F                                  */
/* Register default value on SPRB0: 0x001C031F                                  */
/* Register default value on SPRHBM: 0x001C031F                                 */
/* Register default value on SPRC0: 0x001C031F                                  */
/* Register default value on SPRMCC: 0x001C031F                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktilkbgfcreditcfg
*/


#define KTILKBGFCREDITCFG_KTI_CIOPHYDFX_REG 0x09220380

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxhdr_max_credit : 4;

                            /* Bits[3:0], Access Type=RW/P/L, default=0x0000000F*/

                            /*
                               Configure Rx Header BGF maximum credits. Legal
                               range for programming is: 1-15 => legal range of
                               credits is 2-16. This can be used to create an
                               artificial ceiling on the depth of the BGF.
                            */
    UINT32 rxhdr_cdt_threshold0 : 4;

                            /* Bits[7:4], Access Type=RW/P/L, default=0x00000001*/

                            /*
                               Configure Rx Header BGF threshold 0 level. Will
                               indicate sufficient credits to push flits into
                               Rx Header BGF when available credits >=
                               threshold0. Legal range: 1-11.
                            */
    UINT32 rxhdr_cdt_threshold1 : 4;

                            /* Bits[11:8], Access Type=RW/P/L, default=0x00000003*/

                            /*
                               Configure Rx Header BGF threshold 1 level. Will
                               indicate sufficient credits to push flits into
                               Rx Header BGF when available credits >=
                               threshold1. Legal range: 2-12, should generally
                               be set to rxhdr_cdt_threshold0+1
                            */
    UINT32 rxhdr_use_ext_credit_return : 1;

                            /* Bits[12:12], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Use external (from M3UPI) Rx Header BGF credit
                               return
                            */
    UINT32 rsvd : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxcrd_max_credit : 4;

                            /* Bits[19:16], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               Configure Rx Credit BGF maximum credits. Legal
                               range is: 1-12. This can be used to create an
                               artificial ceiling on the depth of the BGF.
                            */
    UINT32 rxcrd_cdt_threshold0 : 4;

                            /* Bits[23:20], Access Type=RW/P/L, default=0x00000001*/

                            /*
                               Configure Rx Credit BGF threshold 0 level. Will
                               indicate sufficient credits to push payload into
                               Rx Credit BGF when available credits >=
                               threshold0. Legal range: 1-11.
                            */
    UINT32 rsvd_24 : 4;

                            /* Bits[27:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxcrd_use_ext_credit_return : 1;

                            /* Bits[28:28], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Use external (from M3UPI) Rx Credit BGF credit
                               return
                            */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTILKBGFCREDITCFG_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

/* KTIDBGERRST0_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b3a0)                                                      */
/*      SPRB0 (0x21e0b3a0)                                                      */
/*      SPRHBM (0x21e0b3a0)                                                     */
/*      SPRC0 (0x21e0b3a0)                                                      */
/*      SPRMCC (0x21e0b3a0)                                                     */
/*      SPRUCC (0x21e493a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktidbgerrst0
*/


#define KTIDBGERRST0_KTI_CIOPHYDFX_REG 0x092203A0

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 param_excep : 3;

                            /* Bits[2:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               [0] - Received LLCTRL-INIT.Param with SKU Type
                               Check Failure [1] - Received LLCTRL-INIT.Param
                               with Intel UPI Version Check Failure [2] -
                               Unexpected LLCTRL-INIT.Param flit
                            */
    UINT32 rxuuu : 10;

                            /* Bits[12:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               [3] - Unsupported MsgClass/Opcode/VN/RSVD/1/0
                               [4] - Undefined Droute encoding [5] - Unexpected
                               Header Flit [6] - Unexpected Data Flit [7] -
                               Unexpected LLCTRL-POISON [8] - Unexpected Viral
                               (in RETRY.ACK) [9] - Unexpected Retriable flit
                               (prior to INIT.PARAM) [10] - Unexpected LLCTRL
                               or Protocol Header Fixed Field Error [11]
                               -&nbsp;Uninitialized UCE &nbsp;[12] - Reserved
                            */
    UINT32 ctrl_phy_ctrl_err : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Physical Layer Control Error NOTE: Currently
                               nothing causes this
                            */
    UINT32 ctrl_unexp_retryack : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Unexpected RETRY.ACK */
    UINT32 ctrl_unexp_retryreq : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Unexpected RETRY.REQ */
    UINT32 ctrl_rf_parity_err : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Register File parity error, See ktiparerrlog for
                               details on which Register File had the parity
                               error.
                            */
    UINT32 ctrl_rbt_err : 2;

                            /* Bits[18:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Read-Write Conflict Error: [17] - Read-Write
                               Conflict Error detected on SNP(Directed) [18] -
                               Read-Write Conflict Error detected on non-
                               SNP(Directed) (ie. SnpF/REQ/NCS)
                            */
    UINT32 ctrl_unexp_txprot : 3;

                            /* Bits[21:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               [19] - Unexpected M3UPI Tx Protocol Header flit
                               [20] - Unexpected M3UPI Tx Protocol Data flit
                               [21] - Unexpected M3UPI Tx Protocol EOP bit
                               value
                            */
    UINT32 ctrl_bgf : 6;

                            /* Bits[27:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               [22] - TxHdr/DataBgf valids not matching [23] -
                               RxCrdBgf credit overflow [24] - RxCrdBgf credit
                               underflow [25] - RxHdrBgf credit overflow [26] -
                               RxHdrBgf credit underflow [27] - Writing to
                               RxHdr/DataBgf but BGF not ready
                            */
    UINT32 ctrl_llrst_phyl0 : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Link Layer Reset Error: Link Layer Reset still
                               in progress when Phy enters L0 Note: On
                               cold/warm reset, phy training should not be
                               enabled until after LL reset is complete as
                               indicated by KTILCL.LinkLayerReset going back to
                               0. For hot add events, phy layer should be reset
                               via cpreset prior to LL reset.
                            */
    UINT32 ctrl_llrst_prot : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Link Layer Reset initiated while protocol
                               traffic not idle.
                            */
    UINT32 ctrl_txparerr : 2;

                            /* Bits[31:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Link Layer Transmit Parity Error [30] - Slot0 Tx
                               Address Parity Error [31] - Slot1 Tx Address
                               Parity Error Note: TXQ and LLRQ Tx parity errors
                               are logged in the Register File parity error
                               field (ctrl_rf_parity_err).
                            */

  } Bits;
  UINT32 Data;

} KTIDBGERRST0_KTI_CIOPHYDFX_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctrl_rf_parity_err : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Register File parity error, See ktiparerrlog for
                               details on which Register File had the parity
                               error.
                            */
    UINT32 ctrl_rbt_err : 2;

                            /* Bits[18:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Read-Write Conflict Error: [17] - Read-Write
                               Conflict Error detected on SNP(Directed) [18] -
                               Read-Write Conflict Error detected on non-
                               SNP(Directed) (ie. SnpF/REQ/NCS)
                            */
    UINT32 rsvd_19 : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIDBGERRST0_KTI_CIOPHYDFX_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctrl_rf_parity_err : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Register File parity error, See ktiparerrlog for
                               details on which Register File had the parity
                               error.
                            */
    UINT32 ctrl_rbt_err : 2;

                            /* Bits[18:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Read-Write Conflict Error: [17] - Read-Write
                               Conflict Error detected on SNP(Directed) [18] -
                               Read-Write Conflict Error detected on non-
                               SNP(Directed) (ie. SnpF/REQ/NCS)
                            */
    UINT32 rsvd_19 : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIDBGERRST0_KTI_CIOPHYDFX_STRUCT;

/* KTIDBGERRSTDIS0_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b3a4)                                                      */
/*      SPRB0 (0x21e0b3a4)                                                      */
/*      SPRHBM (0x21e0b3a4)                                                     */
/*      SPRC0 (0x21e0b3a4)                                                      */
/*      SPRMCC (0x21e0b3a4)                                                     */
/*      SPRUCC (0x21e493a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* ktidbgerrstdis0
*/


#define KTIDBGERRSTDIS0_KTI_CIOPHYDFX_REG 0x092203A4

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 disable_param_excep : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 disable_rxuuu : 10;

                            /* Bits[12:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               isable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 disable_ctrl_phy_ctrl_err : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 disable_ctrl_unexp_retryack : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 disable_ctrl_unexp_retryreq : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 disable_ctrl_rf_parity_err : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 disable_ctrl_rbt_err : 2;

                            /* Bits[18:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 disable_ctrl_unexp_txprot : 3;

                            /* Bits[21:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 disable_ctrl_bgf : 6;

                            /* Bits[27:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 disable_ctrl_llrst_phyl0 : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               isable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 disable_ctrl_llrst_prot : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 disable_ctrl_txparerr : 2;

                            /* Bits[31:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */

  } Bits;
  UINT32 Data;

} KTIDBGERRSTDIS0_KTI_CIOPHYDFX_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 disable_ctrl_rf_parity_err : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 disable_ctrl_rbt_err : 2;

                            /* Bits[18:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 rsvd_19 : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIDBGERRSTDIS0_KTI_CIOPHYDFX_SPRUCC_STRUCT;
#endif /* (SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 disable_ctrl_rf_parity_err : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 disable_ctrl_rbt_err : 2;

                            /* Bits[18:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST0. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST0.
                            */
    UINT32 rsvd_19 : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIDBGERRSTDIS0_KTI_CIOPHYDFX_STRUCT;

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* KTIDBGERRST1_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b3a8)                                                      */
/*      SPRB0 (0x21e0b3a8)                                                      */
/*      SPRHBM (0x21e0b3a8)                                                     */
/*      SPRC0 (0x21e0b3a8)                                                      */
/*      SPRMCC (0x21e0b3a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* ktidbgerrst1
*/


#define KTIDBGERRST1_KTI_CIOPHYDFX_REG 0x092203A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctrl_overunder : 13;

                            /* Bits[12:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               [0] - NumFreeBuf exceeded the maximum (84 for
                               processor) [1] - Eseq Number in received
                               RETRY.REQ exceeds max (84 for processor) [2] -
                               Intel UPI VNA consumed credit overflow [3] -
                               Intel UPI VNA consumed credit underflow [4] -
                               Intel UPI VNA released credit overflow [5] -
                               Intel UPI VN0/1 consumed credit overflow [6] -
                               Intel UPI VN0/1 consumed credit underflow [7] -
                               Intel UPI VN0/1 released credit overflow [8] -
                               Txq underflow [9] - Txq overflow [10] - Txq
                               credit overflow [11] - Rxq free list overflow
                               [12] - Rxq free list underflow
                            */
    UINT32 ctrl_flowqoverunder : 11;

                            /* Bits[23:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               [13] - AK VNA FlowQ Credit Overflow-or-Underflow
                               [14] - BL VNA FlowQ Credit Overflow-or-Underflow
                               [15] - AD VNA FlowQ Credit Overflow-or-Underflow
                               [16] - BL WB FlowQ Credit Overflow-or-Underflow
                               [17] - BL RSP FlowQ Credit Overflow-or-Underflow
                               [18] - BL NCS FlowQ Credit Overflow-or-Underflow
                               [19] - BL NCB FlowQ Credit Overflow-or-Underflow
                               [20] - AD WB FlowQ Credit Overflow-or-Underflow
                               [21] - AD RSP FlowQ Credit Overflow-or-Underflow
                               [22] - AD SNP FlowQ Credit Overflow-or-Underflow
                               [23] - AD REQ FlowQ Credit Overflow-or-Underflow
                            */
    UINT32 ctrl_other0 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Reserved */
    UINT32 ctrl_other1 : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Reserved */
    UINT32 ctrl_other2 : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Reserved */
    UINT32 ctrl_other3 : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Reserved */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIDBGERRST1_KTI_CIOPHYDFX_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIDBGERRST1_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b3a8)                                                      */
/*      SPRB0 (0x21e0b3a8)                                                      */
/*      SPRHBM (0x21e0b3a8)                                                     */
/*      SPRC0 (0x21e0b3a8)                                                      */
/*      SPRMCC (0x21e0b3a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* ktidbgerrst1
*/


#define KTIDBGERRST1_KTI_CIOPHYDFX_REG 0x092203A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctrl_overunder : 13;

                            /* Bits[12:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               [0] - NumFreeBuf exceeded the maximum (84 for
                               processor) [1] - Eseq Number in received
                               RETRY.REQ exceeds max (84 for processor) [2] -
                               Intel UPI VNA consumed credit overflow [3] -
                               Intel UPI VNA consumed credit underflow [4] -
                               Intel UPI VNA released credit overflow [5] -
                               Intel UPI VN0/1 consumed credit overflow [6] -
                               Intel UPI VN0/1 consumed credit underflow [7] -
                               Intel UPI VN0/1 released credit overflow [8] -
                               Txq underflow [9] - Txq overflow [10] - Txq
                               credit overflow [11] - Rxq free list overflow
                               [12] - Rxq free list underflow
                            */
    UINT32 ctrl_flowqoverunder : 11;

                            /* Bits[23:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               [13] - AK VNA FlowQ Credit Overflow-or-Underflow
                               [14] - BL VNA FlowQ Credit Overflow-or-Underflow
                               [15] - AD VNA FlowQ Credit Overflow-or-Underflow
                               [16] - BL WB FlowQ Credit Overflow-or-Underflow
                               [17] - BL RSP FlowQ Credit Overflow-or-Underflow
                               [18] - BL NCS FlowQ Credit Overflow-or-Underflow
                               [19] - BL NCB FlowQ Credit Overflow-or-Underflow
                               [20] - AD WB FlowQ Credit Overflow-or-Underflow
                               [21] - AD RSP FlowQ Credit Overflow-or-Underflow
                               [22] - AD SNP FlowQ Credit Overflow-or-Underflow
                               [23] - AD REQ FlowQ Credit Overflow-or-Underflow
                            */
    UINT32 aes_key_gen_par : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates Init Key CSR parity error also
                               indicates aes_key_gen logic parity error. This
                               bit combines error from Tx and Rx UCE.
                            */
    UINT32 aes_data_key_par : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates aes_key storage parity error along
                               with aes_data_key parity error. This bit
                               combines error from Tx and Rx UCE.
                            */
    UINT32 lfsr_par : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates data LFSR seed CSR parity error along
                               with LFSR logic parity error. This bit combines
                               error from Tx and Rx UCE.
                            */
    UINT32 data_key_fifo_par : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates data key fifo parity error. This bit
                               combines error from Tx and Rx UCE.
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIDBGERRST1_KTI_CIOPHYDFX_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIDBGERRST1_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b3a8)                                                      */
/*      SPRB0 (0x21e0b3a8)                                                      */
/*      SPRHBM (0x21e0b3a8)                                                     */
/*      SPRC0 (0x21e0b3a8)                                                      */
/*      SPRMCC (0x21e0b3a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* ktidbgerrst1
*/


#define KTIDBGERRST1_KTI_CIOPHYDFX_REG 0x092203A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctrl_overunder : 13;

                            /* Bits[12:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               [0] - NumFreeBuf exceeded the maximum (84 for
                               processor) [1] - Eseq Number in received
                               RETRY.REQ exceeds max (84 for processor) [2] -
                               Intel UPI VNA consumed credit overflow [3] -
                               Intel UPI VNA consumed credit underflow [4] -
                               Intel UPI VNA released credit overflow [5] -
                               Intel UPI VN0/1 consumed credit overflow [6] -
                               Intel UPI VN0/1 consumed credit underflow [7] -
                               Intel UPI VN0/1 released credit overflow [8] -
                               Txq underflow [9] - Txq overflow [10] - Txq
                               credit overflow [11] - Rxq free list overflow
                               [12] - Rxq free list underflow
                            */
    UINT32 ctrl_flowqoverunder : 11;

                            /* Bits[23:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               [13] - AK VNA FlowQ Credit Overflow-or-Underflow
                               [14] - BL VNA FlowQ Credit Overflow-or-Underflow
                               [15] - AD VNA FlowQ Credit Overflow-or-Underflow
                               [16] - BL WB FlowQ Credit Overflow-or-Underflow
                               [17] - BL RSP FlowQ Credit Overflow-or-Underflow
                               [18] - BL NCS FlowQ Credit Overflow-or-Underflow
                               [19] - BL NCB FlowQ Credit Overflow-or-Underflow
                               [20] - AD WB FlowQ Credit Overflow-or-Underflow
                               [21] - AD RSP FlowQ Credit Overflow-or-Underflow
                               [22] - AD SNP FlowQ Credit Overflow-or-Underflow
                               [23] - AD REQ FlowQ Credit Overflow-or-Underflow
                            */
    UINT32 rsvd_24 : 4;

                            /* Bits[27:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIDBGERRST1_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* KTIDBGERRSTDIS1_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b3ac)                                                      */
/*      SPRB0 (0x21e0b3ac)                                                      */
/*      SPRHBM (0x21e0b3ac)                                                     */
/*      SPRC0 (0x21e0b3ac)                                                      */
/*      SPRMCC (0x21e0b3ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktidbgerrstdis1
*/


#define KTIDBGERRSTDIS1_KTI_CIOPHYDFX_REG 0x092203AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 disable_ctrl_overunder : 13;

                            /* Bits[12:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 disable_ctrl_flowqoverunder : 11;

                            /* Bits[23:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 disable_ctrl_other0 : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 disable_ctrl_other1 : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 disable_ctrl_other2 : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 disable_ctrl_other3 : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIDBGERRSTDIS1_KTI_CIOPHYDFX_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIDBGERRSTDIS1_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b3ac)                                                      */
/*      SPRB0 (0x21e0b3ac)                                                      */
/*      SPRHBM (0x21e0b3ac)                                                     */
/*      SPRC0 (0x21e0b3ac)                                                      */
/*      SPRMCC (0x21e0b3ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktidbgerrstdis1
*/


#define KTIDBGERRSTDIS1_KTI_CIOPHYDFX_REG 0x092203AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 disable_ctrl_overunder : 13;

                            /* Bits[12:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 disable_ctrl_flowqoverunder : 11;

                            /* Bits[23:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 disable_aes_key_gen_par : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 disable_aes_data_key_par : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 disable_lfsr_par : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 disable_data_key_fifo_par : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIDBGERRSTDIS1_KTI_CIOPHYDFX_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIDBGERRSTDIS1_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0b3ac)                                                      */
/*      SPRB0 (0x21e0b3ac)                                                      */
/*      SPRHBM (0x21e0b3ac)                                                     */
/*      SPRC0 (0x21e0b3ac)                                                      */
/*      SPRMCC (0x21e0b3ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktidbgerrstdis1
*/


#define KTIDBGERRSTDIS1_KTI_CIOPHYDFX_REG 0x092203AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 disable_ctrl_overunder : 13;

                            /* Bits[12:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 disable_ctrl_flowqoverunder : 11;

                            /* Bits[23:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable bits for each of the corresponding error
                               types defined in KTIDBGERRST1. Setting a given
                               bit to 1 will disable that error type from
                               triggering an error log capture. However, note
                               that if a disabled error type occurs on the same
                               cycle as an undisabled error, it will still be
                               logged into the corresponding location in
                               KTIDBGERRST1.
                            */
    UINT32 rsvd_24 : 4;

                            /* Bits[27:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIDBGERRSTDIS1_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

/* KTIDBGSMST_KTI_CIOPHYDFX_REG supported on:                                   */
/*      SPRA0 (0x21e0b3b0)                                                      */
/*      SPRB0 (0x21e0b3b0)                                                      */
/*      SPRHBM (0x21e0b3b0)                                                     */
/*      SPRC0 (0x21e0b3b0)                                                      */
/*      SPRMCC (0x21e0b3b0)                                                     */
/*      SPRUCC (0x21e493b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktidbgsmst
*/


#define KTIDBGSMST_KTI_CIOPHYDFX_REG 0x092203B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 llrstsm : 2;

                            /* Bits[1:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               LL Reset State 0x0 - Begin 0x1 - Send to M3 0x2
                               - Timer Exit 0x3 - Done
                            */
    UINT32 tism : 2;

                            /* Bits[3:2], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               LL Tx Init State 0x0 - Not Ready For Init 0x1 -
                               Sending Parameter 0x2 - Parameter Sent
                            */
    UINT32 rism : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               LL Rx Init State 0 - Waiting for Parameter 1 -
                               Parameter Received
                            */
    UINT32 lrsm : 3;

                            /* Bits[7:5], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               LL Local Retry State 0x0 - Retry Local Normal
                               0x1 - Retry LLRReq 0x2 - Retry Local Idle 0x3 -
                               Retry Phy Reinit 0x4 - Retry Abort
                            */
    UINT32 rrsm : 2;

                            /* Bits[9:8], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               LL Remote Retry State 0 - Retry Remote Normal 1
                               - Retry LLRAck
                            */
    UINT32 dbgtsm : 3;

                            /* Bits[12:10], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Debug State 0x0 - Idle 0x1 - Force Event 0x2 -
                               Force Relative Timing Packet (RTP) 0x3 -
                               Opportunistic Event 0x4 - Opportunistic Relative
                               Timing Packet (RTP)
                            */
    UINT32 tpsnsm : 1;

                            /* Bits[13:13], Access Type=RO/V/P, default=0x00000000*/

                            /* Tx Poison State 0 - Idle 1 - Arb */
    UINT32 tvrlsm : 1;

                            /* Bits[14:14], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Tx Viral State 0x0 - Idle 0x1 - Bad CRC 0x2 -
                               Deassert
                            */
    UINT32 rx_active : 1;

                            /* Bits[15:15], Access Type=RO/V/P, default=0x00000000*/

                            /* LL Rx 0 - inactive 1 - active */
    UINT32 tx_active : 1;

                            /* Bits[16:16], Access Type=RO/V/P, default=0x00000000*/

                            /* LL Tx 0 - inactive 1 - active */
    UINT32 rx_normal_op : 1;

                            /* Bits[17:17], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Identical to KTILS.Rx_Normal_Op_State 0 - Not Rx
                               Normal Operation 1 - Rx Normal Operation
                            */
    UINT32 rsvd : 5;

                            /* Bits[22:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tx_llr_numfreebuf : 9;

                            /* Bits[31:23], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Link Layer Retry NumFreeBuf. less than&nbsp;179
                               : link still active equal to 179: either active
                               or inactive, but there is no way to know for
                               sure because the Intel UPI spec mandates that
                               remote device should not return the last ACK if
                               that is the only remaining thing to send (in
                               order to avoid potential ACK ping-pong issues).
                               equal to 180: link is inactive.
                            */

  } Bits;
  UINT32 Data;

} KTIDBGSMST_KTI_CIOPHYDFX_STRUCT;

/* KTIA2RCRCTRL0_ACTIVE_KTI_CIOPHYDFX_REG supported on:                         */
/*      SPRA0 (0x21e0b3b8)                                                      */
/*      SPRB0 (0x21e0b3b8)                                                      */
/*      SPRHBM (0x21e0b3b8)                                                     */
/*      SPRC0 (0x21e0b3b8)                                                      */
/*      SPRMCC (0x21e0b3b8)                                                     */
/*      SPRUCC (0x21e493b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ktia2rcrctrl0_active
*/


#define KTIA2RCRCTRL0_ACTIVE_KTI_CIOPHYDFX_REG 0x092203B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ad_vna : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VNA credits currently available to the FlowQ
                               from the Intel UPI Agent
                            */
    UINT32 bl_vna : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VNA credits currently available to the FlowQ
                               from the Intel UPI Agent
                            */
    UINT32 ak_vna : 7;

                            /* Bits[20:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               AK VNA credits currently available to the FlowQ
                               from the Intel UPI Agent
                            */
    UINT32 ad_snp0 : 3;

                            /* Bits[23:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN0 SNP credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 ad_rsp0 : 3;

                            /* Bits[26:24], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN0 RSP credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 bl_rsp0 : 3;

                            /* Bits[29:27], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN0 RSP credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIA2RCRCTRL0_ACTIVE_KTI_CIOPHYDFX_STRUCT;

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIA2RCRCTRL1_ACTIVE_KTI_CIOPHYDFX_REG supported on:                         */
/*      SPRA0 (0x21e0b3bc)                                                      */
/*      SPRB0 (0x21e0b3bc)                                                      */
/*      SPRHBM (0x21e0b3bc)                                                     */
/*      SPRC0 (0x21e0b3bc)                                                      */
/*      SPRMCC (0x21e0b3bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* ktia2rcrctrl1_active
*/


#define KTIA2RCRCTRL1_ACTIVE_KTI_CIOPHYDFX_REG 0x092203BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_rsp1 : 3;

                            /* Bits[2:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN1 RSP credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 bl_wb1 : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN1 WB credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 bl_ncs1 : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN1 NCS credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 bl_ncb1 : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN1 NCB credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 ad_wb1 : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN1 WB credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 ad_req1 : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN1 REQ credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 ad_rsp1 : 3;

                            /* Bits[10:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN1 RSP credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 ad_snp1 : 3;

                            /* Bits[13:11], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN1 SNP credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 bl_wb0 : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN0 WB credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 bl_ncs0 : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN0 NCS credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 bl_ncb0 : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               BL VN0 NCB credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 ad_wb0 : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN0 WB credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 ad_req0 : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               AD VN0 REQ credits currently available to the
                               FlowQ from the Intel UPI Agent
                            */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIA2RCRCTRL1_ACTIVE_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* DFX_LCK_CTL_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b800)                                                      */
/*      SPRB0 (0x21e0b800)                                                      */
/*      SPRHBM (0x21e0b800)                                                     */
/*      SPRC0 (0x21e0b800)                                                      */
/*      SPRMCC (0x21e0b800)                                                     */
/* Register default value on SPRA0: 0x00000007                                  */
/* Register default value on SPRB0: 0x00000007                                  */
/* Register default value on SPRHBM: 0x00000007                                 */
/* Register default value on SPRC0: 0x00000007                                  */
/* Register default value on SPRMCC: 0x00000007                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* dfx_lck_ctl_cfg
*/


#define DFX_LCK_CTL_KTI_CIOPHYDFX_REG 0x09220800

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reutlck : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /* reut eng ia spce csr lock */
    UINT32 reutenglck : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /* reut eng csr lock */
    UINT32 privdbg_phy_l0_sync_lck : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /* phy L0 Sync csr lock */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DFX_LCK_CTL_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* PQ_CSR_PTR0_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b804)                                                      */
/*      SPRB0 (0x21e0b804)                                                      */
/*      SPRHBM (0x21e0b804)                                                     */
/*      SPRC0 (0x21e0b804)                                                      */
/*      SPRMCC (0x21e0b804)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* pq_csr_ptr0
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* PQ_CSR_PTR1_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b808)                                                      */
/*      SPRB0 (0x21e0b808)                                                      */
/*      SPRHBM (0x21e0b808)                                                     */
/*      SPRC0 (0x21e0b808)                                                      */
/*      SPRMCC (0x21e0b808)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* pq_csr_ptr1
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* PQ_CSR_PTR2_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b80c)                                                      */
/*      SPRB0 (0x21e0b80c)                                                      */
/*      SPRHBM (0x21e0b80c)                                                     */
/*      SPRC0 (0x21e0b80c)                                                      */
/*      SPRMCC (0x21e0b80c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* pq_csr_ptr2
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* PQ_CSR_PTR3_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b810)                                                      */
/*      SPRB0 (0x21e0b810)                                                      */
/*      SPRHBM (0x21e0b810)                                                     */
/*      SPRC0 (0x21e0b810)                                                      */
/*      SPRMCC (0x21e0b810)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* pq_csr_ptr3
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* PQ_CSR_IBTXLER_KTI_CIOPHYDFX_REG supported on:                               */
/*      SPRA0 (0x21e0b818)                                                      */
/*      SPRB0 (0x21e0b818)                                                      */
/*      SPRHBM (0x21e0b818)                                                     */
/*      SPRC0 (0x21e0b818)                                                      */
/*      SPRMCC (0x21e0b818)                                                     */
/* Register default value on SPRA0: 0x00FFFFFF                                  */
/* Register default value on SPRB0: 0x00FFFFFF                                  */
/* Register default value on SPRHBM: 0x00FFFFFF                                 */
/* Register default value on SPRC0: 0x00FFFFFF                                  */
/* Register default value on SPRMCC: 0x00FFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* pq_csr_ibtxler
*/


#define PQ_CSR_IBTXLER_KTI_CIOPHYDFX_REG 0x09220818

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_lane_en : 24;

                            /* Bits[23:0], Access Type=RW/P/L, default=0x00FFFFFF*/

                            /*
                               Lane having 0 in this bit-map will transmit a
                               differential 0 This value may get modified by
                               pattern inversion and PQ_CSR_TTDDC register
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PQ_CSR_IBTXLER_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* PQ_CSR_PLLFCR_KTI_CIOPHYDFX_REG supported on:                                */
/*      SPRA0 (0x21e0b81c)                                                      */
/*      SPRB0 (0x21e0b81c)                                                      */
/*      SPRHBM (0x21e0b81c)                                                     */
/*      SPRC0 (0x21e0b81c)                                                      */
/*      SPRMCC (0x21e0b81c)                                                     */
/* Register default value on SPRA0: 0x00000020                                  */
/* Register default value on SPRB0: 0x00000020                                  */
/* Register default value on SPRHBM: 0x00000020                                 */
/* Register default value on SPRC0: 0x00000020                                  */
/* Register default value on SPRMCC: 0x00000020                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* pq_csr_pllfcr
*/


#define PQ_CSR_PLLFCR_KTI_CIOPHYDFX_REG 0x0922081C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iopll_ratio : 6;

                            /* Bits[5:0], Access Type=RO/V/P, default=0x00000020*/

                            /*
                               IO PLL freq to Ref clock ratio. Default is 32.
                               If Ref clock is 100MHz, IO PLL clock is 3.2 GHz.
                               Allow values are 48, 30, 32, and 24. This is a
                               read only status. The IO PLL ratio is set in
                               PMA.
                            */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PQ_CSR_PLLFCR_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* PQ_CSR_MCSR0_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b820)                                                      */
/*      SPRB0 (0x21e0b820)                                                      */
/*      SPRHBM (0x21e0b820)                                                     */
/*      SPRC0 (0x21e0b820)                                                      */
/*      SPRMCC (0x21e0b820)                                                     */
/* Register default value on SPRA0: 0x04008000                                  */
/* Register default value on SPRB0: 0x04008000                                  */
/* Register default value on SPRHBM: 0x04008000                                 */
/* Register default value on SPRC0: 0x04008000                                  */
/* Register default value on SPRMCC: 0x04008000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* pq_csr_mcsr0
*/


#define PQ_CSR_MCSR0_KTI_CIOPHYDFX_REG 0x09220820

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 15;

                            /* Bits[14:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rstcal_csr_rst_en : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000001*/

                            /*
                               1 - enable CSR based reset in Reset.Calibrate
                               state of UPI state machine
                            */
    UINT32 rstc_single_step : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* Single step applies to RST_C only */
    UINT32 rsvd_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 slave_wait_for_pa : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Wait for PA after receiving SDS to generate
                               PhyL0Sync and send SDS during UPI link
                               initialization
                            */
    UINT32 rsvd_19 : 3;

                            /* Bits[21:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pa_before_l1_exit : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               1- After L1 normal exit conditions are met, wait
                               for next PA before exiting L1 state
                            */
    UINT32 dfx_rx_fifo_rst : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               when dfx_rx_fifo_rst_override_en is set, user
                               can set this bit to reset the RX FIFO.
                            */
    UINT32 dfx_rx_fifo_rst_override_en : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               If sets, user can reset the RX FIFO by setting
                               bit 23 of this register.
                            */
    UINT32 stagger_dis : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               1 - Disable staggered turn on/off of Tx drivers
                               to mitigate di/dt
                            */
    UINT32 assume_pa_valid : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000001*/

                            /*
                               1 - Assume PA is seen as soon as state machine
                               enters Reset.Calibrate This control has higher
                               priority than pa_before_rstcal_exit
                            */
    UINT32 pa_before_rstcal_exit : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               1 - After Reset Calibration normal exit
                               conditions are met, wait for next PA before
                               exiting KTI Reset.Calibrate state
                            */
    UINT32 sync_ctr_rst_en : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               1 - Enable reset of the Sync Counter when
                               entering RST_C
                            */
    UINT32 rsvd_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 data_sdt_override : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               Override for lane squelch break detect used
                               during UPI state DetectTx If set, assume remote
                               terminations on all lanes detectedd
                            */
    UINT32 data_ldt_override : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               Override for data lane link detect used during
                               UPI state DetectRx If set, assume remote
                               terminations on all lanes are detected
                            */

  } Bits;
  UINT32 Data;

} PQ_CSR_MCSR0_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* PQ_CSR_MCSR1_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b824)                                                      */
/*      SPRB0 (0x21e0b824)                                                      */
/*      SPRHBM (0x21e0b824)                                                     */
/*      SPRC0 (0x21e0b824)                                                      */
/*      SPRMCC (0x21e0b824)                                                     */
/* Register default value on SPRA0: 0x00400007                                  */
/* Register default value on SPRB0: 0x00400007                                  */
/* Register default value on SPRHBM: 0x00400007                                 */
/* Register default value on SPRC0: 0x00400007                                  */
/* Register default value on SPRMCC: 0x00400007                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* pq_csr_mcsr1
*/


#define PQ_CSR_MCSR1_KTI_CIOPHYDFX_REG 0x09220824

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bytelock_tmr_ex : 3;

                            /* Bits[2:0], Access Type=RW/P, default=0x00000007*/

                            /*
                               Exponential timeout value for performing byte
                               lock during deskew Bytelock timeout = 4*2(value
                               in this field) supersequence_length
                            */
    UINT32 bytelock_retry_dis : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               Keep trying bytelock, if no lanes successful
                               before bytelock timer expires.
                            */
    UINT32 rsvd : 3;

                            /* Bits[6:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 init_fail_capt_mode : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               Initialization Failure Capture Mode. 0 - Capture
                               first initialization failure type 1 - Capture
                               last initialization failure type
                            */
    UINT32 prev_init_fail_type : 8;

                            /* Bits[15:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Previous Initialization Failure Type The
                               Initialization failure type from previous
                               initialization abort. This field is updated when
                               the state is exited on an init failure 0001 - No
                               RX Termination detected 0010 - No Wake Detected
                               0011 - Adaptation did not complete 0100 - Deskew
                               did not complete 0101 - Latency Fixing did not
                               complete 0110 - No good handshake (AckXchgd)
                               0111 - Link width negotiation failed 1000 - No
                               Ack received 1001 - Failure detected in TX
                               during Flitlock 1010 - No SDS Received 1111 -
                               Time out and all lanes/RX bad Other values are
                               reserved
                            */
    UINT32 l1b_override : 1;

                            /* Bits[16:16], Access Type=RW/1S/V/P, default=0x00000000*/

                            /*
                               1'b1: When the L1B override is set, the LTSSM
                               will move to L1B state from TXDetect immediately
                               upon detecting the squelch exit instead of going
                               to polling state. HW will clear it on entering
                               in L1b state
                            */
    UINT32 chkn_ovrd_comp_cond_4l1b : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               1'b1: chicken bit to enable LTSSM state
                               transition to L1b in compliance mode when
                               ph_ctr2.cecnrdp bit is set
                            */
    UINT32 l2_en : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS will program this register to enable L2 or
                               UPI port disable state with pipe reset not de-
                               asserted
                            */
    UINT32 rx_l0p_exit_sdstran_tov_sel : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Chicken bit to select rx_l0p_exit_sdstran
                               timeout value 1'b0 : select value from deskew
                               logic 1'b0: select value from CSR
                            */
    UINT32 chkn_lbm_nodeskew4ack : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Chicken bit disable wait for deskew before
                               sending Ack in Loopback patter in LB Marker
                               state 1'b0 : wait for deskew done before sending
                               ack, Added for SPR 1'b1: do not wait for deskew
                               done before sending ack. Legacy behavior till
                               ICX
                            */
    UINT32 chkn_sds_timer : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Chicken bit to set the start value fo SDS Timer
                               0: Value is 3 (Default) 1: value is 2
                            */
    UINT32 chkn_pol_adapt_tmr : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Chicken bit disable for pol adapt tmr 1'b0 :
                               disable 1'b1: Enable(default)
                            */
    UINT32 rsvd_23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PQ_CSR_MCSR1_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* PQ_CSR_MCSR2_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b828)                                                      */
/*      SPRB0 (0x21e0b828)                                                      */
/*      SPRHBM (0x21e0b828)                                                     */
/*      SPRC0 (0x21e0b828)                                                      */
/*      SPRMCC (0x21e0b828)                                                     */
/* Register default value on SPRA0: 0x58000000                                  */
/* Register default value on SPRB0: 0x58000000                                  */
/* Register default value on SPRHBM: 0x58000000                                 */
/* Register default value on SPRC0: 0x58000000                                  */
/* Register default value on SPRMCC: 0x58000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* pq_csr_mcsr2
*/


#define PQ_CSR_MCSR2_KTI_CIOPHYDFX_REG 0x09220828

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reseed_retry_dis : 1;

                            /* Bits[0:0], Access Type=RW/P/L, default=0x00000000*/

                            /* Disable reseeding retries */
    UINT32 reseed_after_pol_dis : 1;

                            /* Bits[1:1], Access Type=RW/P/L, default=0x00000000*/

                            /* Disables reseeding retry after Polling */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 skew_selx : 3;

                            /* Bits[5:3], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Register value * 4UI = Amount of skew to add to
                               Tx lane X. This skew is only applied to the
                               Digital Near End Loopback path. Supported range
                               is 0 - 5 (up to 20UI). PQ0 - Lane X = 2 PQ1 -
                               Lane X = 0 PQ2 - Lane X = 1 PQ3 - Lane X = 0 PQ4
                               - Lane X = 4 PQ5 - Lane X = 2
                            */
    UINT32 skew_sely : 3;

                            /* Bits[8:6], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Register value * 4UI = Amount of skew to add to
                               Tx lane Y. This skew is only applied to the
                               Digital Near End Loopback path. Supported range
                               is 0 - 5 (up to 20UI). PQ0 - Lane Y = 3 PQ1 -
                               Lane Y = 5 PQ2 - Lane Y = 0 PQ3 - Lane Y = 3 PQ4
                               - Lane Y = 0 PQ5 - Lane Y = 0
                            */
    UINT32 err_inj_l0pidle_mask : 1;

                            /* Bits[9:9], Access Type=RW/P/L, default=0x00000000*/

                            /* 1 - Do not inject reset during L0P (idle) */
    UINT32 err_inj_l0pl0_mask : 1;

                            /* Bits[10:10], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               1 - Do not inject error during L0, L0P (active),
                               LB_PATTERN states
                            */
    UINT32 err_inj_l0c_mask : 1;

                            /* Bits[11:11], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               A value of 1 means error will not be injected on
                               L0c cycles
                            */
    UINT32 err_inj_other_mask : 1;

                            /* Bits[12:12], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               A value of 1 means error will not be injected on
                               non L0p, L0p(active/idle), L0c, LB_PATTERN
                            */
    UINT32 err_inj_ui_ctl : 8;

                            /* Bits[20:13], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               A value of 1 indicates the corresponding UI has
                               its error injection enabled This field is only
                               applied for data lanes Bit 0 - UI 0 Bit 1 - UI 1
                               ... and so on Note: If err_inj_duration is set,
                               this field should be set to all 1s
                            */
    UINT32 err_inj_stuck0 : 1;

                            /* Bits[21:21], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               0 - Inject error by inverting selected bits 1 -
                               Inject error by forcing selected bits to 0 This
                               bit affects error injection on data lanes only
                            */
    UINT32 rst_inj_l0_mask : 1;

                            /* Bits[22:22], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               1 - Do not inject reset during L0, L0P, L0C,
                               LB_PATTERN
                            */
    UINT32 rst_inj_init_mask : 1;

                            /* Bits[23:23], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               1 - Do not inject reset during non L0, L0P, L0C,
                               LB_PATTERN states
                            */
    UINT32 rsvd_24 : 3;

                            /* Bits[26:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dis_txclk : 1;

                            /* Bits[27:27], Access Type=RW/P/L, default=0x00000001*/

                            /* Disable txclk when in L1 or waiting in RSTC. */
    UINT32 scrmrst_on_inbrst : 1;

                            /* Bits[28:28], Access Type=RW/P/L, default=0x00000001*/

                            /* Reset Tx scramblers on inband reset */
    UINT32 dne_lb_en : 1;

                            /* Bits[29:29], Access Type=RW/P/L, default=0x00000000*/

                            /* Digital near end loopback enable */
    UINT32 scram_en : 1;

                            /* Bits[30:30], Access Type=RW/P/L, default=0x00000001*/

                            /*
                               Enable Data Scramble with LFSR 1 - data
                               scrambled/descrambled with LFSR 0 - data not
                               scrambled/descrambled
                            */
    UINT32 fail_on_drop : 1;

                            /* Bits[31:31], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Cause init to fail if any lane is dropped before
                               Config. Use with freeze on init abort and or
                               failover disabled.
                            */

  } Bits;
  UINT32 Data;

} PQ_CSR_MCSR2_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* PQ_CSR_MCSR3_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b82c)                                                      */
/*      SPRB0 (0x21e0b82c)                                                      */
/*      SPRHBM (0x21e0b82c)                                                     */
/*      SPRC0 (0x21e0b82c)                                                      */
/*      SPRMCC (0x21e0b82c)                                                     */
/* Register default value on SPRA0: 0x00FFFFFF                                  */
/* Register default value on SPRB0: 0x00FFFFFF                                  */
/* Register default value on SPRHBM: 0x00FFFFFF                                 */
/* Register default value on SPRC0: 0x00FFFFFF                                  */
/* Register default value on SPRMCC: 0x00FFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* pq_csr_mcsr3
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* PQ_CSR_AFEIAR_KTI_CIOPHYDFX_REG supported on:                                */
/*      SPRA0 (0x21e0b838)                                                      */
/*      SPRB0 (0x21e0b838)                                                      */
/*      SPRHBM (0x21e0b838)                                                     */
/*      SPRC0 (0x21e0b838)                                                      */
/*      SPRMCC (0x21e0b838)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* pq_csr_afeiar
*/


#define PQ_CSR_AFEIAR_KTI_CIOPHYDFX_REG 0x09220838

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 write_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               1 - Write enable Software sets this bit and
                               hardware clears it once the write operation is
                               complete. Software should never clear this bit.
                            */
    UINT32 hw_override_en : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               1 - Override enable Must be set by software for
                               Read operations. Software may choose to leave
                               this bit set.
                            */
    UINT32 inc_setting : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               1 - increment setting of the IAR addressed by
                               the unit and register IDs
                            */
    UINT32 dec_setting : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               1 - decrement setting of the IAR addressed by
                               the unit and register IDs
                            */
    UINT32 unit_id : 6;

                            /* Bits[9:4], Access Type=RW, default=0x00000000*/

                            /* Unit ID (example lane ID, Tx Rcomp etc) */
    UINT32 register_id : 6;

                            /* Bits[15:10], Access Type=RW, default=0x00000000*/

                            /*
                               Register ID within the unit addressed by the
                               unit ID
                            */
    UINT32 wr_data : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               Write data to be applied to the IAR addressed by
                               unit and register IDs
                            */
    UINT32 read_data : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /*
                               Read data from the IAR addressed by unit and
                               register IDs
                            */

  } Bits;
  UINT32 Data;

} PQ_CSR_AFEIAR_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPHPTDS_KTI_CIOPHYDFX_REG supported on:                                    */
/*      SPRA0 (0x21e0b840)                                                      */
/*      SPRB0 (0x21e0b840)                                                      */
/*      SPRHBM (0x21e0b840)                                                     */
/*      SPRC0 (0x21e0b840)                                                      */
/*      SPRMCC (0x21e0b840)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* ktiphptds
*/


#define KTIPHPTDS_KTI_CIOPHYDFX_REG 0x09220840

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_dlane_sts : 24;

                            /* Bits[23:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               A bit value of 1 indicates the TX lane has been
                               dropped Bit 0: Status of lane 0. Bit 1: Status
                               of lane 1. .. and so on..
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPHPTDS_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPHPRDS_KTI_CIOPHYDFX_REG supported on:                                    */
/*      SPRA0 (0x21e0b844)                                                      */
/*      SPRB0 (0x21e0b844)                                                      */
/*      SPRHBM (0x21e0b844)                                                     */
/*      SPRC0 (0x21e0b844)                                                      */
/*      SPRMCC (0x21e0b844)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* ktiphprds
*/


#define KTIPHPRDS_KTI_CIOPHYDFX_REG 0x09220844

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx_dlane_sts : 24;

                            /* Bits[23:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               A bit value of 1 indicates the RX lane has been
                               dropped Bit 0: Status of lane 0. Bit 1: Status
                               of lane 1. .. and so on..
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPHPRDS_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* NTL_RX_INVERT_KTI_CIOPHYDFX_REG supported on:                                */
/*      SPRA0 (0x21e0b870)                                                      */
/*      SPRB0 (0x21e0b870)                                                      */
/*      SPRHBM (0x21e0b870)                                                     */
/*      SPRC0 (0x21e0b870)                                                      */
/*      SPRMCC (0x21e0b870)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* ntl_rx_invert
*/


#define NTL_RX_INVERT_KTI_CIOPHYDFX_REG 0x09220870

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ntl_rx_inv : 24;

                            /* Bits[23:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Invert the NTL RX results on a lane-by-lane
                               basis. This bit controls logic, not an AFE pin.
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} NTL_RX_INVERT_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* NTL_TX_INVERT_KTI_CIOPHYDFX_REG supported on:                                */
/*      SPRA0 (0x21e0b874)                                                      */
/*      SPRB0 (0x21e0b874)                                                      */
/*      SPRHBM (0x21e0b874)                                                     */
/*      SPRC0 (0x21e0b874)                                                      */
/*      SPRMCC (0x21e0b874)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* ntl_tx_invert
*/


#define NTL_TX_INVERT_KTI_CIOPHYDFX_REG 0x09220874

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ntl_tx_inv : 24;

                            /* Bits[23:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Invert the NTL TX results on a lane-by-lane
                               basis
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} NTL_TX_INVERT_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* RXSQ_DET_KTI_CIOPHYDFX_REG supported on:                                     */
/*      SPRA0 (0x21e0b878)                                                      */
/*      SPRB0 (0x21e0b878)                                                      */
/*      SPRHBM (0x21e0b878)                                                     */
/*      SPRC0 (0x21e0b878)                                                      */
/*      SPRMCC (0x21e0b878)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* rxsq_det
*/


#define RXSQ_DET_KTI_CIOPHYDFX_REG 0x09220878

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rxsq_det : 24;

                            /* Bits[23:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Comparator output of squelch detect/ ntl /
                               rterm_legchk_en. In squelch detect mode: 0=exit
                               not detected 1=exit detected In
                               ntl/rterm_legchk_en mode: 0=input ltreference
                               voltage 1=input gtreference voltage AGENT NOTE:
                               CSR will be updated based on the capt_mode bits
                               of NTL_CTL. Will contain lane based results of
                               ntl, squelch, or rx detect based on other
                               configuration bits. Bit 23 to 0 is data lanes.
                            */
    UINT32 rsvd : 7;

                            /* Bits[30:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rst_sqexitflop : 1;

                            /* Bits[31:31], Access Type=RW/V, default=0x00000000*/

                            /*
                               Clears squelch detection flop. Self-clears to
                               zero.
                            */

  } Bits;
  UINT32 Data;

} RXSQ_DET_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* TX_RX_DET_KTI_CIOPHYDFX_REG supported on:                                    */
/*      SPRA0 (0x21e0b87c)                                                      */
/*      SPRB0 (0x21e0b87c)                                                      */
/*      SPRHBM (0x21e0b87c)                                                     */
/*      SPRC0 (0x21e0b87c)                                                      */
/*      SPRMCC (0x21e0b87c)                                                     */
/* Register default value on SPRA0: 0x00FFFFFF                                  */
/* Register default value on SPRB0: 0x00FFFFFF                                  */
/* Register default value on SPRHBM: 0x00FFFFFF                                 */
/* Register default value on SPRC0: 0x00FFFFFF                                  */
/* Register default value on SPRMCC: 0x00FFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* tx_rx_det
*/


#define TX_RX_DET_KTI_CIOPHYDFX_REG 0x0922087C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tx_rx_det : 24;

                            /* Bits[23:0], Access Type=RO/V, default=0x00FFFFFF*/

                            /* TX RX Detect 1 = RX detected 0 = none. */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TX_RX_DET_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* LBC_PER_IOU_CONTROL_KTI_CIOPHYDFX_REG supported on:                          */
/*      SPRA0 (0x21e0b900)                                                      */
/*      SPRB0 (0x21e0b900)                                                      */
/*      SPRHBM (0x21e0b900)                                                     */
/*      SPRC0 (0x21e0b900)                                                      */
/*      SPRMCC (0x21e0b900)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* lbc_per_iou_control
*/


#define LBC_PER_IOU_CONTROL_KTI_CIOPHYDFX_REG 0x09220900

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lbc_start : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               The SA sets this bit when a new request is sent
                               out to the load bus controller. The Ack from the
                               load bus controller resets this bit once the
                               operation is completed.
                            */
    UINT32 lbc_req : 2;

                            /* Bits[2:1], Access Type=RW, default=0x00000000*/

                            /*
                               00: Reserved 01: read 10: write 11: read-modify-
                               write
                            */
    UINT32 lbc_ld_sel : 7;

                            /* Bits[9:3], Access Type=RW, default=0x00000000*/

                            /* Selects the EP on the load bus or monitor bus. */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=0x00000000*/

                            /* Reserved Bits */

  } Bits;
  UINT32 Data;

} LBC_PER_IOU_CONTROL_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* LBC_PER_IOU_DATA_KTI_CIOPHYDFX_REG supported on:                             */
/*      SPRA0 (0x21e0b904)                                                      */
/*      SPRB0 (0x21e0b904)                                                      */
/*      SPRHBM (0x21e0b904)                                                     */
/*      SPRC0 (0x21e0b904)                                                      */
/*      SPRMCC (0x21e0b904)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* lbc_per_iou_data
*/


#define LBC_PER_IOU_DATA_KTI_CIOPHYDFX_REG 0x09220904

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lbc_data_valid : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               The LBC sets this bit in a read transaction when
                               the data is ready to be read.
                            */
    UINT32 lbc_data : 14;

                            /* Bits[14:1], Access Type=RW, default=0x00000000*/

                            /*
                               Reads: This is the data that is read out from
                               the particular lane with the monitor bus.
                               Writes: This is the data which needs to be
                               written into the selected lanes using the load
                               bus. Read-modify-Writes: This is the data that
                               needs to be used as an offset for the RMW on the
                               lanes specified by the lane select.
                            */
    UINT32 rsvd : 16;

                            /* Bits[30:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lbc_double_tmr : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               Set to 1 to double the load bus controller wait
                               timer.
                            */

  } Bits;
  UINT32 Data;

} LBC_PER_IOU_DATA_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* LBC_PER_IOU_LN_SEL_CONTROL_KTI_CIOPHYDFX_REG supported on:                   */
/*      SPRA0 (0x21e0b908)                                                      */
/*      SPRB0 (0x21e0b908)                                                      */
/*      SPRHBM (0x21e0b908)                                                     */
/*      SPRC0 (0x21e0b908)                                                      */
/*      SPRMCC (0x21e0b908)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* lbc_per_iou_ln_sel_control
*/


#define LBC_PER_IOU_LN_SEL_CONTROL_KTI_CIOPHYDFX_REG 0x09220908

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lbc_ln_sel : 24;

                            /* Bits[23:0], Access Type=RW, default=0x00000000*/

                            /*
                               For writes, reads, and read-modify-writes this
                               acts as the selection of which lanes have to go
                               through the respective transaction. Note that
                               for read this has to be one hot. [23:0] - Data
                               Lanes[23:0]
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Reserved Bits */

  } Bits;
  UINT32 Data;

} LBC_PER_IOU_LN_SEL_CONTROL_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPHPRIVPTV_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b938)                                                      */
/*      SPRB0 (0x21e0b938)                                                      */
/*      SPRHBM (0x21e0b938)                                                     */
/*      SPRC0 (0x21e0b938)                                                      */
/*      SPRMCC (0x21e0b938)                                                     */
/* Register default value on SPRA0: 0x0478C592                                  */
/* Register default value on SPRB0: 0x0478C592                                  */
/* Register default value on SPRHBM: 0x0478C592                                 */
/* Register default value on SPRC0: 0x0478C592                                  */
/* Register default value on SPRMCC: 0x0478C592                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktiphprivptv
*/


#define KTIPHPRIVPTV_KTI_CIOPHYDFX_REG 0x09220938

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 etoc_sl : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000002*/

                            /*
                               offset calibration time inside ResetC for slow
                               mode. Require time is 173us. (2 exp value) *
                               4096 UI
                            */
    UINT32 etoc : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000009*/

                            /*
                               offset calibration time inside ResetC for op
                               speed. Require time is 173us. (2 exp value) *
                               4096 UI
                            */
    UINT32 etl1oc_sl : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000005*/

                            /*
                               Delay between l1 deassertion to uniphy and
                               ocstart assertion for slow mode. Required time
                               is 30us. (2 exp value) * 128 UI
                            */
    UINT32 etl1oc : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Delay between l1 deassertion to uniphy and
                               ocstart assertion for op speed. Required time is
                               30us. (2 exp value) * 128 UI
                            */
    UINT32 etadapt : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000008*/

                            /*
                               Rx adapdation time during Polling state (2 exp
                               value) * 4096 UI
                            */
    UINT32 etbypadapt : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000007*/

                            /*
                               Rx adapdation time during Polling state adapt
                               was already done is bypassed. (2 exp value) *
                               128 UI
                            */
    UINT32 etl0padapt : 4;

                            /* Bits[27:24], Access Type=RW/P, default=0x00000004*/

                            /* L0p Exit adapt time (2 exp value) * 128 UI */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPHPRIVPTV_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPH_SQB_CTL_KTI_CIOPHYDFX_REG supported on:                                */
/*      SPRA0 (0x21e0b93c)                                                      */
/*      SPRB0 (0x21e0b93c)                                                      */
/*      SPRHBM (0x21e0b93c)                                                     */
/*      SPRC0 (0x21e0b93c)                                                      */
/*      SPRMCC (0x21e0b93c)                                                     */
/* Register default value on SPRA0: 0x008088A2                                  */
/* Register default value on SPRB0: 0x008088A2                                  */
/* Register default value on SPRHBM: 0x008088A2                                 */
/* Register default value on SPRC0: 0x008088A2                                  */
/* Register default value on SPRMCC: 0x008088A2                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktiph_sqb_ctl
*/


#define KTIPH_SQB_CTL_KTI_CIOPHYDFX_REG 0x0922093C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sqb_cnt_threshold : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000002*/

                            /*
                               Number of assertions of squelch break from IO
                               before considered debounced
                            */
    UINT32 sqb_deb_threshold : 2;

                            /* Bits[5:4], Access Type=RW/P, default=0x00000002*/

                            /*
                               Number of times debounced before initially
                               considered good
                            */
    UINT32 sqb_cnt_threshold_l0p : 4;

                            /* Bits[9:6], Access Type=RW/P, default=0x00000002*/

                            /*
                               Number of assertions of squelch break from IO
                               before considered debounced for l0p
                            */
    UINT32 sqb_deb_threshold_l0p : 2;

                            /* Bits[11:10], Access Type=RW/P, default=0x00000002*/

                            /*
                               Number of times debounced before initially
                               considered good for l0p
                            */
    UINT32 sqb_update_interval : 8;

                            /* Bits[19:12], Access Type=RW/P, default=0x00000008*/

                            /*
                               Interval during which squelch break is checked
                               during Polling/Config 512ui granularity
                            */
    UINT32 sqb_deb_interval : 5;

                            /* Bits[24:20], Access Type=RW/P, default=0x00000008*/

                            /*
                               Min interval between incrementing debounce
                               count. 8ui granularity
                            */
    UINT32 sqb_deb_ignore_cnt : 2;

                            /* Bits[26:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               After clearing sqb flop, additional time beyond
                               32ui to wait before checking sqb flop again.
                            */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPH_SQB_CTL_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* KTIPHPRIVC1_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b940)                                                      */
/*      SPRB0 (0x21e0b940)                                                      */
/*      SPRHBM (0x21e0b940)                                                     */
/*      SPRC0 (0x21e0b940)                                                      */
/*      SPRMCC (0x21e0b940)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktiphprivc1
*/


#define KTIPHPRIVC1_KTI_CIOPHYDFX_REG 0x09220940

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 spare : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* spare bits */

  } Bits;
  UINT32 Data;

} KTIPHPRIVC1_KTI_CIOPHYDFX_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPHPRIVC1_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b940)                                                      */
/*      SPRB0 (0x21e0b940)                                                      */
/*      SPRHBM (0x21e0b940)                                                     */
/*      SPRC0 (0x21e0b940)                                                      */
/*      SPRMCC (0x21e0b940)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktiphprivc1
*/


#define KTIPHPRIVC1_KTI_CIOPHYDFX_REG 0x09220940

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 l0p_entry_exit_mode : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               1'b1: master-follower mode of L0p entry/exit.
                               1'b0: legacy mode of L0p entry/exit
                            */
    UINT32 l0p_master_socket : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               1'b1: socket is programmed as master in master
                               follower L0p entry/exit mode. 1'b0: socket is
                               programmed as follower
                            */
    UINT32 csr_master_follower_tmr : 4;

                            /* Bits[5:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               programmable timer to complete Master follower
                               mode L0p entry/exit. If enabled. This timer will
                               trigger phy reset when
                               master_follower_entry_exit_time time is more
                               than programmed. here is encoding of this timer
                               4'h0: timeout value = 8k kclk cycles (8192 kclk)
                               4'h1: timeout value = 8704 kclk cycles 4'h2:
                               timeout value = 9216 kclk cycles 512 kclk
                               increase with each increment
                            */
    UINT32 chkn_rst_on_master_follower_exp : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               chicken bit to enable reset if master follower
                               mode L0p entry/exit do not complete in
                               programmable time.
                            */
    UINT32 master_follower_entry_exit_time : 14;

                            /* Bits[20:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               timer value to complete master follower mode L0p
                               entry/exit.
                            */
    UINT32 spare : 11;

                            /* Bits[31:21], Access Type=RW/P, default=0x00000000*/

                            /* spare bits */

  } Bits;
  UINT32 Data;

} KTIPHPRIVC1_KTI_CIOPHYDFX_SPRC0_SPRMCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPHPRIVC2_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b944)                                                      */
/*      SPRB0 (0x21e0b944)                                                      */
/*      SPRHBM (0x21e0b944)                                                     */
/*      SPRC0 (0x21e0b944)                                                      */
/*      SPRMCC (0x21e0b944)                                                     */
/* Register default value on SPRA0: 0xCA000000                                  */
/* Register default value on SPRB0: 0xCA000000                                  */
/* Register default value on SPRHBM: 0xCA000000                                 */
/* Register default value on SPRC0: 0xCA000000                                  */
/* Register default value on SPRMCC: 0xCA000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* ktiphprivc2
*/


#define KTIPHPRIVC2_KTI_CIOPHYDFX_REG 0x09220944

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 spare : 24;

                            /* Bits[23:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Lower two bits used for chicken bits for HSD
                               312593 : Slowmode sample logic bug when sample
                               point aligns rougly to the internal slowmode
                               clock can cause 1UI shifts of data when data
                               edges move slightly. Bit 0 : When set to 1 -
                               Allow drift buffer reset during deskew in
                               slowmode Bit 1 : When set to 1 - Allow slowmode
                               sample lock during deskew
                            */
    UINT32 openloop_start_sel : 4;

                            /* Bits[27:24], Access Type=RW/P/L, default=0x0000000A*/

                            /*
                               Start selection function. 0000 - Micro Break
                               Point 0 (MBP[0]) 0001 - Micro Break Point 1
                               (MBP[1]) 0010 - Cluster Trigger Out 0 (CTO[0])
                               0011 - Cluster Trigger Out 1 (CTO[1]) 0100 -
                               Artificial Starvation Counter 0 (ASC[0]) 0101 -
                               Artificial Starvation Counter 1 (ASC[1]) 0110 -
                               Common Trigger Sequencer TrigOut (CTS_Trig_Out)
                               0111 - Reserved 1000 - MCA Correctable Error
                               1001 - MCA Uncorrectable Error 1010 - Response
                               never occurs 1011 - Response occurs immediately
                               1100-1111 - Reserved
                            */
    UINT32 openloop_stop_sel : 4;

                            /* Bits[31:28], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               Stop selection response function. 0000 - Micro
                               Break Point 0 (MBP[0] 0001 - Micro Break Point 1
                               (MBP[1]) 0010 - Cluster Trigger Out 0 (CTO[0])
                               0011 - Cluster Trigger Out 1 (CTO[1]) 0100 -
                               Artificial Starvation Counter 0 (ASC[0]) 0101 -
                               Artificial Starvation Counter 1 (ASC[1]) 0110 -
                               Common Trigger Sequencer TrigOut (CTS_Trig_Out)
                               0111 - Reserved 1000 - MCA Correctable Error
                               1001 - MCA Uncorrectable Error 1010 - Deactivate
                               one clock after activation. Error injection on
                               one transaction will occur on the interface that
                               is enabled. 1011 - Response is never
                               deactivated. 1100 - Deactivate the response when
                               the activating signal becomes false. 1101-1111 -
                               Reserve
                            */

  } Bits;
  UINT32 Data;

} KTIPHPRIVC2_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPHPRIVC3_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b948)                                                      */
/*      SPRB0 (0x21e0b948)                                                      */
/*      SPRHBM (0x21e0b948)                                                     */
/*      SPRC0 (0x21e0b948)                                                      */
/*      SPRMCC (0x21e0b948)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* ktiphprivc3
*/


#define KTIPHPRIVC3_KTI_CIOPHYDFX_REG 0x09220948

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 12;

                            /* Bits[11:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pat_det_only : 1;

                            /* Bits[12:12], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Send 1K supersequences instead of 4k
                               supersequences
                            */
    UINT32 rst_txalign_en : 1;

                            /* Bits[13:13], Access Type=RW/P/L, default=0x00000000*/

                            /* If set, reset txfifo on soft reset */
    UINT32 tx_rxdetect_en : 1;

                            /* Bits[14:14], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Transmit RX Detect Enable Override value 1 = TX
                               AFE RX detect circuit enabled. 0 = disabled.
                            */
    UINT32 tx_rxdetect_en_ovr : 1;

                            /* Bits[15:15], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               tx_rxdetect_en AFE pin is normally controlled by
                               agent logic. This bit enables the tx_rxdetect_en
                               CSR bit to override that logic.
                            */
    UINT32 lnrevst : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */
    UINT32 sso_lock_first : 1;

                            /* Bits[17:17], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Lock each lanes slow mode period on first edge
                               detect
                            */
    UINT32 sso_init_flipphase : 1;

                            /* Bits[18:18], Access Type=RW/P/L, default=0x00000000*/

                            /* Slow mode initial flip phase setting */
    UINT32 sso_init_shiftby1 : 1;

                            /* Bits[19:19], Access Type=RW/P/L, default=0x00000000*/

                            /* Slow mode initial shift by 1 setting */
    UINT32 sso_period_ovr : 5;

                            /* Bits[24:20], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Slow mode period override. Specified in KF
                               clocks.
                            */
    UINT32 sso_period_ovr_en : 1;

                            /* Bits[25:25], Access Type=RW/P/L, default=0x00000000*/

                            /* Enable override of the slow mode period. */
    UINT32 sso_sample_ovr : 5;

                            /* Bits[30:26], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Override the sampling time from the slow mode
                               edge. Speficied in KF clocks.
                            */
    UINT32 sso_sample_ovr_en : 1;

                            /* Bits[31:31], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Enable override of the slow mode bit sampling
                               time.
                            */

  } Bits;
  UINT32 Data;

} KTIPHPRIVC3_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPHPRIVC4_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b94c)                                                      */
/*      SPRB0 (0x21e0b94c)                                                      */
/*      SPRHBM (0x21e0b94c)                                                     */
/*      SPRC0 (0x21e0b94c)                                                      */
/*      SPRMCC (0x21e0b94c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* ktiphprivc4
*/


#define KTIPHPRIVC4_KTI_CIOPHYDFX_REG 0x0922094C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bypass_slow_icomp_tmr : 1;

                            /* Bits[0:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               When set, bypass slow ICOMP timer in
                               Detect.FwdClk or Compliance state
                            */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bypass_slow_oc_tmr : 1;

                            /* Bits[2:2], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               When set, bypass slow OC timer in Detect.FwdClk
                               or Compliance state
                            */
    UINT32 bypass_fast_icomp_tmr : 1;

                            /* Bits[3:3], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               When set, bypass fast ICOMP timer in
                               Detect.FwdClk or Compliance state
                            */
    UINT32 bypass_fast_adapt_tmr : 1;

                            /* Bits[4:4], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               When set, bypass fast DCC timer in Detect.FwdClk
                               or Compliance state
                            */
    UINT32 bypass_fast_oc_tmr : 1;

                            /* Bits[5:5], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               When set, bypass fast OC timer in Detect.FwdClk
                               or Compliance state
                            */
    UINT32 det_clk_ovr_en : 1;

                            /* Bits[6:6], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               When set, it override the uniphy rx detect clock
                               signal with det_clk_ovr value in this register
                            */
    UINT32 det_clk_ovr : 1;

                            /* Bits[7:7], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               rx detect clock override value when override
                               enable is set.
                            */
    UINT32 lb_pat_openloop : 1;

                            /* Bits[8:8], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Open Loop in Loopback.Pattern state, instead of
                               close loop.
                            */
    UINT32 l1_slave : 1;

                            /* Bits[9:9], Access Type=RW/P/L, default=0x00000000*/

                            /* When set, the agent will not master L1 requests */
    UINT32 rsvd_10 : 16;

                            /* Bits[25:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 phl0sync : 1;

                            /* Bits[26:26], Access Type=RW/P/L, default=0x00000000*/

                            /* Master mode phyL0sync. Note: Obsolete from JKT */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPHPRIVC4_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPHPRIVC5_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b950)                                                      */
/*      SPRB0 (0x21e0b950)                                                      */
/*      SPRHBM (0x21e0b950)                                                     */
/*      SPRC0 (0x21e0b950)                                                      */
/*      SPRMCC (0x21e0b950)                                                     */
/* Register default value on SPRA0: 0x04000000                                  */
/* Register default value on SPRB0: 0x04000000                                  */
/* Register default value on SPRHBM: 0x04000000                                 */
/* Register default value on SPRC0: 0x04000000                                  */
/* Register default value on SPRMCC: 0x04000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* ktiphprivc5
*/


#define KTIPHPRIVC5_KTI_CIOPHYDFX_REG 0x09220950

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dis_l0p_l1_tx : 1;

                            /* Bits[0:0], Access Type=RW/P/L, default=0x00000000*/

                            /* Disable power down of tx to l1 mode during L0p */
    UINT32 dis_l0p_l1_rx : 1;

                            /* Bits[1:1], Access Type=RW/P/L, default=0x00000000*/

                            /* Disable power down of rx to l1 mode during L0p */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 l0p_fts_dskw_dis : 1;

                            /* Bits[3:3], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               L0p rx uses EIEOS instead of FTS to deskew on
                               L0p exit.
                            */
    UINT32 stagger_fts_dis : 1;

                            /* Bits[4:4], Access Type=RW/P/L, default=0x00000000*/

                            /* Disable FTS staggering */
    UINT32 rsvd_5 : 11;

                            /* Bits[15:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rxl0c_mmerr_log_dis : 1;

                            /* Bits[16:16], Access Type=RW/P/L, default=0x00000000*/

                            /* Do not log rx l0c mismatches */
    UINT32 rxl0c_lnerr_log_dis : 1;

                            /* Bits[17:17], Access Type=RW/P/L, default=0x00000000*/

                            /* Do not log rx l0c lane errors */
    UINT32 rsvd_18 : 7;

                            /* Bits[24:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 l0c_upper : 1;

                            /* Bits[25:25], Access Type=RW/P/L, default=0x00000000*/

                            /* Use upper lanes for L0c if available */
    UINT32 l0c_err_threshold : 2;

                            /* Bits[27:26], Access Type=RW/P/L, default=0x00000001*/

                            /*
                               Number of acceptable lane errors before
                               considering mismatch.
                            */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPHPRIVC5_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPHPRIVC7_KTI_CIOPHYDFX_REG supported on:                                  */
/*      SPRA0 (0x21e0b954)                                                      */
/*      SPRB0 (0x21e0b954)                                                      */
/*      SPRHBM (0x21e0b954)                                                     */
/*      SPRC0 (0x21e0b954)                                                      */
/*      SPRMCC (0x21e0b954)                                                     */
/* Register default value on SPRA0: 0x00001222                                  */
/* Register default value on SPRB0: 0x00001222                                  */
/* Register default value on SPRHBM: 0x00001222                                 */
/* Register default value on SPRC0: 0x00001222                                  */
/* Register default value on SPRMCC: 0x00001222                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* ktiphprivc7
*/


#define KTIPHPRIVC7_KTI_CIOPHYDFX_REG 0x09220954

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 et_pml1_spwrdn_s1 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000002*/

                            /*
                               Exponential Timer for PML1 Stagger Power down
                               stage 1 (for asserting L1 signals to uniphy)
                               Time-out value is 2(value) * 1 TSL (Training
                               Sequence Length) This timer should be greater
                               than or equal to 19 * (stagger cnt + 1) 18 is
                               based on the number of stagger data lanes (in
                               this case data only lanes)
                            */
    UINT32 et_pml1_spwrdn_s0 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000002*/

                            /*
                               Exponential Timer for PML1 Stagger Power down
                               stage 0 (for asserting L0s signals to uniphy)
                               Time-out value is 2(value) * 1 TSL (Training
                               Sequence Length) This timer should be greater
                               than or equal to 19 * (stagger cnt + 1) 18 is
                               based on the number of stagger data lanes (in
                               this case data only lanes)
                            */
    UINT32 et_lbm_sync : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000002*/

                            /*
                               Exponential Timer to loopback marker sync when
                               enhance adaption is enabled. Time-out value is
                               2(value) * 1 TSL (Training Sequence Length)
                            */
    UINT32 et_pml0_min : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000001*/

                            /*
                               Exponential Timer for minimum time to spend in
                               TX L0 state before ktiph PM logic allow link
                               layer to enter any power state. Value of 0 means
                               no mnimum time requirement. Time-out value is
                               2(value) * 1 TSL (Training Sequence Length)
                            */
    UINT32 rsvd : 8;

                            /* Bits[23:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rst_tmg_sel : 2;

                            /* Bits[25:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field controls the assertion timing of
                               assorted reset signals based on a clock enable.
                               Bit 1 controls the timing for all internal
                               resets except Tx datapath soft reset; bit 1
                               covers Tx datapath soft reset. A bit value of 0
                               is consistent with a B-step bug fix that delayed
                               all of the resets by one cycle. A bit value of 1
                               reverts to the A-step timings.
                            */
    UINT32 rsvd_26 : 2;

                            /* Bits[27:26], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 enhance_adapt_done : 1;

                            /* Bits[28:28], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Reset to 0 at UPI Cold Reset or UPI warm reset
                               Hardware will set to 1 once enhance Adaptive EQ
                               is complete.
                            */
    UINT32 enhance_adapt_en : 1;

                            /* Bits[29:29], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               Enhance adapation enable. Clear by hardware when
                               enhance adapt is completed.
                            */
    UINT32 dis_tx_afe_clkdist_l1 : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable TX AFE clock distribution when ISM in L1
                               state
                            */
    UINT32 dis_rx_afe_clkdist_l1 : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               Disable RX AFE clock distribution when ISM in L1
                               state
                            */

  } Bits;
  UINT32 Data;

} KTIPHPRIVC7_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPHPRIVC10_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b958)                                                      */
/*      SPRB0 (0x21e0b958)                                                      */
/*      SPRHBM (0x21e0b958)                                                     */
/*      SPRC0 (0x21e0b958)                                                      */
/*      SPRMCC (0x21e0b958)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* ktiphprivc10
*/


#define KTIPHPRIVC10_KTI_CIOPHYDFX_REG 0x09220958

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 20;

                            /* Bits[19:0], Access Type=RO, default=0x00000000*/

                            /* Reserved bits */
    UINT32 fast_icomp_done : 1;

                            /* Bits[20:20], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Fast ICOMP done. It is set by HW when ICOMP
                               timer is expired in operation speed
                               initialization. Besides RW1C by SW, this bit
                               also clear by HW when link speed or TX clock
                               pointer is changed. 0 - wait for ICOMP timer to
                               advance AINIT state 1 - bypass ICOMP time
                            */
    UINT32 slow_icomp_done : 1;

                            /* Bits[21:21], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Slow ICOMP done. It is set by HW when ICOMP
                               timer is expired in slow speed initialization.
                               Besides RW1C by SW, this bit also clear by HW
                               when link speed or TX clock pointer is changed.
                               0 - wait for ICOMP timer to advance AINIT state
                               1 - bypass ICOMP timer
                            */
    UINT32 fast_adapt_done : 1;

                            /* Bits[22:22], Access Type=RW/1C/V, default=0x00000000*/

                            /* Indicates Fast Adapt has been completed. */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fast_oc_done : 1;

                            /* Bits[24:24], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Fast OC done. It is set by HW when OC timer is
                               expired in operation speed initialization.
                               Besides RW1C by SW, this bit also clear by HW
                               when link speed or RX clock pointer is changed.
                               0 - wait for OC timer to advance AINIT state 1 -
                               bypass OCC time
                            */
    UINT32 slow_oc_done : 1;

                            /* Bits[25:25], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Slow OC done. It is set by HW when OC timer is
                               expired in slow speed initialization. Besides
                               RW1C by SW, this bit also clear by HW when link
                               speed or RX clock pointer is changed. 0 - wait
                               for OC timer to advance AINIT state 1 - bypass
                               OC timer
                            */
    UINT32 ignore_fast_icomp_done : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               when set, ignore fast ICOMP done bit. It is used
                               to force to wait for ICOMP timer if byass ICOMP
                               is not set in privc4.
                            */
    UINT32 ignore_slow_icomp_done : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               when set, ignore slow ICOMP done bit. It is used
                               to force to wait for ICOMP timer if byass ICOMP
                               is not set in privc4.
                            */
    UINT32 ignore_fast_adapt_done : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               when set, ignore fast ADAPT done bit. It is used
                               to force to wait for ADAP Timer if bypass ADAPT
                               is not set in privc4.
                            */
    UINT32 recal_req_oc_en : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /*
                               when set, re-calibration request from AFE will
                               trigger OC
                            */
    UINT32 ignore_fast_oc_done : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               when set, ignore fast OC done bit. It is used to
                               force to wait for OC timer if byass OC is not
                               set in privc4.
                            */
    UINT32 ignore_slow_oc_done : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               when set, ignore slow OC done bit. It is used to
                               force to wait for OC timer if byass OC is not
                               set in privc4.
                            */

  } Bits;
  UINT32 Data;

} KTIPHPRIVC10_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPH_RXRETRY_OPENLOOP_KTI_CIOPHYDFX_REG supported on:                       */
/*      SPRA0 (0x21e0b96c)                                                      */
/*      SPRB0 (0x21e0b96c)                                                      */
/*      SPRHBM (0x21e0b96c)                                                     */
/*      SPRC0 (0x21e0b96c)                                                      */
/*      SPRMCC (0x21e0b96c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* ktiph_rxretry_openloop
*/


#define KTIPH_RXRETRY_OPENLOOP_KTI_CIOPHYDFX_REG 0x0922096C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 et_rx_retry_openloop : 4;

                            /* Bits[3:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Exponential Time for Rx retry openloop Count not
                               0: When either L0p or L0s low power state is
                               enabled and link layer detect CRC error, it will
                               signal Physical layer to enable open loop until
                               next good flit is received. This Exponential
                               timer is used to count the time for Rx retry
                               openloop. If the timer is expired, it will
                               trigger a soft reset to reset the link. Timeout
                               value is 2(count in this field)*TSL UI Count is
                               0: timer is disabled
                            */
    UINT32 rsvd : 27;

                            /* Bits[30:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rx_retry_openloop_timeout : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               A value of 1 indicate RX retry timer expried and
                               a soft reset is trigger to reset the link.
                            */

  } Bits;
  UINT32 Data;

} KTIPH_RXRETRY_OPENLOOP_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPHPRIVC13_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0b970)                                                      */
/*      SPRB0 (0x21e0b970)                                                      */
/*      SPRHBM (0x21e0b970)                                                     */
/*      SPRC0 (0x21e0b970)                                                      */
/*      SPRMCC (0x21e0b970)                                                     */
/* Register default value on SPRA0: 0x00080080                                  */
/* Register default value on SPRB0: 0x00080080                                  */
/* Register default value on SPRHBM: 0x00080080                                 */
/* Register default value on SPRC0: 0x00080080                                  */
/* Register default value on SPRMCC: 0x00080080                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* ktiphprivc13
*/


#define KTIPHPRIVC13_KTI_CIOPHYDFX_REG 0x09220970

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rx_dbg_lane_mode0 : 1;

                            /* Bits[0:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               pre-visa mux for selecting a) raw data or b)
                               descrambled polarity-inversion-corrected masked-
                               for-bad-lanes data. 0 - raw data 1 - descrambled
                               polarity-inversion-corrected masked-for-bad-
                               lanes
                            */
    UINT32 rx_dbg_lane_mode1 : 1;

                            /* Bits[1:1], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               pre-visa mux for selecting a) raw data or b)
                               descrambled polarity-inversion-corrected masked-
                               for-bad-lanes data. 0 - raw data 1 - descrambled
                               polarity-inversion-corrected masked-for-bad-
                               lanes
                            */
    UINT32 rx_dbg_lane_sel0 : 5;

                            /* Bits[6:2], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               pre-visa mux for selecting rx data lane to send
                               to visa
                            */
    UINT32 rx_dbg_lane_sel1 : 5;

                            /* Bits[11:7], Access Type=RW/P/L, default=0x00000001*/

                            /*
                               pre-visa mux for selecting rx data lane to send
                               to visa
                            */
    UINT32 tx_dbg_lane_mode0 : 1;

                            /* Bits[12:12], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               pre-visa mux for selecting tx unscrambled or
                               scrambled data. 0 - unscrambled data 1 -
                               scrambled data
                            */
    UINT32 tx_dbg_lane_mode1 : 1;

                            /* Bits[13:13], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               pre-visa mux for selecting tx unscrambled or
                               scrambled data. 0 - unscrambled data 1 -
                               scrambled data
                            */
    UINT32 tx_dbg_lane_sel0 : 5;

                            /* Bits[18:14], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               pre-visa mux for selecting tx data lane to send
                               to visa
                            */
    UINT32 tx_dbg_lane_sel1 : 5;

                            /* Bits[23:19], Access Type=RW/P/L, default=0x00000001*/

                            /*
                               pre-visa mux for selecting tx data lane to send
                               to visa
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPHPRIVC13_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTI_RESP_TXINV_CTL_KTI_CIOPHYDFX_REG supported on:                           */
/*      SPRA0 (0x21e0b9d4)                                                      */
/*      SPRB0 (0x21e0b9d4)                                                      */
/*      SPRHBM (0x21e0b9d4)                                                     */
/*      SPRC0 (0x21e0b9d4)                                                      */
/*      SPRMCC (0x21e0b9d4)                                                     */
/* Register default value on SPRA0: 0xCA000000                                  */
/* Register default value on SPRB0: 0xCA000000                                  */
/* Register default value on SPRHBM: 0xCA000000                                 */
/* Register default value on SPRC0: 0xCA000000                                  */
/* Register default value on SPRMCC: 0xCA000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* kti_resp_txinv_ctl
*/


#define KTI_RESP_TXINV_CTL_KTI_CIOPHYDFX_REG 0x092209D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lnen : 24;

                            /* Bits[23:0], Access Type=RW/P/L, default=0x00000000*/

                            /* error inject lane select */
    UINT32 start_sel : 4;

                            /* Bits[27:24], Access Type=RW/P/L, default=0x0000000A*/

                            /*
                               Start selection function. 0000 - Micro Break
                               Point 0 (MBP[0]) 0001 - Micro Break Point 1
                               (MBP[1]) 0010 - Cluster Trigger Out 0 (CTO[0])
                               0011 - Cluster Trigger Out 1 (CTO[1]) 0100 -
                               Artificial Starvation Counter 0 (ASC[0]) 0101 -
                               Artificial Starvation Counter 1 (ASC[1]) 0110 -
                               Common Trigger Sequencer TrigOut (CTS_Trig_Out)
                               0111 - Reserved 1000 - MCA Correctable Error
                               1001 - MCA Uncorrectable Error 1010 - Response
                               never occurs 1011 - Response occurs immediately
                               1100-1111 - Reserved
                            */
    UINT32 stop_sel : 4;

                            /* Bits[31:28], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               Stop selection response function. 0000 - Micro
                               Break Point 0 (MBP[0] 0001 - Micro Break Point 1
                               (MBP[1]) 0010 - Cluster Trigger Out 0 (CTO[0])
                               0011 - Cluster Trigger Out 1 (CTO[1]) 0100 -
                               Artificial Starvation Counter 0 (ASC[0]) 0101 -
                               Artificial Starvation Counter 1 (ASC[1]) 0110 -
                               Common Trigger Sequencer TrigOut (CTS_Trig_Out)
                               0111 - Reserved 1000 - MCA Correctable Error
                               1001 - MCA Uncorrectable Error 1010 - Deactivate
                               one clock after activation. Error injection on
                               one transaction will occur on the interface that
                               is enabled. 1011 - Response is never
                               deactivated. 1100 - Deactivate the response when
                               the activating signal becomes false. 1101-1111 -
                               Reserved
                            */

  } Bits;
  UINT32 Data;

} KTI_RESP_TXINV_CTL_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTI_RESP_PHRST_CTL_KTI_CIOPHYDFX_REG supported on:                           */
/*      SPRA0 (0x21e0b9dc)                                                      */
/*      SPRB0 (0x21e0b9dc)                                                      */
/*      SPRHBM (0x21e0b9dc)                                                     */
/*      SPRC0 (0x21e0b9dc)                                                      */
/*      SPRMCC (0x21e0b9dc)                                                     */
/* Register default value on SPRA0: 0xCA000000                                  */
/* Register default value on SPRB0: 0xCA000000                                  */
/* Register default value on SPRHBM: 0xCA000000                                 */
/* Register default value on SPRC0: 0xCA000000                                  */
/* Register default value on SPRMCC: 0xCA000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* kti_resp_phrst_ctl
*/


#define KTI_RESP_PHRST_CTL_KTI_CIOPHYDFX_REG 0x092209DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 24;

                            /* Bits[23:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 start_sel : 4;

                            /* Bits[27:24], Access Type=RW/P/L, default=0x0000000A*/

                            /*
                               Start selection function. 0000 - Micro Break
                               Point 0 (MBP[0]) 0001 - Micro Break Point 1
                               (MBP[1]) 0010 - Cluster Trigger Out 0 (CTO[0])
                               0011 - Cluster Trigger Out 1 (CTO[1]) 0100 -
                               Artificial Starvation Counter 0 (ASC[0]) 0101 -
                               Artificial Starvation Counter 1 (ASC[1]) 0110 -
                               Common Trigger Sequencer TrigOut (CTS_Trig_Out)
                               0111 - Reserved 1000 - MCA Correctable Error
                               1001 - MCA Uncorrectable Error 1010 - Response
                               never occurs 1011 - Response occurs immediately
                               1100-1111 - Reserved
                            */
    UINT32 stop_sel : 4;

                            /* Bits[31:28], Access Type=RW/P/L, default=0x0000000C*/

                            /*
                               Stop selection response function. 0000 - Micro
                               Break Point 0 (MBP[0] 0001 - Micro Break Point 1
                               (MBP[1]) 0010 - Cluster Trigger Out 0 (CTO[0])
                               0011 - Cluster Trigger Out 1 (CTO[1]) 0100 -
                               Artificial Starvation Counter 0 (ASC[0]) 0101 -
                               Artificial Starvation Counter 1 (ASC[1]) 0110 -
                               Common Trigger Sequencer TrigOut (CTS_Trig_Out)
                               0111 - Reserved 1000 - MCA Correctable Error
                               1001 - MCA Uncorrectable Error 1010 - Deactivate
                               one clock after activation. Error injection on
                               one transaction will occur on the interface that
                               is enabled. 1011 - Response is never
                               deactivated. 1100 - Deactivate the response when
                               the activating signal becomes false. 1101-1111 -
                               Reserved
                            */

  } Bits;
  UINT32 Data;

} KTI_RESP_PHRST_CTL_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPH_RXL0C_ERR_STS_KTI_CIOPHYDFX_REG supported on:                          */
/*      SPRA0 (0x21e0b9f0)                                                      */
/*      SPRB0 (0x21e0b9f0)                                                      */
/*      SPRHBM (0x21e0b9f0)                                                     */
/*      SPRC0 (0x21e0b9f0)                                                      */
/*      SPRMCC (0x21e0b9f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: COR_CNT                                          */
/* SPRB0 Security PolicyGroup: COR_CNT                                          */
/* SPRHBM Security PolicyGroup: COR_CNT                                         */
/* SPRC0 Security PolicyGroup: COR_CNT                                          */
/* SPRMCC Security PolicyGroup: COR_CNT                                         */
/* ktiph_rxl0c_err_sts
*/


#define KTIPH_RXL0C_ERR_STS_KTI_CIOPHYDFX_REG 0x092209F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lane_err : 8;

                            /* Bits[7:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Lanes that had errors */
    UINT32 force_mismatch : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* L0c mismatch due to >= 4 lanes with error */
    UINT32 code_mismatch : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* L0c code did not match */
    UINT32 clm : 3;

                            /* Bits[12:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* CLM during l0c error */
    UINT32 lane_rev : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Lane reversal status during l0c error */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPH_RXL0C_ERR_STS_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPH_RXL0C_ERR_LOG0_KTI_CIOPHYDFX_REG supported on:                         */
/*      SPRA0 (0x21e0b9f4)                                                      */
/*      SPRB0 (0x21e0b9f4)                                                      */
/*      SPRHBM (0x21e0b9f4)                                                     */
/*      SPRC0 (0x21e0b9f4)                                                      */
/*      SPRMCC (0x21e0b9f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: COR_CNT                                          */
/* SPRB0 Security PolicyGroup: COR_CNT                                          */
/* SPRHBM Security PolicyGroup: COR_CNT                                         */
/* SPRC0 Security PolicyGroup: COR_CNT                                          */
/* SPRMCC Security PolicyGroup: COR_CNT                                         */
/* ktiph_rxl0c_err_log0
*/


#define KTIPH_RXL0C_ERR_LOG0_KTI_CIOPHYDFX_REG 0x092209F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lane0 : 8;

                            /* Bits[7:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* lane 0 l0c code */
    UINT32 lane1 : 8;

                            /* Bits[15:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* lane 1 l0c code */
    UINT32 lane2 : 8;

                            /* Bits[23:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* lane 2 l0c code */
    UINT32 lane3 : 8;

                            /* Bits[31:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* lane 3 l0c code */

  } Bits;
  UINT32 Data;

} KTIPH_RXL0C_ERR_LOG0_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPH_RXL0C_ERR_LOG1_KTI_CIOPHYDFX_REG supported on:                         */
/*      SPRA0 (0x21e0b9f8)                                                      */
/*      SPRB0 (0x21e0b9f8)                                                      */
/*      SPRHBM (0x21e0b9f8)                                                     */
/*      SPRC0 (0x21e0b9f8)                                                      */
/*      SPRMCC (0x21e0b9f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: COR_CNT                                          */
/* SPRB0 Security PolicyGroup: COR_CNT                                          */
/* SPRHBM Security PolicyGroup: COR_CNT                                         */
/* SPRC0 Security PolicyGroup: COR_CNT                                          */
/* SPRMCC Security PolicyGroup: COR_CNT                                         */
/* ktiph_rxl0c_err_log1
*/


#define KTIPH_RXL0C_ERR_LOG1_KTI_CIOPHYDFX_REG 0x092209F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lane4 : 8;

                            /* Bits[7:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* lane 4 l0c code */
    UINT32 lane5 : 8;

                            /* Bits[15:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* lane 5 l0c code */
    UINT32 lane6 : 8;

                            /* Bits[23:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* lane 6 l0c code */
    UINT32 lane7 : 8;

                            /* Bits[31:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* lane 7 l0c code */

  } Bits;
  UINT32 Data;

} KTIPH_RXL0C_ERR_LOG1_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPH_RXRESEED_ERR_KTI_CIOPHYDFX_REG supported on:                           */
/*      SPRA0 (0x21e0b9fc)                                                      */
/*      SPRB0 (0x21e0b9fc)                                                      */
/*      SPRHBM (0x21e0b9fc)                                                     */
/*      SPRC0 (0x21e0b9fc)                                                      */
/*      SPRMCC (0x21e0b9fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* ktiph_rxreseed_err
*/


#define KTIPH_RXRESEED_ERR_KTI_CIOPHYDFX_REG 0x092209FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lane : 24;

                            /* Bits[23:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Lane encoutered an error in sync bits after
                               reseeding
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPH_RXRESEED_ERR_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPH_STGR_CTL_KTI_CIOPHYDFX_REG supported on:                               */
/*      SPRA0 (0x21e0ba00)                                                      */
/*      SPRB0 (0x21e0ba00)                                                      */
/*      SPRHBM (0x21e0ba00)                                                     */
/*      SPRC0 (0x21e0ba00)                                                      */
/*      SPRMCC (0x21e0ba00)                                                     */
/* Register default value on SPRA0: 0x0000B108                                  */
/* Register default value on SPRB0: 0x0000B108                                  */
/* Register default value on SPRHBM: 0x0000B108                                 */
/* Register default value on SPRC0: 0x0000B108                                  */
/* Register default value on SPRMCC: 0x0000B108                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktiph_stgr_ctl
*/


#define KTIPH_STGR_CTL_KTI_CIOPHYDFX_REG 0x09220A00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 stgr_cnt : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000008*/

                            /*
                               Wait time between shifting tokens. Count = value
                               in this field + 1.
                            */
    UINT32 l0s_wait_cnt : 6;

                            /* Bits[9:4], Access Type=RW/P, default=0x00000010*/

                            /*
                               Wait time from l0s change to the next l0s/l1
                               change on the same lane
                            */
    UINT32 l1_wait_cnt : 6;

                            /* Bits[15:10], Access Type=RW/P, default=0x0000002C*/

                            /*
                               Wait time from l1 change to the next l0s/l1
                               change on the same lane
                            */
    UINT32 l0p_tkn_dis : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               0:use ktiph_stgt_tkn.token_l0p for l0p tokens 1:
                               use ktiph_stgt_tkn.token for l0p tokens
                            */
    UINT32 rsvd : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPH_STGR_CTL_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPH_STGR_TKN_KTI_CIOPHYDFX_REG supported on:                               */
/*      SPRA0 (0x21e0ba04)                                                      */
/*      SPRB0 (0x21e0ba04)                                                      */
/*      SPRHBM (0x21e0ba04)                                                     */
/*      SPRC0 (0x21e0ba04)                                                      */
/*      SPRMCC (0x21e0ba04)                                                     */
/* Register default value on SPRA0: 0x00001001                                  */
/* Register default value on SPRB0: 0x00001001                                  */
/* Register default value on SPRHBM: 0x00001001                                 */
/* Register default value on SPRC0: 0x00001001                                  */
/* Register default value on SPRMCC: 0x00001001                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktiph_stgr_tkn
*/


#define KTIPH_STGR_TKN_KTI_CIOPHYDFX_REG 0x09220A04

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 token : 24;

                            /* Bits[23:0], Access Type=RW/P, default=0x00001001*/

                            /* Initial tokens for io power staggering */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Reserved Bits */

  } Bits;
  UINT32 Data;

} KTIPH_STGR_TKN_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPH_STGR_TKN_L0P_KTI_CIOPHYDFX_REG supported on:                           */
/*      SPRA0 (0x21e0ba08)                                                      */
/*      SPRB0 (0x21e0ba08)                                                      */
/*      SPRHBM (0x21e0ba08)                                                     */
/*      SPRC0 (0x21e0ba08)                                                      */
/*      SPRMCC (0x21e0ba08)                                                     */
/* Register default value on SPRA0: 0x00000101                                  */
/* Register default value on SPRB0: 0x00000101                                  */
/* Register default value on SPRHBM: 0x00000101                                 */
/* Register default value on SPRC0: 0x00000101                                  */
/* Register default value on SPRMCC: 0x00000101                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktiph_stgr_tkn_l0p
*/


#define KTIPH_STGR_TKN_L0P_KTI_CIOPHYDFX_REG 0x09220A08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 token_l0p : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000101*/

                            /* Initial tokens for io power staggering l0p lanes */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* Reserved Bits */

  } Bits;
  UINT32 Data;

} KTIPH_STGR_TKN_L0P_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPH_RXDET_CTL_KTI_CIOPHYDFX_REG supported on:                              */
/*      SPRA0 (0x21e0ba0c)                                                      */
/*      SPRB0 (0x21e0ba0c)                                                      */
/*      SPRHBM (0x21e0ba0c)                                                     */
/*      SPRC0 (0x21e0ba0c)                                                      */
/*      SPRMCC (0x21e0ba0c)                                                     */
/* Register default value on SPRA0: 0x00605376                                  */
/* Register default value on SPRB0: 0x00605376                                  */
/* Register default value on SPRHBM: 0x00605376                                 */
/* Register default value on SPRC0: 0x00605376                                  */
/* Register default value on SPRMCC: 0x00605376                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktiph_rxdet_ctl
*/


#define KTIPH_RXDET_CTL_KTI_CIOPHYDFX_REG 0x09220A0C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 legchk : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000006*/

                            /*
                               Wait time after assertion of rxdetect_en.
                               8ui*2legchk. Min 30ns
                            */
    UINT32 debtmr : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000007*/

                            /*
                               Duration between 2 samples of a leg debounce.
                               8ui*debtmr
                            */
    UINT32 debcntr : 2;

                            /* Bits[9:8], Access Type=RW/P, default=0x00000003*/

                            /* Number of samples for a leg debounce */
    UINT32 interval : 4;

                            /* Bits[13:10], Access Type=RW/P, default=0x00000004*/

                            /*
                               Interval between deassertion of rxdetect_en to
                               its reassertion. 8ui*2interval.
                            */
    UINT32 consec_legchk : 2;

                            /* Bits[15:14], Access Type=RW/P, default=0x00000001*/

                            /*
                               Number of times to check legs. 2 means, debounce
                               N leg once, and debounce P leg once
                            */
    UINT32 itercnt : 3;

                            /* Bits[18:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Number of attempts to redo rx detect before
                               completing, if one or more but not all lanes
                               detected.
                            */
    UINT32 init_wait : 5;

                            /* Bits[23:19], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Number of clocks to wait before asserting
                               rxdet_en after entry to DET_RX. 8ui*2init_wait.
                               Min 150ns
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPH_RXDET_CTL_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPH_L0PEXIT_ERR_KTI_CIOPHYDFX_REG supported on:                            */
/*      SPRA0 (0x21e0ba10)                                                      */
/*      SPRB0 (0x21e0ba10)                                                      */
/*      SPRHBM (0x21e0ba10)                                                     */
/*      SPRC0 (0x21e0ba10)                                                      */
/*      SPRMCC (0x21e0ba10)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktiph_l0pexit_err
*/


#define KTIPH_L0PEXIT_ERR_KTI_CIOPHYDFX_REG 0x09220A10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lane : 24;

                            /* Bits[23:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Physical Rx lane associated with L0p Exit error */
    UINT32 l0p_deskew_err : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Lane error encountered during L0p Exit deskew */
    UINT32 offset_err : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Active/Idle lane offset correction failed */
    UINT32 bn_mismatch : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BN/BNb mismatch in received SDSp */
    UINT32 timeout : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* TL0pest timeout occurred */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPH_L0PEXIT_ERR_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPH_TXL0P_ALARM_KTI_CIOPHYDFX_REG supported on:                            */
/*      SPRA0 (0x21e0ba18)                                                      */
/*      SPRB0 (0x21e0ba18)                                                      */
/*      SPRHBM (0x21e0ba18)                                                     */
/*      SPRC0 (0x21e0ba18)                                                      */
/*      SPRMCC (0x21e0ba18)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* ktiph_txl0p_alarm
*/


#define KTIPH_TXL0P_ALARM_KTI_CIOPHYDFX_REG 0x09220A18

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 t_entry_alarm : 10;

                            /* Bits[9:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enter counter value at which ktiph will assert
                               l0p to ktiln 0h: Reserved
                            */
    UINT32 t_exit_alarm : 13;

                            /* Bits[22:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               Exit counter value at which ktiph will deassert
                               l0p to ktiln 0h: Reserved
                            */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} KTIPH_TXL0P_ALARM_KTI_CIOPHYDFX_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST)
/* KTIPHYMODLCK_KTI_CIOPHYDFX_REG supported on:                                 */
/*      SPRA0 (0x21e0bb00)                                                      */
/*      SPRB0 (0x21e0bb00)                                                      */
/*      SPRHBM (0x21e0bb00)                                                     */
/*      SPRC0 (0x21e0bb00)                                                      */
/*      SPRMCC (0x21e0bb00)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiph/CFG_KTI0_2           */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* ktiphymodlck
*/

#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) */

/* KTILKSCRATCHCFG32EXT_KTI_CIOPHYDFX_REG supported on:                         */
/*      SPRA0 (0x21e0bff0)                                                      */
/*      SPRB0 (0x21e0bff0)                                                      */
/*      SPRHBM (0x21e0bff0)                                                     */
/*      SPRC0 (0x21e0bff0)                                                      */
/*      SPRMCC (0x21e0bff0)                                                     */
/*      SPRUCC (0x21e49ff0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRB0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRHBM Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRC0 Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2            */
/* SPRMCC Register File:    sprsp_top/upi_1x_creg[0]/ktiln/CFG_KTI0_2           */
/* SPRUCC Register File:    sprsp_top/upimdf[0]/ktiln/CFG_KTI0_2                */
/* Struct generated from SPRA0 BDF: 30_1_3                                      */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* ktilkscratchcfg32ext
*/


#define KTILKSCRATCHCFG32EXT_KTI_CIOPHYDFX_REG 0x09220FF0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scratch_dn : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               Scratch CFG Lower 16-bits (non-sticky) register
                               field Extended Offset with LT Lock.
                            */
    UINT32 scratch_up : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Scratch CFG Upper 16-bits (sticky) register
                               field Extended offset with LT Lock..
                            */

  } Bits;
  UINT32 Data;

} KTILKSCRATCHCFG32EXT_KTI_CIOPHYDFX_STRUCT;
#endif /* _KTI_CIOPHYDFX_h */
