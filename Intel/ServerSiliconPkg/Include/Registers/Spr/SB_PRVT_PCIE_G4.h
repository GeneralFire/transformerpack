
/** @file
  SB_PRVT_PCIE_G4.h

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
/* BTSMM_W                                                                      */
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
/* DFX1                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* DFX2                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OPEN                                                                         */
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
/* BTSMM_W                                                                      */
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
/* DFX1                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* DFX2                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OPEN                                                                         */
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
/* BTSMM_W                                                                      */
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
/* DFX1                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* DFX2                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OPEN                                                                         */
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
/* BTSMM_W                                                                      */
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
/* DFX1                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* DFX2                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OPEN                                                                         */
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
/* BTSMM_W                                                                      */
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
/* DFX1                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* DFX2                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OPEN                                                                         */
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
/* BTSMM_W                                                                      */
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
/* DFX1                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* DFX2                                                                         */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | */
/*    OOB_MSM_SAI                                                               */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* OPEN                                                                         */
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


#ifndef _SB_PRVT_PCIE_G4_h
#define _SB_PRVT_PCIE_G4_h
#include <Base.h>

/* PPBIFCTL_PRIV_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x200000c0)                                                      */
/*      SPRB0 (0x200000c0)                                                      */
/*      SPRHBM (0x200000c0)                                                     */
/*      SPRC0 (0x200000c0)                                                      */
/*      SPRMCC (0x200000c0)                                                     */
/*      SPRUCC (0x200000c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BTSMM_W                                          */
/* SPRB0 Security PolicyGroup: BTSMM_W                                          */
/* SPRHBM Security PolicyGroup: BTSMM_W                                         */
/* SPRC0 Security PolicyGroup: BTSMM_W                                          */
/* SPRMCC Security PolicyGroup: BTSMM_W                                         */
/* SPRUCC Security PolicyGroup: BTSMM_W                                         */
/* PCIe Port Bifurcation Control
*/


#define PPBIFCTL_PRIV_SB_PRVT_PCIE_G4_REG 0x245200C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bifctl0 : 3;

                            /* Bits[2:0], Access Type=RW/V/L, default=None*/

                            /*
                               111-101: Reserved ------P3 P2 P1 P0 100: __ __
                               __ x16 011: __ x8 __ x8 (15:8 operates as x8,
                               7:0 operates as x8) 010: __ x8 x4 x4 (15:8
                               operates as x8, 7:4 operates as x4, 3:0 operates
                               as x4) 001: x4 x4 __ x8 (15:12 operates as x4,
                               11:8 operates as x4, 7:0 operates as x8) 000: x4
                               x4 x4 x4 (15:12 operates as x4, 11:8 operates as
                               x4, 7:4 operates as x4 and 3:0 operates as x4) A
                               write of '1' on the CL will lock this register
                               field down as bifurcation control status
                               information. Note: Lock Key bit is located in
                               the PPBIFCTL_PRIV Register .
                            */
    UINT32 rsvd : 28;

                            /* Bits[30:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cl : 1;

                            /* Bits[31:31], Access Type=RW/L, default=None*/

                            /*
                               Lock key bit for all RW-L bits of PPBIFCTL
                               register only. 1b: Lock 0b: Unlocked Note: Lock
                               Key bit is located in the PPBIFCTL_PRIV Register
                               . This bit is self-locking. Once this bit is set
                               to a 1b, this key bit can not be unlocked.
                               Writing a 0b has no effect on this bit.
                            */

  } Bits;
  UINT32 Data;

} PPBIFCTL_PRIV_SB_PRVT_PCIE_G4_STRUCT;

/* PPD0_SB_PRVT_PCIE_G4_REG supported on:                                       */
/*      SPRA0 (0x200000d4)                                                      */
/*      SPRB0 (0x200000d4)                                                      */
/*      SPRHBM (0x200000d4)                                                     */
/*      SPRC0 (0x200000d4)                                                      */
/*      SPRMCC (0x200000d4)                                                     */
/*      SPRUCC (0x200000d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe Port Definition Control Register 0
*/


#define PPD0_SB_PRVT_PCIE_G4_REG 0x245200D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 clrtrnenonlnkdn : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               when set to 1, a link down event will cause the
                               link train enable field (ILINKTRN0) of this
                               register to be cleared and block subsequent
                               training until that train enable is set again.
                               If 0, a link down will not clear the train
                               enable unless a link down causes a reset.
                            */
    UINT32 rsvd : 2;

                            /* Bits[2:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ilinktrn0 : 1;

                            /* Bits[3:3], Access Type=RW/V, default=0x00000000*/

                            /*
                               This bit controls PCI Express port link training
                               for the associated port. A value of '1'
                               initiates link training on the corresponding
                               port. After writing this bit to a '1', software
                               can poll the Data Link Layer link active bit in
                               the LINKSTS register to determine if a port is
                               up and running. This bit clears on warm reset,
                               i.e. the associated port will not automatically
                               initiate link training after warm reset.
                               Hardware-aware of software should set
                               PPD0.ClrTrnEnOnLnkDn to '1' on DL_Down events if
                               desirable, such as in NTB mode. 0: The
                               associated PCI Express port has not initiated
                               link training. 1: The associated PCI Express
                               port is initiating link training or has trained.
                            */
    UINT32 rsvd_4 : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PPD0_SB_PRVT_PCIE_G4_STRUCT;

/* IOSFDEVCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                             */
/*      SPRA0 (0x100002e4)                                                      */
/*      SPRB0 (0x100002e4)                                                      */
/*      SPRHBM (0x100002e4)                                                     */
/*      SPRC0 (0x100002e4)                                                      */
/*      SPRMCC (0x100002e4)                                                     */
/*      SPRUCC (0x100002e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* The IOSFDEVCLKGCTL is not defined in eEP for BWD. 
It is included for DMI in RC as an extension of Rootport definition.
*/


#define IOSFDEVCLKGCTL_SB_PRVT_PCIE_G4_REG 0x245102E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block controls the delay before going
                               into IDLE. 16 (default value) is the minimum
                               number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /* This bit when set enables clock gating to occur. */

  } Bits;
  UINT16 Data;

} IOSFDEVCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* SBDEVCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                               */
/*      SPRA0 (0x100002e6)                                                      */
/*      SPRB0 (0x100002e6)                                                      */
/*      SPRHBM (0x100002e6)                                                     */
/*      SPRC0 (0x100002e6)                                                      */
/*      SPRMCC (0x100002e6)                                                     */
/*      SPRUCC (0x100002e6)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Sideband Device Clock Gate Control
*/


#define SBDEVCLKGCTL_SB_PRVT_PCIE_G4_REG 0x245102E6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer. On SPR, this also enables the
                               controller to request gating of the ART clock
                               used for PTM master time.
                            */

  } Bits;
  UINT16 Data;

} SBDEVCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* NTBDEVCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x100002ea)                                                      */
/*      SPRB0 (0x100002ea)                                                      */
/*      SPRHBM (0x100002ea)                                                     */
/*      SPRC0 (0x100002ea)                                                      */
/*      SPRMCC (0x100002ea)                                                     */
/*      SPRUCC (0x100002ea)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* NTB Device Clock Gate Control
*/


#define NTBDEVCLKGCTL_SB_PRVT_PCIE_G4_REG 0x245102EA

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block controls the delay before going
                               into IDLE. 16 (default value) is the minimum
                               number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /* This bit when set enables clock gating to occur. */

  } Bits;
  UINT16 Data;

} NTBDEVCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* PPD1_SB_PRVT_PCIE_G4_REG supported on:                                       */
/*      SPRA0 (0x200004c0)                                                      */
/*      SPRB0 (0x200004c0)                                                      */
/*      SPRHBM (0x200004c0)                                                     */
/*      SPRC0 (0x200004c0)                                                      */
/*      SPRMCC (0x200004c0)                                                     */
/*      SPRUCC (0x200004c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BTSMM_W                                          */
/* SPRB0 Security PolicyGroup: BTSMM_W                                          */
/* SPRHBM Security PolicyGroup: BTSMM_W                                         */
/* SPRC0 Security PolicyGroup: BTSMM_W                                          */
/* SPRMCC Security PolicyGroup: BTSMM_W                                         */
/* SPRUCC Security PolicyGroup: BTSMM_W                                         */
/* PCIe Port Definition Control Register 1
*/


#define PPD1_SB_PRVT_PCIE_G4_REG 0x245204C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 8;

                            /* Bits[7:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 npd : 3;

                            /* Bits[10:8], Access Type=RW/V/L, default=None*/

                            /*
                               000b - Reserved 001b - Operate as RP 010b -
                               Operate as NTB-NTB (NT Port) if supported 011b -
                               Operate as NTB-> DSP (NTB EP) if supported 100b
                               - Operate as DMI if supported The remaining
                               encoding are reserved. Note: Lock Key bit is
                               located in the Personality Lock Key Control
                               Register . The default value is determined by
                               soft straps/fuse proxy. (DSP and NTB only)
                            */
    UINT32 rsvd_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pcfgor : 2;

                            /* Bits[13:12], Access Type=RW/V/L, default=None*/

                            /*
                               11 - Force Port to DSD/USP. 10 - Force Port to
                               USD/DSP. 01 - Reserved 00 - Use external Port
                               Configuration Strap pin if applicable to
                               determine USD (default) or DSD Note: Lock Key
                               bit is located in the Personality Lock Key
                               Control Register . This field will be reserved
                               in case of pure DSP. Refer to Table 3-27, 'Port
                               Configuration' for more details.
                            */
    UINT32 pcfgsts : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /*
                               This bit is written by hardware and shows the
                               result of the Port Configuration strap combined
                               with the Port Configuration override settings,
                               or an indication of IAL.io mode. 0 - Port is
                               configured as USD/DSP for NTB or proprietary RP,
                               else in non-IAL.io 1 - Port is configured as
                               DSD/USP for NTB or proprietary RP, else in
                               IAL.io. This field will be reserved in all other
                               devices.
                            */
    UINT32 cre : 1;

                            /* Bits[15:15], Access Type=RW/V/L, default=None*/

                            /*
                               This bit controls the ability of the external
                               side of the NTB to respond to transactions
                               (normal, UR, CRS). The PCIe link is expected to
                               be active after training in about 10-100ms from
                               platform reset deassertion. However, the
                               internal controller may take longer to boot, and
                               to initialize its registers. To allow longer
                               boot time, the CRE bit is used to ensure that
                               the Internal side and related functionality is
                               up and operational before allowing transactions
                               from the external side to be processed even
                               though link training could have been completed
                               prior. 0: External Link handles PCIe
                               transactions (configuration, memory, messages)
                               in the normal fashion during linkup state. 1:
                               External Link presents Configuration Retry
                               Status (CRS) to the incoming configuration cycle
                               on the external cycle in the linkup state. All
                               other requests will be URed. Note: Lock Key bit
                               is located in the Personality Lock Key Control
                               Register . The NTB will respond with
                               Configuration Retry Status (CRS) on the PCIe
                               link for configuration cycles until such time
                               when the internal firmware completes
                               initialization of all internal resources. If the
                               external side sends any other request (memory,
                               messages) before this initialization is done, it
                               will be master aborted (UR) on the external
                               side. It is the responsibility of software to
                               adhere to this flow. This bit is reserved in
                               Downstream Port .
                            */
    UINT32 safemode_en : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               The default value of 1 is for non-RC ports (0
                               for RC ports)
                            */
    UINT32 safemode_exit : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               def 0 (write to unblock clearing of safemode),
                               self clears once safemode_en state has been
                               updated to 0 internally
                            */
    UINT32 safemode_mlw : 6;

                            /* Bits[23:18], Access Type=RO, default=0x00000000*/

                            /*
                               max link width to support in safe mode, 0 is
                               reserved. refer to MLW definition.
                            */
    UINT32 rsvd_24 : 7;

                            /* Bits[30:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cl : 1;

                            /* Bits[31:31], Access Type=RW/L, default=None*/

                            /*
                               Lock key bit for all RW-L bits of PPD1 register
                               only. 1b: Lock 0b: Unlocked Note: Lock Key bit
                               is located in the PPD1 Register . This bit is
                               self-locking. Once this bit is set to a 1b, this
                               key bit can not be unlocked. Writing a 0b has no
                               effect on this bit.
                            */

  } Bits;
  UINT32 Data;

} PPD1_SB_PRVT_PCIE_G4_STRUCT;

/* RCRBBAR_N0_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x200004c8)                                                      */
/*      SPRB0 (0x200004c8)                                                      */
/*      SPRHBM (0x200004c8)                                                     */
/*      SPRC0 (0x200004c8)                                                      */
/*      SPRMCC (0x200004c8)                                                     */
/*      SPRUCC (0x200004c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Port Configuration
Bifurcation Control for MLW (x8) vs Port number
*/


#define RCRBBAR_N0_SB_PRVT_PCIE_G4_REG 0x245204C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disable responding to memory transactions
                               that hit the BADDR. 1 - Enable responding to
                               memory transactions that hit the BADDR.
                            */
    UINT32 rsvd : 12;

                            /* Bits[12:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baddr : 19;

                            /* Bits[31:13], Access Type=RW, default=0x00000000*/

                            /*
                               This marks the base address of the 8KB region
                               for the memory mapped space RCRB. The lower 4KB
                               hosts the DP RCRB while the upper 4KB is
                               allocated for UP RCRB. Addr[12] = 0 for DP RCRB
                               - Request forwarded to internal DP RCRB.
                               Addr[12] = 1 for UP RCRB - Request forwarded
                               down the Link for UP RCRB.
                            */

  } Bits;
  UINT32 Data;

} RCRBBAR_N0_SB_PRVT_PCIE_G4_STRUCT;

/* RCRBBAR_N1_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x200004cc)                                                      */
/*      SPRB0 (0x200004cc)                                                      */
/*      SPRHBM (0x200004cc)                                                     */
/*      SPRC0 (0x200004cc)                                                      */
/*      SPRMCC (0x200004cc)                                                     */
/*      SPRUCC (0x200004cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Port Configuration
Bifurcation Control for MLW (x8) vs Port number
*/


#define RCRBBAR_N1_SB_PRVT_PCIE_G4_REG 0x245204CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 baddr : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               This marks the base address of the 8KB region
                               for the memory mapped space RCRB. The lower 4KB
                               hosts the DP RCRB while the upper 4KB is
                               allocated for UP RCRB. Addr[12] = 0 for DP RCRB
                               - Request forwarded to internal DP RCRB.
                               Addr[12] = 1 for UP RCRB - Request forwarded
                               down the Link for UP RCRB.
                            */

  } Bits;
  UINT32 Data;

} RCRBBAR_N1_SB_PRVT_PCIE_G4_STRUCT;

/* PT0PHYCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x10000544)                                                      */
/*      SPRB0 (0x10000544)                                                      */
/*      SPRHBM (0x10000544)                                                     */
/*      SPRC0 (0x10000544)                                                      */
/*      SPRMCC (0x10000544)                                                     */
/*      SPRUCC (0x10000544)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Port 0 Physical Clock Gate Control
*/


#define PT0PHYCLKGCTL_SB_PRVT_PCIE_G4_REG 0x24510544

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer.
                            */

  } Bits;
  UINT16 Data;

} PT0PHYCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* PT1PHYCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x10000546)                                                      */
/*      SPRB0 (0x10000546)                                                      */
/*      SPRHBM (0x10000546)                                                     */
/*      SPRC0 (0x10000546)                                                      */
/*      SPRMCC (0x10000546)                                                     */
/*      SPRUCC (0x10000546)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* This register is valid only for multi-port configuration.
*/


#define PT1PHYCLKGCTL_SB_PRVT_PCIE_G4_REG 0x24510546

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer.
                            */

  } Bits;
  UINT16 Data;

} PT1PHYCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* PT2PHYCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x10000548)                                                      */
/*      SPRB0 (0x10000548)                                                      */
/*      SPRHBM (0x10000548)                                                     */
/*      SPRC0 (0x10000548)                                                      */
/*      SPRMCC (0x10000548)                                                     */
/*      SPRUCC (0x10000548)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* This register is valid only for multi-port configuration.
*/


#define PT2PHYCLKGCTL_SB_PRVT_PCIE_G4_REG 0x24510548

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer.
                            */

  } Bits;
  UINT16 Data;

} PT2PHYCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* PT3PHYCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x1000054a)                                                      */
/*      SPRB0 (0x1000054a)                                                      */
/*      SPRHBM (0x1000054a)                                                     */
/*      SPRC0 (0x1000054a)                                                      */
/*      SPRMCC (0x1000054a)                                                     */
/*      SPRUCC (0x1000054a)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* This register is valid only for multi-port configuration.
*/


#define PT3PHYCLKGCTL_SB_PRVT_PCIE_G4_REG 0x2451054A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer.
                            */

  } Bits;
  UINT16 Data;

} PT3PHYCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* PLLSTBYCTL_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x2000054c)                                                      */
/*      SPRB0 (0x2000054c)                                                      */
/*      SPRHBM (0x2000054c)                                                     */
/*      SPRC0 (0x2000054c)                                                      */
/*      SPRMCC (0x2000054c)                                                     */
/*      SPRUCC (0x2000054c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* This register is only valid for project specific w/ PLLSTBY feature.
*/


#define PLLSTBYCTL_SB_PRVT_PCIE_G4_REG 0x2452054C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pclk_trunk_cge : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: disable pclk_req de-assertion 1: allow
                               pclk_req to de-assert on idle
                            */
    UINT32 rsvd_5 : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p2vsp1 : 1;

                            /* Bits[8:8], Access Type=RW, default=None*/

                            /*
                               0 = lane in P1 equal to idle for PLL standby
                               (see PIPECTL for P2 disable) 1 = lane in P2
                               equal to idle for PLL standby
                            */
    UINT32 rsvd_9 : 3;

                            /* Bits[11:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pclk_ack1toreq0 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               Min time to assert pclk_req after pclk_ack
                               asserts 0: 3 AUXCLK periods (i.e. auxclk=25Mhz,
                               120ns) 1-15: (val+1)*64ns
                            */
    UINT32 pclk_idletoreq0 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Time to wait after reaching idle state before
                               de-asserting pclk_req 0: 3 AUXCLK periods (i.e.
                               auxclk=25Mhz, 120ns) 1-15: (val+1)*64ns
                            */
    UINT32 rsvd_20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PLLSTBYCTL_SB_PRVT_PCIE_G4_STRUCT;





















/* ACRDPR3_N0_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000630)                                                      */
/*      SPRB0 (0x20000630)                                                      */
/*      SPRHBM (0x20000630)                                                     */
/*      SPRC0 (0x20000630)                                                      */
/*      SPRMCC (0x20000630)                                                     */
/*      SPRUCC (0x20000630)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Access Control Read Policy
*/


#define ACRDPR3_N0_SB_PRVT_PCIE_G4_REG 0x24520630

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Read Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has read access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               read access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACRDPR3_N0_SB_PRVT_PCIE_G4_STRUCT;

/* ACRDPR3_N1_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000634)                                                      */
/*      SPRB0 (0x20000634)                                                      */
/*      SPRHBM (0x20000634)                                                     */
/*      SPRC0 (0x20000634)                                                      */
/*      SPRMCC (0x20000634)                                                     */
/*      SPRUCC (0x20000634)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Access Control Read Policy
*/


#define ACRDPR3_N1_SB_PRVT_PCIE_G4_REG 0x24520634

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Read Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has read access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               read access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACRDPR3_N1_SB_PRVT_PCIE_G4_STRUCT;

/* ACWRPR3_N0_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000638)                                                      */
/*      SPRB0 (0x20000638)                                                      */
/*      SPRHBM (0x20000638)                                                     */
/*      SPRC0 (0x20000638)                                                      */
/*      SPRMCC (0x20000638)                                                     */
/*      SPRUCC (0x20000638)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Access Control Write Policy
*/


#define ACWRPR3_N0_SB_PRVT_PCIE_G4_REG 0x24520638

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Write Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has write access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               write access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACWRPR3_N0_SB_PRVT_PCIE_G4_STRUCT;

/* ACWRPR3_N1_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x2000063c)                                                      */
/*      SPRB0 (0x2000063c)                                                      */
/*      SPRHBM (0x2000063c)                                                     */
/*      SPRC0 (0x2000063c)                                                      */
/*      SPRMCC (0x2000063c)                                                     */
/*      SPRUCC (0x2000063c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Access Control Write Policy
*/


#define ACWRPR3_N1_SB_PRVT_PCIE_G4_REG 0x2452063C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Write Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has write access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               write access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACWRPR3_N1_SB_PRVT_PCIE_G4_STRUCT;

/* ACCRPR3_N0_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000640)                                                      */
/*      SPRB0 (0x20000640)                                                      */
/*      SPRHBM (0x20000640)                                                     */
/*      SPRC0 (0x20000640)                                                      */
/*      SPRMCC (0x20000640)                                                     */
/*      SPRUCC (0x20000640)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Access Control CR Policy
*/


#define ACCRPR3_N0_SB_PRVT_PCIE_G4_REG 0x24520640

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC CR Policy value. The SAI on incoming
                               configuration requests is hashed down to 6 bits
                               and used as an index into the 64-bit of three AC
                               policy registers If bit[n] is set, SAIn has both
                               read and write access to all the corresponding
                               policy group registers, i.e. used to limit write
                               access to the RAC, WAC and CP registers. (read
                               access to these registers is always allowed)
                            */

  } Bits;
  UINT32 Data;

} ACCRPR3_N0_SB_PRVT_PCIE_G4_STRUCT;

/* ACCRPR3_N1_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000644)                                                      */
/*      SPRB0 (0x20000644)                                                      */
/*      SPRHBM (0x20000644)                                                     */
/*      SPRC0 (0x20000644)                                                      */
/*      SPRMCC (0x20000644)                                                     */
/*      SPRUCC (0x20000644)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Access Control CR Policy
*/


#define ACCRPR3_N1_SB_PRVT_PCIE_G4_REG 0x24520644

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC CR Policy value. The SAI on incoming
                               configuration requests is hashed down to 6 bits
                               and used as an index into the 64-bit of three AC
                               policy registers If bit[n] is set, SAIn has both
                               read and write access to all the corresponding
                               policy group registers, i.e. used to limit write
                               access to the RAC, WAC and CP registers. (read
                               access to these registers is always allowed)
                            */

  } Bits;
  UINT32 Data;

} ACCRPR3_N1_SB_PRVT_PCIE_G4_STRUCT;

/* ACRDPR4_N0_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000648)                                                      */
/*      SPRB0 (0x20000648)                                                      */
/*      SPRHBM (0x20000648)                                                     */
/*      SPRC0 (0x20000648)                                                      */
/*      SPRMCC (0x20000648)                                                     */
/*      SPRUCC (0x20000648)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OPEN                                             */
/* SPRB0 Security PolicyGroup: OPEN                                             */
/* SPRHBM Security PolicyGroup: OPEN                                            */
/* SPRC0 Security PolicyGroup: OPEN                                             */
/* SPRMCC Security PolicyGroup: OPEN                                            */
/* SPRUCC Security PolicyGroup: OPEN                                            */
/* Access Control Read Policy
*/


#define ACRDPR4_N0_SB_PRVT_PCIE_G4_REG 0x24520648

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Read Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has read access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               read access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACRDPR4_N0_SB_PRVT_PCIE_G4_STRUCT;

/* ACRDPR4_N1_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x2000064c)                                                      */
/*      SPRB0 (0x2000064c)                                                      */
/*      SPRHBM (0x2000064c)                                                     */
/*      SPRC0 (0x2000064c)                                                      */
/*      SPRMCC (0x2000064c)                                                     */
/*      SPRUCC (0x2000064c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OPEN                                             */
/* SPRB0 Security PolicyGroup: OPEN                                             */
/* SPRHBM Security PolicyGroup: OPEN                                            */
/* SPRC0 Security PolicyGroup: OPEN                                             */
/* SPRMCC Security PolicyGroup: OPEN                                            */
/* SPRUCC Security PolicyGroup: OPEN                                            */
/* Access Control Read Policy
*/


#define ACRDPR4_N1_SB_PRVT_PCIE_G4_REG 0x2452064C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Read Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has read access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               read access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACRDPR4_N1_SB_PRVT_PCIE_G4_STRUCT;

/* ACWRPR4_N0_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000650)                                                      */
/*      SPRB0 (0x20000650)                                                      */
/*      SPRHBM (0x20000650)                                                     */
/*      SPRC0 (0x20000650)                                                      */
/*      SPRMCC (0x20000650)                                                     */
/*      SPRUCC (0x20000650)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OPEN                                             */
/* SPRB0 Security PolicyGroup: OPEN                                             */
/* SPRHBM Security PolicyGroup: OPEN                                            */
/* SPRC0 Security PolicyGroup: OPEN                                             */
/* SPRMCC Security PolicyGroup: OPEN                                            */
/* SPRUCC Security PolicyGroup: OPEN                                            */
/* Access Control Write Policy
*/


#define ACWRPR4_N0_SB_PRVT_PCIE_G4_REG 0x24520650

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Write Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has write access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               write access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACWRPR4_N0_SB_PRVT_PCIE_G4_STRUCT;

/* ACWRPR4_N1_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000654)                                                      */
/*      SPRB0 (0x20000654)                                                      */
/*      SPRHBM (0x20000654)                                                     */
/*      SPRC0 (0x20000654)                                                      */
/*      SPRMCC (0x20000654)                                                     */
/*      SPRUCC (0x20000654)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OPEN                                             */
/* SPRB0 Security PolicyGroup: OPEN                                             */
/* SPRHBM Security PolicyGroup: OPEN                                            */
/* SPRC0 Security PolicyGroup: OPEN                                             */
/* SPRMCC Security PolicyGroup: OPEN                                            */
/* SPRUCC Security PolicyGroup: OPEN                                            */
/* Access Control Write Policy
*/


#define ACWRPR4_N1_SB_PRVT_PCIE_G4_REG 0x24520654

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Write Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has write access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               write access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACWRPR4_N1_SB_PRVT_PCIE_G4_STRUCT;

/* ACCRPR4_N0_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000658)                                                      */
/*      SPRB0 (0x20000658)                                                      */
/*      SPRHBM (0x20000658)                                                     */
/*      SPRC0 (0x20000658)                                                      */
/*      SPRMCC (0x20000658)                                                     */
/*      SPRUCC (0x20000658)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OPEN                                             */
/* SPRB0 Security PolicyGroup: OPEN                                             */
/* SPRHBM Security PolicyGroup: OPEN                                            */
/* SPRC0 Security PolicyGroup: OPEN                                             */
/* SPRMCC Security PolicyGroup: OPEN                                            */
/* SPRUCC Security PolicyGroup: OPEN                                            */
/* Access Control CR Policy
*/


#define ACCRPR4_N0_SB_PRVT_PCIE_G4_REG 0x24520658

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC CR Policy value. The SAI on incoming
                               configuration requests is hashed down to 6 bits
                               and used as an index into the 64-bit of three AC
                               policy registers If bit[n] is set, SAIn has both
                               read and write access to all the corresponding
                               policy group registers, i.e. used to limit write
                               access to the RAC, WAC and CP registers. (read
                               access to these registers is always allowed)
                            */

  } Bits;
  UINT32 Data;

} ACCRPR4_N0_SB_PRVT_PCIE_G4_STRUCT;

/* ACCRPR4_N1_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x2000065c)                                                      */
/*      SPRB0 (0x2000065c)                                                      */
/*      SPRHBM (0x2000065c)                                                     */
/*      SPRC0 (0x2000065c)                                                      */
/*      SPRMCC (0x2000065c)                                                     */
/*      SPRUCC (0x2000065c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OPEN                                             */
/* SPRB0 Security PolicyGroup: OPEN                                             */
/* SPRHBM Security PolicyGroup: OPEN                                            */
/* SPRC0 Security PolicyGroup: OPEN                                             */
/* SPRMCC Security PolicyGroup: OPEN                                            */
/* SPRUCC Security PolicyGroup: OPEN                                            */
/* Access Control CR Policy
*/


#define ACCRPR4_N1_SB_PRVT_PCIE_G4_REG 0x2452065C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC CR Policy value. The SAI on incoming
                               configuration requests is hashed down to 6 bits
                               and used as an index into the 64-bit of three AC
                               policy registers If bit[n] is set, SAIn has both
                               read and write access to all the corresponding
                               policy group registers, i.e. used to limit write
                               access to the RAC, WAC and CP registers. (read
                               access to these registers is always allowed)
                            */

  } Bits;
  UINT32 Data;

} ACCRPR4_N1_SB_PRVT_PCIE_G4_STRUCT;

/* ACRDPR5_N0_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000660)                                                      */
/*      SPRB0 (0x20000660)                                                      */
/*      SPRHBM (0x20000660)                                                     */
/*      SPRC0 (0x20000660)                                                      */
/*      SPRMCC (0x20000660)                                                     */
/*      SPRUCC (0x20000660)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Access Control Read Policy
*/


#define ACRDPR5_N0_SB_PRVT_PCIE_G4_REG 0x24520660

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Read Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has read access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               read access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACRDPR5_N0_SB_PRVT_PCIE_G4_STRUCT;

/* ACRDPR5_N1_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000664)                                                      */
/*      SPRB0 (0x20000664)                                                      */
/*      SPRHBM (0x20000664)                                                     */
/*      SPRC0 (0x20000664)                                                      */
/*      SPRMCC (0x20000664)                                                     */
/*      SPRUCC (0x20000664)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Access Control Read Policy
*/


#define ACRDPR5_N1_SB_PRVT_PCIE_G4_REG 0x24520664

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Read Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has read access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               read access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACRDPR5_N1_SB_PRVT_PCIE_G4_STRUCT;

/* ACWRPR5_N0_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000668)                                                      */
/*      SPRB0 (0x20000668)                                                      */
/*      SPRHBM (0x20000668)                                                     */
/*      SPRC0 (0x20000668)                                                      */
/*      SPRMCC (0x20000668)                                                     */
/*      SPRUCC (0x20000668)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Access Control Write Policy
*/


#define ACWRPR5_N0_SB_PRVT_PCIE_G4_REG 0x24520668

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Write Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has write access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               write access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACWRPR5_N0_SB_PRVT_PCIE_G4_STRUCT;

/* ACWRPR5_N1_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x2000066c)                                                      */
/*      SPRB0 (0x2000066c)                                                      */
/*      SPRHBM (0x2000066c)                                                     */
/*      SPRC0 (0x2000066c)                                                      */
/*      SPRMCC (0x2000066c)                                                     */
/*      SPRUCC (0x2000066c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Access Control Write Policy
*/


#define ACWRPR5_N1_SB_PRVT_PCIE_G4_REG 0x2452066C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC Write Policy value. The SAI on
                               incoming configuration requests is hashed down
                               to 6 bits and used as an index into the 64-bit
                               of three AC policy registers If bit[n] is set,
                               SAIn has write access to the assigned SAI that
                               policy register governs. If the bit is not set,
                               write access is not allowed.
                            */

  } Bits;
  UINT32 Data;

} ACWRPR5_N1_SB_PRVT_PCIE_G4_STRUCT;

/* ACCRPR5_N0_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000670)                                                      */
/*      SPRB0 (0x20000670)                                                      */
/*      SPRHBM (0x20000670)                                                     */
/*      SPRC0 (0x20000670)                                                      */
/*      SPRMCC (0x20000670)                                                     */
/*      SPRUCC (0x20000670)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Access Control CR Policy
*/


#define ACCRPR5_N0_SB_PRVT_PCIE_G4_REG 0x24520670

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC CR Policy value. The SAI on incoming
                               configuration requests is hashed down to 6 bits
                               and used as an index into the 64-bit of three AC
                               policy registers If bit[n] is set, SAIn has both
                               read and write access to all the corresponding
                               policy group registers, i.e. used to limit write
                               access to the RAC, WAC and CP registers. (read
                               access to these registers is always allowed)
                            */

  } Bits;
  UINT32 Data;

} ACCRPR5_N0_SB_PRVT_PCIE_G4_STRUCT;

/* ACCRPR5_N1_SB_PRVT_PCIE_G4_REG supported on:                                 */
/*      SPRA0 (0x20000674)                                                      */
/*      SPRB0 (0x20000674)                                                      */
/*      SPRHBM (0x20000674)                                                     */
/*      SPRC0 (0x20000674)                                                      */
/*      SPRMCC (0x20000674)                                                     */
/*      SPRUCC (0x20000674)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Access Control CR Policy
*/


#define ACCRPR5_N1_SB_PRVT_PCIE_G4_REG 0x24520674

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acpv : 32;

                            /* Bits[31:0], Access Type=RW/V, default=None*/

                            /*
                               Default AC CR Policy value. The SAI on incoming
                               configuration requests is hashed down to 6 bits
                               and used as an index into the 64-bit of three AC
                               policy registers If bit[n] is set, SAIn has both
                               read and write access to all the corresponding
                               policy group registers, i.e. used to limit write
                               access to the RAC, WAC and CP registers. (read
                               access to these registers is always allowed)
                            */

  } Bits;
  UINT32 Data;

} ACCRPR5_N1_SB_PRVT_PCIE_G4_STRUCT;







/* ERR_INJ_LCK_SB_PRVT_PCIE_G4_REG supported on:                                */
/*      SPRA0 (0x200007c4)                                                      */
/*      SPRB0 (0x200007c4)                                                      */
/*      SPRHBM (0x200007c4)                                                     */
/*      SPRC0 (0x200007c4)                                                      */
/*      SPRMCC (0x200007c4)                                                     */
/*      SPRUCC (0x200007c4)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Error Injection Lock
*/


#define ERR_INJ_LCK_SB_PRVT_PCIE_G4_REG 0x245207C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 eil : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               When set to 1, Error Injection is locked. When
                               cleared to 0, error injection via the error
                               injection registers in CFG space is allowed.
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERR_INJ_LCK_SB_PRVT_PCIE_G4_STRUCT;

/* SBERRMSG_SB_PRVT_PCIE_G4_REG supported on:                                   */
/*      SPRA0 (0x200007c8)                                                      */
/*      SPRB0 (0x200007c8)                                                      */
/*      SPRHBM (0x200007c8)                                                     */
/*      SPRC0 (0x200007c8)                                                      */
/*      SPRMCC (0x200007c8)                                                     */
/*      SPRUCC (0x200007c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* IOSF Sidband Error Message Bitmap
*/


#define SBERRMSG_SB_PRVT_PCIE_G4_REG 0x245207C8

#if defined(SPRA0_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bitmap : 5;

                            /* Bits[4:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               The value of this register is used to drive the
                               Bitmap field in the 'PCIe Error' SB msg when the
                               PCIe ERR_* message tag !=0. The 'default' value
                               of this register is loaded by the HW when
                               side_rst_b is de-asserted, the value loaded
                               shall come from the strap_sb_err_bitmap pin.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SBERRMSG_SB_PRVT_PCIE_G4_SPRA0_STRUCT;
#endif /* (SPRA0_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bitmap0 : 5;

                            /* Bits[4:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               The value of this register is used to drive the
                               Bitmap field in the 'PCIe Error' SB msg when the
                               PCIe ERR_* message tag !=0. The 'default' value
                               of this register is loaded by the HW when
                               side_rst_b is de-asserted, the value loaded
                               shall come from the strap_sb_err_bitmap pin.
                            */
    UINT32 bitmap1 : 5;

                            /* Bits[9:5], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               The value of this register is used to drive the
                               Bitmap field in the 'PCIe Error' SB msg when the
                               PCIe ERR_* message tag !=0. The 'default' value
                               of this register is loaded by the HW when
                               side_rst_b is de-asserted, the value loaded
                               shall come from the strap_sb_err_bitmap pin.
                            */
    UINT32 bitmap2 : 5;

                            /* Bits[14:10], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               The value of this register is used to drive the
                               Bitmap field in the 'PCIe Error' SB msg when the
                               PCIe ERR_* message tag !=0. The 'default' value
                               of this register is loaded by the HW when
                               side_rst_b is de-asserted, the value loaded
                               shall come from the strap_sb_err_bitmap pin.
                            */
    UINT32 bitmap3 : 5;

                            /* Bits[19:15], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               The value of this register is used to drive the
                               Bitmap field in the 'PCIe Error' SB msg when the
                               PCIe ERR_* message tag !=0. The 'default' value
                               of this register is loaded by the HW when
                               side_rst_b is de-asserted, the value loaded
                               shall come from the strap_sb_err_bitmap pin.
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SBERRMSG_SB_PRVT_PCIE_G4_SPRB0_SPRHBM_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */


/* DEVHIDE_SB_PRVT_PCIE_G4_REG supported on:                                    */
/*      SPRA0 (0x200007d8)                                                      */
/*      SPRB0 (0x200007d8)                                                      */
/*      SPRHBM (0x200007d8)                                                     */
/*      SPRC0 (0x200007d8)                                                      */
/*      SPRMCC (0x200007d8)                                                     */
/*      SPRUCC (0x200007d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Device Hide
*/


#define DEVHIDE_SB_PRVT_PCIE_G4_REG 0x245207D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 p0cfgh : 1;

                            /* Bits[0:0], Access Type=RW/L, default=None*/

                            /*
                               When set to '1', the port will not claim any
                               downstream configuration transactions including
                               type 0 and type 1. All configuration reads will
                               be UR'd, and all configuration writes are
                               unaffected if PCFGHP=0, or UR'd if PCFGHP=1.
                               This will hide the port and any devices behind
                               it from being discovered by the OS. Note: Lock
                               Key bit is located in the Personality Lock Key
                               Control Register . This field is Read-Only when
                               the DEVHIDE.DHCL bit is set.
                            */
    UINT32 p1cfgh : 1;

                            /* Bits[1:1], Access Type=RW/L, default=None*/

                            /*
                               When set to '1', the port will not claim any
                               downstream configuration transactions including
                               type 0 and type 1. All configuration reads will
                               be UR'd, and all configuration writes are
                               unaffected if PCFGHP=0, or UR'd if PCFGHP=1.
                               This will hide the port and any devices behind
                               it from being discovered by the OS. Note: Lock
                               Key bit is located in the Personality Lock Key
                               Control Register . This field is Read-Only when
                               the DEVHIDE.DHCL bit is set. This field is only
                               applicable to multi-port device and should be
                               treated as reserved for single port device.
                            */
    UINT32 p2cfgh : 1;

                            /* Bits[2:2], Access Type=RW/L, default=None*/

                            /*
                               When set to '1', the port will not claim any
                               downstream configuration transactions including
                               type 0 and type 1. All configuration reads will
                               be UR'd, and all configuration writes are
                               unaffected if PCFGHP=0, or UR'd if PCFGHP=1.
                               This will hide the port and any devices behind
                               it from being discovered by the OS. Note: Lock
                               Key bit is located in the Personality Lock Key
                               Control Register . This field is Read-Only when
                               the DEVHIDE.DHCL bit is set. This field is only
                               applicable to multi-port device and should be
                               treated as reserved for single port device.
                            */
    UINT32 p3cfgh : 1;

                            /* Bits[3:3], Access Type=RW/L, default=None*/

                            /*
                               When set to '1', the port will not claim any
                               downstream configuration transactions including
                               type 0 and type 1. All configuration reads will
                               be UR'd, and all configuration writes are
                               unaffected if PCFGHP=0, or UR'd if PCFGHP=1.
                               This will hide the port and any devices behind
                               it from being discovered by the OS. Note: Lock
                               Key bit is located in the Personality Lock Key
                               Control Register . This field is Read-Only when
                               the DEVHIDE.DHCL bit is set. This field is only
                               applicable to multi-port device and should be
                               treated as reserved for single port device.
                            */
    UINT32 pcfghp : 1;

                            /* Bits[4:4], Access Type=RW/L, default=None*/

                            /*
                               When set to '1', the port will not claim any
                               downstream configuration transactions including
                               type 0 and type 1, regardless reads or writes.
                               This field is valid only when Port CFG Hide
                               field is set to 1. Note: Lock Key bit is located
                               in the Personality Lock Key Control Register .
                               This field is Read-Only when the DEVHIDE.DHCL
                               bit is set.
                            */
    UINT32 p0gbe : 1;

                            /* Bits[5:5], Access Type=RW/L, default=None*/

                            /*
                               When set to '1', the port will suppress and
                               block all errors or interrupts storm caused by
                               un-terminated trace leading to PCIe RPs being
                               hidden from OS. Note: Lock Key bit is located in
                               the Personality Lock Key Control Register . This
                               field is Read-Only when the DEVHIDE.DHCL bit is
                               set. This field is only applicable to RPs and
                               should be treated as reserved for other devices.
                            */
    UINT32 p1gbe : 1;

                            /* Bits[6:6], Access Type=RW/L, default=None*/

                            /*
                               When set to '1', the port will suppress and
                               block all errors or interrupts storm caused by
                               un-terminated trace leading to PCIe RPs being
                               hidden from OS. Note: Lock Key bit is located in
                               the Personality Lock Key Control Register . This
                               field is Read-Only when the DEVHIDE.DHCL bit is
                               set. This field is only applicable to RPs with
                               multi-port device and should be treated as
                               reserved if corresponding port is not valid.
                            */
    UINT32 p2gbe : 1;

                            /* Bits[7:7], Access Type=RW/L, default=None*/

                            /*
                               When set to '1', the port will suppress and
                               block all errors or interrupts storm caused by
                               un-terminated trace leading to PCIe RPs being
                               hidden from OS. Note: Lock Key bit is located in
                               the Personality Lock Key Control Register . This
                               field is Read-Only when the DEVHIDE.DHCL bit is
                               set. This field is only applicable to RPs with
                               multi-port device and should be treated as
                               reserved if corresponding port is not valid.
                            */
    UINT32 p3gbe : 1;

                            /* Bits[8:8], Access Type=RW/L, default=None*/

                            /*
                               When set to '1', the port will suppress and
                               block all errors or interrupts storm caused by
                               un-terminated trace leading to PCIe RPs being
                               hidden from OS. Note: Lock Key bit is located in
                               the Personality Lock Key Control Register . This
                               field is Read-Only when the DEVHIDE.DHCL bit is
                               set. This field is only applicable to RPs with
                               multi-port device and should be treated as
                               reserved if corresponding port is not valid.
                            */
    UINT32 rsvd : 22;

                            /* Bits[30:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dhcl : 1;

                            /* Bits[31:31], Access Type=RW/L, default=None*/

                            /*
                               Lock key bit for DEVHIDE. 1b: Lock 0b: Unlocked
                               Note: Lock Key bit is located in the Personality
                               Lock Key Control Register . This bit is self-
                               locking. Once this bit is set to a 1b, this key
                               bit can not be unlocked. Writing a 0b has no
                               affect on this bit.
                            */

  } Bits;
  UINT32 Data;

} DEVHIDE_SB_PRVT_PCIE_G4_STRUCT;







/* PT0TXNCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x100008e4)                                                      */
/*      SPRB0 (0x100008e4)                                                      */
/*      SPRHBM (0x100008e4)                                                     */
/*      SPRC0 (0x100008e4)                                                      */
/*      SPRMCC (0x100008e4)                                                     */
/*      SPRUCC (0x100008e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Port 0 Transaction Clock Gate Control
*/


#define PT0TXNCLKGCTL_SB_PRVT_PCIE_G4_REG 0x245108E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer.
                            */

  } Bits;
  UINT16 Data;

} PT0TXNCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* PT1TXNCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x100008e6)                                                      */
/*      SPRB0 (0x100008e6)                                                      */
/*      SPRHBM (0x100008e6)                                                     */
/*      SPRC0 (0x100008e6)                                                      */
/*      SPRMCC (0x100008e6)                                                     */
/*      SPRUCC (0x100008e6)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Port 1 Transaction Clock Gate Control
*/


#define PT1TXNCLKGCTL_SB_PRVT_PCIE_G4_REG 0x245108E6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer.
                            */

  } Bits;
  UINT16 Data;

} PT1TXNCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* PT2TXNCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x100008e8)                                                      */
/*      SPRB0 (0x100008e8)                                                      */
/*      SPRHBM (0x100008e8)                                                     */
/*      SPRC0 (0x100008e8)                                                      */
/*      SPRMCC (0x100008e8)                                                     */
/*      SPRUCC (0x100008e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Port 2 Transaction Clock Gate Control
*/


#define PT2TXNCLKGCTL_SB_PRVT_PCIE_G4_REG 0x245108E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer.
                            */

  } Bits;
  UINT16 Data;

} PT2TXNCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* PT3TXNCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x100008ea)                                                      */
/*      SPRB0 (0x100008ea)                                                      */
/*      SPRHBM (0x100008ea)                                                     */
/*      SPRC0 (0x100008ea)                                                      */
/*      SPRMCC (0x100008ea)                                                     */
/*      SPRUCC (0x100008ea)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Port 3 Transaction Clock Gate Control
*/


#define PT3TXNCLKGCTL_SB_PRVT_PCIE_G4_REG 0x245108EA

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer.
                            */

  } Bits;
  UINT16 Data;

} PT3TXNCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* PT0LNKCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x100009e4)                                                      */
/*      SPRB0 (0x100009e4)                                                      */
/*      SPRHBM (0x100009e4)                                                     */
/*      SPRC0 (0x100009e4)                                                      */
/*      SPRMCC (0x100009e4)                                                     */
/*      SPRUCC (0x100009e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Port 0 Link Clock Gate Control
*/


#define PT0LNKCLKGCTL_SB_PRVT_PCIE_G4_REG 0x245109E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer.
                            */

  } Bits;
  UINT16 Data;

} PT0LNKCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* PT1LNKCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x100009e6)                                                      */
/*      SPRB0 (0x100009e6)                                                      */
/*      SPRHBM (0x100009e6)                                                     */
/*      SPRC0 (0x100009e6)                                                      */
/*      SPRMCC (0x100009e6)                                                     */
/*      SPRUCC (0x100009e6)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Port 1 Link Clock Gate Control
*/


#define PT1LNKCLKGCTL_SB_PRVT_PCIE_G4_REG 0x245109E6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer.
                            */

  } Bits;
  UINT16 Data;

} PT1LNKCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* PT2LNKCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x100009e8)                                                      */
/*      SPRB0 (0x100009e8)                                                      */
/*      SPRHBM (0x100009e8)                                                     */
/*      SPRC0 (0x100009e8)                                                      */
/*      SPRMCC (0x100009e8)                                                     */
/*      SPRUCC (0x100009e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Port 2 Link Clock Gate Control
*/


#define PT2LNKCLKGCTL_SB_PRVT_PCIE_G4_REG 0x245109E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer.
                            */

  } Bits;
  UINT16 Data;

} PT2LNKCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* PT3LNKCLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                              */
/*      SPRA0 (0x100009ea)                                                      */
/*      SPRB0 (0x100009ea)                                                      */
/*      SPRHBM (0x100009ea)                                                     */
/*      SPRC0 (0x100009ea)                                                      */
/*      SPRMCC (0x100009ea)                                                     */
/*      SPRUCC (0x100009ea)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Port 3 Link Clock Gate Control
*/


#define PT3LNKCLKGCTL_SB_PRVT_PCIE_G4_REG 0x245109EA

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer.
                            */

  } Bits;
  UINT16 Data;

} PT3LNKCLKGCTL_SB_PRVT_PCIE_G4_STRUCT;

/* CFGPCIECLKGCTL_SB_PRVT_PCIE_G4_REG supported on:                             */
/*      SPRA0 (0x100009ec)                                                      */
/*      SPRB0 (0x100009ec)                                                      */
/*      SPRHBM (0x100009ec)                                                     */
/*      SPRC0 (0x100009ec)                                                      */
/*      SPRMCC (0x100009ec)                                                     */
/*      SPRUCC (0x100009ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst         */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/rp0/msg_inst        */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Configuration PCIe Clock Gate Control
*/


#define CFGPCIECLKGCTL_SB_PRVT_PCIE_G4_REG 0x245109EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field indicates the number of clocks that
                               the IP block must be idle before the clock
                               disable process begins. 16 (default value) is
                               the minimum number of clocks recommended.
                            */
    UINT16 rsvd : 7;

                            /* Bits[14:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=None*/

                            /*
                               This bit when set enables clock gating to occur
                               when the IP block is idle longer then the Idle
                               Clock Timer.
                            */

  } Bits;
  UINT16 Data;

} CFGPCIECLKGCTL_SB_PRVT_PCIE_G4_STRUCT;
#endif /* _SB_PRVT_PCIE_G4_h */
