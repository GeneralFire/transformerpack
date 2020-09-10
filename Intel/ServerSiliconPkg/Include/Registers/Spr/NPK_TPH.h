
/** @file
  NPK_TPH.h

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
/* NPK_MMIO_POLICY                                                              */
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
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_BOOT_SAI | HOSTIA_SMM_SAI | HOSTIA_UCODE_SAI */
/*    | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | DFX_INTEL_PRODUCTION_SAI |            */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | OOB_MSM_SAI |           */
/*    DFX_THIRDPARTY_SAI                                                        */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* NPK_VRC_POLICY                                                               */
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
/*    HOSTIA_BOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_INTEL_MANUFACTURING_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRB0 Security Policy Groups:                                                */
/* NPK_MMIO_POLICY                                                              */
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
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_BOOT_SAI | HOSTIA_SMM_SAI | HOSTIA_UCODE_SAI */
/*    | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | DFX_INTEL_PRODUCTION_SAI |            */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | OOB_MSM_SAI |           */
/*    DFX_THIRDPARTY_SAI                                                        */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* NPK_VRC_POLICY                                                               */
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
/*    HOSTIA_BOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_INTEL_MANUFACTURING_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRHBM Security Policy Groups:                                               */
/* NPK_MMIO_POLICY                                                              */
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
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_BOOT_SAI | HOSTIA_SMM_SAI | HOSTIA_UCODE_SAI */
/*    | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | DFX_INTEL_PRODUCTION_SAI |            */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | OOB_MSM_SAI |           */
/*    DFX_THIRDPARTY_SAI                                                        */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* NPK_VRC_POLICY                                                               */
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
/*    HOSTIA_BOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_INTEL_MANUFACTURING_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRC0 Security Policy Groups:                                                */
/* NPK_MMIO_POLICY                                                              */
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
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_BOOT_SAI | HOSTIA_SMM_SAI | HOSTIA_UCODE_SAI */
/*    | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | DFX_INTEL_PRODUCTION_SAI |            */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | OOB_MSM_SAI |           */
/*    DFX_THIRDPARTY_SAI                                                        */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* NPK_VRC_POLICY                                                               */
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
/*    HOSTIA_BOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_INTEL_MANUFACTURING_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRMCC Security Policy Groups:                                               */
/* NPK_MMIO_POLICY                                                              */
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
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_BOOT_SAI | HOSTIA_SMM_SAI | HOSTIA_UCODE_SAI */
/*    | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | DFX_INTEL_PRODUCTION_SAI |            */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | OOB_MSM_SAI |           */
/*    DFX_THIRDPARTY_SAI                                                        */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* NPK_VRC_POLICY                                                               */
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
/*    HOSTIA_BOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_INTEL_MANUFACTURING_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */

/* SPRUCC Security Policy Groups:                                               */
/* NPK_MMIO_POLICY                                                              */
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
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_BOOT_SAI | HOSTIA_SMM_SAI | HOSTIA_UCODE_SAI */
/*    | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | DFX_INTEL_PRODUCTION_SAI |            */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | OOB_MSM_SAI |           */
/*    DFX_THIRDPARTY_SAI                                                        */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* NPK_VRC_POLICY                                                               */
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
/*    HOSTIA_BOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_INTEL_MANUFACTURING_SAI                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _NPK_TPH_h
#define _NPK_TPH_h
#include <Base.h>

/* GTHOPT0_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20000000)                                                      */
/*      SPRB0 (0x20000000)                                                      */
/*      SPRHBM (0x20000000)                                                     */
/*      SPRC0 (0x20000000)                                                      */
/*      SPRMCC (0x20000000)                                                     */
/*      SPRUCC (0x20000000)                                                     */
/* Register default value on SPRA0: 0x00000101                                  */
/* Register default value on SPRB0: 0x00000101                                  */
/* Register default value on SPRHBM: 0x00000101                                 */
/* Register default value on SPRC0: 0x00000101                                  */
/* Register default value on SPRMCC: 0x00000101                                 */
/* Register default value on SPRUCC: 0x00000101                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* GTH Output Ports 0-3
*/


#define GTHOPT0_NPK_TPH_REG 0x1E120000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 p0type : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000001*/

                            /*
                               GTH Output Port 0 type. 0h: No port 1h - System
                               Memory / USB (DCI) 2h - MIPI-HTI 3h - Low Power
                               Path (PTI, External Bridge) 4h - MIPI-PTI Others
                               - reserved
                            */
    UINT32 p0null : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               NULL Packet Generation for output port 0. 0:
                               NULL Packets are suppressed. 1: NULL Packets are
                               generated.
                            */
    UINT32 p0drp : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               GTH Data Retention Policy for Port 0. This bit
                               defines the behavior of the GTH when Port 0 is
                               in a not ready or reset condition. Specifically,
                               it is when portReset is asserted. The conditions
                               under which the portReset signal is asserted for
                               a given port is defined by the port's logic, and
                               is outside the scope of this specification. This
                               condition might occur when the output port is
                               unconfigured, held in reset, or otherwise not
                               fully operational. When the portReset is
                               asserted, the BPB will take action based on the
                               setting of the PnDRP bit. Specifically: 0:
                               Hold/retain data. In this mode, the GTH will
                               hold, or retain, all the trace data it has. The
                               Byte Packing Buffer will very quickly fill up,
                               and stall its data path. When the affected Input
                               Buffers fill up, they will de-assert their get
                               signal, indicating to their trace source(s) that
                               they cannot accept any more input data. 1: Drop
                               data. In this mode, the Byte Packing Buffer will
                               ignore the deassertion of its get input,
                               behaving as if it is asserted continuously. This
                               will have the net effect of unloading data from
                               the BPB and dropping it on the floor (it is
                               permanently lost).
                            */
    UINT32 p0rst : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               Port 0 in Reset. When asserted, indicates the
                               port is in reset, and is not accepting data.
                               When cleared, indicates the port is operational,
                               and can accept trace data.
                            */
    UINT32 p0enc : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Port 0 Encoder Disable/Bypass. 0: Port 0 MIPI
                               STPv2.1 encoder is enabled (Default) 1: Port 0
                               MIPI STPv2.1 encoder is disabled/bypassed. Data
                               will flow directly from input to output with no
                               changes (specifically, only the data field. The
                               master, channel, type, etc, should not be passed
                               through, but dropped)
                            */
    UINT32 p0flush : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Port 0 Flush. Setting this bit will assert the
                               flush signal to the byte packing buffer for port
                               0.
                            */
    UINT32 p1type : 3;

                            /* Bits[10:8], Access Type=RO, default=0x00000001*/

                            /*
                               GTH Output Port 1 type. 0h: No port 1h - System
                               Memory / USB (DCI) 2h - MIPI-HTI 3h - Low Power
                               Path (PTI, External Bridge) 4h - MIPI-PTI Others
                               - reserved
                            */
    UINT32 p1null : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /* NULL Packet Generation for output port 1. */
    UINT32 p1drp : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               GTH Data Retention Policy for Port 1. See P0DRP
                               for details.
                            */
    UINT32 p1rst : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /*
                               Port 1 in Reset. When asserted, indicates the
                               port is in reset, and is not accepting data.
                               When cleared, indicates the port is operational,
                               and can accept trace data.
                            */
    UINT32 p1enc : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Port 1 Encoder Disable/Bypass */
    UINT32 p1flush : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Port 1 Flush. Setting this bit will assert the
                               flush signal to the byte packing buffer for port
                               1.
                            */
    UINT32 p2type : 3;

                            /* Bits[18:16], Access Type=RO, default=0x00000000*/

                            /*
                               GTH Output Port 0 type. 0h: No port 1h - System
                               Memory / USB (DCI) 2h - MIPI-HTI 3h - Low Power
                               Path (PTI, External Bridge) 4h - MIPI-PTI Others
                               - reserved
                            */
    UINT32 p2null : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /* NULL Packet Generation for output port 2. */
    UINT32 p2drp : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               GTH Data Retention Policy for Port 2. See P0DRP
                               for details.
                            */
    UINT32 p2rst : 1;

                            /* Bits[21:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               Port 2 in Reset. When asserted, indicates the
                               port is in reset, and is not accepting data.
                               When cleared, indicates the port is operational,
                               and can accept trace data.
                            */
    UINT32 p2enc : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* Port 2 Encoder Disable/Bypass. */
    UINT32 p2flush : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Port 2 Flush. Setting this bit will assert the
                               flush signal to the byte packing buffer for port
                               2
                            */
    UINT32 p3type : 3;

                            /* Bits[26:24], Access Type=RO, default=0x00000000*/

                            /*
                               GTH Output Port 0 type. 0h: No port 1h - System
                               Memory / USB (DCI) 2h - MIPI-HTI 3h - Low Power
                               Path (PTI, External Bridge) 4h - MIPI-PTI Others
                               - reserved
                            */
    UINT32 p3null : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* NULL Packet Generation for output port 3 */
    UINT32 p3drp : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               GTH Data Retention Policy for Port 3. See P0DRP
                               for details.
                            */
    UINT32 p3rst : 1;

                            /* Bits[29:29], Access Type=RO/V, default=0x00000000*/

                            /*
                               Port 3 in Reset. When asserted, indicates the
                               port is in reset, and is not accepting data.
                               When cleared, indicates the port is operational,
                               and can accept trace data.
                            */
    UINT32 p3enc : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* Port 3 Encoder Disable/Bypass. */
    UINT32 p3flush : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Port 3 Flush. Setting this bit will assert the
                               flush signal to the byte packing buffer for port
                               3
                            */

  } Bits;
  UINT32 Data;

} GTHOPT0_NPK_TPH_STRUCT;

/* GTHOPT1_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20000004)                                                      */
/*      SPRB0 (0x20000004)                                                      */
/*      SPRHBM (0x20000004)                                                     */
/*      SPRC0 (0x20000004)                                                      */
/*      SPRMCC (0x20000004)                                                     */
/*      SPRUCC (0x20000004)                                                     */
/* Register default value on SPRA0: 0x03000000                                  */
/* Register default value on SPRB0: 0x03000000                                  */
/* Register default value on SPRHBM: 0x03000000                                 */
/* Register default value on SPRC0: 0x03000000                                  */
/* Register default value on SPRMCC: 0x03000000                                 */
/* Register default value on SPRUCC: 0x03000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* GTH Output Ports 4-7
*/


#define GTHOPT1_NPK_TPH_REG 0x1E120004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 p4type : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000000*/

                            /*
                               GTH Output Port 0 type. 0h: No port 1h - System
                               Memory / USB (DCI) 2h - MIPI-HTI 3h - Low Power
                               Path (PTI, External Bridge) 4h - MIPI-PTI Others
                               - reserved
                            */
    UINT32 p4null : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               NULL Packet Generation for output port 4 0: NULL
                               Packets are suppressed 1: NULL Packets are
                               generated
                            */
    UINT32 p4drp : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               GTH Data Retention Policy for Port 4. This bit
                               defines the behavior of the GTH when Port 4 is
                               in a not ready or reset condition. Specifically,
                               it is when portReset is asserted. The conditions
                               under which the portReset signal is asserted for
                               a given port is defined by the port's logic, and
                               is outside the scope of this specification. This
                               condition might occur when the output port is
                               unconfigured, held in reset, or otherwise not
                               fully operational. When the portReset is
                               asserted, the BPB will take action based on the
                               setting of the PnDRP bit. Specifically: 0:
                               Hold/retain data. In this mode, the GTH will
                               hold, or retain, all the trace data it has. The
                               Byte Packing Buffer will very quickly fill up,
                               and stall its data path. When the affected Input
                               Buffers fill up, they will de-assert their get
                               signal, indicating to their trace source(s) that
                               they cannot accept any more input data. 1: Drop
                               data. In this mode, the Byte Packing Buffer will
                               ignore the deassertion of its get input,
                               behaving as if it is asserted continuously. This
                               will have the net effect of unloading data from
                               the BPB and dropping it on the floor (it is
                               permanently lost).
                            */
    UINT32 p4rst : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               Port 4 in Reset. When asserted, indicates the
                               port is in reset, and is not accepting data.
                               When cleared, indicates the port is operational,
                               and can accept trace data.
                            */
    UINT32 p4enc : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Port 4 Encoder Disable/Bypass. 0: Port 4 MIPI
                               STPv2.1 encoder is enabled (Default) 1: Port 4
                               MIPI STPv2.1 encoder is disabled/bypassed. Data
                               will flow directly from input to output with no
                               changes (specifically, only the data field. The
                               master, channel, type, etc, should not be passed
                               through, but dropped)
                            */
    UINT32 p4flush : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Port 4 Flush. Setting this bit will assert the
                               flush signal to the byte packing buffer for port
                               4.
                            */
    UINT32 p5type : 3;

                            /* Bits[10:8], Access Type=RO, default=0x00000000*/

                            /*
                               GTH Output Port 0 type. 0h: No port 1h - System
                               Memory / USB (DCI) 2h - MIPI-HTI 3h - Low Power
                               Path (PTI, External Bridge) 4h - MIPI-PTI Others
                               - reserved
                            */
    UINT32 p5null : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               NULL Packet Generation for output port 5 0: NULL
                               Packets are suppressed 1: NULL Packets are
                               generated
                            */
    UINT32 p5drp : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               GTH Data Retention Policy for Port 5. This bit
                               defines the behavior of the GTH when Port 5 is
                               in a not ready or reset condition. Specifically,
                               it is when portReset is asserted. The conditions
                               under which the portReset signal is asserted for
                               a given port is defined by the port's logic, and
                               is outside the scope of this specification. This
                               condition might occur when the output port is
                               unconfigured, held in reset, or otherwise not
                               fully operational. When the portReset is
                               asserted, the BPB will take action based on the
                               setting of the PnDRP bit. Specifically: 0:
                               Hold/retain data. In this mode, the GTH will
                               hold, or retain, all the trace data it has. The
                               Byte Packing Buffer will very quickly fill up,
                               and stall its data path. When the affected Input
                               Buffers fill up, they will de-assert their get
                               signal, indicating to their trace source(s) that
                               they cannot accept any more input data. 1: Drop
                               data. In this mode, the Byte Packing Buffer will
                               ignore the deassertion of its get input,
                               behaving as if it is asserted continuously. This
                               will have the net effect of unloading data from
                               the BPB and dropping it on the floor (it is
                               permanently lost).
                            */
    UINT32 p5rst : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /*
                               Port 5 in Reset. When asserted, indicates the
                               port is in reset, and is not accepting data.
                               When cleared, indicates the port is operational,
                               and can accept trace data.
                            */
    UINT32 p5enc : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Port 5 Encoder Disable/Bypass. 0: Port 5 MIPI
                               STPv2.1 encoder is enabled (Default) 1: Port 5
                               MIPI STPv2.1 encoder is disabled/bypassed. Data
                               will flow directly from input to output with no
                               changes (specifically, only the data field. The
                               master, channel, type, etc, should not be passed
                               through, but dropped)
                            */
    UINT32 p5flush : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Port 5 Flush. Setting this bit will assert the
                               flush signal to the byte packing buffer for port
                               5.
                            */
    UINT32 p6type : 3;

                            /* Bits[18:16], Access Type=RO, default=0x00000000*/

                            /*
                               GTH Output Port 0 type. 0h: No port 1h - System
                               Memory / USB (DCI) 2h - MIPI-HTI 3h - Low Power
                               Path (PTI, External Bridge) 4h - MIPI-PTI Others
                               - reserved
                            */
    UINT32 p6null : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               NULL Packet Generation for output port 6 0: NULL
                               Packets are suppressed 1: NULL Packets are
                               generated
                            */
    UINT32 p6drp : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               GTH Data Retention Policy for Port 6. This bit
                               defines the behavior of the GTH when Port 6 is
                               in a not ready or reset condition. Specifically,
                               it is when portReset is asserted. The conditions
                               under which the portReset signal is asserted for
                               a given port is defined by the port's logic, and
                               is outside the scope of this specification. This
                               condition might occur when the output port is
                               unconfigured, held in reset, or otherwise not
                               fully operational. When the portReset is
                               asserted, the BPB will take action based on the
                               setting of the PnDRP bit. Specifically: 0:
                               Hold/retain data. In this mode, the GTH will
                               hold, or retain, all the trace data it has. The
                               Byte Packing Buffer will very quickly fill up,
                               and stall its data path. When the affected Input
                               Buffers fill up, they will de-assert their get
                               signal, indicating to their trace source(s) that
                               they cannot accept any more input data. 1: Drop
                               data. In this mode, the Byte Packing Buffer will
                               ignore the deassertion of its get input,
                               behaving as if it is asserted continuously. This
                               will have the net effect of unloading data from
                               the BPB and dropping it on the floor (it is
                               permanently lost).
                            */
    UINT32 p6rst : 1;

                            /* Bits[21:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               Port 6 in Reset. When asserted, indicates the
                               port is in reset, and is not accepting data.
                               When cleared, indicates the port is operational,
                               and can accept trace data.
                            */
    UINT32 p6enc : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Port 6 Encoder Disable/Bypass. 0: Port 6 MIPI
                               STPv2.1 encoder is enabled (Default) 1: Port 6
                               MIPI STPv2.1 encoder is disabled/bypassed. Data
                               will flow directly from input to output with no
                               changes (specifically, only the data field. The
                               master, channel, type, etc, should not be passed
                               through, but dropped)
                            */
    UINT32 p6flush : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Port 6 Flush. Setting this bit will assert the
                               flush signal to the byte packing buffer for port
                               6.
                            */
    UINT32 p7type : 3;

                            /* Bits[26:24], Access Type=RO, default=0x00000003*/

                            /*
                               GTH Output Port 0 type. 0h: No port 1h - System
                               Memory / USB (DCI) 2h - MIPI-HTI 3h - Low Power
                               Path (PTI, External Bridge) 4h - MIPI-PTI Others
                               - reserved
                            */
    UINT32 p7null : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               NULL Packet Generation for output port 7 0: NULL
                               Packets are suppressed 1: NULL Packets are
                               generated
                            */
    UINT32 p7drp : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               GTH Data Retention Policy for Port 7. This bit
                               defines the behavior of the GTH when Port 7 is
                               in a not ready or reset condition. Specifically,
                               it is when portReset is asserted. The conditions
                               under which the portReset signal is asserted for
                               a given port is defined by the port's logic, and
                               is outside the scope of this specification. This
                               condition might occur when the output port is
                               unconfigured, held in reset, or otherwise not
                               fully operational. When the portReset is
                               asserted, the BPB will take action based on the
                               setting of the PnDRP bit. Specifically: 0:
                               Hold/retain data. In this mode, the GTH will
                               hold, or retain, all the trace data it has. The
                               Byte Packing Buffer will very quickly fill up,
                               and stall its data path. When the affected Input
                               Buffers fill up, they will de-assert their get
                               signal, indicating to their trace source(s) that
                               they cannot accept any more input data. 1: Drop
                               data. In this mode, the Byte Packing Buffer will
                               ignore the deassertion of its get input,
                               behaving as if it is asserted continuously. This
                               will have the net effect of unloading data from
                               the BPB and dropping it on the floor (it is
                               permanently lost).
                            */
    UINT32 p7rst : 1;

                            /* Bits[29:29], Access Type=RO/V, default=0x00000000*/

                            /*
                               Port 7 in Reset. When asserted, indicates the
                               port is in reset, and is not accepting data.
                               When cleared, indicates the port is operational,
                               and can accept trace data.
                            */
    UINT32 p7enc : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Port 7 Encoder Disable/Bypass. 0: Port 7 MIPI
                               STPv2.1 encoder is enabled (Default) 1: Port 7
                               MIPI STPv2.1 encoder is disabled/bypassed. Data
                               will flow directly from input to output with no
                               changes (specifically, only the data field. The
                               master, channel, type, etc, should not be passed
                               through, but dropped)
                            */
    UINT32 p7flush : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Port 7 Flush. Setting this bit will assert the
                               flush signal to the byte packing buffer for port
                               7.
                            */

  } Bits;
  UINT32 Data;

} GTHOPT1_NPK_TPH_STRUCT;

/* SWDEST_0_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20000008)                                                      */
/*      SPRB0 (0x20000008)                                                      */
/*      SPRHBM (0x20000008)                                                     */
/*      SPRC0 (0x20000008)                                                      */
/*      SPRMCC (0x20000008)                                                     */
/*      SPRUCC (0x20000008)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [0]
*/


#define SWDEST_0_NPK_TPH_REG 0x1E120008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast0dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 0 Destination. Specifies the destination
                               port number (refer to GTHOPT register for
                               details) for Master 0 trace data. 0x0: Master 0
                               is routed to Output Port 0 0x1: Master 0 is
                               routed to Output Port 1 & 0x7: Master 0 is
                               routed to Output Port 0x7 The default value of
                               this bit field is controlled by the
                               corresponding MDEST[N] parameter.
                            */
    UINT32 mast0en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 0 Enable. Enables tracing for Master 0.
                               0: tracing for Master 0 is disabled. All data
                               received from Master 0 is received at the Input
                               Buffer and immediately dropped. 1: tracing for
                               Master 0 is enabled (Default)
                            */
    UINT32 mast1dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 1 Destination. See MAST0DEST for
                               definition.
                            */
    UINT32 mast1en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 1 Enable. See MAST0EN for definition. */
    UINT32 mast2dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 2 Destination. See MAST0DEST for
                               definition.
                            */
    UINT32 mast2en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 2 Enable. See MAST0EN for definition. */
    UINT32 mast3dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 3 Destination. See MAST1DEST for
                               definition.
                            */
    UINT32 mast3en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 3 Enable. See MAST0EN for definition. */
    UINT32 mast4dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 4 Destination. See MAST1DEST for
                               definition.
                            */
    UINT32 mast4en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 4 Enable. See MAST0EN for definition. */
    UINT32 mast5dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /* Master 4 Enable. See MAST0EN for definition. */
    UINT32 mast5en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 5 Enable. See MAST0EN for definition. */
    UINT32 mast6dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 6 Destination. See MAST1DEST for
                               definition.
                            */
    UINT32 mast6en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 6 Enable. See MAST0EN for definition. */
    UINT32 mast7dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 7 Destination. See MAST1DEST for
                               definition.
                            */
    UINT32 mast7en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 7 Enable. See MAST0EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_0_NPK_TPH_STRUCT;

/* SWDEST_1_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x2000000c)                                                      */
/*      SPRB0 (0x2000000c)                                                      */
/*      SPRHBM (0x2000000c)                                                     */
/*      SPRC0 (0x2000000c)                                                      */
/*      SPRMCC (0x2000000c)                                                     */
/*      SPRUCC (0x2000000c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [1]
*/


#define SWDEST_1_NPK_TPH_REG 0x1E12000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast8dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 8 Destination. Specifies the destination
                               port number (refer to GTHOPT register for
                               details) for Master 8 trace data. 0x0: Master 8
                               is routed to Output Port 0 0x1: Master 8 is
                               routed to Output Port 1 & 0x7: Master 8 is
                               routed to Output Port 0x7 The default value of
                               this bit field is controlled by the
                               corresponding MDEST[N] parameter.
                            */
    UINT32 mast8en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 8 Enable. Enables tracing for Master 8.
                               0: tracing for Master 8 is disabled. All data
                               received from Master 8 is received at the Input
                               Buffer and immediately dropped. 1: tracing for
                               Master 8 is enabled (Default)
                            */
    UINT32 mast9dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 9 Destination. See MAST8DEST for
                               definition.
                            */
    UINT32 mast9en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 9 Enable. See MAST8EN for definition. */
    UINT32 mast10dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 10 Destination. See MAST8DEST for
                               definition.
                            */
    UINT32 mast10en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 10 Enable. See MAST8EN for definition. */
    UINT32 mast11dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 11 Destination. See MAST8DEST for
                               definition.
                            */
    UINT32 mast11en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 11 Enable. See MAST8EN for definition. */
    UINT32 mast12dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 12 Destination. See MAST8DEST for
                               definition.
                            */
    UINT32 mast12en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 12 Enable. See MAST8EN for definition. */
    UINT32 mast13dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 13 Destination. See MAST8DEST for
                               definition.
                            */
    UINT32 mast13en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 13 Enable. See MAST8EN for definition. */
    UINT32 mast14dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 14 Destination. See MAST8DEST for
                               definition.
                            */
    UINT32 mast14en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 14 Enable. See MAST8EN for definition. */
    UINT32 mast15dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 15 Destination. See MAST8DEST for
                               definition.
                            */
    UINT32 mast15en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 15 Enable. See MAST8EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_1_NPK_TPH_STRUCT;

/* SWDEST_2_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20000010)                                                      */
/*      SPRB0 (0x20000010)                                                      */
/*      SPRHBM (0x20000010)                                                     */
/*      SPRC0 (0x20000010)                                                      */
/*      SPRMCC (0x20000010)                                                     */
/*      SPRUCC (0x20000010)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [2]
*/


#define SWDEST_2_NPK_TPH_REG 0x1E120010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast16dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 16 Destination. Specifies the destination
                               port number (refer to GTHOPT register for
                               details) for Master 16 trace data. 0x0: Master
                               16 is routed to Output Port 0 0x1: Master 16 is
                               routed to Output Port 1 & 0x7: Master 16 is
                               routed to Output Port 0x7 The default value of
                               this bit field is controlled by the
                               corresponding MDEST[N] parameter.
                            */
    UINT32 mast16en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 16 Enable. Enables tracing for Master 16.
                               0: tracing for Master 16 is disabled. All data
                               received from Master 16 is received at the Input
                               Buffer and immediately dropped. 1: tracing for
                               Master 16 is enabled (Default)
                            */
    UINT32 mast17dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 17 Destination. See MAST16DEST for
                               definition.
                            */
    UINT32 mast17en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 17 Enable. See MAST16EN for definition. */
    UINT32 mast18dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 18 Destination. See MAST16DEST for
                               definition.
                            */
    UINT32 mast18en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 18 Enable. See MAST16EN for definition. */
    UINT32 mast19dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 19 Destination. See MAST16DEST for
                               definition.
                            */
    UINT32 mast19en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 19 Enable. See MAST16EN for definition. */
    UINT32 mast20dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 20 Destination. See MAST16DEST for
                               definition.
                            */
    UINT32 mast20en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 20 Enable. See MAST16EN for definition. */
    UINT32 mast21dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 21 Destination. See MAST16DEST for
                               definition.
                            */
    UINT32 mast21en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 21 Enable. See MAST16EN for definition. */
    UINT32 mast22dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 22 Destination. See MAST16DEST for
                               definition.
                            */
    UINT32 mast22en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 22 Enable. See MAST16EN for definition. */
    UINT32 mast23dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 23 Destination. See MAST16DEST for
                               definition.
                            */
    UINT32 mast23en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 23 Enable. See MAST16EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_2_NPK_TPH_STRUCT;

/* SWDEST_3_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20000014)                                                      */
/*      SPRB0 (0x20000014)                                                      */
/*      SPRHBM (0x20000014)                                                     */
/*      SPRC0 (0x20000014)                                                      */
/*      SPRMCC (0x20000014)                                                     */
/*      SPRUCC (0x20000014)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [3]
*/


#define SWDEST_3_NPK_TPH_REG 0x1E120014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast24dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 24 Destination. Specifies the destination
                               port number (refer to GTHOPT register for
                               details) for Master 24 trace data. 0x0: Master
                               24 is routed to Output Port 0 0x1: Master 24 is
                               routed to Output Port 1 & 0x7: Master 24 is
                               routed to Output Port 0x7 The default value of
                               this bit field is controlled by the
                               corresponding MDEST[N] parameter.
                            */
    UINT32 mast24en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 24 Enable. Enables tracing for Master 24.
                               0: tracing for Master 24 is disabled. All data
                               received from Master 24 is received at the Input
                               Buffer and immediately dropped. 1: tracing for
                               Master 24 is enabled (Default)
                            */
    UINT32 mast25dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 25 Destination. See MAST24DEST for
                               definition.
                            */
    UINT32 mast25en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 25 Enable. See MAST24EN for definition. */
    UINT32 mast26dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 26 Destination. See MAST24DEST for
                               definition.
                            */
    UINT32 mast26en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 26 Enable. See MAST24EN for definition. */
    UINT32 mast27dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 27 Destination. See MAST24DEST for
                               definition.
                            */
    UINT32 mast27en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 27 Enable. See MAST24EN for definition. */
    UINT32 mast28dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 28 Destination. See MAST24DEST for
                               definition.
                            */
    UINT32 mast28en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 28 Enable. See MAST24EN for definition. */
    UINT32 mast29dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 29 Destination. See MAST24DEST for
                               definition.
                            */
    UINT32 mast29en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 29 Enable. See MAST24EN for definition. */
    UINT32 mast30dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 30 Destination. See MAST24DEST for
                               definition.
                            */
    UINT32 mast30en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 30 Enable. See MAST24EN for definition. */
    UINT32 mast31dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 31 Destination. See MAST24DEST for
                               definition.
                            */
    UINT32 mast31en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 31 Enable. See MAST24EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_3_NPK_TPH_STRUCT;

/* SWDEST_4_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20000018)                                                      */
/*      SPRB0 (0x20000018)                                                      */
/*      SPRHBM (0x20000018)                                                     */
/*      SPRC0 (0x20000018)                                                      */
/*      SPRMCC (0x20000018)                                                     */
/*      SPRUCC (0x20000018)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [4]
*/


#define SWDEST_4_NPK_TPH_REG 0x1E120018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast32dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 32 Destination. Specifies the destination
                               port number (refer to GTHOPT register for
                               details) for Master 32 trace data. 0x0: Master
                               32 is routed to Output Port 0 0x1: Master 32 is
                               routed to Output Port 1 & 0x7: Master 32 is
                               routed to Output Port 0x7 The default value of
                               this bit field is controlled by the
                               corresponding MDEST[N] parameter.
                            */
    UINT32 mast32en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 32 Enable. Enables tracing for Master 32.
                               0: tracing for Master 32 is disabled. All data
                               received from Master 32 is received at the Input
                               Buffer and immediately dropped. 1: tracing for
                               Master 32 is enabled (Default)
                            */
    UINT32 mast33dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 33 Destination. See MAST32DEST for
                               definition.
                            */
    UINT32 mast33en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 33 Enable. See MAST32EN for definition. */
    UINT32 mast34dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 34 Destination. See MAST32DEST for
                               definition.
                            */
    UINT32 mast34en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 34 Enable. See MAST32EN for definition. */
    UINT32 mast35dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 35 Destination. See MAST32DEST for
                               definition.
                            */
    UINT32 mast35en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 35 Enable. See MAST32EN for definition. */
    UINT32 mast36dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 36 Destination. See MAST32DEST for
                               definition.
                            */
    UINT32 mast36en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 36 Enable. See MAST32EN for definition. */
    UINT32 mast37dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 37 Destination. See MAST32DEST for
                               definition.
                            */
    UINT32 mast37en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 37 Enable. See MAST32EN for definition. */
    UINT32 mast38dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 38 Destination. See MAST32DEST for
                               definition.
                            */
    UINT32 mast38en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 38 Enable. See MAST32EN for definition. */
    UINT32 mast39dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 39 Destination. See MAST32DEST for
                               definition.
                            */
    UINT32 mast39en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 39 Enable. See MAST32EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_4_NPK_TPH_STRUCT;

/* SWDEST_5_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x2000001c)                                                      */
/*      SPRB0 (0x2000001c)                                                      */
/*      SPRHBM (0x2000001c)                                                     */
/*      SPRC0 (0x2000001c)                                                      */
/*      SPRMCC (0x2000001c)                                                     */
/*      SPRUCC (0x2000001c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [5]
*/


#define SWDEST_5_NPK_TPH_REG 0x1E12001C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast40dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 40 Destination. Specifies the destination
                               port number (refer to GTHOPT register for
                               details) for Master 40 trace data. 0x0: Master
                               40 is routed to Output Port 0 0x1: Master 40 is
                               routed to Output Port 1 & 0x7: Master 40 is
                               routed to Output Port 0x7 The default value of
                               this bit field is controlled by the
                               corresponding MDEST[N] parameter.
                            */
    UINT32 mast40en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 40 Enable. Enables tracing for Master 40.
                               0: tracing for Master 40 is disabled. All data
                               received from Master 40 is received at the Input
                               Buffer and immediately dropped. 1: tracing for
                               Master 40 is enabled (Default)
                            */
    UINT32 mast41dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 41 Destination. See MAST40DEST for
                               definition.
                            */
    UINT32 mast41en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 41 Enable. See MAST40EN for definition. */
    UINT32 mast42dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 42 Destination. See MAST40DEST for
                               definition.
                            */
    UINT32 mast42en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 42 Enable. See MAST40EN for definition. */
    UINT32 mast43dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 43 Destination. See MAST40DEST for
                               definition.
                            */
    UINT32 mast43en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 43 Enable. See MAST40EN for definition. */
    UINT32 mast44dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 44 Destination. See MAST40DEST for
                               definition.
                            */
    UINT32 mast44en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 44 Enable. See MAST40EN for definition. */
    UINT32 mast45dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 45 Destination. See MAST40DEST for
                               definition.
                            */
    UINT32 mast45en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 45 Enable. See MAST40EN for definition. */
    UINT32 mast46dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 46 Destination. See MAST40DEST for
                               definition.
                            */
    UINT32 mast46en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 46 Enable. See MAST40EN for definition. */
    UINT32 mast47dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 47 Destination. See MAST40DEST for
                               definition.
                            */
    UINT32 mast47en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 47 Enable. See MAST40EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_5_NPK_TPH_STRUCT;

/* SWDEST_6_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20000020)                                                      */
/*      SPRB0 (0x20000020)                                                      */
/*      SPRHBM (0x20000020)                                                     */
/*      SPRC0 (0x20000020)                                                      */
/*      SPRMCC (0x20000020)                                                     */
/*      SPRUCC (0x20000020)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [6]
*/


#define SWDEST_6_NPK_TPH_REG 0x1E120020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast48dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 48 Destination. Specifies the destination
                               port number (refer to GTHOPT register for
                               details) for Master 48 trace data. 0x0: Master
                               48 is routed to Output Port 0 0x1: Master 48 is
                               routed to Output Port 1 & 0x7: Master 48 is
                               routed to Output Port 0x7 The default value of
                               this bit field is controlled by the
                               corresponding MDEST[N] parameter.
                            */
    UINT32 mast48en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 48 Enable. Enables tracing for Master 48.
                               0: tracing for Master 48 is disabled. All data
                               received from Master 48 is received at the Input
                               Buffer and immediately dropped. 1: tracing for
                               Master 48 is enabled (Default)
                            */
    UINT32 mast49dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 49 Destination. See MAST48DEST for
                               definition.
                            */
    UINT32 mast49en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 49 Enable. See MAST48EN for definition. */
    UINT32 mast50dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 50 Destination. See MAST48DEST for
                               definition.
                            */
    UINT32 mast50en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 50 Enable. See MAST48EN for definition. */
    UINT32 mast51dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 51 Destination. See MAST48DEST for
                               definition.
                            */
    UINT32 mast51en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 51 Enable. See MAST48EN for definition. */
    UINT32 mast52dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 52 Destination. See MAST48DEST for
                               definition.
                            */
    UINT32 mast52en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 52 Enable. See MAST48EN for definition. */
    UINT32 mast53dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 53 Destination. See MAST48DEST for
                               definition.
                            */
    UINT32 mast53en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 53 Enable. See MAST48EN for definition. */
    UINT32 mast54dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 54 Destination. See MAST48DEST for
                               definition.
                            */
    UINT32 mast54en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 54 Enable. See MAST48EN for definition. */
    UINT32 mast55dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 55 Destination. See MAST48DEST for
                               definition.
                            */
    UINT32 mast55en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 55 Enable. See MAST48EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_6_NPK_TPH_STRUCT;

/* SWDEST_7_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20000024)                                                      */
/*      SPRB0 (0x20000024)                                                      */
/*      SPRHBM (0x20000024)                                                     */
/*      SPRC0 (0x20000024)                                                      */
/*      SPRMCC (0x20000024)                                                     */
/*      SPRUCC (0x20000024)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [7]
*/


#define SWDEST_7_NPK_TPH_REG 0x1E120024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast56dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 56 Destination. Specifies the destination
                               port number (refer to GTHOPT register for
                               details) for Master 56 trace data. 0x0: Master
                               56 is routed to Output Port 0 0x1: Master 56 is
                               routed to Output Port 1 & 0x7: Master 56 is
                               routed to Output Port 0x7 The default value of
                               this bit field is controlled by the
                               corresponding MDEST[N] parameter.
                            */
    UINT32 mast56en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 56 Enable. Enables tracing for Master 56.
                               0: tracing for Master 56 is disabled. All data
                               received from Master 56 is received at the Input
                               Buffer and immediately dropped. 1: tracing for
                               Master 56 is enabled (Default)
                            */
    UINT32 mast57dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 57 Destination. See MAST56DEST for
                               definition.
                            */
    UINT32 mast57en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 57 Enable. See MAST56EN for definition. */
    UINT32 mast58dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 58 Destination. See MAST56DEST for
                               definition.
                            */
    UINT32 mast58en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 58 Enable. See MAST56EN for definition. */
    UINT32 mast59dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 59 Destination. See MAST56DEST for
                               definition.
                            */
    UINT32 mast59en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 59 Enable. See MAST56EN for definition. */
    UINT32 mast60dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 60 Destination. See MAST56DEST for
                               definition.
                            */
    UINT32 mast60en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 60 Enable. See MAST56EN for definition. */
    UINT32 mast61dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 61 Destination. See MAST56DEST for
                               definition.
                            */
    UINT32 mast61en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 61 Enable. See MAST56EN for definition. */
    UINT32 mast62dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 62 Destination. See MAST56DEST for
                               definition.
                            */
    UINT32 mast62en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 62 Enable. See MAST56EN for definition. */
    UINT32 mast63dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 63 Destination. See MAST56DEST for
                               definition.
                            */
    UINT32 mast63en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 63 Enable. See MAST56EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_7_NPK_TPH_STRUCT;

/* SWDEST_8_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20000028)                                                      */
/*      SPRB0 (0x20000028)                                                      */
/*      SPRHBM (0x20000028)                                                     */
/*      SPRC0 (0x20000028)                                                      */
/*      SPRMCC (0x20000028)                                                     */
/*      SPRUCC (0x20000028)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [8]
*/


#define SWDEST_8_NPK_TPH_REG 0x1E120028

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast64dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 64 Destination. Specifies the destination
                               port number (refer to GTHOPT register for
                               details) for Master 64 trace data. 0x0: Master
                               64 is routed to Output Port 0 0x1: Master 64 is
                               routed to Output Port 1 & 0x7: Master 64 is
                               routed to Output Port 0x7 The default value of
                               this bit field is controlled by the
                               corresponding MDEST[N] parameter.
                            */
    UINT32 mast64en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 64 Enable. Enables tracing for Master 64.
                               0: tracing for Master 64 is disabled. All data
                               received from Master 64 is received at the Input
                               Buffer and immediately dropped. 1: tracing for
                               Master 64 is enabled (Default)
                            */
    UINT32 mast65dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 65 Destination. See MAST64DEST for
                               definition.
                            */
    UINT32 mast65en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 65 Enable. See MAST64EN for definition. */
    UINT32 mast66dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 66 Destination. See MAST64DEST for
                               definition.
                            */
    UINT32 mast66en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 66 Enable. See MAST64EN for definition. */
    UINT32 mast67dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 67 Destination. See MAST64DEST for
                               definition.
                            */
    UINT32 mast67en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 67 Enable. See MAST64EN for definition. */
    UINT32 mast68dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 68 Destination. See MAST64DEST for
                               definition.
                            */
    UINT32 mast68en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 68 Enable. See MAST64EN for definition. */
    UINT32 mast69dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 69 Destination. See MAST64DEST for
                               definition.
                            */
    UINT32 mast69en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 69 Enable. See MAST64EN for definition. */
    UINT32 mast70dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 70 Destination. See MAST64DEST for
                               definition.
                            */
    UINT32 mast70en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 70 Enable. See MAST64EN for definition. */
    UINT32 mast71dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 71 Destination. See MAST64DEST for
                               definition.
                            */
    UINT32 mast71en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 71 Enable. See MAST64EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_8_NPK_TPH_STRUCT;

/* SWDEST_9_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x2000002c)                                                      */
/*      SPRB0 (0x2000002c)                                                      */
/*      SPRHBM (0x2000002c)                                                     */
/*      SPRC0 (0x2000002c)                                                      */
/*      SPRMCC (0x2000002c)                                                     */
/*      SPRUCC (0x2000002c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [9]
*/


#define SWDEST_9_NPK_TPH_REG 0x1E12002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast72dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 72 Destination. Specifies the destination
                               port number (refer to GTHOPT register for
                               details) for Master 72 trace data. 0x0: Master
                               72 is routed to Output Port 0 0x1: Master 72 is
                               routed to Output Port 1 & 0x7: Master 72 is
                               routed to Output Port 0x7 The default value of
                               this bit field is controlled by the
                               corresponding MDEST[N] parameter.
                            */
    UINT32 mast72en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 72 Enable. Enables tracing for Master 72.
                               0: tracing for Master 72 is disabled. All data
                               received from Master 72 is received at the Input
                               Buffer and immediately dropped. 1: tracing for
                               Master 72 is enabled (Default)
                            */
    UINT32 mast73dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 73 Destination. See MAST72DEST for
                               definition.
                            */
    UINT32 mast73en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 73 Enable. See MAST72EN for definition. */
    UINT32 mast74dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 74 Destination. See MAST72DEST for
                               definition.
                            */
    UINT32 mast74en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 74 Enable. See MAST72EN for definition. */
    UINT32 mast75dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 75 Destination. See MAST72DEST for
                               definition.
                            */
    UINT32 mast75en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 75 Enable. See MAST72EN for definition. */
    UINT32 mast76dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 76 Destination. See MAST72DEST for
                               definition.
                            */
    UINT32 mast76en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 76 Enable. See MAST72EN for definition. */
    UINT32 mast77dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 77 Destination. See MAST72DEST for
                               definition.
                            */
    UINT32 mast77en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 77 Enable. See MAST72EN for definition. */
    UINT32 mast78dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 78 Destination. See MAST72DEST for
                               definition.
                            */
    UINT32 mast78en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 78 Enable. See MAST72EN for definition. */
    UINT32 mast79dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 79 Destination. See MAST72DEST for
                               definition.
                            */
    UINT32 mast79en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 79 Enable. See MAST72EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_9_NPK_TPH_STRUCT;

/* SWDEST_10_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000030)                                                      */
/*      SPRB0 (0x20000030)                                                      */
/*      SPRHBM (0x20000030)                                                     */
/*      SPRC0 (0x20000030)                                                      */
/*      SPRMCC (0x20000030)                                                     */
/*      SPRUCC (0x20000030)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [10]
*/


#define SWDEST_10_NPK_TPH_REG 0x1E120030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast80dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 80 Destination. Specifies the destination
                               port number (refer to GTHOPT register for
                               details) for Master 80 trace data. 0x0: Master
                               80 is routed to Output Port 0 0x1: Master 80 is
                               routed to Output Port 1 & 0x7: Master 80 is
                               routed to Output Port 0x7 The default value of
                               this bit field is controlled by the
                               corresponding MDEST[N] parameter.
                            */
    UINT32 mast80en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 80 Enable. Enables tracing for Master 80.
                               0: tracing for Master 80 is disabled. All data
                               received from Master 80 is received at the Input
                               Buffer and immediately dropped. 1: tracing for
                               Master 80 is enabled (Default)
                            */
    UINT32 mast81dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 81 Destination. See MAST80DEST for
                               definition.
                            */
    UINT32 mast81en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 81 Enable. See MAST80EN for definition. */
    UINT32 mast82dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 82 Destination. See MAST80DEST for
                               definition.
                            */
    UINT32 mast82en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 82 Enable. See MAST80EN for definition. */
    UINT32 mast83dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 83 Destination. See MAST80DEST for
                               definition.
                            */
    UINT32 mast83en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 83 Enable. See MAST80EN for definition. */
    UINT32 mast84dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 84 Destination. See MAST80DEST for
                               definition.
                            */
    UINT32 mast84en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 84 Enable. See MAST80EN for definition. */
    UINT32 mast85dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 85 Destination. See MAST80DEST for
                               definition.
                            */
    UINT32 mast85en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 85 Enable. See MAST80EN for definition. */
    UINT32 mast86dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 86 Destination. See MAST80DEST for
                               definition.
                            */
    UINT32 mast86en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 86 Enable. See MAST80EN for definition. */
    UINT32 mast87dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 87 Destination. See MAST80DEST for
                               definition.
                            */
    UINT32 mast87en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 87 Enable. See MAST80EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_10_NPK_TPH_STRUCT;

/* SWDEST_11_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000034)                                                      */
/*      SPRB0 (0x20000034)                                                      */
/*      SPRHBM (0x20000034)                                                     */
/*      SPRC0 (0x20000034)                                                      */
/*      SPRMCC (0x20000034)                                                     */
/*      SPRUCC (0x20000034)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [11]
*/


#define SWDEST_11_NPK_TPH_REG 0x1E120034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast88dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 88 Destination. Specifies the destination
                               port number (refer to GTHOPT register for
                               details) for Master 88 trace data. 0x0: Master
                               88 is routed to Output Port 0 0x1: Master 88 is
                               routed to Output Port 1 & 0x7: Master 88 is
                               routed to Output Port 0x7 The default value of
                               this bit field is controlled by the
                               corresponding MDEST[N] parameter.
                            */
    UINT32 mast88en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 88 Enable. Enables tracing for Master 88.
                               0: tracing for Master 88 is disabled. All data
                               received from Master 88 is received at the Input
                               Buffer and immediately dropped. 1: tracing for
                               Master 88 is enabled (Default)
                            */
    UINT32 mast89dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 89 Destination. See MAST88DEST for
                               definition.
                            */
    UINT32 mast89en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 89 Enable. See MAST88EN for definition. */
    UINT32 mast90dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 90 Destination. See MAST88DEST for
                               definition.
                            */
    UINT32 mast90en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 90 Enable. See MAST88EN for definition. */
    UINT32 mast91dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 91 Destination. See MAST88DEST for
                               definition.
                            */
    UINT32 mast91en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 91 Enable. See MAST88EN for definition. */
    UINT32 mast92dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 92 Destination. See MAST88DEST for
                               definition.
                            */
    UINT32 mast92en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 92 Enable. See MAST88EN for definition. */
    UINT32 mast93dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 93 Destination. See MAST88DEST for
                               definition.
                            */
    UINT32 mast93en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 93 Enable. See MAST88EN for definition. */
    UINT32 mast94dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 94 Destination. See MAST88DEST for
                               definition.
                            */
    UINT32 mast94en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 94 Enable. See MAST88EN for definition. */
    UINT32 mast95dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 95 Destination. See MAST88DEST for
                               definition.
                            */
    UINT32 mast95en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 95 Enable. See MAST88EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_11_NPK_TPH_STRUCT;

/* SWDEST_12_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000038)                                                      */
/*      SPRB0 (0x20000038)                                                      */
/*      SPRHBM (0x20000038)                                                     */
/*      SPRC0 (0x20000038)                                                      */
/*      SPRMCC (0x20000038)                                                     */
/*      SPRUCC (0x20000038)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [12]
*/


#define SWDEST_12_NPK_TPH_REG 0x1E120038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast96dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 96 Destination. Specifies the destination
                               port number (refer to GTHOPT register for
                               details) for Master 96 trace data. 0x0: Master
                               96 is routed to Output Port 0 0x1: Master 96 is
                               routed to Output Port 1 & 0x7: Master 96 is
                               routed to Output Port 0x7 The default value of
                               this bit field is controlled by the
                               corresponding MDEST[N] parameter.
                            */
    UINT32 mast96en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 96 Enable. Enables tracing for Master 96.
                               0: tracing for Master 96 is disabled. All data
                               received from Master 96 is received at the Input
                               Buffer and immediately dropped. 1: tracing for
                               Master 96 is enabled (Default)
                            */
    UINT32 mast97dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 97 Destination. See MAST96DEST for
                               definition.
                            */
    UINT32 mast97en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 97 Enable. See MAST96EN for definition. */
    UINT32 mast98dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 98 Destination. See MAST96DEST for
                               definition.
                            */
    UINT32 mast98en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 98 Enable. See MAST96EN for definition. */
    UINT32 mast99dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 99 Destination. See MAST96DEST for
                               definition.
                            */
    UINT32 mast99en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 99 Enable. See MAST96EN for definition. */
    UINT32 mast100dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 100 Destination. See MAST96DEST for
                               definition.
                            */
    UINT32 mast100en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 100 Enable. See MAST96EN for definition. */
    UINT32 mast101dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 101 Destination. See MAST96DEST for
                               definition.
                            */
    UINT32 mast101en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 101 Enable. See MAST96EN for definition. */
    UINT32 mast102dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 102 Destination. See MAST96DEST for
                               definition.
                            */
    UINT32 mast102en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 102 Enable. See MAST96EN for definition. */
    UINT32 mast103dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 103 Destination. See MAST96DEST for
                               definition.
                            */
    UINT32 mast103en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 103 Enable. See MAST96EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_12_NPK_TPH_STRUCT;

/* SWDEST_13_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x2000003c)                                                      */
/*      SPRB0 (0x2000003c)                                                      */
/*      SPRHBM (0x2000003c)                                                     */
/*      SPRC0 (0x2000003c)                                                      */
/*      SPRMCC (0x2000003c)                                                     */
/*      SPRUCC (0x2000003c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [13]
*/


#define SWDEST_13_NPK_TPH_REG 0x1E12003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast104dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 104 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 104 trace data.
                               0x0: Master 104 is routed to Output Port 0 0x1:
                               Master 104 is routed to Output Port 1 & 0x7:
                               Master 104 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast104en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 104 Enable. Enables tracing for Master
                               104. 0: tracing for Master 104 is disabled. All
                               data received from Master 104 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 104 is enabled (Default)
                            */
    UINT32 mast105dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 105 Destination. See MAST104DEST for
                               definition.
                            */
    UINT32 mast105en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 105 Enable. See MAST104EN for definition. */
    UINT32 mast106dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 106 Destination. See MAST104DEST for
                               definition.
                            */
    UINT32 mast106en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 106 Enable. See MAST104EN for definition. */
    UINT32 mast107dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 107 Destination. See MAST104DEST for
                               definition.
                            */
    UINT32 mast107en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 107 Enable. See MAST104EN for definition. */
    UINT32 mast108dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 108 Destination. See MAST104DEST for
                               definition.
                            */
    UINT32 mast108en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 108 Enable. See MAST104EN for definition. */
    UINT32 mast109dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 109 Destination. See MAST104DEST for
                               definition.
                            */
    UINT32 mast109en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 109 Enable. See MAST104EN for definition. */
    UINT32 mast110dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 110 Destination. See MAST104DEST for
                               definition.
                            */
    UINT32 mast110en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 110 Enable. See MAST104EN for definition. */
    UINT32 mast111dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 111 Destination. See MAST104DEST for
                               definition.
                            */
    UINT32 mast111en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 111 Enable. See MAST104EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_13_NPK_TPH_STRUCT;

/* SWDEST_14_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000040)                                                      */
/*      SPRB0 (0x20000040)                                                      */
/*      SPRHBM (0x20000040)                                                     */
/*      SPRC0 (0x20000040)                                                      */
/*      SPRMCC (0x20000040)                                                     */
/*      SPRUCC (0x20000040)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [14]
*/


#define SWDEST_14_NPK_TPH_REG 0x1E120040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast112dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 112 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 112 trace data.
                               0x0: Master 112 is routed to Output Port 0 0x1:
                               Master 112 is routed to Output Port 1 & 0x7:
                               Master 112 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast112en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 112 Enable. Enables tracing for Master
                               112. 0: tracing for Master 112 is disabled. All
                               data received from Master 112 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 112 is enabled (Default)
                            */
    UINT32 mast113dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 113 Destination. See MAST112DEST for
                               definition.
                            */
    UINT32 mast113en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 113 Enable. See MAST112EN for definition. */
    UINT32 mast114dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 114 Destination. See MAST112DEST for
                               definition.
                            */
    UINT32 mast114en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 114 Enable. See MAST112EN for definition. */
    UINT32 mast115dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 115 Destination. See MAST112DEST for
                               definition.
                            */
    UINT32 mast115en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 115 Enable. See MAST112EN for definition. */
    UINT32 mast116dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 116 Destination. See MAST112DEST for
                               definition.
                            */
    UINT32 mast116en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 116 Enable. See MAST112EN for definition. */
    UINT32 mast117dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 117 Destination. See MAST112DEST for
                               definition.
                            */
    UINT32 mast117en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 117 Enable. See MAST112EN for definition. */
    UINT32 mast118dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 118 Destination. See MAST112DEST for
                               definition.
                            */
    UINT32 mast118en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 118 Enable. See MAST112EN for definition. */
    UINT32 mast119dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 119 Destination. See MAST112DEST for
                               definition.
                            */
    UINT32 mast119en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 119 Enable. See MAST112EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_14_NPK_TPH_STRUCT;

/* SWDEST_15_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000044)                                                      */
/*      SPRB0 (0x20000044)                                                      */
/*      SPRHBM (0x20000044)                                                     */
/*      SPRC0 (0x20000044)                                                      */
/*      SPRMCC (0x20000044)                                                     */
/*      SPRUCC (0x20000044)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [15]
*/


#define SWDEST_15_NPK_TPH_REG 0x1E120044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast120dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 120 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 120 trace data.
                               0x0: Master 120 is routed to Output Port 0 0x1:
                               Master 120 is routed to Output Port 1 & 0x7:
                               Master 120 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast120en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 120 Enable. Enables tracing for Master
                               120. 0: tracing for Master 120 is disabled. All
                               data received from Master 120 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 120 is enabled (Default)
                            */
    UINT32 mast121dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 121 Destination. See MAST120DEST for
                               definition.
                            */
    UINT32 mast121en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 121 Enable. See MAST120EN for definition. */
    UINT32 mast122dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 122 Destination. See MAST120DEST for
                               definition.
                            */
    UINT32 mast122en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 122 Enable. See MAST120EN for definition. */
    UINT32 mast123dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 123 Destination. See MAST120DEST for
                               definition.
                            */
    UINT32 mast123en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 123 Enable. See MAST120EN for definition. */
    UINT32 mast124dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 124 Destination. See MAST120DEST for
                               definition.
                            */
    UINT32 mast124en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 124 Enable. See MAST120EN for definition. */
    UINT32 mast125dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 125 Destination. See MAST120DEST for
                               definition.
                            */
    UINT32 mast125en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 125 Enable. See MAST120EN for definition. */
    UINT32 mast126dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 126 Destination. See MAST120DEST for
                               definition.
                            */
    UINT32 mast126en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 126 Enable. See MAST120EN for definition. */
    UINT32 mast127dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 127 Destination. See MAST120DEST for
                               definition.
                            */
    UINT32 mast127en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 127 Enable. See MAST120EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_15_NPK_TPH_STRUCT;

/* SWDEST_16_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000048)                                                      */
/*      SPRB0 (0x20000048)                                                      */
/*      SPRHBM (0x20000048)                                                     */
/*      SPRC0 (0x20000048)                                                      */
/*      SPRMCC (0x20000048)                                                     */
/*      SPRUCC (0x20000048)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [16]
*/


#define SWDEST_16_NPK_TPH_REG 0x1E120048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast128dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 128 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 128 trace data.
                               0x0: Master 128 is routed to Output Port 0 0x1:
                               Master 128 is routed to Output Port 1 & 0x7:
                               Master 128 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast128en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 128 Enable. Enables tracing for Master
                               128. 0: tracing for Master 128 is disabled. All
                               data received from Master 128 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 128 is enabled (Default)
                            */
    UINT32 mast129dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 129 Destination. See MAST128DEST for
                               definition.
                            */
    UINT32 mast129en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 129 Enable. See MAST128EN for definition. */
    UINT32 mast130dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 130 Destination. See MAST128DEST for
                               definition.
                            */
    UINT32 mast130en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 130 Enable. See MAST128EN for definition. */
    UINT32 mast131dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 131 Destination. See MAST128DEST for
                               definition.
                            */
    UINT32 mast131en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 131 Enable. See MAST128EN for definition. */
    UINT32 mast132dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 132 Destination. See MAST128DEST for
                               definition.
                            */
    UINT32 mast132en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 132 Enable. See MAST128EN for definition. */
    UINT32 mast133dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 133 Destination. See MAST128DEST for
                               definition.
                            */
    UINT32 mast133en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 133 Enable. See MAST128EN for definition. */
    UINT32 mast134dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 134 Destination. See MAST128DEST for
                               definition.
                            */
    UINT32 mast134en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 134 Enable. See MAST128EN for definition. */
    UINT32 mast135dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 135 Destination. See MAST128DEST for
                               definition.
                            */
    UINT32 mast135en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 135 Enable. See MAST128EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_16_NPK_TPH_STRUCT;

/* SWDEST_17_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x2000004c)                                                      */
/*      SPRB0 (0x2000004c)                                                      */
/*      SPRHBM (0x2000004c)                                                     */
/*      SPRC0 (0x2000004c)                                                      */
/*      SPRMCC (0x2000004c)                                                     */
/*      SPRUCC (0x2000004c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [17]
*/


#define SWDEST_17_NPK_TPH_REG 0x1E12004C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast136dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 136 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 136 trace data.
                               0x0: Master 136 is routed to Output Port 0 0x1:
                               Master 136 is routed to Output Port 1 & 0x7:
                               Master 136 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast136en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 136 Enable. Enables tracing for Master
                               136. 0: tracing for Master 136 is disabled. All
                               data received from Master 136 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 136 is enabled (Default)
                            */
    UINT32 mast137dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 137 Destination. See MAST136DEST for
                               definition.
                            */
    UINT32 mast137en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 137 Enable. See MAST136EN for definition. */
    UINT32 mast138dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 138 Destination. See MAST136DEST for
                               definition.
                            */
    UINT32 mast138en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 138 Enable. See MAST136EN for definition. */
    UINT32 mast139dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 139 Destination. See MAST136DEST for
                               definition.
                            */
    UINT32 mast139en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 139 Enable. See MAST136EN for definition. */
    UINT32 mast140dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 140 Destination. See MAST136DEST for
                               definition.
                            */
    UINT32 mast140en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 140 Enable. See MAST136EN for definition. */
    UINT32 mast141dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 141 Destination. See MAST136DEST for
                               definition.
                            */
    UINT32 mast141en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 141 Enable. See MAST136EN for definition. */
    UINT32 mast142dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 142 Destination. See MAST136DEST for
                               definition.
                            */
    UINT32 mast142en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 142 Enable. See MAST136EN for definition. */
    UINT32 mast143dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 143 Destination. See MAST136DEST for
                               definition.
                            */
    UINT32 mast143en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 143 Enable. See MAST136EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_17_NPK_TPH_STRUCT;

/* SWDEST_18_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000050)                                                      */
/*      SPRB0 (0x20000050)                                                      */
/*      SPRHBM (0x20000050)                                                     */
/*      SPRC0 (0x20000050)                                                      */
/*      SPRMCC (0x20000050)                                                     */
/*      SPRUCC (0x20000050)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [18]
*/


#define SWDEST_18_NPK_TPH_REG 0x1E120050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast144dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 144 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 144 trace data.
                               0x0: Master 144 is routed to Output Port 0 0x1:
                               Master 144 is routed to Output Port 1 & 0x7:
                               Master 144 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast144en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 144 Enable. Enables tracing for Master
                               144. 0: tracing for Master 144 is disabled. All
                               data received from Master 144 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 144 is enabled (Default)
                            */
    UINT32 mast145dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 145 Destination. See MAST144DEST for
                               definition.
                            */
    UINT32 mast145en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 145 Enable. See MAST144EN for definition. */
    UINT32 mast146dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 146 Destination. See MAST144DEST for
                               definition.
                            */
    UINT32 mast146en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 146 Enable. See MAST144EN for definition. */
    UINT32 mast147dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 147 Destination. See MAST144DEST for
                               definition.
                            */
    UINT32 mast147en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 147 Enable. See MAST144EN for definition. */
    UINT32 mast148dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 148 Destination. See MAST144DEST for
                               definition.
                            */
    UINT32 mast148en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 148 Enable. See MAST144EN for definition. */
    UINT32 mast149dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 149 Destination. See MAST144DEST for
                               definition.
                            */
    UINT32 mast149en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 149 Enable. See MAST144EN for definition. */
    UINT32 mast150dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 150 Destination. See MAST144DEST for
                               definition.
                            */
    UINT32 mast150en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 150 Enable. See MAST144EN for definition. */
    UINT32 mast151dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 151 Destination. See MAST144DEST for
                               definition.
                            */
    UINT32 mast151en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 151 Enable. See MAST144EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_18_NPK_TPH_STRUCT;

/* SWDEST_19_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000054)                                                      */
/*      SPRB0 (0x20000054)                                                      */
/*      SPRHBM (0x20000054)                                                     */
/*      SPRC0 (0x20000054)                                                      */
/*      SPRMCC (0x20000054)                                                     */
/*      SPRUCC (0x20000054)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [19]
*/


#define SWDEST_19_NPK_TPH_REG 0x1E120054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast152dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 152 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 152 trace data.
                               0x0: Master 152 is routed to Output Port 0 0x1:
                               Master 152 is routed to Output Port 1 & 0x7:
                               Master 152 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast152en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 152 Enable. Enables tracing for Master
                               152. 0: tracing for Master 152 is disabled. All
                               data received from Master 152 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 152 is enabled (Default)
                            */
    UINT32 mast153dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 153 Destination. See MAST152DEST for
                               definition.
                            */
    UINT32 mast153en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 153 Enable. See MAST152EN for definition. */
    UINT32 mast154dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 154 Destination. See MAST152DEST for
                               definition.
                            */
    UINT32 mast154en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 154 Enable. See MAST152EN for definition. */
    UINT32 mast155dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 155 Destination. See MAST152DEST for
                               definition.
                            */
    UINT32 mast155en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 155 Enable. See MAST152EN for definition. */
    UINT32 mast156dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 156 Destination. See MAST152DEST for
                               definition.
                            */
    UINT32 mast156en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 156 Enable. See MAST152EN for definition. */
    UINT32 mast157dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 157 Destination. See MAST152DEST for
                               definition.
                            */
    UINT32 mast157en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 157 Enable. See MAST152EN for definition. */
    UINT32 mast158dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 158 Destination. See MAST152DEST for
                               definition.
                            */
    UINT32 mast158en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 158 Enable. See MAST152EN for definition. */
    UINT32 mast159dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 159 Destination. See MAST152DEST for
                               definition.
                            */
    UINT32 mast159en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 159 Enable. See MAST152EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_19_NPK_TPH_STRUCT;

/* SWDEST_20_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000058)                                                      */
/*      SPRB0 (0x20000058)                                                      */
/*      SPRHBM (0x20000058)                                                     */
/*      SPRC0 (0x20000058)                                                      */
/*      SPRMCC (0x20000058)                                                     */
/*      SPRUCC (0x20000058)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [20]
*/


#define SWDEST_20_NPK_TPH_REG 0x1E120058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast160dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 160 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 160 trace data.
                               0x0: Master 160 is routed to Output Port 0 0x1:
                               Master 160 is routed to Output Port 1 & 0x7:
                               Master 160 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast160en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 160 Enable. Enables tracing for Master
                               160. 0: tracing for Master 160 is disabled. All
                               data received from Master 160 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 160 is enabled (Default)
                            */
    UINT32 mast161dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 161 Destination. See MAST160DEST for
                               definition.
                            */
    UINT32 mast161en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 161 Enable. See MAST160EN for definition. */
    UINT32 mast162dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 162 Destination. See MAST160DEST for
                               definition.
                            */
    UINT32 mast162en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 162 Enable. See MAST160EN for definition. */
    UINT32 mast163dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 163 Destination. See MAST160DEST for
                               definition.
                            */
    UINT32 mast163en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 163 Enable. See MAST160EN for definition. */
    UINT32 mast164dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 164 Destination. See MAST160DEST for
                               definition.
                            */
    UINT32 mast164en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 164 Enable. See MAST160EN for definition. */
    UINT32 mast165dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 165 Destination. See MAST160DEST for
                               definition.
                            */
    UINT32 mast165en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 165 Enable. See MAST160EN for definition. */
    UINT32 mast166dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 166 Destination. See MAST160DEST for
                               definition.
                            */
    UINT32 mast166en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 166 Enable. See MAST160EN for definition. */
    UINT32 mast167dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 167 Destination. See MAST160DEST for
                               definition.
                            */
    UINT32 mast167en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 167 Enable. See MAST160EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_20_NPK_TPH_STRUCT;

/* SWDEST_21_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x2000005c)                                                      */
/*      SPRB0 (0x2000005c)                                                      */
/*      SPRHBM (0x2000005c)                                                     */
/*      SPRC0 (0x2000005c)                                                      */
/*      SPRMCC (0x2000005c)                                                     */
/*      SPRUCC (0x2000005c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [21]
*/


#define SWDEST_21_NPK_TPH_REG 0x1E12005C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast168dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 168 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 168 trace data.
                               0x0: Master 168 is routed to Output Port 0 0x1:
                               Master 168 is routed to Output Port 1 & 0x7:
                               Master 168 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast168en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 168 Enable. Enables tracing for Master
                               168. 0: tracing for Master 168 is disabled. All
                               data received from Master 168 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 168 is enabled (Default)
                            */
    UINT32 mast169dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 169 Destination. See MAST168DEST for
                               definition.
                            */
    UINT32 mast169en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 169 Enable. See MAST168EN for definition. */
    UINT32 mast170dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 170 Destination. See MAST168DEST for
                               definition.
                            */
    UINT32 mast170en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 170 Enable. See MAST168EN for definition. */
    UINT32 mast171dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 171 Destination. See MAST168DEST for
                               definition.
                            */
    UINT32 mast171en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 171 Enable. See MAST168EN for definition. */
    UINT32 mast172dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 172 Destination. See MAST168DEST for
                               definition.
                            */
    UINT32 mast172en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 172 Enable. See MAST168EN for definition. */
    UINT32 mast173dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 173 Destination. See MAST168DEST for
                               definition.
                            */
    UINT32 mast173en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 173 Enable. See MAST168EN for definition. */
    UINT32 mast174dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 174 Destination. See MAST168DEST for
                               definition.
                            */
    UINT32 mast174en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 174 Enable. See MAST168EN for definition. */
    UINT32 mast175dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 175 Destination. See MAST168DEST for
                               definition.
                            */
    UINT32 mast175en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 175 Enable. See MAST168EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_21_NPK_TPH_STRUCT;

/* SWDEST_22_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000060)                                                      */
/*      SPRB0 (0x20000060)                                                      */
/*      SPRHBM (0x20000060)                                                     */
/*      SPRC0 (0x20000060)                                                      */
/*      SPRMCC (0x20000060)                                                     */
/*      SPRUCC (0x20000060)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [22]
*/


#define SWDEST_22_NPK_TPH_REG 0x1E120060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast176dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 176 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 176 trace data.
                               0x0: Master 176 is routed to Output Port 0 0x1:
                               Master 176 is routed to Output Port 1 & 0x7:
                               Master 176 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast176en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 176 Enable. Enables tracing for Master
                               176. 0: tracing for Master 176 is disabled. All
                               data received from Master 176 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 176 is enabled (Default)
                            */
    UINT32 mast177dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 177 Destination. See MAST176DEST for
                               definition.
                            */
    UINT32 mast177en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 177 Enable. See MAST176EN for definition. */
    UINT32 mast178dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 178 Destination. See MAST176DEST for
                               definition.
                            */
    UINT32 mast178en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 178 Enable. See MAST176EN for definition. */
    UINT32 mast179dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 179 Destination. See MAST176DEST for
                               definition.
                            */
    UINT32 mast179en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 179 Enable. See MAST176EN for definition. */
    UINT32 mast180dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 180 Destination. See MAST176DEST for
                               definition.
                            */
    UINT32 mast180en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 180 Enable. See MAST176EN for definition. */
    UINT32 mast181dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 181 Destination. See MAST176DEST for
                               definition.
                            */
    UINT32 mast181en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 181 Enable. See MAST176EN for definition. */
    UINT32 mast182dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 182 Destination. See MAST176DEST for
                               definition.
                            */
    UINT32 mast182en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 182 Enable. See MAST176EN for definition. */
    UINT32 mast183dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 183 Destination. See MAST176DEST for
                               definition.
                            */
    UINT32 mast183en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 183 Enable. See MAST176EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_22_NPK_TPH_STRUCT;

/* SWDEST_23_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000064)                                                      */
/*      SPRB0 (0x20000064)                                                      */
/*      SPRHBM (0x20000064)                                                     */
/*      SPRC0 (0x20000064)                                                      */
/*      SPRMCC (0x20000064)                                                     */
/*      SPRUCC (0x20000064)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [23]
*/


#define SWDEST_23_NPK_TPH_REG 0x1E120064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast184dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 184 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 184 trace data.
                               0x0: Master 184 is routed to Output Port 0 0x1:
                               Master 184 is routed to Output Port 1 & 0x7:
                               Master 184 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast184en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 184 Enable. Enables tracing for Master
                               184. 0: tracing for Master 184 is disabled. All
                               data received from Master 184 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 184 is enabled (Default)
                            */
    UINT32 mast185dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 185 Destination. See MAST184DEST for
                               definition.
                            */
    UINT32 mast185en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 185 Enable. See MAST184EN for definition. */
    UINT32 mast186dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 186 Destination. See MAST184DEST for
                               definition.
                            */
    UINT32 mast186en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 186 Enable. See MAST184EN for definition. */
    UINT32 mast187dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 187 Destination. See MAST184DEST for
                               definition.
                            */
    UINT32 mast187en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 187 Enable. See MAST184EN for definition. */
    UINT32 mast188dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 188 Destination. See MAST184DEST for
                               definition.
                            */
    UINT32 mast188en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 188 Enable. See MAST184EN for definition. */
    UINT32 mast189dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 189 Destination. See MAST184DEST for
                               definition.
                            */
    UINT32 mast189en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 189 Enable. See MAST184EN for definition. */
    UINT32 mast190dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 190 Destination. See MAST184DEST for
                               definition.
                            */
    UINT32 mast190en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 190 Enable. See MAST184EN for definition. */
    UINT32 mast191dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 191 Destination. See MAST184DEST for
                               definition.
                            */
    UINT32 mast191en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 191 Enable. See MAST184EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_23_NPK_TPH_STRUCT;

/* SWDEST_24_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000068)                                                      */
/*      SPRB0 (0x20000068)                                                      */
/*      SPRHBM (0x20000068)                                                     */
/*      SPRC0 (0x20000068)                                                      */
/*      SPRMCC (0x20000068)                                                     */
/*      SPRUCC (0x20000068)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [24]
*/


#define SWDEST_24_NPK_TPH_REG 0x1E120068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast192dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 192 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 192 trace data.
                               0x0: Master 192 is routed to Output Port 0 0x1:
                               Master 192 is routed to Output Port 1 & 0x7:
                               Master 192 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast192en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 192 Enable. Enables tracing for Master
                               192. 0: tracing for Master 192 is disabled. All
                               data received from Master 192 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 192 is enabled (Default)
                            */
    UINT32 mast193dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 193 Destination. See MAST192DEST for
                               definition.
                            */
    UINT32 mast193en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 193 Enable. See MAST192EN for definition. */
    UINT32 mast194dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 194 Destination. See MAST192DEST for
                               definition.
                            */
    UINT32 mast194en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 194 Enable. See MAST192EN for definition. */
    UINT32 mast195dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 195 Destination. See MAST192DEST for
                               definition.
                            */
    UINT32 mast195en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 195 Enable. See MAST192EN for definition. */
    UINT32 mast196dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 196 Destination. See MAST192DEST for
                               definition.
                            */
    UINT32 mast196en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 196 Enable. See MAST192EN for definition. */
    UINT32 mast197dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 197 Destination. See MAST192DEST for
                               definition.
                            */
    UINT32 mast197en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 197 Enable. See MAST192EN for definition. */
    UINT32 mast198dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 198 Destination. See MAST192DEST for
                               definition.
                            */
    UINT32 mast198en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 198 Enable. See MAST192EN for definition. */
    UINT32 mast199dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 199 Destination. See MAST192DEST for
                               definition.
                            */
    UINT32 mast199en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 199 Enable. See MAST192EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_24_NPK_TPH_STRUCT;

/* SWDEST_25_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x2000006c)                                                      */
/*      SPRB0 (0x2000006c)                                                      */
/*      SPRHBM (0x2000006c)                                                     */
/*      SPRC0 (0x2000006c)                                                      */
/*      SPRMCC (0x2000006c)                                                     */
/*      SPRUCC (0x2000006c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [25]
*/


#define SWDEST_25_NPK_TPH_REG 0x1E12006C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast200dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 200 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 200 trace data.
                               0x0: Master 200 is routed to Output Port 0 0x1:
                               Master 200 is routed to Output Port 1 & 0x7:
                               Master 200 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast200en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 200 Enable. Enables tracing for Master
                               200. 0: tracing for Master 200 is disabled. All
                               data received from Master 200 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 200 is enabled (Default)
                            */
    UINT32 mast201dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 201 Destination. See MAST200DEST for
                               definition.
                            */
    UINT32 mast201en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 201 Enable. See MAST200EN for definition. */
    UINT32 mast202dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 202 Destination. See MAST200DEST for
                               definition.
                            */
    UINT32 mast202en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 202 Enable. See MAST200EN for definition. */
    UINT32 mast203dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 203 Destination. See MAST200DEST for
                               definition.
                            */
    UINT32 mast203en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 203 Enable. See MAST200EN for definition. */
    UINT32 mast204dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 204 Destination. See MAST200DEST for
                               definition.
                            */
    UINT32 mast204en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 204 Enable. See MAST200EN for definition. */
    UINT32 mast205dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 205 Destination. See MAST200DEST for
                               definition.
                            */
    UINT32 mast205en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 205 Enable. See MAST200EN for definition. */
    UINT32 mast206dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 206 Destination. See MAST200DEST for
                               definition.
                            */
    UINT32 mast206en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 206 Enable. See MAST200EN for definition. */
    UINT32 mast207dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 207 Destination. See MAST200DEST for
                               definition.
                            */
    UINT32 mast207en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 207 Enable. See MAST200EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_25_NPK_TPH_STRUCT;

/* SWDEST_26_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000070)                                                      */
/*      SPRB0 (0x20000070)                                                      */
/*      SPRHBM (0x20000070)                                                     */
/*      SPRC0 (0x20000070)                                                      */
/*      SPRMCC (0x20000070)                                                     */
/*      SPRUCC (0x20000070)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [26]
*/


#define SWDEST_26_NPK_TPH_REG 0x1E120070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast208dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 208 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 208 trace data.
                               0x0: Master 208 is routed to Output Port 0 0x1:
                               Master 208 is routed to Output Port 1 & 0x7:
                               Master 208 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast208en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 208 Enable. Enables tracing for Master
                               208. 0: tracing for Master 208 is disabled. All
                               data received from Master 208 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 208 is enabled (Default)
                            */
    UINT32 mast209dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 209 Destination. See MAST208DEST for
                               definition.
                            */
    UINT32 mast209en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 209 Enable. See MAST208EN for definition. */
    UINT32 mast210dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 210 Destination. See MAST208DEST for
                               definition.
                            */
    UINT32 mast210en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 210 Enable. See MAST208EN for definition. */
    UINT32 mast211dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 211 Destination. See MAST208DEST for
                               definition.
                            */
    UINT32 mast211en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 211 Enable. See MAST208EN for definition. */
    UINT32 mast212dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 212 Destination. See MAST208DEST for
                               definition.
                            */
    UINT32 mast212en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 212 Enable. See MAST208EN for definition. */
    UINT32 mast213dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 213 Destination. See MAST208DEST for
                               definition.
                            */
    UINT32 mast213en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 213 Enable. See MAST208EN for definition. */
    UINT32 mast214dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 214 Destination. See MAST208DEST for
                               definition.
                            */
    UINT32 mast214en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 214 Enable. See MAST208EN for definition. */
    UINT32 mast215dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 215 Destination. See MAST208DEST for
                               definition.
                            */
    UINT32 mast215en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 215 Enable. See MAST208EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_26_NPK_TPH_STRUCT;

/* SWDEST_27_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000074)                                                      */
/*      SPRB0 (0x20000074)                                                      */
/*      SPRHBM (0x20000074)                                                     */
/*      SPRC0 (0x20000074)                                                      */
/*      SPRMCC (0x20000074)                                                     */
/*      SPRUCC (0x20000074)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [27]
*/


#define SWDEST_27_NPK_TPH_REG 0x1E120074

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast216dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 216 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 216 trace data.
                               0x0: Master 216 is routed to Output Port 0 0x1:
                               Master 216 is routed to Output Port 1 & 0x7:
                               Master 216 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast216en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 216 Enable. Enables tracing for Master
                               216. 0: tracing for Master 216 is disabled. All
                               data received from Master 216 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 216 is enabled (Default)
                            */
    UINT32 mast217dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 217 Destination. See MAST216DEST for
                               definition.
                            */
    UINT32 mast217en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 217 Enable. See MAST216EN for definition. */
    UINT32 mast218dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 218 Destination. See MAST216DEST for
                               definition.
                            */
    UINT32 mast218en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 218 Enable. See MAST216EN for definition. */
    UINT32 mast219dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 219 Destination. See MAST216DEST for
                               definition.
                            */
    UINT32 mast219en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 219 Enable. See MAST216EN for definition. */
    UINT32 mast220dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 220 Destination. See MAST216DEST for
                               definition.
                            */
    UINT32 mast220en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 220 Enable. See MAST216EN for definition. */
    UINT32 mast221dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 221 Destination. See MAST216DEST for
                               definition.
                            */
    UINT32 mast221en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 221 Enable. See MAST216EN for definition. */
    UINT32 mast222dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 222 Destination. See MAST216DEST for
                               definition.
                            */
    UINT32 mast222en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 222 Enable. See MAST216EN for definition. */
    UINT32 mast223dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 223 Destination. See MAST216DEST for
                               definition.
                            */
    UINT32 mast223en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 223 Enable. See MAST216EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_27_NPK_TPH_STRUCT;

/* SWDEST_28_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000078)                                                      */
/*      SPRB0 (0x20000078)                                                      */
/*      SPRHBM (0x20000078)                                                     */
/*      SPRC0 (0x20000078)                                                      */
/*      SPRMCC (0x20000078)                                                     */
/*      SPRUCC (0x20000078)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [28]
*/


#define SWDEST_28_NPK_TPH_REG 0x1E120078

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast224dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 224 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 224 trace data.
                               0x0: Master 224 is routed to Output Port 0 0x1:
                               Master 224 is routed to Output Port 1 & 0x7:
                               Master 224 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast224en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 224 Enable. Enables tracing for Master
                               224. 0: tracing for Master 224 is disabled. All
                               data received from Master 224 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 224 is enabled (Default)
                            */
    UINT32 mast225dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 225 Destination. See MAST224DEST for
                               definition.
                            */
    UINT32 mast225en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 225 Enable. See MAST224EN for definition. */
    UINT32 mast226dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 226 Destination. See MAST224DEST for
                               definition.
                            */
    UINT32 mast226en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 226 Enable. See MAST224EN for definition. */
    UINT32 mast227dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 227 Destination. See MAST224DEST for
                               definition.
                            */
    UINT32 mast227en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 227 Enable. See MAST224EN for definition. */
    UINT32 mast228dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 228 Destination. See MAST224DEST for
                               definition.
                            */
    UINT32 mast228en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 228 Enable. See MAST224EN for definition. */
    UINT32 mast229dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 229 Destination. See MAST224DEST for
                               definition.
                            */
    UINT32 mast229en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 229 Enable. See MAST224EN for definition. */
    UINT32 mast230dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 230 Destination. See MAST224DEST for
                               definition.
                            */
    UINT32 mast230en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 230 Enable. See MAST224EN for definition. */
    UINT32 mast231dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 231 Destination. See MAST224DEST for
                               definition.
                            */
    UINT32 mast231en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 231 Enable. See MAST224EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_28_NPK_TPH_STRUCT;

/* SWDEST_29_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x2000007c)                                                      */
/*      SPRB0 (0x2000007c)                                                      */
/*      SPRHBM (0x2000007c)                                                     */
/*      SPRC0 (0x2000007c)                                                      */
/*      SPRMCC (0x2000007c)                                                     */
/*      SPRUCC (0x2000007c)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [29]
*/


#define SWDEST_29_NPK_TPH_REG 0x1E12007C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast232dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 232 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 232 trace data.
                               0x0: Master 232 is routed to Output Port 0 0x1:
                               Master 232 is routed to Output Port 1 & 0x7:
                               Master 232 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast232en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 232 Enable. Enables tracing for Master
                               232. 0: tracing for Master 232 is disabled. All
                               data received from Master 232 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 232 is enabled (Default)
                            */
    UINT32 mast233dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 233 Destination. See MAST232DEST for
                               definition.
                            */
    UINT32 mast233en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 233 Enable. See MAST232EN for definition. */
    UINT32 mast234dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 234 Destination. See MAST232DEST for
                               definition.
                            */
    UINT32 mast234en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 234 Enable. See MAST232EN for definition. */
    UINT32 mast235dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 235 Destination. See MAST232DEST for
                               definition.
                            */
    UINT32 mast235en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 235 Enable. See MAST232EN for definition. */
    UINT32 mast236dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 236 Destination. See MAST232DEST for
                               definition.
                            */
    UINT32 mast236en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 236 Enable. See MAST232EN for definition. */
    UINT32 mast237dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 237 Destination. See MAST232DEST for
                               definition.
                            */
    UINT32 mast237en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 237 Enable. See MAST232EN for definition. */
    UINT32 mast238dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 238 Destination. See MAST232DEST for
                               definition.
                            */
    UINT32 mast238en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 238 Enable. See MAST232EN for definition. */
    UINT32 mast239dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 239 Destination. See MAST232DEST for
                               definition.
                            */
    UINT32 mast239en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 239 Enable. See MAST232EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_29_NPK_TPH_STRUCT;

/* SWDEST_30_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000080)                                                      */
/*      SPRB0 (0x20000080)                                                      */
/*      SPRHBM (0x20000080)                                                     */
/*      SPRC0 (0x20000080)                                                      */
/*      SPRMCC (0x20000080)                                                     */
/*      SPRUCC (0x20000080)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [30]
*/


#define SWDEST_30_NPK_TPH_REG 0x1E120080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast240dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 240 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 240 trace data.
                               0x0: Master 240 is routed to Output Port 0 0x1:
                               Master 240 is routed to Output Port 1 & 0x7:
                               Master 240 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast240en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 240 Enable. Enables tracing for Master
                               240. 0: tracing for Master 240 is disabled. All
                               data received from Master 240 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 240 is enabled (Default)
                            */
    UINT32 mast241dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 241 Destination. See MAST240DEST for
                               definition.
                            */
    UINT32 mast241en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 241 Enable. See MAST240EN for definition. */
    UINT32 mast242dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 242 Destination. See MAST240DEST for
                               definition.
                            */
    UINT32 mast242en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 242 Enable. See MAST240EN for definition. */
    UINT32 mast243dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 243 Destination. See MAST240DEST for
                               definition.
                            */
    UINT32 mast243en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 243 Enable. See MAST240EN for definition. */
    UINT32 mast244dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 244 Destination. See MAST240DEST for
                               definition.
                            */
    UINT32 mast244en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 244 Enable. See MAST240EN for definition. */
    UINT32 mast245dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 245 Destination. See MAST240DEST for
                               definition.
                            */
    UINT32 mast245en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 245 Enable. See MAST240EN for definition. */
    UINT32 mast246dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 246 Destination. See MAST240DEST for
                               definition.
                            */
    UINT32 mast246en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 246 Enable. See MAST240EN for definition. */
    UINT32 mast247dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 247 Destination. See MAST240DEST for
                               definition.
                            */
    UINT32 mast247en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 247 Enable. See MAST240EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_30_NPK_TPH_STRUCT;

/* SWDEST_31_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20000084)                                                      */
/*      SPRB0 (0x20000084)                                                      */
/*      SPRHBM (0x20000084)                                                     */
/*      SPRC0 (0x20000084)                                                      */
/*      SPRMCC (0x20000084)                                                     */
/*      SPRUCC (0x20000084)                                                     */
/* Register default value on SPRA0: 0x88888888                                  */
/* Register default value on SPRB0: 0x88888888                                  */
/* Register default value on SPRHBM: 0x88888888                                 */
/* Register default value on SPRC0: 0x88888888                                  */
/* Register default value on SPRMCC: 0x88888888                                 */
/* Register default value on SPRUCC: 0x88888888                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Switching Destination [31]
*/


#define SWDEST_31_NPK_TPH_REG 0x1E120084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast248dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 248 Destination. Specifies the
                               destination port number (refer to GTHOPT
                               register for details) for Master 248 trace data.
                               0x0: Master 248 is routed to Output Port 0 0x1:
                               Master 248 is routed to Output Port 1 & 0x7:
                               Master 248 is routed to Output Port 0x7 The
                               default value of this bit field is controlled by
                               the corresponding MDEST[N] parameter.
                            */
    UINT32 mast248en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 248 Enable. Enables tracing for Master
                               248. 0: tracing for Master 248 is disabled. All
                               data received from Master 248 is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Master 248 is enabled (Default)
                            */
    UINT32 mast249dest : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Master 249 Destination. See MAST248DEST for
                               definition.
                            */
    UINT32 mast249en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Master 249 Enable. See MAST248EN for definition. */
    UINT32 mast250dest : 3;

                            /* Bits[10:8], Access Type=RW, default=0x00000000*/

                            /*
                               Master 250 Destination. See MAST248DEST for
                               definition.
                            */
    UINT32 mast250en : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Master 250 Enable. See MAST248EN for definition. */
    UINT32 mast251dest : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /*
                               Master 251 Destination. See MAST248DEST for
                               definition.
                            */
    UINT32 mast251en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /* Master 251 Enable. See MAST248EN for definition. */
    UINT32 mast252dest : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Master 252 Destination. See MAST248DEST for
                               definition.
                            */
    UINT32 mast252en : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /* Master 252 Enable. See MAST248EN for definition. */
    UINT32 mast253dest : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Master 253 Destination. See MAST248DEST for
                               definition.
                            */
    UINT32 mast253en : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Master 253 Enable. See MAST248EN for definition. */
    UINT32 mast254dest : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               Master 254 Destination. See MAST248DEST for
                               definition.
                            */
    UINT32 mast254en : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /* Master 254 Enable. See MAST248EN for definition. */
    UINT32 mast255dest : 3;

                            /* Bits[30:28], Access Type=RW, default=0x00000000*/

                            /*
                               Master 255 Destination. See MAST248DEST for
                               definition.
                            */
    UINT32 mast255en : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Master 255 Enable. See MAST248EN for definition. */

  } Bits;
  UINT32 Data;

} SWDEST_31_NPK_TPH_STRUCT;

/* GSWDEST_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20000088)                                                      */
/*      SPRB0 (0x20000088)                                                      */
/*      SPRHBM (0x20000088)                                                     */
/*      SPRC0 (0x20000088)                                                      */
/*      SPRMCC (0x20000088)                                                     */
/*      SPRUCC (0x20000088)                                                     */
/* Register default value on SPRA0: 0x00000008                                  */
/* Register default value on SPRB0: 0x00000008                                  */
/* Register default value on SPRHBM: 0x00000008                                 */
/* Register default value on SPRC0: 0x00000008                                  */
/* Register default value on SPRMCC: 0x00000008                                 */
/* Register default value on SPRUCC: 0x00000008                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* General Software Trace Destination
*/


#define GSWDEST_NPK_TPH_REG 0x1E120088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mast256dest : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Master 256 (and higher) Destination. Specifies
                               the destination port number (refer to GTHOPT
                               register for details) for trace data from
                               Masters numbered 256 and higher. 0x0: Trace data
                               is routed to Output Port 0 0x1: Trace data is
                               routed to Output Port 1 & 0x7: Trace data is
                               routed to Output Port 0x7
                            */
    UINT32 mast256en : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Master 256 (and higher) Enable. Enables tracing
                               for Masters numbered 256 and higher. 0: tracing
                               for Masters 256 and higher is disabled. All data
                               received from these masters is received at the
                               Input Buffer and immediately dropped. 1: tracing
                               for Masters 256 and higher is enabled (Default)
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} GSWDEST_NPK_TPH_STRUCT;

/* LWM0_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x2000008c)                                                      */
/*      SPRB0 (0x2000008c)                                                      */
/*      SPRHBM (0x2000008c)                                                     */
/*      SPRC0 (0x2000008c)                                                      */
/*      SPRMCC (0x2000008c)                                                     */
/*      SPRUCC (0x2000008c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Low WaterMark for Sources 0-7
*/


#define LWM0_NPK_TPH_REG 0x1E12008C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lwm0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Low Water Mark for Trace Source #0. Indicates
                               the maximum level to which the input buffer for
                               trace source #0 can be filled before the
                               lowWaterMark signal will be de-asserted. That
                               is, the lowWaterMark signal is asserted when the
                               input buffer has LWM0 entries or less.
                               Conversely, when greater than LWM0 entries are
                               filled, lowWaterMark is deasserted. With a
                               default value of 0, the lowWaterMark signal
                               serves as an empty signal. In practice the LWM
                               value for a trace source should be set to the
                               grant duration minus 1 (GrantDur-1).
                            */
    UINT32 lwm1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #1. See LWM0. */
    UINT32 lwm2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #2. See LWM0. */
    UINT32 lwm3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #3. See LWM0. */
    UINT32 lwm4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #4. See LWM0. */
    UINT32 lwm5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #5. See LWM0. */
    UINT32 lwm6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #6. See LWM0. */
    UINT32 lwm7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #7. See LWM0. */

  } Bits;
  UINT32 Data;

} LWM0_NPK_TPH_STRUCT;

/* LWM1_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20000090)                                                      */
/*      SPRB0 (0x20000090)                                                      */
/*      SPRHBM (0x20000090)                                                     */
/*      SPRC0 (0x20000090)                                                      */
/*      SPRMCC (0x20000090)                                                     */
/*      SPRUCC (0x20000090)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Low WaterMark for Sources 7-15
*/


#define LWM1_NPK_TPH_REG 0x1E120090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lwm8 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #8. See LWM0. */
    UINT32 lwm9 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #9. See LWM0. */
    UINT32 lwm10 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #10. See LWM0. */
    UINT32 lwm11 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #11. See LWM0. */
    UINT32 lwm12 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #12. See LWM0. */
    UINT32 lwm13 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #13. See LWM0. */
    UINT32 lwm14 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #14. See LWM0. */
    UINT32 lwm15 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /* Low Water Mark for Trace Source #15. See LWM0. */

  } Bits;
  UINT32 Data;

} LWM1_NPK_TPH_STRUCT;

/* GTH_INFO_1_NPK_TPH_REG supported on:                                         */
/*      SPRA0 (0x20000094)                                                      */
/*      SPRB0 (0x20000094)                                                      */
/*      SPRHBM (0x20000094)                                                     */
/*      SPRC0 (0x20000094)                                                      */
/*      SPRMCC (0x20000094)                                                     */
/*      SPRUCC (0x20000094)                                                     */
/* Register default value on SPRA0: 0x00000004                                  */
/* Register default value on SPRB0: 0x00000004                                  */
/* Register default value on SPRHBM: 0x00000004                                 */
/* Register default value on SPRC0: 0x00000004                                  */
/* Register default value on SPRMCC: 0x00000004                                 */
/* Register default value on SPRUCC: 0x00000004                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* GTH Parameter Info 1
*/


#define GTH_INFO_1_NPK_TPH_REG 0x1E120094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cts_signal_width : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000004*/

                            /*
                               CTS External Signal Width. Indicates the number
                               of external (external to NPK, not the SoC)
                               signals available to the SOC for its own use
                               (implementation specific use).
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} GTH_INFO_1_NPK_TPH_STRUCT;

/* GTH_MISC_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20000098)                                                      */
/*      SPRB0 (0x20000098)                                                      */
/*      SPRHBM (0x20000098)                                                     */
/*      SPRC0 (0x20000098)                                                      */
/*      SPRMCC (0x20000098)                                                     */
/*      SPRUCC (0x20000098)                                                     */
/* Register default value on SPRA0: 0x00010000                                  */
/* Register default value on SPRB0: 0x00010000                                  */
/* Register default value on SPRHBM: 0x00010000                                 */
/* Register default value on SPRC0: 0x00010000                                  */
/* Register default value on SPRMCC: 0x00010000                                 */
/* Register default value on SPRUCC: 0x00010000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* GTH Miscellaneous Register
*/


#define GTH_MISC_NPK_TPH_REG 0x1E120098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 force_idle : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Force Idle. When set, this bit will cause North
                               Peak to indicate to power management circuitry
                               that it (NPK) is idle, and so can be power
                               gated. This bit is used for low power mode
                               support. See NPK top level HAS for more details.
                            */
    UINT32 rsvd : 7;

                            /* Bits[7:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tuevt6src : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               Trigger Unit Event 6 Source. 000: VER6 001:
                               bpb_data_xfer[0] (MSC0) 010: bpb_data_xfer[1]
                               (MSC1) 011: bpb_data_xfer[2] 100:
                               bpb_data_xfer[3] ... 111: bpb_data_xfer[6] Note
                               that not all values are valid for all
                               implementations. Only values that correspond to
                               the number of trace destinations are valid. All
                               other values will alias.
                            */
    UINT32 tuevt7src : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000000*/

                            /*
                               Trigger Unit Event 7 Source. 000: VER7 001:
                               bpb_data_xfer[0] (MSC0) 010: bpb_data_xfer[1]
                               (MSC1) 011: bpb_data_xfer[2] 100:
                               bpb_data_xfer[3] ... 111: bpb_data_xfer[6] Note
                               that not all values are valid for all
                               implementations. Only values that correspond to
                               the number of trace destinations are valid. All
                               other values will alias.
                            */
    UINT32 stp22en : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               STPv2.2 Enable: Clearing this bit will disable
                               all STP stream features that are specific to
                               MIPI STPv2.2 (including the STPv2.2 VER and DIP
                               packets) and will run North Peak in STPv2.1
                               compatibility mode.
                            */
    UINT32 rsvd_17 : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} GTH_MISC_NPK_TPH_STRUCT;

/* SMCR0_NPK_TPH_REG supported on:                                              */
/*      SPRA0 (0x2000009c)                                                      */
/*      SPRB0 (0x2000009c)                                                      */
/*      SPRHBM (0x2000009c)                                                     */
/*      SPRC0 (0x2000009c)                                                      */
/*      SPRMCC (0x2000009c)                                                     */
/*      SPRUCC (0x2000009c)                                                     */
/* Register default value on SPRA0: 0x80008000                                  */
/* Register default value on SPRB0: 0x80008000                                  */
/* Register default value on SPRHBM: 0x80008000                                 */
/* Register default value on SPRC0: 0x80008000                                  */
/* Register default value on SPRMCC: 0x80008000                                 */
/* Register default value on SPRUCC: 0x80008000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* STP Maintenance Control Register 0
*/


#define SMCR0_NPK_TPH_REG 0x1E12009C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 syncf0 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00008000*/

                            /*
                               Sync Packet Frequency for Port 0. Specifies the
                               number of data sets between Maintenance packets
                               for port 0. A value of 0 (zero) turns off
                               Maintenance packet generation for the port.
                               However, an initial maintenance packet will be
                               sent at the start of the trace so that
                               reconstruction software can successfully
                               reconstruct the trace data.
                            */
    UINT32 syncf1 : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00008000*/

                            /*
                               Sync Packet Frequency for Port 1. Specifies the
                               number data sets between Maintenance packets for
                               port 1. A value of 0 (zero) turns off
                               Maintenance packet generation for the port.
                               However, an initial maintenance packet will be
                               sent at the start of the trace so that
                               reconstruction software can successfully
                               reconstruct the trace data.
                            */

  } Bits;
  UINT32 Data;

} SMCR0_NPK_TPH_STRUCT;

/* SMCR1_NPK_TPH_REG supported on:                                              */
/*      SPRA0 (0x200000a0)                                                      */
/*      SPRB0 (0x200000a0)                                                      */
/*      SPRHBM (0x200000a0)                                                     */
/*      SPRC0 (0x200000a0)                                                      */
/*      SPRMCC (0x200000a0)                                                     */
/*      SPRUCC (0x200000a0)                                                     */
/* Register default value on SPRA0: 0x80008000                                  */
/* Register default value on SPRB0: 0x80008000                                  */
/* Register default value on SPRHBM: 0x80008000                                 */
/* Register default value on SPRC0: 0x80008000                                  */
/* Register default value on SPRMCC: 0x80008000                                 */
/* Register default value on SPRUCC: 0x80008000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* STP Maintenance Control Register 1
*/


#define SMCR1_NPK_TPH_REG 0x1E1200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 syncf2 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00008000*/

                            /*
                               Sync Packet Frequency for Port 2. Specifies the
                               number of data sets between Maintenance packets
                               for port 2. A value of 0 (zero) turns off
                               Maintenance packet generation for the port.
                               However, an initial maintenance packet will be
                               sent at the start of the trace so that
                               reconstruction software can successfully
                               reconstruct the trace data.
                            */
    UINT32 syncf3 : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00008000*/

                            /*
                               Sync Packet Frequency for Port 3. Specifies the
                               number of data sets between Maintenance packets
                               for port 3. A value of 0 (zero) turns off
                               Maintenance packet generation for the port.
                               However, an initial maintenance packet will be
                               sent at the start of the trace so that
                               reconstruction software can successfully
                               reconstruct the trace data.
                            */

  } Bits;
  UINT32 Data;

} SMCR1_NPK_TPH_STRUCT;

/* SMCR2_NPK_TPH_REG supported on:                                              */
/*      SPRA0 (0x200000a4)                                                      */
/*      SPRB0 (0x200000a4)                                                      */
/*      SPRHBM (0x200000a4)                                                     */
/*      SPRC0 (0x200000a4)                                                      */
/*      SPRMCC (0x200000a4)                                                     */
/*      SPRUCC (0x200000a4)                                                     */
/* Register default value on SPRA0: 0x80008000                                  */
/* Register default value on SPRB0: 0x80008000                                  */
/* Register default value on SPRHBM: 0x80008000                                 */
/* Register default value on SPRC0: 0x80008000                                  */
/* Register default value on SPRMCC: 0x80008000                                 */
/* Register default value on SPRUCC: 0x80008000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* STP Maintenance Control Register 2
*/


#define SMCR2_NPK_TPH_REG 0x1E1200A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 syncf4 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00008000*/

                            /*
                               Sync Packet Frequency for Port 4. Specifies the
                               number of data sets between Maintenance packets
                               for port 4. A value of 0 (zero) turns off
                               Maintenance packet generation for the port.
                               However, an initial maintenance packet will be
                               sent at the start of the trace so that
                               reconstruction software can successfully
                               reconstruct the trace data.
                            */
    UINT32 syncf5 : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00008000*/

                            /*
                               Sync Packet Frequency for Port 5. Specifies the
                               number of data sets between Maintenence packets
                               for port 5. A value of 0 (zero) turns off
                               Maintenance packet generation for the port.
                               However, an initial maintenance packet will be
                               sent at the start of the trace so that
                               reconstruction software can successfully
                               reconstruct the trace data.
                            */

  } Bits;
  UINT32 Data;

} SMCR2_NPK_TPH_STRUCT;

/* SMCR3_NPK_TPH_REG supported on:                                              */
/*      SPRA0 (0x200000a8)                                                      */
/*      SPRB0 (0x200000a8)                                                      */
/*      SPRHBM (0x200000a8)                                                     */
/*      SPRC0 (0x200000a8)                                                      */
/*      SPRMCC (0x200000a8)                                                     */
/*      SPRUCC (0x200000a8)                                                     */
/* Register default value on SPRA0: 0x80008000                                  */
/* Register default value on SPRB0: 0x80008000                                  */
/* Register default value on SPRHBM: 0x80008000                                 */
/* Register default value on SPRC0: 0x80008000                                  */
/* Register default value on SPRMCC: 0x80008000                                 */
/* Register default value on SPRUCC: 0x80008000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* STP Maintenance Control Register 3
*/


#define SMCR3_NPK_TPH_REG 0x1E1200A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 syncf6 : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00008000*/

                            /*
                               Sync Packet Frequency for Port 6. Specifies the
                               number of data sets between Maintenance packets
                               for port 4. A value of 0 (zero) turns off
                               Maintenance packet generation for the port.
                               However, an initial maintenance packet will be
                               sent at the start of the trace so that
                               reconstruction software can successfully
                               reconstruct the trace data.
                            */
    UINT32 syncf7 : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00008000*/

                            /*
                               Sync Packet Frequency for Port 7. Specifies the
                               number of data sets between Maintenance packets
                               for port 5. A value of 0 (zero) turns off
                               Maintenance packet generation for the port.
                               However, an initial maintenance packet will be
                               sent at the start of the trace so that
                               reconstruction software can successfully
                               reconstruct the trace data.
                            */

  } Bits;
  UINT32 Data;

} SMCR3_NPK_TPH_STRUCT;

/* PGD0_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200000ac)                                                      */
/*      SPRB0 (0x200000ac)                                                      */
/*      SPRHBM (0x200000ac)                                                     */
/*      SPRC0 (0x200000ac)                                                      */
/*      SPRMCC (0x200000ac)                                                     */
/*      SPRUCC (0x200000ac)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Programmable Grant Duration Register 0
*/


#define PGD0_NPK_TPH_REG 0x1E1200AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntdur_0 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 0.
                               The value in this register specifies the maximum
                               number of consecutive grants (or gets) that can
                               be given to GTH Input Port 0 (SMU).
                            */
    UINT32 gntdur_1 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 1.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 2
                            */
    UINT32 gntdur_2 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 2.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 3
                            */
    UINT32 gntdur_3 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 3.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 4
                            */
    UINT32 gntdur_4 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 4.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 5
                            */
    UINT32 gntdur_5 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 5.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 6
                            */
    UINT32 gntdur_6 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 6.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 7
                            */
    UINT32 gntdur_7 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 7.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 8
                            */

  } Bits;
  UINT32 Data;

} PGD0_NPK_TPH_STRUCT;

/* PGD1_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200000b0)                                                      */
/*      SPRB0 (0x200000b0)                                                      */
/*      SPRHBM (0x200000b0)                                                     */
/*      SPRC0 (0x200000b0)                                                      */
/*      SPRMCC (0x200000b0)                                                     */
/*      SPRUCC (0x200000b0)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Programmable Grant Duration Register 1
*/


#define PGD1_NPK_TPH_REG 0x1E1200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gntdur_8 : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 8.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 9.
                            */
    UINT32 gntdur_9 : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 9.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 10.
                            */
    UINT32 gntdur_10 : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 10.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 11.
                            */
    UINT32 gntdur_11 : 4;

                            /* Bits[15:12], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 11.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 12.
                            */
    UINT32 gntdur_12 : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 12.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 13.
                            */
    UINT32 gntdur_13 : 4;

                            /* Bits[23:20], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 13.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 14.
                            */
    UINT32 gntdur_14 : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 14.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 15.
                            */
    UINT32 gntdur_15 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000001*/

                            /*
                               Programmable Grant Duration for Input Port 15.
                               See definition for GNTDUR_0 for details. These
                               bits are read-only as 0h if NUMTSOURCE is less
                               than 16.
                            */

  } Bits;
  UINT32 Data;

} PGD1_NPK_TPH_STRUCT;

/* SCR_NPK_TPH_REG supported on:                                                */
/*      SPRA0 (0x200000c8)                                                      */
/*      SPRB0 (0x200000c8)                                                      */
/*      SPRHBM (0x200000c8)                                                     */
/*      SPRC0 (0x200000c8)                                                      */
/*      SPRMCC (0x200000c8)                                                     */
/*      SPRUCC (0x200000c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Source Control Register
*/


#define SCR_NPK_TPH_REG 0x1E1200C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 stm0 : 1;

                            /* Bits[0:0], Access Type=RSV, default=0x00000000*/

                            /*
                               Storage Mode control bit for Source 0. 0: Normal
                               mode. Trace source is stored in single logical
                               buffer. 1: Multi-buffer. Trace source is stored
                               in separate logical buffers with wrapping around
                               trigger assertion
                            */
    UINT32 stm1 : 1;

                            /* Bits[1:1], Access Type=RSV, default=0x00000000*/

                            /*
                               Storage Mode control bit for Source 1. See STM0
                               for details.
                            */
    UINT32 stm2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Storage Mode control bit for Source 2. See STM0
                               for details.
                            */
    UINT32 stm3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Storage Mode control bit for Source 3. See STM0
                               for details.
                            */
    UINT32 stm4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Storage Mode control bit for Source 4. See STM0
                               for details.
                            */
    UINT32 stm5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Storage Mode control bit for Source 5. See STM0
                               for details.
                            */
    UINT32 stm6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Storage Mode control bit for Source 6. See STM0
                               for details.
                            */
    UINT32 stm7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Storage Mode control bit for Source 7. See STM0
                               for details.
                            */
    UINT32 rsvd : 10;

                            /* Bits[17:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 storeenovrd2 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Storage Enable Override 2. Under normal
                               operation the CTS gasket controls the
                               StoreEn[N:0] signals to the trace sources. This
                               Storage Enable Override configuration bit is a
                               method to force the StoreEn[N:0] signals without
                               requiring programming of the CTS or the gasket.
                               The StoreEnOvrd is logically ORd together with
                               the StoreEn from the gasket before being sent to
                               the trace sources.
                            */
    UINT32 storeenovrd3 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Storage Enable Override 3. See StoreEnOvrd2 for
                               details.
                            */
    UINT32 storeenovrd4 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Storage Enable Override 4. See StoreEnOvrd2 for
                               details.
                            */
    UINT32 storeenovrd5 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Storage Enable Override 5. See StoreEnOvrd2 for
                               details.
                            */
    UINT32 storeenovrd6 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Storage Enable Override 6. See StoreEnOvrd2 for
                               details.
                            */
    UINT32 storeenovrd7 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Storage Enable Override 7. See StoreEnOvrd2 for
                               details.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SCR_NPK_TPH_STRUCT;

/* TSCU_FREQ_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x200000cc)                                                      */
/*      SPRB0 (0x200000cc)                                                      */
/*      SPRHBM (0x200000cc)                                                     */
/*      SPRC0 (0x200000cc)                                                      */
/*      SPRMCC (0x200000cc)                                                     */
/*      SPRUCC (0x200000cc)                                                     */
/* Register default value on SPRA0: 0xB71B0000                                  */
/* Register default value on SPRB0: 0xB71B0000                                  */
/* Register default value on SPRHBM: 0xB71B0000                                 */
/* Register default value on SPRC0: 0xB71B0000                                  */
/* Register default value on SPRMCC: 0xB71B0000                                 */
/* Register default value on SPRUCC: 0xB71B0000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* TSCU Frequency Register
*/


#define TSCU_FREQ_NPK_TPH_REG 0x1E1200CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tscu_freq : 32;

                            /* Bits[31:0], Access Type=RW, default=0xB71B0000*/

                            /*
                               TSCU Operating Frequency. Specifies the apparent
                               operating frequency of the GTH, in Hertz. This
                               is also clock with which timestamps are
                               generated.
                            */

  } Bits;
  UINT32 Data;

} TSCU_FREQ_NPK_TPH_STRUCT;

/* BPB_FRAME_SIZE_NPK_TPH_REG supported on:                                     */
/*      SPRA0 (0x200000d0)                                                      */
/*      SPRB0 (0x200000d0)                                                      */
/*      SPRHBM (0x200000d0)                                                     */
/*      SPRC0 (0x200000d0)                                                      */
/*      SPRMCC (0x200000d0)                                                     */
/*      SPRUCC (0x200000d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Byte Packing Buffer max frame size
*/


#define BPB_FRAME_SIZE_NPK_TPH_REG 0x1E1200D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 frame_sz0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               BPB Max Frame Size for output port 0. Specifies
                               the maximum frame size for output port 0, in
                               terms of number of BPB rows. To determine number
                               bytes, multiply by the width of the BPB. 0:
                               Normal Mode (default) 1-255: Framing Mode.
                            */
    UINT32 frame_sz1 : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /*
                               BPB Max Frame Size for output port 1. Specifies
                               the maximum frame size for output port 0, in
                               terms of number of BPB rows. To determine number
                               bytes, multiply by the width of the BPB. 0:
                               Normal Mode (default) 1-255: Framing Mode.
                            */
    UINT32 frame_sz2 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /*
                               BPB Max Frame Size for output port 2. Specifies
                               the maximum frame size for output port 0, in
                               terms of number of BPB rows. To determine number
                               bytes, multiply by the width of the BPB. 0:
                               Normal Mode (default) 1-255: Framing Mode.
                            */
    UINT32 frame_sz3 : 8;

                            /* Bits[31:24], Access Type=RW, default=0x00000000*/

                            /*
                               BPB Max Frame Size for output port 3. Specifies
                               the maximum frame size for output port 0, in
                               terms of number of BPB rows. To determine number
                               bytes, multiply by the width of the BPB. 0:
                               Normal Mode (default) 1-255: Framing Mode.
                            */

  } Bits;
  UINT32 Data;

} BPB_FRAME_SIZE_NPK_TPH_STRUCT;

/* GTHSTAT_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x200000d4)                                                      */
/*      SPRB0 (0x200000d4)                                                      */
/*      SPRHBM (0x200000d4)                                                     */
/*      SPRC0 (0x200000d4)                                                      */
/*      SPRMCC (0x200000d4)                                                     */
/*      SPRUCC (0x200000d4)                                                     */
/* Register default value on SPRA0: 0x0000FFFF                                  */
/* Register default value on SPRB0: 0x0000FFFF                                  */
/* Register default value on SPRHBM: 0x0000FFFF                                 */
/* Register default value on SPRC0: 0x0000FFFF                                  */
/* Register default value on SPRMCC: 0x0000FFFF                                 */
/* Register default value on SPRUCC: 0x0000FFFF                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* GTH Status Register
*/


#define GTHSTAT_NPK_TPH_REG 0x1E1200D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ple0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /*
                               Pipe Line Empty, Output port 0. Indicates that
                               the data pipeline through GTH, from Input Buffer
                               to Byte Packing Buffer, is empty. Software can
                               read this bit to determine if the pipeline is
                               empty. Along with an equivalent bit from the
                               output port controller (e.g., MSU, PTI, HTI),
                               software can determine if all the data has been
                               sent to the destination, and processing of the
                               data can begin.
                            */
    UINT32 ple1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Pipe Line Empty, Output port 1. See PLE0 for
                               details
                            */
    UINT32 ple2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000001*/

                            /*
                               Pipe Line Empty, Output port 2. See PLE0 for
                               details.
                            */
    UINT32 ple3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Pipe Line Empty, Output port 3. See PLE0 for
                               details.
                            */
    UINT32 ple4 : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Pipe Line Empty, Output port 4. See PLE0 for
                               details.
                            */
    UINT32 ple5 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Pipe Line Empty, Output port 5. See PLE0 for
                               details.
                            */
    UINT32 ple6 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000001*/

                            /*
                               Pipe Line Empty, Output port 6. See PLE0 for
                               details.
                            */
    UINT32 ple7 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               Pipe Line Empty, Output port 7. See PLE0 for
                               details.
                            */
    UINT32 ibe0 : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000001*/

                            /*
                               Input Buffer Empty, Trace Source 0. Indicates
                               that the input buffer for trace source 0 is
                               empty, and has absolutely no data in it. This
                               information will be used to determine whether or
                               not the trace sources on VNN (e.g., STH, ODLA)
                               are inactive and drained of data before
                               switching VNN power off.
                            */
    UINT32 ibe1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Input Buffer Empty, Trace Source 1. See IBE0 for
                               details.
                            */
    UINT32 ibe2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Input Buffer Empty, Trace Source 2. See IBE0 for
                               details.
                            */
    UINT32 ibe3 : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000001*/

                            /*
                               Input Buffer Empty, Trace Source 3. See IBE0 for
                               details.
                            */
    UINT32 ibe4 : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000001*/

                            /*
                               Input Buffer Empty, Trace Source 4. See IBE0 for
                               details.
                            */
    UINT32 ibe5 : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000001*/

                            /*
                               Input Buffer Empty, Trace Source 5. See IBE0 for
                               details.
                            */
    UINT32 ibe6 : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000001*/

                            /*
                               Input Buffer Empty, Trace Source 6. See IBE0 for
                               details.
                            */
    UINT32 ibe7 : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /*
                               Input Buffer Empty, Trace Source 7. See IBE0 for
                               details.
                            */
    UINT32 rsvd : 2;

                            /* Bits[17:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tuse2 : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               TU Store Enable, Trace Source 2. Indicates
                               whether the trigger unit Store Enable signal is
                               asserted or de-asserted. See the CTS Gasket
                               detail drawing for more information.
                            */
    UINT32 tuse3 : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               TU Store Enable, Trace Source 3. Indicates
                               whether the trigger unit Store Enable signal is
                               asserted or de-asserted. See the CTS Gasket
                               detail drawing for more information.
                            */
    UINT32 tuse4 : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               TU Store Enable, Trace Source 4. Indicates
                               whether the trigger unit Store Enable signal is
                               asserted or de-asserted. See the CTS Gasket
                               detail drawing for more information.
                            */
    UINT32 tuse5 : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               TU Store Enable, Trace Source 5. Indicates
                               whether the trigger unit Store Enable signal is
                               asserted or de-asserted. See the CTS Gasket
                               detail drawing for more information.
                            */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} GTHSTAT_NPK_TPH_STRUCT;

/* SCR2_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200000d8)                                                      */
/*      SPRB0 (0x200000d8)                                                      */
/*      SPRHBM (0x200000d8)                                                     */
/*      SPRC0 (0x200000d8)                                                      */
/*      SPRMCC (0x200000d8)                                                     */
/*      SPRUCC (0x200000d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Source Control Register 2
*/


#define SCR2_NPK_TPH_REG 0x1E1200D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fcd : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Force Capture Done. When set, this bit will
                               force the captureDone signal going from CTS to
                               the CTS Gasket to be asserted.
                            */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fseoff2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Force StoreEn Off for Trace Source 2. Overrides
                               StoreEn[N] signal to force it off. This will
                               override the StoreEnOvrd setting, forcing the
                               storeEn to off (see the CTS Gasket detailed
                               diagram)
                            */
    UINT32 fseoff3 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Force StoreEn Off for Trace Source 3. See
                               FSEOFF2 for details.
                            */
    UINT32 fseoff4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Force StoreEn Off for Trace Source 4. See
                               FSEOFF2 for details.
                            */
    UINT32 fseoff5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Force StoreEn Off for Trace Source 5. See
                               FSEOFF2 for details.
                            */
    UINT32 fseoff6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Force StoreEn Off for Trace Source 6. See
                               FSEOFF2 for details.
                            */
    UINT32 fseoff7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Force StoreEn Off for Trace Source 7. See
                               FSEOFF2 for details.
                            */
    UINT32 rsvd_8 : 10;

                            /* Bits[17:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fsqoff2 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Force StoreQual Off for Trace Source 2.
                               Overrides StoreQual[N] signal from the CTS, to
                               force it off.
                            */
    UINT32 fsqoff3 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Force StoreQual Off for Trace Source 2.
                               Overrides StoreQual[N] signal from the CTS, to
                               force it off.
                            */
    UINT32 fsqoff4 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Force StoreQual Off for Trace Source 4.
                               Overrides StoreQual[N] signal from the CTS, to
                               force it off.
                            */
    UINT32 fsqoff5 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Force StoreQual Off for Trace Source 5.
                               Overrides StoreQual[N] signal from the CTS, to
                               force it off.
                            */
    UINT32 fsqoff6 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Force StoreQual Off for Trace Source 6.
                               Overrides StoreQual[N] signal from the CTS, to
                               force it off.
                            */
    UINT32 fsqoff7 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Force StoreQual Off for Trace Source 7.
                               Overrides StoreQual[N] signal from the CTS, to
                               force it off.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SCR2_NPK_TPH_STRUCT;

/* DESTOVR_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x200000dc)                                                      */
/*      SPRB0 (0x200000dc)                                                      */
/*      SPRHBM (0x200000dc)                                                     */
/*      SPRC0 (0x200000dc)                                                      */
/*      SPRMCC (0x200000dc)                                                     */
/*      SPRUCC (0x200000dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Destination Override Register
*/


#define DESTOVR_NPK_TPH_REG 0x1E1200DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 destovr : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Destination Override. Overrides all Mast[N]Dest
                               bits in the SWDEST and GSWDEST registers with
                               the value specified in this register field. This
                               has the net effect of sending all trace data to
                               the port specified by these bits. These bits use
                               the same encoding as MAST[N]DEST bits in SWDEST
                               registers.
                            */
    UINT32 destovren : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Destination Override Enable. Setting this bit
                               activates the Destination Override function. All
                               trace data from all masters will be sent to the
                               output port specified by bits 2:0.
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DESTOVR_NPK_TPH_STRUCT;

/* SCRPD0_NPK_TPH_REG supported on:                                             */
/*      SPRA0 (0x200000e0)                                                      */
/*      SPRB0 (0x200000e0)                                                      */
/*      SPRHBM (0x200000e0)                                                     */
/*      SPRC0 (0x200000e0)                                                      */
/*      SPRMCC (0x200000e0)                                                     */
/*      SPRUCC (0x200000e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* ScratchPad[0] Register
*/


#define SCRPD0_NPK_TPH_REG 0x1E1200E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 memisprimdest : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Used during save/restore flows to indicate to
                               PMU that North Peak s primary tracing
                               destination is system memory
                            */
    UINT32 dbcisprimdest : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Used during save/restore flows to indicate to
                               PMU that North Peak s primary tracing
                               destination is USB DbC
                            */
    UINT32 ptiisprimdest : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Used during save/restore flows to indicate to
                               PMU that North Peak s primary tracing
                               destination is PTI
                            */
    UINT32 bssbisprimdest : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Used during save/restore flows to indicate to
                               PMU that North Peak s primary tracing
                               destination is BSSB
                            */
    UINT32 ptiisaltdest : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Used during save/restore flows to indicate to
                               PMU that PTI should be used as an alternate
                               destination
                            */
    UINT32 bssbisaltdest : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Used during save/restore flows to indicate to
                               PMU that BSSB should be used as an alternate
                               destination
                            */
    UINT32 deepsxexit : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Used to communicate with driver that a DeepSx
                               event occurred
                            */
    UINT32 s4exit : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Used to communicate with driver that a S4 event
                               occurred
                            */
    UINT32 s5exit : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Used to communicate with driver that a S5 event
                               occurred
                            */
    UINT32 msc0isenabled : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Can be used in several flows to indicate that
                               MSC0 is enabled/active
                            */
    UINT32 msc1isenabled : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Can be used in several flows to indicate that
                               MSC1 is enabled/active
                            */
    UINT32 tracesxexit : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Used to indicate to driver that a trace session
                               has been active across an Sx event
                            */
    UINT32 triggerisenabled : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Can be used in several flows to indicate that a
                               trigger is enabled/active
                            */
    UINT32 odlaisenabled : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Can be used in several flows to indicate that
                               ODLA is enabled/active
                            */
    UINT32 sochapisenabled : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Can be used in several flows to indicate that
                               SoCHAP is enabled/active
                            */
    UINT32 sthisenabled : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Can be used in several flows to indicate that
                               STH is enabled/active
                            */
    UINT32 npkhisenabled : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Can be used in several flows to indicate that
                               NPKH is enabled/active
                            */
    UINT32 verisenabled : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Can be used in several flows to indicate that
                               VER is enabled/active
                            */
    UINT32 ctpisprimdest : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Used during save/restore flows to indicate to
                               PMU that North Peak s primary tracing
                               destination is HTI
                            */
    UINT32 rsvd_1 : 5;

                            /* Bits[23:19], Access Type=RW, default=0x00000000*/

                            /* Scratch Pad bits, for use by software/firmware. */
    UINT32 debuggerinuse : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Debugger In Use. Used during initialization
                               flows to indicate to BIOS/IA firmware that an
                               external host debugger is actively using this
                               North Peak instance
                            */
    UINT32 rsvd : 7;

                            /* Bits[31:25], Access Type=RW, default=0x00000000*/

                            /*
                               Scratch Pad bits, for use by software/firmware.
                               These bits have no defined usage or function,
                               but may have a defined usage or function in the
                               future. Software/firmware is recommended to
                               treat these as reserved for future use
                            */

  } Bits;
  UINT32 Data;

} SCRPD0_NPK_TPH_STRUCT;

/* SCRPD1_NPK_TPH_REG supported on:                                             */
/*      SPRA0 (0x200000e4)                                                      */
/*      SPRB0 (0x200000e4)                                                      */
/*      SPRHBM (0x200000e4)                                                     */
/*      SPRC0 (0x200000e4)                                                      */
/*      SPRMCC (0x200000e4)                                                     */
/*      SPRUCC (0x200000e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* ScratchPad[1] Register
*/


#define SCRPD1_NPK_TPH_REG 0x1E1200E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 blockaddress : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               If BIOS used the MTB to store boot data and
                               later relocated that data to system memory
                               (post-MRC), it can save the physical memory
                               block address in this register to communicate it
                               with software driver
                            */

  } Bits;
  UINT32 Data;

} SCRPD1_NPK_TPH_STRUCT;

/* SCRPD2_NPK_TPH_REG supported on:                                             */
/*      SPRA0 (0x200000e8)                                                      */
/*      SPRB0 (0x200000e8)                                                      */
/*      SPRHBM (0x200000e8)                                                     */
/*      SPRC0 (0x200000e8)                                                      */
/*      SPRMCC (0x200000e8)                                                     */
/*      SPRUCC (0x200000e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* ScratchPad[2] Register
*/


#define SCRPD2_NPK_TPH_REG 0x1E1200E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iafw_ctrl : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               IA Firmware (BIOS) tracing verbosity and
                               enabling. These bits are used to communicate to
                               IA Firmware (BIOS) the desired verbosity level,
                               and whether or not tracing to North Peak, is
                               enabled. Refer to the BIOS Writer's Guide for
                               details on bit encodings.
                            */
    UINT32 scpd2 : 28;

                            /* Bits[31:4], Access Type=RW, default=0x00000000*/

                            /*
                               Scratch Pad bits, for use by software/firmware.
                               These bits have no defined usage or function,
                               but may have a defined usage or function in the
                               future. Software/firmware is recommended to
                               treat these as reserved for future use
                            */

  } Bits;
  UINT32 Data;

} SCRPD2_NPK_TPH_STRUCT;

/* SCRPD3_NPK_TPH_REG supported on:                                             */
/*      SPRA0 (0x200000ec)                                                      */
/*      SPRB0 (0x200000ec)                                                      */
/*      SPRHBM (0x200000ec)                                                     */
/*      SPRC0 (0x200000ec)                                                      */
/*      SPRMCC (0x200000ec)                                                     */
/*      SPRUCC (0x200000ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* ScratchPad[3] Register
*/


#define SCRPD3_NPK_TPH_REG 0x1E1200EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scpd3 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Scratch Pad bits, for use by software/firmware.
                               These bits have no defined usage or function,
                               but may have a defined usage or function in the
                               future. Software/firmware is recommended to
                               treat these as reserved for future use
                            */

  } Bits;
  UINT32 Data;

} SCRPD3_NPK_TPH_STRUCT;

/* NDB_CTRL_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x200000f0)                                                      */
/*      SPRB0 (0x200000f0)                                                      */
/*      SPRHBM (0x200000f0)                                                     */
/*      SPRC0 (0x200000f0)                                                      */
/*      SPRMCC (0x200000f0)                                                     */
/*      SPRUCC (0x200000f0)                                                     */
/* Register default value on SPRA0: 0x00030000                                  */
/* Register default value on SPRB0: 0x00030000                                  */
/* Register default value on SPRHBM: 0x00030000                                 */
/* Register default value on SPRC0: 0x00030000                                  */
/* Register default value on SPRMCC: 0x00030000                                 */
/* Register default value on SPRUCC: 0x00030000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* NPK DTF Control Register
*/


#define NDB_CTRL_NPK_TPH_REG 0x1E1200F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ndb_waitempty : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               NDB Wait for Empty. Specifies the number of
                               npclk periods the NDB will wait after a storeEn
                               deassertion before it considers the DTF to be
                               empty, and forcibly deassert its valid and
                               sourceActive outputs.
                            */
    UINT32 ndbmw0 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               MSC0MemWrap -> dtf_sync. 0: MSC0MemWrap signal
                               assertion will not cause dtf_sync to assert, 1:
                               MS0MemWrap signal assertion will cause dtf_sync
                               to assert.
                            */
    UINT32 ndbmw1 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               MSC1MemWrap -> dtf_sync. 0: MSC1MemWrap signal
                               assertion will not cause dtf_sync to assert, 1:
                               MS1MemWrap signal assertion will cause dtf_sync
                               to assert.
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} NDB_CTRL_NPK_TPH_STRUCT;

/* DTFERR1_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20000100)                                                      */
/*      SPRB0 (0x20000100)                                                      */
/*      SPRHBM (0x20000100)                                                     */
/*      SPRC0 (0x20000100)                                                      */
/*      SPRMCC (0x20000100)                                                     */
/*      SPRUCC (0x20000100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* DTF Error Reporting Register 1
*/


#define DTFERR1_NPK_TPH_REG 0x1E120100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dtf_err_cnt : 8;

                            /* Bits[7:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               DTF Error Count. The number of DTF packets
                               received with DTF protocol violations. See
                               section the NDB Error Handling section for
                               details.
                            */
    UINT32 dtf_errcnt_of : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               DTF Error Counter Overflow. This bit is set
                               if/when the DTF_ERR_CNT value exceeds 255, and
                               will remain set until explicitly cleared with a
                               write (write 1).
                            */
    UINT32 dtf_debug : 1;

                            /* Bits[9:9], Access Type=RW/AC, default=0x00000000*/

                            /*
                               DTF Debug mode. Turns the error registers into a
                               latch next DTF transaction set of registers.
                               This is useful for debug. When this bit is set,
                               the DTFERRn registers will latch the next
                               incoming DTF transaction
                            */
    UINT32 dtf_debug_sts : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /*
                               DTF Debug Status. When set, indicates a packet
                               from DTF was latched into the DTFERRn registers.
                               This bit is set when DTF_DEBUG is set, and the
                               dtf_active signal asserts. Software can
                               differentiate between a valid packet and an
                               error packet using the DTF_ERR bit (if DTF_ERR
                               is set AND this bit is set, the packet was
                               received with at least one error; if only
                               DTF_DEBUG_STS is set, the transaction had no
                               errors)
                            */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DTFERR1_NPK_TPH_STRUCT;

/* DTFERR2_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20000104)                                                      */
/*      SPRB0 (0x20000104)                                                      */
/*      SPRHBM (0x20000104)                                                     */
/*      SPRC0 (0x20000104)                                                      */
/*      SPRMCC (0x20000104)                                                     */
/*      SPRUCC (0x20000104)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* DTF Error Reporting Register 2
*/


#define DTFERR2_NPK_TPH_REG 0x1E120104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dtfcerr : 8;

                            /* Bits[7:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Channel with Error. Channel number of first
                               packet with DTF protocol
                            */
    UINT32 rsvd : 8;

                            /* Bits[15:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dtfmerr : 8;

                            /* Bits[23:16], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Master with Error. Master number of first packet
                               with DTF protocol violation
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DTFERR2_NPK_TPH_STRUCT;

/* DTFERR3_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20000108)                                                      */
/*      SPRB0 (0x20000108)                                                      */
/*      SPRHBM (0x20000108)                                                     */
/*      SPRC0 (0x20000108)                                                      */
/*      SPRMCC (0x20000108)                                                     */
/*      SPRUCC (0x20000108)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* DTF Error Reporting Register 3
*/


#define DTFERR3_NPK_TPH_REG 0x1E120108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dtf_type_err : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               DTF_TYPE Error value. This bit field holds the
                               actual value seen on the type input when a DTF
                               protocol error violation was seen.
                            */
    UINT32 dtf_err : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               DTF Error Detected. This bit is set when a DTF
                               packet is received with a protocol violation.
                               This bit is present in this register so that
                               software can detect an error and capture some of
                               the relevant data in a single register. Writing
                               a 1 to this register will re-arm the error
                               latching circuitry for DTFERR registers except
                               DTFERR1 (the DTF Error counter register). The
                               DTFERR register values will not be updated again
                               until the DTF_ERR bit is set.
                            */
    UINT32 dtferr_clearall : 1;

                            /* Bits[9:9], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Clear all DTF Error related registers. This bit
                               provides a single place for software to clear
                               all DTFErr related registers (specifically, this
                               is added to clear the DTF error counter from
                               within the DTFERR2 register). Software should
                               write a 1 to this bit to clear all DTF Error
                               related registers. This bit is self-clearing,
                               and will always read as 0.
                            */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DTFERR3_NPK_TPH_STRUCT;

/* DTFERR4_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x2000010c)                                                      */
/*      SPRB0 (0x2000010c)                                                      */
/*      SPRHBM (0x2000010c)                                                     */
/*      SPRC0 (0x2000010c)                                                      */
/*      SPRMCC (0x2000010c)                                                     */
/*      SPRUCC (0x2000010c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* DTF Error Reporting Register 4
*/


#define DTFERR4_NPK_TPH_REG 0x1E12010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dtf_err_datalo : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Data of Error Packet. Low 32b of data bus when
                               the DTF error was detected.
                            */

  } Bits;
  UINT32 Data;

} DTFERR4_NPK_TPH_STRUCT;

/* DTFERR5_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20000110)                                                      */
/*      SPRB0 (0x20000110)                                                      */
/*      SPRHBM (0x20000110)                                                     */
/*      SPRC0 (0x20000110)                                                      */
/*      SPRMCC (0x20000110)                                                     */
/*      SPRUCC (0x20000110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* DTF Error Reporting Register 5
*/


#define DTFERR5_NPK_TPH_REG 0x1E120110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dtf_err_datahi : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Data of Error Packet. High 32b of data bus when
                               the DTF error was detected.
                            */

  } Bits;
  UINT32 Data;

} DTFERR5_NPK_TPH_STRUCT;

/* BPB_CSR0_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20000120)                                                      */
/*      SPRB0 (0x20000120)                                                      */
/*      SPRHBM (0x20000120)                                                     */
/*      SPRC0 (0x20000120)                                                      */
/*      SPRMCC (0x20000120)                                                     */
/*      SPRUCC (0x20000120)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* BPB Control/Status Register  0
*/


#define BPB_CSR0_NPK_TPH_REG 0x1E120120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bpb_fto0 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               BPB0 Frame Timeout. For trace destinations used
                               in framing mode, this register specifies the
                               number of clocks the BPB will wait while not
                               sending data, before forcibly closing the frame
                               (asserting eof). 00h: no timeout. eof will not
                               be asserted, regardless of how long the BPB is
                               not sending data. 01h to FFh: eof will be
                               asserted after BPB_FTO0 clocks have passed with
                               no data being sent.
                            */
    UINT32 bpb_fm0 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               BPB0 Framing Mode. 0: Maximum Size framing mode,
                               1: Fixed Size framing mode
                            */
    UINT32 rsvd : 7;

                            /* Bits[15:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bpb_fto1 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* See BPB_FTO0. */
    UINT32 bpb_fm1 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               BPB1 Framing Mode. 0: Maximum Size framing mode,
                               1: Fixed Size framing mode
                            */
    UINT32 rsvd_25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} BPB_CSR0_NPK_TPH_STRUCT;

/* BPB_CSR1_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20000124)                                                      */
/*      SPRB0 (0x20000124)                                                      */
/*      SPRHBM (0x20000124)                                                     */
/*      SPRC0 (0x20000124)                                                      */
/*      SPRMCC (0x20000124)                                                     */
/*      SPRUCC (0x20000124)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* BPB Control/Status Register  0
*/


#define BPB_CSR1_NPK_TPH_REG 0x1E120124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bpb_fto2 : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* See BPB_FTO0. */
    UINT32 bpb_fm2 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               BPB2 Framing Mode. 0: Maximum Size framing mode,
                               1: Fixed Size framing mode
                            */
    UINT32 rsvd : 7;

                            /* Bits[15:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bpb_fto3 : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* See BPB_FTO0. */
    UINT32 bpb_fm3 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               BPB3 Framing Mode. 0: Maximum Size framing mode,
                               1: Fixed Size framing mode
                            */
    UINT32 rsvd_25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} BPB_CSR1_NPK_TPH_STRUCT;

/* EVRT0_NPK_TPH_REG supported on:                                              */
/*      SPRA0 (0x20000150)                                                      */
/*      SPRB0 (0x20000150)                                                      */
/*      SPRHBM (0x20000150)                                                     */
/*      SPRC0 (0x20000150)                                                      */
/*      SPRMCC (0x20000150)                                                     */
/*      SPRUCC (0x20000150)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event Routing Register 0
*/


#define EVRT0_NPK_TPH_REG 0x1E120150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} EVRT0_NPK_TPH_STRUCT;

/* EVRT1_NPK_TPH_REG supported on:                                              */
/*      SPRA0 (0x20000154)                                                      */
/*      SPRB0 (0x20000154)                                                      */
/*      SPRHBM (0x20000154)                                                     */
/*      SPRC0 (0x20000154)                                                      */
/*      SPRMCC (0x20000154)                                                     */
/*      SPRUCC (0x20000154)                                                     */
/* Register default value on SPRA0: 0x00111100                                  */
/* Register default value on SPRB0: 0x00111100                                  */
/* Register default value on SPRHBM: 0x00111100                                 */
/* Register default value on SPRC0: 0x00111100                                  */
/* Register default value on SPRMCC: 0x00111100                                 */
/* Register default value on SPRUCC: 0x00111100                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event Routing Register 1
*/


#define EVRT1_NPK_TPH_REG 0x1E120154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_4 : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 sthevt0en : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               STH Event 0 Enable 0: STH Event 0 does not
                               propagate to CTS Event 10 input 1: STH Event 0
                               propagates to CTS Event 10 input
                            */
    UINT32 dtfevt0en : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               DTF Event 0 Enable. 0: DTF Event 0 does not
                               propagate to CTS Event 10 input 1: DTF Event 0
                               propagates to CTS Event 10 input
                            */
    UINT32 rsvd_3 : 2;

                            /* Bits[11:10], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 sthevt1en : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               STH Event 1 Enable 0: STH Event 1 does not
                               propagate to CTS Event 11 input 1: STH Event 1
                               propagates to CTS Event 11 input
                            */
    UINT32 dtfevt1en : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               DTF Event 1 Enable. 0: DTF Event 1 does not
                               propagate to CTS Event 11 input 1: DTF Event 1
                               propagates to CTS Event 11 input
                            */
    UINT32 rsvd_2 : 2;

                            /* Bits[15:14], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 sthevt2en : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               STH Event 2 Enable 0: STH Event 2 does not
                               propagate to CTS Event 13 input 1: STH Event 2
                               propagates to CTS Event 12 input
                            */
    UINT32 dtfevt2en : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               DTF Event 2 Enable. 0: DTF Event 2 does not
                               propagate to CTS Event 12 input 1: DTF Event 2
                               propagates to CTS Event 12 input
                            */
    UINT32 rsvd_1 : 2;

                            /* Bits[19:18], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 sthevt3en : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               STH Event 3 Enable 0: STH Event 3 does not
                               propagate to CTS Event 13 input 1: STH Event 3
                               propagates to CTS Event 13 input
                            */
    UINT32 dtfevt3en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               DTF Event 3 Enable. 0: DTF Event 3 does not
                               propagate to CTS Event 13 input 1: DTF Event 3
                               propagates to CTS Event 13 input
                            */
    UINT32 rsvd : 10;

                            /* Bits[31:22], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} EVRT1_NPK_TPH_STRUCT;

/* TSCU_FREQ_HI_NPK_TPH_REG supported on:                                       */
/*      SPRA0 (0x20000168)                                                      */
/*      SPRB0 (0x20000168)                                                      */
/*      SPRHBM (0x20000168)                                                     */
/*      SPRC0 (0x20000168)                                                      */
/*      SPRMCC (0x20000168)                                                     */
/*      SPRUCC (0x20000168)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* TSCU Frequency Register High
*/


#define TSCU_FREQ_HI_NPK_TPH_REG 0x1E120168

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gth_freq_hi : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               TSCU Operating Frequency High. Specifies the
                               upper bits of apparent operating frequency of
                               the GTH, in Hertz. This is also clock with which
                               timestamps are generated.See register address
                               0xCC for lower bits
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* TSCU Operating Frequency Reserved. */

  } Bits;
  UINT32 Data;

} TSCU_FREQ_HI_NPK_TPH_STRUCT;

/* DIP_CSR0_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20000170)                                                      */
/*      SPRB0 (0x20000170)                                                      */
/*      SPRHBM (0x20000170)                                                     */
/*      SPRC0 (0x20000170)                                                      */
/*      SPRMCC (0x20000170)                                                     */
/*      SPRUCC (0x20000170)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Data Integrity Packet Control and Status Register 0
*/


#define DIP_CSR0_NPK_TPH_REG 0x1E120170

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 p0dip : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* Output Port 0 DIP Support */
    UINT32 p0crc : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /* Output Port 0 CRC Support */
    UINT32 rsvd : 2;

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p0dip_en : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /* Output Port 0 DIP Enable */
    UINT32 rsvd_6 : 2;

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p1dip : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /* Output Port 1 DIP Support */
    UINT32 p1crc : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* Output Port 1 CRC Support */
    UINT32 rsvd_10 : 2;

                            /* Bits[11:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p1dip_en : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /* Output Port 1 DIP Enable */
    UINT32 rsvd_14 : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p2dip : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /* Output Port 2 DIP Support */
    UINT32 p2crc : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /* Output Port 2 CRC Support */
    UINT32 rsvd_18 : 2;

                            /* Bits[19:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p2dip_en : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /* Output Port 2 DIP Enable */
    UINT32 rsvd_22 : 2;

                            /* Bits[23:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p3dip : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000000*/

                            /* Output Port 3 DIP Support */
    UINT32 p3crc : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /* Output Port 3 CRC Support */
    UINT32 rsvd_26 : 2;

                            /* Bits[27:26], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p3dip_en : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /* Output Port 3 DIP Enable */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DIP_CSR0_NPK_TPH_STRUCT;

/* DIP_CSR1_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20000174)                                                      */
/*      SPRB0 (0x20000174)                                                      */
/*      SPRHBM (0x20000174)                                                     */
/*      SPRC0 (0x20000174)                                                      */
/*      SPRMCC (0x20000174)                                                     */
/*      SPRUCC (0x20000174)                                                     */
/* Register default value on SPRA0: 0x03000000                                  */
/* Register default value on SPRB0: 0x03000000                                  */
/* Register default value on SPRHBM: 0x03000000                                 */
/* Register default value on SPRC0: 0x03000000                                  */
/* Register default value on SPRMCC: 0x03000000                                 */
/* Register default value on SPRUCC: 0x03000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Data Integrity Packet Control and Status Register 1
*/


#define DIP_CSR1_NPK_TPH_REG 0x1E120174

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 p4dip : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* Output Port 4 DIP Support */
    UINT32 p4crc : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /* Output Port 4 CRC Support */
    UINT32 rsvd : 2;

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p4dip_en : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /* Output Port 4 DIP Enable */
    UINT32 rsvd_6 : 2;

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p5dip : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /* Output Port 5 DIP Support */
    UINT32 p5crc : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* Output Port 5 CRC Support */
    UINT32 rsvd_10 : 2;

                            /* Bits[11:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p5dip_en : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /* Output Port 5 DIP Enable */
    UINT32 rsvd_14 : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p6dip : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /* Output Port 6 DIP Support */
    UINT32 p6crc : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /* Output Port 6 CRC Support */
    UINT32 rsvd_18 : 2;

                            /* Bits[19:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p6dip_en : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /* Output Port 6 DIP Enable */
    UINT32 rsvd_22 : 2;

                            /* Bits[23:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p7dip : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /* Output Port 7 DIP Support */
    UINT32 p7crc : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000001*/

                            /* Output Port 7 CRC Support */
    UINT32 rsvd_26 : 2;

                            /* Bits[27:26], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p7dip_en : 2;

                            /* Bits[29:28], Access Type=RW, default=0x00000000*/

                            /* Output Port 7 DIP Enable */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DIP_CSR1_NPK_TPH_STRUCT;

/* DIP_SN0_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20000178)                                                      */
/*      SPRB0 (0x20000178)                                                      */
/*      SPRHBM (0x20000178)                                                     */
/*      SPRC0 (0x20000178)                                                      */
/*      SPRMCC (0x20000178)                                                     */
/*      SPRUCC (0x20000178)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Data Integrity Packet Sequence Number 0
*/


#define DIP_SN0_NPK_TPH_REG 0x1E120178

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 p0dsn : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Output Port 0 DIP Sequence Number */
    UINT32 p1dsn : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /* Output Port 1 DIP Sequence Number */
    UINT32 p2dsn : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000000*/

                            /* Output Port 2 DIP Sequence Number */
    UINT32 p3dsn : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Output Port 3 DIP Sequence Number */

  } Bits;
  UINT32 Data;

} DIP_SN0_NPK_TPH_STRUCT;

/* DIP_SN1_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x2000017c)                                                      */
/*      SPRB0 (0x2000017c)                                                      */
/*      SPRHBM (0x2000017c)                                                     */
/*      SPRC0 (0x2000017c)                                                      */
/*      SPRMCC (0x2000017c)                                                     */
/*      SPRUCC (0x2000017c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Data Integrity Packet Sequence Number 1
*/


#define DIP_SN1_NPK_TPH_REG 0x1E12017C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 p4dsn : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Output Port 4 DIP Sequence Number */
    UINT32 p5dsn : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /* Output Port 5 DIP Sequence Number */
    UINT32 p6dsn : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000000*/

                            /* Output Port 6 DIP Sequence Number */
    UINT32 p7dsn : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Output Port 7 DIP Sequence Number */

  } Bits;
  UINT32 Data;

} DIP_SN1_NPK_TPH_STRUCT;

/* LPP_CTL_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20001c00)                                                      */
/*      SPRB0 (0x20001c00)                                                      */
/*      SPRHBM (0x20001c00)                                                     */
/*      SPRC0 (0x20001c00)                                                      */
/*      SPRMCC (0x20001c00)                                                     */
/*      SPRUCC (0x20001c00)                                                     */
/* Register default value on SPRA0: 0x01000013                                  */
/* Register default value on SPRB0: 0x01000013                                  */
/* Register default value on SPRHBM: 0x01000013                                 */
/* Register default value on SPRC0: 0x01000013                                  */
/* Register default value on SPRMCC: 0x01000013                                 */
/* Register default value on SPRUCC: 0x01000013                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* LPP Control Register
*/


#define LPP_CTL_NPK_TPH_REG 0x1E121C00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lppen : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               LPP Enable: 0: LPP output path/port is disabled
                               and clock-gated. 1: LPP output path/port is
                               enabled. Note: This bit has to be set in both
                               full-power and low-power modes to enable the
                               output port selected by the DEST bit.
                            */
    UINT32 ptifreeclk : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               PTI Free-Running Clock. 0: The trace_clock acts
                               as a strobe which only toggles when valid data
                               is present on trace_data pins. 1: The
                               trace_clock will act as a free-running clock.
                               Note : If PTI Free-running clock mode is
                               enabled, the PnFLUSH bit in the GTHOPT register
                               must be set to avoid STP protocol errors at the
                               PTI interface.
                            */
    UINT32 rsvd : 2;

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ptimodesel : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000001*/

                            /*
                               PTI Mode Select: This register selects the PTI
                               port output Mode. The encoding for the field is
                               as follows: 0000: 2-bit mode 0001: 4-bit mode
                               0010: 8-bit mode 1000: 16-bit mode All others
                               reserved for future use
                            */
    UINT32 ptipresent : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               PTI Present: If set indicates that the PTI path
                               of LPP is connected to the GPIO and PTI mode is
                               valid.
                            */
    UINT32 bssbpresent : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               External Bridge Present: If set indicates that
                               the External Bridge path of LPP is connected to
                               an external (to North Peak) Bridge and this mode
                               is valid.
                            */
    UINT32 rsvd_10 : 6;

                            /* Bits[15:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pticlkdiv : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /*
                               Clock Divider: This field selects the scaled
                               down version of the clock to use for the trace
                               clock and send to GPIO pins: 00: 1x (Use North
                               Peak clock), 01: 2x (Use 1/2 North Peak clock),
                               10: 4x (Use 1/4 North Peak clock), 11: 8x (Use
                               1/8 North Peak clock).
                            */
    UINT32 rsvd_18 : 2;

                            /* Bits[19:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 patgenmod : 3;

                            /* Bits[22:20], Access Type=RW, default=0x00000000*/

                            /*
                               Pattern Generation Mode: This field selects one
                               of the five MIPI-PTI training/calibration
                               patterns to drive onto the PTI Port pins. See
                               MIPI PTI Specification for details. 001: Pattern
                               1 010: Pattern 2 011: Pattern 3 100: Pattern 4
                               101: Pattern 5 110: Pattern 6 All others
                               reserved for future use.
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lpmen : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               Low Power Mode Enable: 0: Full power mode / S0/
                               VNN on, 1: Low power mode Setting this bit will
                               start the sequence to switch to low power mode
                               (kicks off the LPP Switch FSM).
                            */
    UINT32 dest : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               LPP Destination. 0: PTI 1: External Bridge Note
                               : the default state of this bit depends on the
                               LPP path in the SoC. If only PTI is supported
                               the default is PTI. If only an eternal bridge is
                               supported, the default is the external bridge.
                               If both are supported, the default is PTI.
                            */
    UINT32 rsvd_26 : 2;

                            /* Bits[27:26], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 xclk_npclk_ovr : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               XCLK_NPCLK_OVERRIDE. When set, it overrides the
                               xclock and NPK clock requests
                            */
    UINT32 lppsten : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               LPP Store Enable: If this bit is set, LPP store
                               enable will be asserted as long as LPP is in a
                               low power state. If cleared, LPP store enable
                               will not be asserted independent of the state of
                               LPP. This bit can be used to enable/disable
                               tracing manually through writing this bit if it
                               is not desired to start tracing on reset de-
                               assertion. The default value is set using the
                               LPMSTEN_DEFAULT synthesis parameter.
                            */
    UINT32 bssbact : 1;

                            /* Bits[30:30], Access Type=RW/1C, default=0x00000000*/

                            /*
                               External Bridge Path Active: If set indicates
                               that LPP is waiting on one or more credits from
                               the external bridge. Once cleared then there is
                               no data in flight between LPP and the external
                               bridge. This bit can be cleared by writing 1 to
                               it which will also reset the external bridge
                               credits.
                            */
    UINT32 lppbusy : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /*
                               LPP Busy. When set, indicates that there is data
                               in the LPP output path, waiting to be driven
                               onto the PTI pins or the External Bridge
                               interface (whichever is applicable).
                            */

  } Bits;
  UINT32 Data;

} LPP_CTL_NPK_TPH_STRUCT;

/* AON_MISC_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20001c10)                                                      */
/*      SPRB0 (0x20001c10)                                                      */
/*      SPRHBM (0x20001c10)                                                     */
/*      SPRC0 (0x20001c10)                                                      */
/*      SPRMCC (0x20001c10)                                                     */
/*      SPRUCC (0x20001c10)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* AON MISC Register
*/


#define AON_MISC_NPK_TPH_REG 0x1E121C10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xclkpsd_en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               XCLKPSD_EN - xclk prescalar divider for xclk
                               division
                            */
    UINT32 lpm_noxclk : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               NO_XCLK in LPM Mode: This bit masks effect of
                               LPMEN and LPMSTEN bit to request for XCLK. 1:
                               When LPMEN =1 & LPMSTEN = 1, XCLK_req will not
                               be asserted to 1. 0: No effect on XCLK_req Note:
                               This bit will not affect any other activity
                               indicator for XCLK_req
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} AON_MISC_NPK_TPH_STRUCT;

/* PGCB_DEBUG_NPK_TPH_REG supported on:                                         */
/*      SPRA0 (0x20001c14)                                                      */
/*      SPRB0 (0x20001c14)                                                      */
/*      SPRHBM (0x20001c14)                                                     */
/*      SPRC0 (0x20001c14)                                                      */
/*      SPRMCC (0x20001c14)                                                     */
/*      SPRUCC (0x20001c14)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* PGCB Debug Register
*/


#define PGCB_DEBUG_NPK_TPH_REG 0x1E121C14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 aon_pgcbcg_bypass : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               AON_PGCBCG_BYPASS: When set, indicates that AON
                               PGCB Clock gating module will be bypassed and
                               ungated clock will be used for PGCB and CDC
                               module.
                            */
    UINT32 aon_pgcb_cg_disable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               AON_PGCB_CG_DISABLE: When set, indicates that
                               AON PGCB Clock gating module once out of clock
                               gating state due to some wake will not clock
                               gate again.
                            */
    UINT32 vnn_pgcbcg_bypass : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               VNN_PGCBCG_BYPASS: When set, indicates that VNN
                               PGCB Clock gating module will be bypassed and
                               ungated clock will be used for PGCB and CDC
                               module.
                            */
    UINT32 vnn_pgcb_cg_disable : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               VNN_PGCB_CG_DISABLE: When set, indicates that
                               VNN PGCB Clock gating module once out of clock
                               gating state due to some wake will not clock
                               gate again.
                            */
    UINT32 rsvd_1 : 16;

                            /* Bits[19:4], Access Type=RO, default=0x00000000*/

                            /* Reserved for future use */
    UINT32 aon_pgcb_pwrgate_ack : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               AON_PGCB_PWRGATE_ACK: PMC giving acknowledgement
                               to the PGCB for its request.
                            */
    UINT32 aon_pgcb_pwrgate_req : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               AON_PGCB_PWRGATE_REQ: Power Gating request from
                               PGCB to PMC.
                            */
    UINT32 aon_sw_pwrgate_req : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               AON_SW_PWRGATE_REQ: Software power gate request
                               for AON PGCB.
                            */
    UINT32 aon_pgcb_wake : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /* AON_PGCB_WAKE: Wake from PMC to PGCB. */
    UINT32 rsvd : 4;

                            /* Bits[27:24], Access Type=RO, default=0x00000000*/

                            /* Reserved for future use */
    UINT32 vnn_pgcb_pwrgate_ack : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000000*/

                            /*
                               VNN_PGCB_PWRGATE_ACK: PMC giving acknowledgement
                               to the PGCB for its request.
                            */
    UINT32 vnn_pgcb_pwrgate_req : 1;

                            /* Bits[29:29], Access Type=RO, default=0x00000000*/

                            /*
                               VNN_PGCB_PWRGATE_REQ: Power Gating request from
                               PGCB to PMC.
                            */
    UINT32 vnn_sw_pwrgate_req : 1;

                            /* Bits[30:30], Access Type=RO, default=0x00000000*/

                            /*
                               VNN_SW_PWRGATE_REQ: Software power gate request
                               for PGCB.
                            */
    UINT32 vnn_pgcb_wake : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000000*/

                            /* VNN_PGCB_WAKE: Wake from PMC to PGCB. */

  } Bits;
  UINT32 Data;

} PGCB_DEBUG_NPK_TPH_STRUCT;

/* TSUCTRL_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20002000)                                                      */
/*      SPRB0 (0x20002000)                                                      */
/*      SPRHBM (0x20002000)                                                     */
/*      SPRC0 (0x20002000)                                                      */
/*      SPRMCC (0x20002000)                                                     */
/*      SPRUCC (0x20002000)                                                     */
/* Register default value on SPRA0: 0x00000011                                  */
/* Register default value on SPRB0: 0x00000011                                  */
/* Register default value on SPRHBM: 0x00000011                                 */
/* Register default value on SPRC0: 0x00000011                                  */
/* Register default value on SPRMCC: 0x00000011                                 */
/* Register default value on SPRUCC: 0x00000011                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* TSCU Control Register
*/


#define TSUCTRL_NPK_TPH_REG 0x1E122000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctcresync : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               CTC Resync. When set, forces the CTC control
                               state machine to go to the INIT state. When
                               cleared, allows the CTC control state machine to
                               begin the CTC initialization process if the
                               ctc_enable signal is high and if the XCLK is
                               running.
                            */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 snapshottsc : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Snapshot Timestamp Counter. Writing a 1 causes
                               the current value of both CTC to be written to
                               the snapshot registers so that it can be safely
                               read by software. This bit is automatically
                               cleared one North Peak clock after being written
                               to remove the need for a separate write to clear
                               it before getting subsequent snapshot updates.
                               Writing zero has no effect. Reads will always
                               return 0 for this field.
                            */
    UINT32 xclkovrd : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               XCLK Override. This is for debug purposes only
                               in order to work around possible problems with
                               XCLK behaviour. Writing a 1 enables XclkCycle to
                               assert on every NPK clock cycle rather than only
                               when XCLK cycles are detected. When set to 1,
                               timestamp correlation with external sources
                               (e.g. instruction trace) is not possible.
                            */
    UINT32 maxalloweddelay : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000001*/

                            /*
                               MAXALLOWEDDELAY. Specifies the maximum allowed
                               round trip time of the non-posted LocalStamp
                               message when the sync command is bounded. The
                               agent can choose one of three bound values pre-
                               defined by the corresponding ARU register. The
                               agent must choose a value which is longer than
                               the minimum round trip delay of a non-posted
                               message from the ARU to the target agent.
                               Encoding: 0000 => Bound Range Low 0001 => Bound
                               Range 2 0010 => Bound Range Max 0011-1111 =>
                               Reserved
                            */
    UINT32 rsvd_8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSUCTRL_NPK_TPH_STRUCT;

/* TSCUSTAT_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20002004)                                                      */
/*      SPRB0 (0x20002004)                                                      */
/*      SPRHBM (0x20002004)                                                     */
/*      SPRC0 (0x20002004)                                                      */
/*      SPRMCC (0x20002004)                                                     */
/*      SPRUCC (0x20002004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* TSCU Status Register
*/


#define TSCUSTAT_NPK_TPH_REG 0x1E122004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctcvalidstat : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               CTC Valid Status. Indicates the current state of
                               the CTCvalid signal. 0: CTC is not in sync with
                               the ART. 1: CTC initialization was successful
                               and CTC is in sync with the ART.
                            */
    UINT32 ctcsyncingstat : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /*
                               CTC Syncing Status. Indicates the current state
                               of the CTCsyncing signal. 0: CTCsyncing is not
                               asserted. CTC initialization is not in progress.
                               1: CTCsyncing is asserted. CTC initialization is
                               in progress.
                            */
    UINT32 ctcerrorstat : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /*
                               CTC Error Status. Indicates the current state of
                               the CTCvalid signal. 0: CTCerror signal is not
                               asserted. 1: CTCerror signal is asserted
                               indicating an error occurred while performing
                               CTC initialization.
                            */
    UINT32 rsvd : 2;

                            /* Bits[4:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ctcovrflw : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               CTC Overflow. Indicates that an overflow of the
                               entire CTC has occurred. 0: no overflow
                               detected. 1: an overflow has occurred.
                            */
    UINT32 npktscovrflw : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               NPKTSC Overflow. This bit indicates that an
                               overflow of the entire NPKTSC has occurred. 0:
                               no overflow detected. 1: an overflow has
                               occurred.
                            */
    UINT32 xclkstat : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               XCLK Status. Indicates the status of the XCLK
                               based on the number of NPK clocks since the last
                               XCLK toggle change assertion and the value of
                               the NPK offset counter. 0: XCLK is inactive NPK
                               offset counter has reached 128 2**FAST_CNT_WIDTH
                               clocks without having been reset by XCLK toggle
                               change). 1: XCLK is active.
                            */
    UINT32 ctccsmstat : 3;

                            /* Bits[10:8], Access Type=RO, default=0x00000000*/

                            /*
                               CTC Control State Machine Status. The current
                               state of the CTC control state machine. 000:
                               INIT 001: IRDY 010: WAIT 011: ARMED 100: SYNC
                               101: ERROR 110: N/A 111: N/A
                            */
    UINT32 lta_width : 2;

                            /* Bits[12:11], Access Type=RO, default=0x00000000*/

                            /*
                               LTA Fast count width : 00: 7bits 01: 8bits 10:
                               6bits
                            */
    UINT32 lta_nvalue : 11;

                            /* Bits[23:13], Access Type=RO, default=0x00000000*/

                            /* LTA N VALUE */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSCUSTAT_NPK_TPH_STRUCT;

/* NPKTSCSSL_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20002010)                                                      */
/*      SPRB0 (0x20002010)                                                      */
/*      SPRHBM (0x20002010)                                                     */
/*      SPRC0 (0x20002010)                                                      */
/*      SPRMCC (0x20002010)                                                     */
/*      SPRUCC (0x20002010)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* NPKTSC SNAPSHOT Lower Register
*/


#define NPKTSCSSL_NPK_TPH_REG 0x1E122010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 npktscsnapshotl : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               NPKTSC SNAPSHOT Lower. Lower DW of the NPKTSC
                               SNAPSHOT value
                            */

  } Bits;
  UINT32 Data;

} NPKTSCSSL_NPK_TPH_STRUCT;

/* NPKTSCSSU_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20002014)                                                      */
/*      SPRB0 (0x20002014)                                                      */
/*      SPRHBM (0x20002014)                                                     */
/*      SPRC0 (0x20002014)                                                      */
/*      SPRMCC (0x20002014)                                                     */
/*      SPRUCC (0x20002014)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* NPKTSC SNAPSHOT Upper Register
*/


#define NPKTSCSSU_NPK_TPH_REG 0x1E122014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 npktscsnapshotu : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               NPKTSC SNAPSHOT Upper. Upper DW of the NPKTSC
                               SNAPSHOT value
                            */

  } Bits;
  UINT32 Data;

} NPKTSCSSU_NPK_TPH_STRUCT;

/* TSCU_INFO_NPK_TPH_REG supported on:                                          */
/*      SPRA0 (0x20002018)                                                      */
/*      SPRB0 (0x20002018)                                                      */
/*      SPRHBM (0x20002018)                                                     */
/*      SPRC0 (0x20002018)                                                      */
/*      SPRMCC (0x20002018)                                                     */
/*      SPRUCC (0x20002018)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* TSCU INFO  Register
*/


#define TSCU_INFO_NPK_TPH_REG 0x1E122018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xclk_divisor : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000001*/

                            /*
                               TSCU INFO NPK_XCLK_PRESCALAR_DIV . contains the
                               XCLK Divisor value (XCLK_PRESCALAR_DIV parameter
                               value).
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=0x00000000*/

                            /* TSCU INFO Reserved fields */

  } Bits;
  UINT32 Data;

} TSCU_INFO_NPK_TPH_STRUCT;

/* STHCAP0_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004000)                                                      */
/*      SPRB0 (0x20004000)                                                      */
/*      SPRHBM (0x20004000)                                                     */
/*      SPRC0 (0x20004000)                                                      */
/*      SPRMCC (0x20004000)                                                     */
/*      SPRUCC (0x20004000)                                                     */
/* Register default value on SPRA0: 0x01010100                                  */
/* Register default value on SPRB0: 0x01010100                                  */
/* Register default value on SPRHBM: 0x01010100                                 */
/* Register default value on SPRC0: 0x01010100                                  */
/* Register default value on SPRMCC: 0x01010100                                 */
/* Register default value on SPRUCC: 0x01010100                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* STH Capability 0
*/


#define STHCAP0_NPK_TPH_REG 0x1E124000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sthmstr : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000100*/

                            /*
                               SW Master Start: Indicates the starting master
                               number for the Software Trace Hub as exposed by
                               the PCI BAR SW_BAR (SW_UBAR + SW_LBAR). This is
                               the same as the value of the hardware parameter
                               SW_MSTR_STRT.
                            */
    UINT32 sthmnum : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000101*/

                            /*
                               Number of Software Masters: Indicates the
                               highest numbered software master. The value of
                               this register is equal to SW_MSTR_STP.
                            */

  } Bits;
  UINT32 Data;

} STHCAP0_NPK_TPH_STRUCT;

/* STHCAP1_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004004)                                                      */
/*      SPRB0 (0x20004004)                                                      */
/*      SPRHBM (0x20004004)                                                     */
/*      SPRC0 (0x20004004)                                                      */
/*      SPRMCC (0x20004004)                                                     */
/*      SPRUCC (0x20004004)                                                     */
/* Register default value on SPRA0: 0xFF7F0080                                  */
/* Register default value on SPRB0: 0xFF7F0080                                  */
/* Register default value on SPRHBM: 0xFF7F0080                                 */
/* Register default value on SPRC0: 0xFF7F0080                                  */
/* Register default value on SPRMCC: 0xFF7F0080                                 */
/* Register default value on SPRUCC: 0xFF7F0080                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* STH Capability 1
*/


#define STHCAP1_NPK_TPH_REG 0x1E124004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 chlcnt : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000080*/

                            /*
                               Software Channels Per Master: Indicates the
                               number of channels per master for STH.
                            */
    UINT32 rsvd : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /* STHCAP1 Reserved */
    UINT32 rtitcnt : 8;

                            /* Bits[23:16], Access Type=RO, default=0x0000007F*/

                            /*
                               Number of RTIT Masters. Indicates the number of
                               RTIT Masters supported in this instantiation of
                               the STH. 0h: RTIT is not implemented and not
                               supported 1h: 1 RTIT master (1 CPU or thread)
                               2h: 2 RTIT masters 3h: 3 RTIT masters etc. 0Fh:
                               15 RTIT masters
                            */
    UINT32 fw_mstr : 8;

                            /* Bits[31:24], Access Type=RW, default=0x000000FF*/

                            /*
                               Firmware Master Stop. This field indicates the
                               highest Master number that is allocated to
                               firmware trace sources. The default (reset)
                               value of FW_MSTR_STP is set by the synthesis
                               parameter FW_MSTR_STP. Thereafter, it can be
                               written by any software or firmware to adjust
                               the number of masters allocated to firmware.
                               This field is intended to be used in two ways:
                               When the STH is a single PCI BAR, this register
                               field can be changed according to whatever
                               software model best suits the use case(s) for
                               the implementation. When the STH is split into a
                               fixed ACPI BAR for firmware, and a movable PCI
                               BAR for software, then this register field
                               should be only read, and not written. This is
                               because the value in this register reflects the
                               dividing line between the ACPI BAR and the PCI
                               BAR. If the dividing line is moved, software
                               will have an inaccurate view of the STH.
                            */

  } Bits;
  UINT32 Data;

} STHCAP1_NPK_TPH_STRUCT;

/* TRIG_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004008)                                                      */
/*      SPRB0 (0x20004008)                                                      */
/*      SPRHBM (0x20004008)                                                     */
/*      SPRC0 (0x20004008)                                                      */
/*      SPRMCC (0x20004008)                                                     */
/*      SPRUCC (0x20004008)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Create Trigger Packet
*/


#define TRIG_NPK_TPH_REG 0x1E124008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 trigdat : 8;

                            /* Bits[7:0], Access Type=WO, default=0x00000000*/

                            /*
                               Trigger Data/Identifier. These bits will be the
                               payload of the TRIG packet
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TRIG_NPK_TPH_STRUCT;

/* TRIG_TS_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x2000400c)                                                      */
/*      SPRB0 (0x2000400c)                                                      */
/*      SPRHBM (0x2000400c)                                                     */
/*      SPRC0 (0x2000400c)                                                      */
/*      SPRMCC (0x2000400c)                                                     */
/*      SPRUCC (0x2000400c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Create Trigger Packet with Time Stamp
*/


#define TRIG_TS_NPK_TPH_REG 0x1E12400C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 trigdat : 8;

                            /* Bits[7:0], Access Type=WO, default=0x00000000*/

                            /*
                               Trigger Data/Identifier. These bits will be the
                               payload of the TRIG_TS packet
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TRIG_TS_NPK_TPH_STRUCT;

/* XSYNC_NPK_TPH_REG supported on:                                              */
/*      SPRA0 (0x20004010)                                                      */
/*      SPRB0 (0x20004010)                                                      */
/*      SPRHBM (0x20004010)                                                     */
/*      SPRC0 (0x20004010)                                                      */
/*      SPRMCC (0x20004010)                                                     */
/*      SPRUCC (0x20004010)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Create Cross-Synchronization Packet
*/


#define XSYNC_NPK_TPH_REG 0x1E124010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xsyncdat : 8;

                            /* Bits[7:0], Access Type=WO, default=0x00000000*/

                            /*
                               Cross-Synchronization Data/Identifier. These
                               bits will be the payload of the XSYNC packet
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XSYNC_NPK_TPH_STRUCT;

/* XSYNC_TS_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x20004014)                                                      */
/*      SPRB0 (0x20004014)                                                      */
/*      SPRHBM (0x20004014)                                                     */
/*      SPRC0 (0x20004014)                                                      */
/*      SPRMCC (0x20004014)                                                     */
/*      SPRUCC (0x20004014)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Create Cross-Synchronization Packet w Time Stamp
*/


#define XSYNC_TS_NPK_TPH_REG 0x1E124014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 xsyncdat : 8;

                            /* Bits[7:0], Access Type=WO, default=0x00000000*/

                            /*
                               Cross-Synchronization Data/Identifier. These
                               bits will be the payload of the XSYNC_TS packet
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} XSYNC_TS_NPK_TPH_STRUCT;

/* GERR_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004018)                                                      */
/*      SPRB0 (0x20004018)                                                      */
/*      SPRHBM (0x20004018)                                                     */
/*      SPRC0 (0x20004018)                                                      */
/*      SPRMCC (0x20004018)                                                     */
/*      SPRUCC (0x20004018)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Create General Error Packet
*/


#define GERR_NPK_TPH_REG 0x1E124018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gerrdat : 8;

                            /* Bits[7:0], Access Type=WO, default=0x00000000*/

                            /*
                               Create General Error Packet,Error
                               Data/Identifier. These bits will be the payload
                               of the GERR packet
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} GERR_NPK_TPH_STRUCT;

/* TRIGTSPP_NPK_TPH_REG supported on:                                           */
/*      SPRA0 (0x2000401c)                                                      */
/*      SPRB0 (0x2000401c)                                                      */
/*      SPRHBM (0x2000401c)                                                     */
/*      SPRC0 (0x2000401c)                                                      */
/*      SPRMCC (0x2000401c)                                                     */
/*      SPRUCC (0x2000401c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* TRIG_TS packet payload
*/


#define TRIGTSPP_NPK_TPH_REG 0x1E12401C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 trigval : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               TRIG Value: Specifies the payload in the TRIG_TS
                               packet when the STH detects the trigger input
                               from the CTS is asserted.
                            */
    UINT32 trigen : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Trigger packet Enable. This bit determines if a
                               TRIG_TS packet is generated when the trigger
                               input from the CTS is asserted 0: do not create
                               a TRIG packet 1: create a TRIG packet with
                               payload as specified in TRIGVAL.
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TRIGTSPP_NPK_TPH_STRUCT;

/* BDC_NPK_TPH_REG supported on:                                                */
/*      SPRA0 (0x20004020)                                                      */
/*      SPRB0 (0x20004020)                                                      */
/*      SPRHBM (0x20004020)                                                     */
/*      SPRC0 (0x20004020)                                                      */
/*      SPRMCC (0x20004020)                                                     */
/*      SPRUCC (0x20004020)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Backpressure Duration Counter
*/


#define BDC_NPK_TPH_REG 0x1E124020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bdc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Backpressure Duration Count value. Indicates the
                               number of clock cycles that STH is allowed to
                               backpressure the host interface. See the
                               Software Trace Hub chapter for details. Note
                               that the default value indicates that STH will
                               start dropping as soon as it cannot process any
                               more data.
                            */

  } Bits;
  UINT32 Data;

} BDC_NPK_TPH_STRUCT;

/* DPLC_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004024)                                                      */
/*      SPRB0 (0x20004024)                                                      */
/*      SPRHBM (0x20004024)                                                     */
/*      SPRC0 (0x20004024)                                                      */
/*      SPRMCC (0x20004024)                                                     */
/*      SPRUCC (0x20004024)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Data Packet Lost Counter
*/


#define DPLC_NPK_TPH_REG 0x1E124024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dplcv : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*
                               Data Packets Lost Counter Value: The number of
                               packets (not including triggers initiated by
                               CTS) that have been dropped during a
                               backpressure release period. This register field
                               is only valid while data is being dropped by the
                               STH. Once the STH is lossless again, the DPLC
                               value is sent as a USER packet in the trace
                               stream, and this register field is invalid.
                            */
    UINT32 dplco : 1;

                            /* Bits[16:16], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Data Packets Lost Counter Overflow: this is a
                               sticky bit that indicates that the DPLC value
                               has wrapped around at least once. This will be
                               sent as the most significant bit of the number
                               of packets lost after existing Drop Mode
                            */
    UINT32 rsvd : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DPLC_NPK_TPH_STRUCT;

/* EV0AMCH_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004028)                                                      */
/*      SPRB0 (0x20004028)                                                      */
/*      SPRHBM (0x20004028)                                                     */
/*      SPRC0 (0x20004028)                                                      */
/*      SPRMCC (0x20004028)                                                     */
/*      SPRUCC (0x20004028)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 0 Address Match Register
*/


#define EV0AMCH_NPK_TPH_REG 0x1E124028

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrmt : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 0 Address Match Register: Address match.
                               Specifies the match value for the address. This
                               effectively maps this event detector to a
                               specific master, channel, and packet type.
                            */

  } Bits;
  UINT32 Data;

} EV0AMCH_NPK_TPH_STRUCT;

/* EV0AMSK_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x2000402c)                                                      */
/*      SPRB0 (0x2000402c)                                                      */
/*      SPRHBM (0x2000402c)                                                     */
/*      SPRC0 (0x2000402c)                                                      */
/*      SPRMCC (0x2000402c)                                                     */
/*      SPRUCC (0x2000402c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 0 Address Mask Register
*/


#define EV0AMSK_NPK_TPH_REG 0x1E12402C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrmk : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 0 Address Mask Register: Address mask.
                               Specifies the mask value for the address. 0:
                               Means the corresponding bit is not included in
                               the match. 1: Means the corresponding bit is
                               included in the match. Note: To trigger an event
                               at least one bit of this register has to be set
                            */

  } Bits;
  UINT32 Data;

} EV0AMSK_NPK_TPH_STRUCT;

/* EV0DMCH_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004030)                                                      */
/*      SPRB0 (0x20004030)                                                      */
/*      SPRHBM (0x20004030)                                                     */
/*      SPRC0 (0x20004030)                                                      */
/*      SPRMCC (0x20004030)                                                     */
/*      SPRUCC (0x20004030)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 0 Data Match Register
*/


#define EV0DMCH_NPK_TPH_REG 0x1E124030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 datamt : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 0 Data Match Register: Data match.
                               Specifies the match value for the data.
                            */

  } Bits;
  UINT32 Data;

} EV0DMCH_NPK_TPH_STRUCT;

/* EV0DMSK_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004034)                                                      */
/*      SPRB0 (0x20004034)                                                      */
/*      SPRHBM (0x20004034)                                                     */
/*      SPRC0 (0x20004034)                                                      */
/*      SPRMCC (0x20004034)                                                     */
/*      SPRUCC (0x20004034)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 0 Data Match Register
*/


#define EV0DMSK_NPK_TPH_REG 0x1E124034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 datamsk : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 0 Data Mask Register: Data Mask. Specifies
                               the mask value for the data. 0: Means the
                               corresponding bit is not included in the match.
                               1: Means the corresponding bit is included in
                               the match. Note: To trigger an event at least
                               one bit of this register has to be set
                            */

  } Bits;
  UINT32 Data;

} EV0DMSK_NPK_TPH_STRUCT;

/* EV0CTRL_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004038)                                                      */
/*      SPRB0 (0x20004038)                                                      */
/*      SPRHBM (0x20004038)                                                     */
/*      SPRC0 (0x20004038)                                                      */
/*      SPRMCC (0x20004038)                                                     */
/*      SPRUCC (0x20004038)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 0 Match/Mask Control Register
*/


#define EV0CTRL_NPK_TPH_REG 0x1E124038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enevt0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Event 0. 0: do not generate Event 0
                               signal to CTS when Event 0 is detected 1:
                               generate Event 0 signal to CTS when Event 0 is
                               detected.
                            */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 andmatch : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               AND Match: Sets the type of the trigger match
                               function. 0: The match function is OR. When any
                               bit enabled by its mask bit matches the value of
                               its match bit the Match output is asserted. 1:
                               The match function is AND. All bits enabled by
                               their mask bits must match the value of their
                               match bits for the Match output to be asserted.
                            */
    UINT32 invmatch : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Invert Match: When set, makes the trigger match
                               function either NAND or NOR, depending upon the
                               state of bit 2. 0: The match function type is as
                               specified by bit 2. 1: The match function type
                               is inverted from the specification of bit 2.
                            */
    UINT32 bar_msk : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               BAR Mask bits. Specifies the mask value for the
                               BAR (Base Address Register). 0: Means the
                               corresponding bit is not included in the match.
                               1: Means the corresponding bit is included in
                               the match.
                            */
    UINT32 bar_mch : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               BAR Match bits. Specifies the match value for
                               the BAR (Base Address Register). 00: BAR 0,
                               CSR_MTB_BAR 01: BAR 1, STMR BAR. 10: BAR 2, RTIT
                               BAR 11: BAR 3, Firmware BAR (FW_BAR)
                            */
    UINT32 rsvd_8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} EV0CTRL_NPK_TPH_STRUCT;

/* EV1AMCH_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x2000403c)                                                      */
/*      SPRB0 (0x2000403c)                                                      */
/*      SPRHBM (0x2000403c)                                                     */
/*      SPRC0 (0x2000403c)                                                      */
/*      SPRMCC (0x2000403c)                                                     */
/*      SPRUCC (0x2000403c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 1 Address Match Register
*/


#define EV1AMCH_NPK_TPH_REG 0x1E12403C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrmt : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 1 Address Match Register: Address match.
                               Specifies the match value for the address. This
                               effectively maps this event detector to a
                               specific master, channel, and packet type.
                            */

  } Bits;
  UINT32 Data;

} EV1AMCH_NPK_TPH_STRUCT;

/* EV1AMSK_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004040)                                                      */
/*      SPRB0 (0x20004040)                                                      */
/*      SPRHBM (0x20004040)                                                     */
/*      SPRC0 (0x20004040)                                                      */
/*      SPRMCC (0x20004040)                                                     */
/*      SPRUCC (0x20004040)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 1 Address Mask Register
*/


#define EV1AMSK_NPK_TPH_REG 0x1E124040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrmk : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 1 Address Mask Register:Address mask.
                               Specifies the mask value for the address. 0:
                               Means the corresponding bit is not included in
                               the match. 1: Means the corresponding bit is
                               included in the match. Note: To trigger an event
                               at least one bit of this register has to be set
                            */

  } Bits;
  UINT32 Data;

} EV1AMSK_NPK_TPH_STRUCT;

/* EV1DMCH_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004044)                                                      */
/*      SPRB0 (0x20004044)                                                      */
/*      SPRHBM (0x20004044)                                                     */
/*      SPRC0 (0x20004044)                                                      */
/*      SPRMCC (0x20004044)                                                     */
/*      SPRUCC (0x20004044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 1 Data Match Register
*/


#define EV1DMCH_NPK_TPH_REG 0x1E124044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 datamt : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 1 Data Match Register: Data match.
                               Specifies the match value for the data.
                            */

  } Bits;
  UINT32 Data;

} EV1DMCH_NPK_TPH_STRUCT;

/* EV1DMSK_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004048)                                                      */
/*      SPRB0 (0x20004048)                                                      */
/*      SPRHBM (0x20004048)                                                     */
/*      SPRC0 (0x20004048)                                                      */
/*      SPRMCC (0x20004048)                                                     */
/*      SPRUCC (0x20004048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 1 Data Match Register
*/


#define EV1DMSK_NPK_TPH_REG 0x1E124048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 datamsk : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 1 Data Mask Register:Data Mask. Specifies
                               the mask value for the data. 0: Means the
                               corresponding bit is not included in the match.
                               1: Means the corresponding bit is included in
                               the match. Note: To trigger an event at least
                               one bit of this register has to be set
                            */

  } Bits;
  UINT32 Data;

} EV1DMSK_NPK_TPH_STRUCT;

/* EV1CTRL_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x2000404c)                                                      */
/*      SPRB0 (0x2000404c)                                                      */
/*      SPRHBM (0x2000404c)                                                     */
/*      SPRC0 (0x2000404c)                                                      */
/*      SPRMCC (0x2000404c)                                                     */
/*      SPRUCC (0x2000404c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 1 Match/Mask Control Register
*/


#define EV1CTRL_NPK_TPH_REG 0x1E12404C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enevt1 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Event 1. 0: do not generate Event 1
                               signal to CTS when Event 1 is detected 1:
                               generate Event 1 signal to CTS when Event 1 is
                               detected.
                            */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 andmatch : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               AND Match: Sets the type of the trigger match
                               function. 0:The match function is OR. When any
                               bit enabled by its mask bit matches the value of
                               its match bit the Match output is asserted. 1:
                               The match function is AND. All bits enabled by
                               their mask bits must match the value of their
                               match bits for the Match output to be asserted.
                            */
    UINT32 invmatch : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Invert Match: When set, makes the trigger match
                               function either NAND or NOR, depending upon the
                               state of bit 2. 0: The match function type is as
                               specified by bit 2. 1: The match function type
                               is inverted from the specification of bit 2.
                            */
    UINT32 bar_msk : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               BAR Mask bits. Specifies the mask value for the
                               BAR (Base Address Register). 0: Means the
                               corresponding bit is not included in the match.
                               1: Means the corresponding bit is included in
                               the match.
                            */
    UINT32 bar_mch : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               Event 1 Match/Mask Control Register: BAR Match
                               bits. Specifies the match value for the BAR
                               (Base Address Register). 00: BAR 0, CSR_MTB_BAR
                               01: BAR 1, STMR BAR. 10: BAR 2, RTIT BAR 11: BAR
                               3, Firmware BAR (FW_BAR)
                            */
    UINT32 rsvd_8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} EV1CTRL_NPK_TPH_STRUCT;

/* EV2AMCH_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004050)                                                      */
/*      SPRB0 (0x20004050)                                                      */
/*      SPRHBM (0x20004050)                                                     */
/*      SPRC0 (0x20004050)                                                      */
/*      SPRMCC (0x20004050)                                                     */
/*      SPRUCC (0x20004050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 2 Address Match Register
*/


#define EV2AMCH_NPK_TPH_REG 0x1E124050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrmt : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 2 Address Match Register: Address match.
                               Specifies the match value for the address. This
                               effectively maps this event detector to a
                               specific master, channel, and packet type.
                            */

  } Bits;
  UINT32 Data;

} EV2AMCH_NPK_TPH_STRUCT;

/* EV2AMSK_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004054)                                                      */
/*      SPRB0 (0x20004054)                                                      */
/*      SPRHBM (0x20004054)                                                     */
/*      SPRC0 (0x20004054)                                                      */
/*      SPRMCC (0x20004054)                                                     */
/*      SPRUCC (0x20004054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 2 Address Mask Register
*/


#define EV2AMSK_NPK_TPH_REG 0x1E124054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrmk : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 2 Address Mask Register:Address mask.
                               Specifies the mask value for the address. 0:
                               Means the corresponding bit is not included in
                               the match. 1: Means the corresponding bit is
                               included in the match. Note: To trigger an event
                               at least one bit of this register has to be set
                            */

  } Bits;
  UINT32 Data;

} EV2AMSK_NPK_TPH_STRUCT;

/* EV2DMCH_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004058)                                                      */
/*      SPRB0 (0x20004058)                                                      */
/*      SPRHBM (0x20004058)                                                     */
/*      SPRC0 (0x20004058)                                                      */
/*      SPRMCC (0x20004058)                                                     */
/*      SPRUCC (0x20004058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 2 Data Match Register
*/


#define EV2DMCH_NPK_TPH_REG 0x1E124058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 datamt : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 2 Data Match Register: Data match.
                               Specifies the match value for the data.
                            */

  } Bits;
  UINT32 Data;

} EV2DMCH_NPK_TPH_STRUCT;

/* EV2DMSK_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x2000405c)                                                      */
/*      SPRB0 (0x2000405c)                                                      */
/*      SPRHBM (0x2000405c)                                                     */
/*      SPRC0 (0x2000405c)                                                      */
/*      SPRMCC (0x2000405c)                                                     */
/*      SPRUCC (0x2000405c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 2 Data Match Register
*/


#define EV2DMSK_NPK_TPH_REG 0x1E12405C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 datamsk : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 2 Data Mask Register:Data Mask. Specifies
                               the mask value for the data. 0: Means the
                               corresponding bit is not included in the match.
                               1: Means the corresponding bit is included in
                               the match. Note: To trigger an event at least
                               one bit of this register has to be set
                            */

  } Bits;
  UINT32 Data;

} EV2DMSK_NPK_TPH_STRUCT;

/* EV2CTRL_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004060)                                                      */
/*      SPRB0 (0x20004060)                                                      */
/*      SPRHBM (0x20004060)                                                     */
/*      SPRC0 (0x20004060)                                                      */
/*      SPRMCC (0x20004060)                                                     */
/*      SPRUCC (0x20004060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 2 Match/Mask Control Register
*/


#define EV2CTRL_NPK_TPH_REG 0x1E124060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enevt2 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Event 2. 0: do not generate Event 2
                               signal to CTS when Event 2 is detected 1:
                               generate Event 2 signal to CTS when Event 2 is
                               detected.
                            */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 andmatch : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               AND Match: Sets the type of the trigger match
                               function. 0:The match function is OR. When any
                               bit enabled by its mask bit matches the value of
                               its match bit the Match output is asserted. 1:
                               The match function is AND. All bits enabled by
                               their mask bits must match the value of their
                               match bits for the Match output to be asserted.
                            */
    UINT32 invmatch : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Invert Match: When set, makes the trigger match
                               function either NAND or NOR, depending upon the
                               state of bit 2. 0: The match function type is as
                               specified by bit 2. 1: The match function type
                               is inverted from the specification of bit 2.
                            */
    UINT32 bar_msk : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               BAR Mask bits. Specifies the mask value for the
                               BAR (Base Address Register). 0: Means the
                               corresponding bit is not included in the match.
                               1: Means the corresponding bit is included in
                               the match.
                            */
    UINT32 bar_mch : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               Event 2 Match/Mask Control Register: BAR Match
                               bits. Specifies the match value for the BAR
                               (Base Address Register). 00: BAR 0, CSR_MTB_BAR
                               01: BAR 1, STMR BAR. 10: BAR 2, RTIT BAR 11: BAR
                               3, Firmware BAR (FW_BAR)
                            */
    UINT32 rsvd_8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} EV2CTRL_NPK_TPH_STRUCT;

/* EV3AMCH_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004064)                                                      */
/*      SPRB0 (0x20004064)                                                      */
/*      SPRHBM (0x20004064)                                                     */
/*      SPRC0 (0x20004064)                                                      */
/*      SPRMCC (0x20004064)                                                     */
/*      SPRUCC (0x20004064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 3 Address Match Register
*/


#define EV3AMCH_NPK_TPH_REG 0x1E124064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrmt : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 3 Address Match Register: Address match.
                               Specifies the match value for the address. This
                               effectively maps this event detector to a
                               specific master, channel, and packet type.
                            */

  } Bits;
  UINT32 Data;

} EV3AMCH_NPK_TPH_STRUCT;

/* EV3AMSK_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004068)                                                      */
/*      SPRB0 (0x20004068)                                                      */
/*      SPRHBM (0x20004068)                                                     */
/*      SPRC0 (0x20004068)                                                      */
/*      SPRMCC (0x20004068)                                                     */
/*      SPRUCC (0x20004068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 3 Address Mask Register
*/


#define EV3AMSK_NPK_TPH_REG 0x1E124068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addrmk : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 3 Address Mask Register:Address mask.
                               Specifies the mask value for the address. 0:
                               Means the corresponding bit is not included in
                               the match. 1: Means the corresponding bit is
                               included in the match. Note: To trigger an event
                               at least one bit of this register has to be set
                            */

  } Bits;
  UINT32 Data;

} EV3AMSK_NPK_TPH_STRUCT;

/* EV3DMCH_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x2000406c)                                                      */
/*      SPRB0 (0x2000406c)                                                      */
/*      SPRHBM (0x2000406c)                                                     */
/*      SPRC0 (0x2000406c)                                                      */
/*      SPRMCC (0x2000406c)                                                     */
/*      SPRUCC (0x2000406c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 3 Data Match Register
*/


#define EV3DMCH_NPK_TPH_REG 0x1E12406C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 datamt : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 3 Data Match Register: Data match.
                               Specifies the match value for the data.
                            */

  } Bits;
  UINT32 Data;

} EV3DMCH_NPK_TPH_STRUCT;

/* EV3DMSK_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004070)                                                      */
/*      SPRB0 (0x20004070)                                                      */
/*      SPRHBM (0x20004070)                                                     */
/*      SPRC0 (0x20004070)                                                      */
/*      SPRMCC (0x20004070)                                                     */
/*      SPRUCC (0x20004070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 3 Data Match Register
*/


#define EV3DMSK_NPK_TPH_REG 0x1E124070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 datamsk : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Event 3 Data Mask Register:Data Mask. Specifies
                               the mask value for the data. 0: Means the
                               corresponding bit is not included in the match.
                               1: Means the corresponding bit is included in
                               the match. Note: To trigger an event at least
                               one bit of this register has to be set
                            */

  } Bits;
  UINT32 Data;

} EV3DMSK_NPK_TPH_STRUCT;

/* EV3CTRL_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004074)                                                      */
/*      SPRB0 (0x20004074)                                                      */
/*      SPRHBM (0x20004074)                                                     */
/*      SPRC0 (0x20004074)                                                      */
/*      SPRMCC (0x20004074)                                                     */
/*      SPRUCC (0x20004074)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Event 3 Match/Mask Control Register
*/


#define EV3CTRL_NPK_TPH_REG 0x1E124074

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enevt3 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Enable Event 3. 0: do not generate Event 3
                               signal to CTS when Event 3 is detected 1:
                               generate Event 3 signal to CTS when Event 3 is
                               detected.
                            */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 andmatch : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               AND Match: Sets the type of the trigger match
                               function. 0:The match function is OR. When any
                               bit enabled by its mask bit matches the value of
                               its match bit the Match output is asserted. 1:
                               The match function is AND. All bits enabled by
                               their mask bits must match the value of their
                               match bits for the Match output to be asserted.
                            */
    UINT32 invmatch : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Invert Match: When set, makes the trigger match
                               function either NAND or NOR, depending upon the
                               state of bit 2. 0: The match function type is as
                               specified by bit 2. 1: The match function type
                               is inverted from the specification of bit 2.
                            */
    UINT32 bar_msk : 2;

                            /* Bits[5:4], Access Type=RW, default=0x00000000*/

                            /*
                               BAR Mask bits. Specifies the mask value for the
                               BAR (Base Address Register). 0: Means the
                               corresponding bit is not included in the match.
                               1: Means the corresponding bit is included in
                               the match.
                            */
    UINT32 bar_mch : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /*
                               Event 3 Match/Mask Control Register: BAR Match
                               bits. Specifies the match value for the BAR
                               (Base Address Register). 00: BAR 0, CSR_MTB_BAR
                               01: BAR 1, STMR BAR. 10: BAR 2, RTIT BAR 11: BAR
                               3, Firmware BAR (FW_BAR)
                            */
    UINT32 rsvd_8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} EV3CTRL_NPK_TPH_STRUCT;

/* STHMISC_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x20004078)                                                      */
/*      SPRB0 (0x20004078)                                                      */
/*      SPRHBM (0x20004078)                                                     */
/*      SPRC0 (0x20004078)                                                      */
/*      SPRMCC (0x20004078)                                                     */
/*      SPRUCC (0x20004078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* STH MISC Control register 1
*/


#define STHMISC_NPK_TPH_REG 0x1E124078

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Reserved. This bit is implemented in RTL, but
                               connects to nothing (so it is a RW bit, like a
                               scratchpad bit, and that's it)
                            */
    UINT32 bdcdis : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Backpressure Duration Counter Disable. This bit
                               should be set whenever RTIT data is being sent
                               to the STH. When set, this bit disables the BDC,
                               and STH will not enter Drop Mode due to BDC
                               expiry. This is required so that STH does not
                               drop any RTIT data; rather, RTIT data can only
                               be dropped at/by the CPU.
                            */
    UINT32 rsvd_2 : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} STHMISC_NPK_TPH_STRUCT;

/* STHCAP2_NPK_TPH_REG supported on:                                            */
/*      SPRA0 (0x2000407c)                                                      */
/*      SPRB0 (0x2000407c)                                                      */
/*      SPRHBM (0x2000407c)                                                     */
/*      SPRC0 (0x2000407c)                                                      */
/*      SPRMCC (0x2000407c)                                                     */
/*      SPRUCC (0x2000407c)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* STH Capability 2
*/


#define STHCAP2_NPK_TPH_REG 0x1E12407C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fw_mstr_strt : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000010*/

                            /*
                               Firmware Master Start. Specifies the starting
                               Master number for the firmware ACPI BAR If the
                               firmware ACPI BAR is not present, this field
                               will read all zeros.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} STHCAP2_NPK_TPH_STRUCT;

/* SR1_NPK_TPH_REG supported on:                                                */
/*      SPRA0 (0x20004080)                                                      */
/*      SPRB0 (0x20004080)                                                      */
/*      SPRHBM (0x20004080)                                                     */
/*      SPRC0 (0x20004080)                                                      */
/*      SPRMCC (0x20004080)                                                     */
/*      SPRUCC (0x20004080)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 1
*/


#define SR1_NPK_TPH_REG 0x1E124080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR1_NPK_TPH_STRUCT;

/* SR2_NPK_TPH_REG supported on:                                                */
/*      SPRA0 (0x20004084)                                                      */
/*      SPRB0 (0x20004084)                                                      */
/*      SPRHBM (0x20004084)                                                     */
/*      SPRC0 (0x20004084)                                                      */
/*      SPRMCC (0x20004084)                                                     */
/*      SPRUCC (0x20004084)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 2
*/


#define SR2_NPK_TPH_REG 0x1E124084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR2_NPK_TPH_STRUCT;

/* SR3_NPK_TPH_REG supported on:                                                */
/*      SPRA0 (0x20004088)                                                      */
/*      SPRB0 (0x20004088)                                                      */
/*      SPRHBM (0x20004088)                                                     */
/*      SPRC0 (0x20004088)                                                      */
/*      SPRMCC (0x20004088)                                                     */
/*      SPRUCC (0x20004088)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 3
*/


#define SR3_NPK_TPH_REG 0x1E124088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR3_NPK_TPH_STRUCT;

/* SR4_NPK_TPH_REG supported on:                                                */
/*      SPRA0 (0x2000408c)                                                      */
/*      SPRB0 (0x2000408c)                                                      */
/*      SPRHBM (0x2000408c)                                                     */
/*      SPRC0 (0x2000408c)                                                      */
/*      SPRMCC (0x2000408c)                                                     */
/*      SPRUCC (0x2000408c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 4
*/


#define SR4_NPK_TPH_REG 0x1E12408C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR4_NPK_TPH_STRUCT;

/* SR5_NPK_TPH_REG supported on:                                                */
/*      SPRA0 (0x20004090)                                                      */
/*      SPRB0 (0x20004090)                                                      */
/*      SPRHBM (0x20004090)                                                     */
/*      SPRC0 (0x20004090)                                                      */
/*      SPRMCC (0x20004090)                                                     */
/*      SPRUCC (0x20004090)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 5
*/


#define SR5_NPK_TPH_REG 0x1E124090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR5_NPK_TPH_STRUCT;

/* SR6_NPK_TPH_REG supported on:                                                */
/*      SPRA0 (0x20004094)                                                      */
/*      SPRB0 (0x20004094)                                                      */
/*      SPRHBM (0x20004094)                                                     */
/*      SPRC0 (0x20004094)                                                      */
/*      SPRMCC (0x20004094)                                                     */
/*      SPRUCC (0x20004094)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 6
*/


#define SR6_NPK_TPH_REG 0x1E124094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR6_NPK_TPH_STRUCT;

/* SR7_NPK_TPH_REG supported on:                                                */
/*      SPRA0 (0x20004098)                                                      */
/*      SPRB0 (0x20004098)                                                      */
/*      SPRHBM (0x20004098)                                                     */
/*      SPRC0 (0x20004098)                                                      */
/*      SPRMCC (0x20004098)                                                     */
/*      SPRUCC (0x20004098)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 7
*/


#define SR7_NPK_TPH_REG 0x1E124098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR7_NPK_TPH_STRUCT;

/* SR8_NPK_TPH_REG supported on:                                                */
/*      SPRA0 (0x2000409c)                                                      */
/*      SPRB0 (0x2000409c)                                                      */
/*      SPRHBM (0x2000409c)                                                     */
/*      SPRC0 (0x2000409c)                                                      */
/*      SPRMCC (0x2000409c)                                                     */
/*      SPRUCC (0x2000409c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 8
*/


#define SR8_NPK_TPH_REG 0x1E12409C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR8_NPK_TPH_STRUCT;

/* SR9_NPK_TPH_REG supported on:                                                */
/*      SPRA0 (0x200040a0)                                                      */
/*      SPRB0 (0x200040a0)                                                      */
/*      SPRHBM (0x200040a0)                                                     */
/*      SPRC0 (0x200040a0)                                                      */
/*      SPRMCC (0x200040a0)                                                     */
/*      SPRUCC (0x200040a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 9
*/


#define SR9_NPK_TPH_REG 0x1E1240A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR9_NPK_TPH_STRUCT;

/* SR10_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040a4)                                                      */
/*      SPRB0 (0x200040a4)                                                      */
/*      SPRHBM (0x200040a4)                                                     */
/*      SPRC0 (0x200040a4)                                                      */
/*      SPRMCC (0x200040a4)                                                     */
/*      SPRUCC (0x200040a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 10
*/


#define SR10_NPK_TPH_REG 0x1E1240A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR10_NPK_TPH_STRUCT;

/* SR11_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040a8)                                                      */
/*      SPRB0 (0x200040a8)                                                      */
/*      SPRHBM (0x200040a8)                                                     */
/*      SPRC0 (0x200040a8)                                                      */
/*      SPRMCC (0x200040a8)                                                     */
/*      SPRUCC (0x200040a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 11
*/


#define SR11_NPK_TPH_REG 0x1E1240A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR11_NPK_TPH_STRUCT;

/* SR12_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040ac)                                                      */
/*      SPRB0 (0x200040ac)                                                      */
/*      SPRHBM (0x200040ac)                                                     */
/*      SPRC0 (0x200040ac)                                                      */
/*      SPRMCC (0x200040ac)                                                     */
/*      SPRUCC (0x200040ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 12
*/


#define SR12_NPK_TPH_REG 0x1E1240AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR12_NPK_TPH_STRUCT;

/* SR13_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040b0)                                                      */
/*      SPRB0 (0x200040b0)                                                      */
/*      SPRHBM (0x200040b0)                                                     */
/*      SPRC0 (0x200040b0)                                                      */
/*      SPRMCC (0x200040b0)                                                     */
/*      SPRUCC (0x200040b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 13
*/


#define SR13_NPK_TPH_REG 0x1E1240B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR13_NPK_TPH_STRUCT;

/* SR14_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040b4)                                                      */
/*      SPRB0 (0x200040b4)                                                      */
/*      SPRHBM (0x200040b4)                                                     */
/*      SPRC0 (0x200040b4)                                                      */
/*      SPRMCC (0x200040b4)                                                     */
/*      SPRUCC (0x200040b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 14
*/


#define SR14_NPK_TPH_REG 0x1E1240B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR14_NPK_TPH_STRUCT;

/* SR15_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040b8)                                                      */
/*      SPRB0 (0x200040b8)                                                      */
/*      SPRHBM (0x200040b8)                                                     */
/*      SPRC0 (0x200040b8)                                                      */
/*      SPRMCC (0x200040b8)                                                     */
/*      SPRUCC (0x200040b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 15
*/


#define SR15_NPK_TPH_REG 0x1E1240B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR15_NPK_TPH_STRUCT;

/* SR16_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040bc)                                                      */
/*      SPRB0 (0x200040bc)                                                      */
/*      SPRHBM (0x200040bc)                                                     */
/*      SPRC0 (0x200040bc)                                                      */
/*      SPRMCC (0x200040bc)                                                     */
/*      SPRUCC (0x200040bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 16
*/


#define SR16_NPK_TPH_REG 0x1E1240BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR16_NPK_TPH_STRUCT;

/* SR17_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040c0)                                                      */
/*      SPRB0 (0x200040c0)                                                      */
/*      SPRHBM (0x200040c0)                                                     */
/*      SPRC0 (0x200040c0)                                                      */
/*      SPRMCC (0x200040c0)                                                     */
/*      SPRUCC (0x200040c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 17
*/


#define SR17_NPK_TPH_REG 0x1E1240C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR17_NPK_TPH_STRUCT;

/* SR18_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040c4)                                                      */
/*      SPRB0 (0x200040c4)                                                      */
/*      SPRHBM (0x200040c4)                                                     */
/*      SPRC0 (0x200040c4)                                                      */
/*      SPRMCC (0x200040c4)                                                     */
/*      SPRUCC (0x200040c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 18
*/


#define SR18_NPK_TPH_REG 0x1E1240C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR18_NPK_TPH_STRUCT;

/* SR19_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040c8)                                                      */
/*      SPRB0 (0x200040c8)                                                      */
/*      SPRHBM (0x200040c8)                                                     */
/*      SPRC0 (0x200040c8)                                                      */
/*      SPRMCC (0x200040c8)                                                     */
/*      SPRUCC (0x200040c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 19
*/


#define SR19_NPK_TPH_REG 0x1E1240C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR19_NPK_TPH_STRUCT;

/* SR20_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040cc)                                                      */
/*      SPRB0 (0x200040cc)                                                      */
/*      SPRHBM (0x200040cc)                                                     */
/*      SPRC0 (0x200040cc)                                                      */
/*      SPRMCC (0x200040cc)                                                     */
/*      SPRUCC (0x200040cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 20
*/


#define SR20_NPK_TPH_REG 0x1E1240CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR20_NPK_TPH_STRUCT;

/* SR21_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040d0)                                                      */
/*      SPRB0 (0x200040d0)                                                      */
/*      SPRHBM (0x200040d0)                                                     */
/*      SPRC0 (0x200040d0)                                                      */
/*      SPRMCC (0x200040d0)                                                     */
/*      SPRUCC (0x200040d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 21
*/


#define SR21_NPK_TPH_REG 0x1E1240D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR21_NPK_TPH_STRUCT;

/* SR22_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040d4)                                                      */
/*      SPRB0 (0x200040d4)                                                      */
/*      SPRHBM (0x200040d4)                                                     */
/*      SPRC0 (0x200040d4)                                                      */
/*      SPRMCC (0x200040d4)                                                     */
/*      SPRUCC (0x200040d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 22
*/


#define SR22_NPK_TPH_REG 0x1E1240D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR22_NPK_TPH_STRUCT;

/* SR23_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040d8)                                                      */
/*      SPRB0 (0x200040d8)                                                      */
/*      SPRHBM (0x200040d8)                                                     */
/*      SPRC0 (0x200040d8)                                                      */
/*      SPRMCC (0x200040d8)                                                     */
/*      SPRUCC (0x200040d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 23
*/


#define SR23_NPK_TPH_REG 0x1E1240D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR23_NPK_TPH_STRUCT;

/* SR24_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040dc)                                                      */
/*      SPRB0 (0x200040dc)                                                      */
/*      SPRHBM (0x200040dc)                                                     */
/*      SPRC0 (0x200040dc)                                                      */
/*      SPRMCC (0x200040dc)                                                     */
/*      SPRUCC (0x200040dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 24
*/


#define SR24_NPK_TPH_REG 0x1E1240DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR24_NPK_TPH_STRUCT;

/* SR25_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040e0)                                                      */
/*      SPRB0 (0x200040e0)                                                      */
/*      SPRHBM (0x200040e0)                                                     */
/*      SPRC0 (0x200040e0)                                                      */
/*      SPRMCC (0x200040e0)                                                     */
/*      SPRUCC (0x200040e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 25
*/


#define SR25_NPK_TPH_REG 0x1E1240E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR25_NPK_TPH_STRUCT;

/* SR26_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040e4)                                                      */
/*      SPRB0 (0x200040e4)                                                      */
/*      SPRHBM (0x200040e4)                                                     */
/*      SPRC0 (0x200040e4)                                                      */
/*      SPRMCC (0x200040e4)                                                     */
/*      SPRUCC (0x200040e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 26
*/


#define SR26_NPK_TPH_REG 0x1E1240E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR26_NPK_TPH_STRUCT;

/* SR27_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040e8)                                                      */
/*      SPRB0 (0x200040e8)                                                      */
/*      SPRHBM (0x200040e8)                                                     */
/*      SPRC0 (0x200040e8)                                                      */
/*      SPRMCC (0x200040e8)                                                     */
/*      SPRUCC (0x200040e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 27
*/


#define SR27_NPK_TPH_REG 0x1E1240E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR27_NPK_TPH_STRUCT;

/* SR28_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040ec)                                                      */
/*      SPRB0 (0x200040ec)                                                      */
/*      SPRHBM (0x200040ec)                                                     */
/*      SPRC0 (0x200040ec)                                                      */
/*      SPRMCC (0x200040ec)                                                     */
/*      SPRUCC (0x200040ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 28
*/


#define SR28_NPK_TPH_REG 0x1E1240EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR28_NPK_TPH_STRUCT;

/* SR29_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040f0)                                                      */
/*      SPRB0 (0x200040f0)                                                      */
/*      SPRHBM (0x200040f0)                                                     */
/*      SPRC0 (0x200040f0)                                                      */
/*      SPRMCC (0x200040f0)                                                     */
/*      SPRUCC (0x200040f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 29
*/


#define SR29_NPK_TPH_REG 0x1E1240F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR29_NPK_TPH_STRUCT;

/* SR30_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040f4)                                                      */
/*      SPRB0 (0x200040f4)                                                      */
/*      SPRHBM (0x200040f4)                                                     */
/*      SPRC0 (0x200040f4)                                                      */
/*      SPRMCC (0x200040f4)                                                     */
/*      SPRUCC (0x200040f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 30
*/


#define SR30_NPK_TPH_REG 0x1E1240F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR30_NPK_TPH_STRUCT;

/* SR31_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040f8)                                                      */
/*      SPRB0 (0x200040f8)                                                      */
/*      SPRHBM (0x200040f8)                                                     */
/*      SPRC0 (0x200040f8)                                                      */
/*      SPRMCC (0x200040f8)                                                     */
/*      SPRUCC (0x200040f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 31
*/


#define SR31_NPK_TPH_REG 0x1E1240F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR31_NPK_TPH_STRUCT;

/* SR32_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200040fc)                                                      */
/*      SPRB0 (0x200040fc)                                                      */
/*      SPRHBM (0x200040fc)                                                     */
/*      SPRC0 (0x200040fc)                                                      */
/*      SPRMCC (0x200040fc)                                                     */
/*      SPRUCC (0x200040fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 32
*/


#define SR32_NPK_TPH_REG 0x1E1240FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR32_NPK_TPH_STRUCT;

/* SR33_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004100)                                                      */
/*      SPRB0 (0x20004100)                                                      */
/*      SPRHBM (0x20004100)                                                     */
/*      SPRC0 (0x20004100)                                                      */
/*      SPRMCC (0x20004100)                                                     */
/*      SPRUCC (0x20004100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 33
*/


#define SR33_NPK_TPH_REG 0x1E124100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR33_NPK_TPH_STRUCT;

/* SR34_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004104)                                                      */
/*      SPRB0 (0x20004104)                                                      */
/*      SPRHBM (0x20004104)                                                     */
/*      SPRC0 (0x20004104)                                                      */
/*      SPRMCC (0x20004104)                                                     */
/*      SPRUCC (0x20004104)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 34
*/


#define SR34_NPK_TPH_REG 0x1E124104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR34_NPK_TPH_STRUCT;

/* SR35_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004108)                                                      */
/*      SPRB0 (0x20004108)                                                      */
/*      SPRHBM (0x20004108)                                                     */
/*      SPRC0 (0x20004108)                                                      */
/*      SPRMCC (0x20004108)                                                     */
/*      SPRUCC (0x20004108)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 35
*/


#define SR35_NPK_TPH_REG 0x1E124108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR35_NPK_TPH_STRUCT;

/* SR36_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x2000410c)                                                      */
/*      SPRB0 (0x2000410c)                                                      */
/*      SPRHBM (0x2000410c)                                                     */
/*      SPRC0 (0x2000410c)                                                      */
/*      SPRMCC (0x2000410c)                                                     */
/*      SPRUCC (0x2000410c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 36
*/


#define SR36_NPK_TPH_REG 0x1E12410C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR36_NPK_TPH_STRUCT;

/* SR37_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004110)                                                      */
/*      SPRB0 (0x20004110)                                                      */
/*      SPRHBM (0x20004110)                                                     */
/*      SPRC0 (0x20004110)                                                      */
/*      SPRMCC (0x20004110)                                                     */
/*      SPRUCC (0x20004110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 37
*/


#define SR37_NPK_TPH_REG 0x1E124110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR37_NPK_TPH_STRUCT;

/* SR38_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004114)                                                      */
/*      SPRB0 (0x20004114)                                                      */
/*      SPRHBM (0x20004114)                                                     */
/*      SPRC0 (0x20004114)                                                      */
/*      SPRMCC (0x20004114)                                                     */
/*      SPRUCC (0x20004114)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 38
*/


#define SR38_NPK_TPH_REG 0x1E124114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR38_NPK_TPH_STRUCT;

/* SR39_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004118)                                                      */
/*      SPRB0 (0x20004118)                                                      */
/*      SPRHBM (0x20004118)                                                     */
/*      SPRC0 (0x20004118)                                                      */
/*      SPRMCC (0x20004118)                                                     */
/*      SPRUCC (0x20004118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 39
*/


#define SR39_NPK_TPH_REG 0x1E124118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR39_NPK_TPH_STRUCT;

/* SR40_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x2000411c)                                                      */
/*      SPRB0 (0x2000411c)                                                      */
/*      SPRHBM (0x2000411c)                                                     */
/*      SPRC0 (0x2000411c)                                                      */
/*      SPRMCC (0x2000411c)                                                     */
/*      SPRUCC (0x2000411c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 40
*/


#define SR40_NPK_TPH_REG 0x1E12411C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR40_NPK_TPH_STRUCT;

/* SR41_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004120)                                                      */
/*      SPRB0 (0x20004120)                                                      */
/*      SPRHBM (0x20004120)                                                     */
/*      SPRC0 (0x20004120)                                                      */
/*      SPRMCC (0x20004120)                                                     */
/*      SPRUCC (0x20004120)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 41
*/


#define SR41_NPK_TPH_REG 0x1E124120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR41_NPK_TPH_STRUCT;

/* SR42_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004124)                                                      */
/*      SPRB0 (0x20004124)                                                      */
/*      SPRHBM (0x20004124)                                                     */
/*      SPRC0 (0x20004124)                                                      */
/*      SPRMCC (0x20004124)                                                     */
/*      SPRUCC (0x20004124)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 42
*/


#define SR42_NPK_TPH_REG 0x1E124124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR42_NPK_TPH_STRUCT;

/* SR43_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004128)                                                      */
/*      SPRB0 (0x20004128)                                                      */
/*      SPRHBM (0x20004128)                                                     */
/*      SPRC0 (0x20004128)                                                      */
/*      SPRMCC (0x20004128)                                                     */
/*      SPRUCC (0x20004128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 43
*/


#define SR43_NPK_TPH_REG 0x1E124128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR43_NPK_TPH_STRUCT;

/* SR44_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x2000412c)                                                      */
/*      SPRB0 (0x2000412c)                                                      */
/*      SPRHBM (0x2000412c)                                                     */
/*      SPRC0 (0x2000412c)                                                      */
/*      SPRMCC (0x2000412c)                                                     */
/*      SPRUCC (0x2000412c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 44
*/


#define SR44_NPK_TPH_REG 0x1E12412C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR44_NPK_TPH_STRUCT;

/* SR45_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004130)                                                      */
/*      SPRB0 (0x20004130)                                                      */
/*      SPRHBM (0x20004130)                                                     */
/*      SPRC0 (0x20004130)                                                      */
/*      SPRMCC (0x20004130)                                                     */
/*      SPRUCC (0x20004130)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 45
*/


#define SR45_NPK_TPH_REG 0x1E124130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR45_NPK_TPH_STRUCT;

/* SR46_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004134)                                                      */
/*      SPRB0 (0x20004134)                                                      */
/*      SPRHBM (0x20004134)                                                     */
/*      SPRC0 (0x20004134)                                                      */
/*      SPRMCC (0x20004134)                                                     */
/*      SPRUCC (0x20004134)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 46
*/


#define SR46_NPK_TPH_REG 0x1E124134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR46_NPK_TPH_STRUCT;

/* SR47_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004138)                                                      */
/*      SPRB0 (0x20004138)                                                      */
/*      SPRHBM (0x20004138)                                                     */
/*      SPRC0 (0x20004138)                                                      */
/*      SPRMCC (0x20004138)                                                     */
/*      SPRUCC (0x20004138)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 47
*/


#define SR47_NPK_TPH_REG 0x1E124138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR47_NPK_TPH_STRUCT;

/* SR48_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x2000413c)                                                      */
/*      SPRB0 (0x2000413c)                                                      */
/*      SPRHBM (0x2000413c)                                                     */
/*      SPRC0 (0x2000413c)                                                      */
/*      SPRMCC (0x2000413c)                                                     */
/*      SPRUCC (0x2000413c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 48
*/


#define SR48_NPK_TPH_REG 0x1E12413C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR48_NPK_TPH_STRUCT;

/* SR49_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004140)                                                      */
/*      SPRB0 (0x20004140)                                                      */
/*      SPRHBM (0x20004140)                                                     */
/*      SPRC0 (0x20004140)                                                      */
/*      SPRMCC (0x20004140)                                                     */
/*      SPRUCC (0x20004140)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 49
*/


#define SR49_NPK_TPH_REG 0x1E124140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR49_NPK_TPH_STRUCT;

/* SR50_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004144)                                                      */
/*      SPRB0 (0x20004144)                                                      */
/*      SPRHBM (0x20004144)                                                     */
/*      SPRC0 (0x20004144)                                                      */
/*      SPRMCC (0x20004144)                                                     */
/*      SPRUCC (0x20004144)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 50
*/


#define SR50_NPK_TPH_REG 0x1E124144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR50_NPK_TPH_STRUCT;

/* SR51_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004148)                                                      */
/*      SPRB0 (0x20004148)                                                      */
/*      SPRHBM (0x20004148)                                                     */
/*      SPRC0 (0x20004148)                                                      */
/*      SPRMCC (0x20004148)                                                     */
/*      SPRUCC (0x20004148)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 51
*/


#define SR51_NPK_TPH_REG 0x1E124148

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR51_NPK_TPH_STRUCT;

/* SR52_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x2000414c)                                                      */
/*      SPRB0 (0x2000414c)                                                      */
/*      SPRHBM (0x2000414c)                                                     */
/*      SPRC0 (0x2000414c)                                                      */
/*      SPRMCC (0x2000414c)                                                     */
/*      SPRUCC (0x2000414c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 52
*/


#define SR52_NPK_TPH_REG 0x1E12414C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR52_NPK_TPH_STRUCT;

/* SR53_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004150)                                                      */
/*      SPRB0 (0x20004150)                                                      */
/*      SPRHBM (0x20004150)                                                     */
/*      SPRC0 (0x20004150)                                                      */
/*      SPRMCC (0x20004150)                                                     */
/*      SPRUCC (0x20004150)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 53
*/


#define SR53_NPK_TPH_REG 0x1E124150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR53_NPK_TPH_STRUCT;

/* SR54_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004154)                                                      */
/*      SPRB0 (0x20004154)                                                      */
/*      SPRHBM (0x20004154)                                                     */
/*      SPRC0 (0x20004154)                                                      */
/*      SPRMCC (0x20004154)                                                     */
/*      SPRUCC (0x20004154)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 54
*/


#define SR54_NPK_TPH_REG 0x1E124154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR54_NPK_TPH_STRUCT;

/* SR55_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004158)                                                      */
/*      SPRB0 (0x20004158)                                                      */
/*      SPRHBM (0x20004158)                                                     */
/*      SPRC0 (0x20004158)                                                      */
/*      SPRMCC (0x20004158)                                                     */
/*      SPRUCC (0x20004158)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 55
*/


#define SR55_NPK_TPH_REG 0x1E124158

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR55_NPK_TPH_STRUCT;

/* SR56_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x2000415c)                                                      */
/*      SPRB0 (0x2000415c)                                                      */
/*      SPRHBM (0x2000415c)                                                     */
/*      SPRC0 (0x2000415c)                                                      */
/*      SPRMCC (0x2000415c)                                                     */
/*      SPRUCC (0x2000415c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 56
*/


#define SR56_NPK_TPH_REG 0x1E12415C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR56_NPK_TPH_STRUCT;

/* SR57_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004160)                                                      */
/*      SPRB0 (0x20004160)                                                      */
/*      SPRHBM (0x20004160)                                                     */
/*      SPRC0 (0x20004160)                                                      */
/*      SPRMCC (0x20004160)                                                     */
/*      SPRUCC (0x20004160)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 57
*/


#define SR57_NPK_TPH_REG 0x1E124160

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR57_NPK_TPH_STRUCT;

/* SR58_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004164)                                                      */
/*      SPRB0 (0x20004164)                                                      */
/*      SPRHBM (0x20004164)                                                     */
/*      SPRC0 (0x20004164)                                                      */
/*      SPRMCC (0x20004164)                                                     */
/*      SPRUCC (0x20004164)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 58
*/


#define SR58_NPK_TPH_REG 0x1E124164

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR58_NPK_TPH_STRUCT;

/* SR59_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004168)                                                      */
/*      SPRB0 (0x20004168)                                                      */
/*      SPRHBM (0x20004168)                                                     */
/*      SPRC0 (0x20004168)                                                      */
/*      SPRMCC (0x20004168)                                                     */
/*      SPRUCC (0x20004168)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 59
*/


#define SR59_NPK_TPH_REG 0x1E124168

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR59_NPK_TPH_STRUCT;

/* SR60_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x2000416c)                                                      */
/*      SPRB0 (0x2000416c)                                                      */
/*      SPRHBM (0x2000416c)                                                     */
/*      SPRC0 (0x2000416c)                                                      */
/*      SPRMCC (0x2000416c)                                                     */
/*      SPRUCC (0x2000416c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 60
*/


#define SR60_NPK_TPH_REG 0x1E12416C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR60_NPK_TPH_STRUCT;

/* SR61_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004170)                                                      */
/*      SPRB0 (0x20004170)                                                      */
/*      SPRHBM (0x20004170)                                                     */
/*      SPRC0 (0x20004170)                                                      */
/*      SPRMCC (0x20004170)                                                     */
/*      SPRUCC (0x20004170)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 61
*/


#define SR61_NPK_TPH_REG 0x1E124170

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR61_NPK_TPH_STRUCT;

/* SR62_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004174)                                                      */
/*      SPRB0 (0x20004174)                                                      */
/*      SPRHBM (0x20004174)                                                     */
/*      SPRC0 (0x20004174)                                                      */
/*      SPRMCC (0x20004174)                                                     */
/*      SPRUCC (0x20004174)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 62
*/


#define SR62_NPK_TPH_REG 0x1E124174

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR62_NPK_TPH_STRUCT;

/* SR63_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004178)                                                      */
/*      SPRB0 (0x20004178)                                                      */
/*      SPRHBM (0x20004178)                                                     */
/*      SPRC0 (0x20004178)                                                      */
/*      SPRMCC (0x20004178)                                                     */
/*      SPRUCC (0x20004178)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 63
*/


#define SR63_NPK_TPH_REG 0x1E124178

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR63_NPK_TPH_STRUCT;

/* SR64_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x2000417c)                                                      */
/*      SPRB0 (0x2000417c)                                                      */
/*      SPRHBM (0x2000417c)                                                     */
/*      SPRC0 (0x2000417c)                                                      */
/*      SPRMCC (0x2000417c)                                                     */
/*      SPRUCC (0x2000417c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 64
*/


#define SR64_NPK_TPH_REG 0x1E12417C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR64_NPK_TPH_STRUCT;

/* SR65_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004180)                                                      */
/*      SPRB0 (0x20004180)                                                      */
/*      SPRHBM (0x20004180)                                                     */
/*      SPRC0 (0x20004180)                                                      */
/*      SPRMCC (0x20004180)                                                     */
/*      SPRUCC (0x20004180)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 65
*/


#define SR65_NPK_TPH_REG 0x1E124180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR65_NPK_TPH_STRUCT;

/* SR66_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004184)                                                      */
/*      SPRB0 (0x20004184)                                                      */
/*      SPRHBM (0x20004184)                                                     */
/*      SPRC0 (0x20004184)                                                      */
/*      SPRMCC (0x20004184)                                                     */
/*      SPRUCC (0x20004184)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 66
*/


#define SR66_NPK_TPH_REG 0x1E124184

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR66_NPK_TPH_STRUCT;

/* SR67_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004188)                                                      */
/*      SPRB0 (0x20004188)                                                      */
/*      SPRHBM (0x20004188)                                                     */
/*      SPRC0 (0x20004188)                                                      */
/*      SPRMCC (0x20004188)                                                     */
/*      SPRUCC (0x20004188)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 67
*/


#define SR67_NPK_TPH_REG 0x1E124188

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR67_NPK_TPH_STRUCT;

/* SR68_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x2000418c)                                                      */
/*      SPRB0 (0x2000418c)                                                      */
/*      SPRHBM (0x2000418c)                                                     */
/*      SPRC0 (0x2000418c)                                                      */
/*      SPRMCC (0x2000418c)                                                     */
/*      SPRUCC (0x2000418c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 68
*/


#define SR68_NPK_TPH_REG 0x1E12418C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR68_NPK_TPH_STRUCT;

/* SR69_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004190)                                                      */
/*      SPRB0 (0x20004190)                                                      */
/*      SPRHBM (0x20004190)                                                     */
/*      SPRC0 (0x20004190)                                                      */
/*      SPRMCC (0x20004190)                                                     */
/*      SPRUCC (0x20004190)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 69
*/


#define SR69_NPK_TPH_REG 0x1E124190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR69_NPK_TPH_STRUCT;

/* SR70_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004194)                                                      */
/*      SPRB0 (0x20004194)                                                      */
/*      SPRHBM (0x20004194)                                                     */
/*      SPRC0 (0x20004194)                                                      */
/*      SPRMCC (0x20004194)                                                     */
/*      SPRUCC (0x20004194)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 70
*/


#define SR70_NPK_TPH_REG 0x1E124194

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR70_NPK_TPH_STRUCT;

/* SR71_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x20004198)                                                      */
/*      SPRB0 (0x20004198)                                                      */
/*      SPRHBM (0x20004198)                                                     */
/*      SPRC0 (0x20004198)                                                      */
/*      SPRMCC (0x20004198)                                                     */
/*      SPRUCC (0x20004198)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 71
*/


#define SR71_NPK_TPH_REG 0x1E124198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR71_NPK_TPH_STRUCT;

/* SR72_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x2000419c)                                                      */
/*      SPRB0 (0x2000419c)                                                      */
/*      SPRHBM (0x2000419c)                                                     */
/*      SPRC0 (0x2000419c)                                                      */
/*      SPRMCC (0x2000419c)                                                     */
/*      SPRUCC (0x2000419c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 72
*/


#define SR72_NPK_TPH_REG 0x1E12419C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR72_NPK_TPH_STRUCT;

/* SR73_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041a0)                                                      */
/*      SPRB0 (0x200041a0)                                                      */
/*      SPRHBM (0x200041a0)                                                     */
/*      SPRC0 (0x200041a0)                                                      */
/*      SPRMCC (0x200041a0)                                                     */
/*      SPRUCC (0x200041a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 73
*/


#define SR73_NPK_TPH_REG 0x1E1241A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR73_NPK_TPH_STRUCT;

/* SR74_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041a4)                                                      */
/*      SPRB0 (0x200041a4)                                                      */
/*      SPRHBM (0x200041a4)                                                     */
/*      SPRC0 (0x200041a4)                                                      */
/*      SPRMCC (0x200041a4)                                                     */
/*      SPRUCC (0x200041a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 74
*/


#define SR74_NPK_TPH_REG 0x1E1241A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR74_NPK_TPH_STRUCT;

/* SR75_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041a8)                                                      */
/*      SPRB0 (0x200041a8)                                                      */
/*      SPRHBM (0x200041a8)                                                     */
/*      SPRC0 (0x200041a8)                                                      */
/*      SPRMCC (0x200041a8)                                                     */
/*      SPRUCC (0x200041a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 75
*/


#define SR75_NPK_TPH_REG 0x1E1241A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR75_NPK_TPH_STRUCT;

/* SR76_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041ac)                                                      */
/*      SPRB0 (0x200041ac)                                                      */
/*      SPRHBM (0x200041ac)                                                     */
/*      SPRC0 (0x200041ac)                                                      */
/*      SPRMCC (0x200041ac)                                                     */
/*      SPRUCC (0x200041ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 76
*/


#define SR76_NPK_TPH_REG 0x1E1241AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR76_NPK_TPH_STRUCT;

/* SR77_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041b0)                                                      */
/*      SPRB0 (0x200041b0)                                                      */
/*      SPRHBM (0x200041b0)                                                     */
/*      SPRC0 (0x200041b0)                                                      */
/*      SPRMCC (0x200041b0)                                                     */
/*      SPRUCC (0x200041b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 77
*/


#define SR77_NPK_TPH_REG 0x1E1241B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR77_NPK_TPH_STRUCT;

/* SR78_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041b4)                                                      */
/*      SPRB0 (0x200041b4)                                                      */
/*      SPRHBM (0x200041b4)                                                     */
/*      SPRC0 (0x200041b4)                                                      */
/*      SPRMCC (0x200041b4)                                                     */
/*      SPRUCC (0x200041b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 78
*/


#define SR78_NPK_TPH_REG 0x1E1241B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR78_NPK_TPH_STRUCT;

/* SR79_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041b8)                                                      */
/*      SPRB0 (0x200041b8)                                                      */
/*      SPRHBM (0x200041b8)                                                     */
/*      SPRC0 (0x200041b8)                                                      */
/*      SPRMCC (0x200041b8)                                                     */
/*      SPRUCC (0x200041b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 79
*/


#define SR79_NPK_TPH_REG 0x1E1241B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR79_NPK_TPH_STRUCT;

/* SR80_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041bc)                                                      */
/*      SPRB0 (0x200041bc)                                                      */
/*      SPRHBM (0x200041bc)                                                     */
/*      SPRC0 (0x200041bc)                                                      */
/*      SPRMCC (0x200041bc)                                                     */
/*      SPRUCC (0x200041bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 80
*/


#define SR80_NPK_TPH_REG 0x1E1241BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR80_NPK_TPH_STRUCT;

/* SR81_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041c0)                                                      */
/*      SPRB0 (0x200041c0)                                                      */
/*      SPRHBM (0x200041c0)                                                     */
/*      SPRC0 (0x200041c0)                                                      */
/*      SPRMCC (0x200041c0)                                                     */
/*      SPRUCC (0x200041c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 81
*/


#define SR81_NPK_TPH_REG 0x1E1241C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR81_NPK_TPH_STRUCT;

/* SR82_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041c4)                                                      */
/*      SPRB0 (0x200041c4)                                                      */
/*      SPRHBM (0x200041c4)                                                     */
/*      SPRC0 (0x200041c4)                                                      */
/*      SPRMCC (0x200041c4)                                                     */
/*      SPRUCC (0x200041c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 82
*/


#define SR82_NPK_TPH_REG 0x1E1241C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR82_NPK_TPH_STRUCT;

/* SR83_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041c8)                                                      */
/*      SPRB0 (0x200041c8)                                                      */
/*      SPRHBM (0x200041c8)                                                     */
/*      SPRC0 (0x200041c8)                                                      */
/*      SPRMCC (0x200041c8)                                                     */
/*      SPRUCC (0x200041c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 83
*/


#define SR83_NPK_TPH_REG 0x1E1241C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR83_NPK_TPH_STRUCT;

/* SR84_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041cc)                                                      */
/*      SPRB0 (0x200041cc)                                                      */
/*      SPRHBM (0x200041cc)                                                     */
/*      SPRC0 (0x200041cc)                                                      */
/*      SPRMCC (0x200041cc)                                                     */
/*      SPRUCC (0x200041cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 84
*/


#define SR84_NPK_TPH_REG 0x1E1241CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR84_NPK_TPH_STRUCT;

/* SR85_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041d0)                                                      */
/*      SPRB0 (0x200041d0)                                                      */
/*      SPRHBM (0x200041d0)                                                     */
/*      SPRC0 (0x200041d0)                                                      */
/*      SPRMCC (0x200041d0)                                                     */
/*      SPRUCC (0x200041d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 85
*/


#define SR85_NPK_TPH_REG 0x1E1241D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR85_NPK_TPH_STRUCT;

/* SR86_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041d4)                                                      */
/*      SPRB0 (0x200041d4)                                                      */
/*      SPRHBM (0x200041d4)                                                     */
/*      SPRC0 (0x200041d4)                                                      */
/*      SPRMCC (0x200041d4)                                                     */
/*      SPRUCC (0x200041d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 86
*/


#define SR86_NPK_TPH_REG 0x1E1241D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR86_NPK_TPH_STRUCT;

/* SR87_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041d8)                                                      */
/*      SPRB0 (0x200041d8)                                                      */
/*      SPRHBM (0x200041d8)                                                     */
/*      SPRC0 (0x200041d8)                                                      */
/*      SPRMCC (0x200041d8)                                                     */
/*      SPRUCC (0x200041d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 87
*/


#define SR87_NPK_TPH_REG 0x1E1241D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR87_NPK_TPH_STRUCT;

/* SR88_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041dc)                                                      */
/*      SPRB0 (0x200041dc)                                                      */
/*      SPRHBM (0x200041dc)                                                     */
/*      SPRC0 (0x200041dc)                                                      */
/*      SPRMCC (0x200041dc)                                                     */
/*      SPRUCC (0x200041dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 88
*/


#define SR88_NPK_TPH_REG 0x1E1241DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR88_NPK_TPH_STRUCT;

/* SR89_NPK_TPH_REG supported on:                                               */
/*      SPRA0 (0x200041e0)                                                      */
/*      SPRB0 (0x200041e0)                                                      */
/*      SPRHBM (0x200041e0)                                                     */
/*      SPRC0 (0x200041e0)                                                      */
/*      SPRMCC (0x200041e0)                                                     */
/*      SPRUCC (0x200041e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRB0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRHBM Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRC0 Security PolicyGroup: NPK_MMIO_POLICY                                  */
/* SPRMCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* SPRUCC Security PolicyGroup: NPK_MMIO_POLICY                                 */
/* Save/Restore Register 89
*/


#define SR89_NPK_TPH_REG 0x1E1241E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rtit_state : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The data in this register is the save_state data
                               for RTIT. The exact data contained in the bits
                               of this register varies according to the number
                               of logical processors supported, and the number
                               of cache lines per logical processor.The exact
                               meaning/interpretation of the data is
                               unimportant to save/restore firmware. The
                               important point is that the data that is saved
                               should be restored exactly as it was read.Any
                               bits that are unneeded will always read as zero,
                               and writes will have no effect (nor will the
                               write value be stored, because the bit will not
                               actually connect to a flop).
                            */

  } Bits;
  UINT32 Data;

} SR89_NPK_TPH_STRUCT;

/* VISACTLCLM0_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20020080)                                                      */
/*      SPRB0 (0x20020080)                                                      */
/*      SPRHBM (0x20020080)                                                     */
/*      SPRC0 (0x20020080)                                                      */
/*      SPRMCC (0x20020080)                                                     */
/*      SPRUCC (0x20020080)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLCLM0
*/


#define VISACTLCLM0_NPK_TPH_REG 0x1E120080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaclmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLCLM0_NPK_TPH_STRUCT;

/* VISACTLCLM1_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20020084)                                                      */
/*      SPRB0 (0x20020084)                                                      */
/*      SPRHBM (0x20020084)                                                     */
/*      SPRC0 (0x20020084)                                                      */
/*      SPRMCC (0x20020084)                                                     */
/*      SPRUCC (0x20020084)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLCLM1
*/


#define VISACTLCLM1_NPK_TPH_REG 0x1E120084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaclmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLCLM1_NPK_TPH_STRUCT;

/* VISACTLCLM2_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20020088)                                                      */
/*      SPRB0 (0x20020088)                                                      */
/*      SPRHBM (0x20020088)                                                     */
/*      SPRC0 (0x20020088)                                                      */
/*      SPRMCC (0x20020088)                                                     */
/*      SPRUCC (0x20020088)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLCLM2
*/


#define VISACTLCLM2_NPK_TPH_REG 0x1E120088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaclmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLCLM2_NPK_TPH_STRUCT;

/* VISACTLCLM3_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x2002008c)                                                      */
/*      SPRB0 (0x2002008c)                                                      */
/*      SPRHBM (0x2002008c)                                                     */
/*      SPRC0 (0x2002008c)                                                      */
/*      SPRMCC (0x2002008c)                                                     */
/*      SPRUCC (0x2002008c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLCLM3
*/


#define VISACTLCLM3_NPK_TPH_REG 0x1E12008C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaclmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLCLM3_NPK_TPH_STRUCT;

/* VISACTLCLM4_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20020090)                                                      */
/*      SPRB0 (0x20020090)                                                      */
/*      SPRHBM (0x20020090)                                                     */
/*      SPRC0 (0x20020090)                                                      */
/*      SPRMCC (0x20020090)                                                     */
/*      SPRUCC (0x20020090)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLCLM4
*/


#define VISACTLCLM4_NPK_TPH_REG 0x1E120090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaclmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLCLM4_NPK_TPH_STRUCT;

/* VISACTLCLM5_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20020094)                                                      */
/*      SPRB0 (0x20020094)                                                      */
/*      SPRHBM (0x20020094)                                                     */
/*      SPRC0 (0x20020094)                                                      */
/*      SPRMCC (0x20020094)                                                     */
/*      SPRUCC (0x20020094)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLCLM5
*/


#define VISACTLCLM5_NPK_TPH_REG 0x1E120094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaclmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLCLM5_NPK_TPH_STRUCT;

/* VISACTLCLM6_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20020098)                                                      */
/*      SPRB0 (0x20020098)                                                      */
/*      SPRHBM (0x20020098)                                                     */
/*      SPRC0 (0x20020098)                                                      */
/*      SPRMCC (0x20020098)                                                     */
/*      SPRUCC (0x20020098)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLCLM6
*/


#define VISACTLCLM6_NPK_TPH_REG 0x1E120098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaclmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLCLM6_NPK_TPH_STRUCT;

/* VISACTLCLM7_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x2002009c)                                                      */
/*      SPRB0 (0x2002009c)                                                      */
/*      SPRHBM (0x2002009c)                                                     */
/*      SPRC0 (0x2002009c)                                                      */
/*      SPRMCC (0x2002009c)                                                     */
/*      SPRUCC (0x2002009c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLCLM7
*/


#define VISACTLCLM7_NPK_TPH_REG 0x1E12009C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaclmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLCLM7_NPK_TPH_STRUCT;

/* VISACTLCLM8_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x200200a0)                                                      */
/*      SPRB0 (0x200200a0)                                                      */
/*      SPRHBM (0x200200a0)                                                     */
/*      SPRC0 (0x200200a0)                                                      */
/*      SPRMCC (0x200200a0)                                                     */
/*      SPRUCC (0x200200a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLCLM8
*/


#define VISACTLCLM8_NPK_TPH_REG 0x1E1200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaclmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLCLM8_NPK_TPH_STRUCT;

/* VISACTLCLM9_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x200200a4)                                                      */
/*      SPRB0 (0x200200a4)                                                      */
/*      SPRHBM (0x200200a4)                                                     */
/*      SPRC0 (0x200200a4)                                                      */
/*      SPRMCC (0x200200a4)                                                     */
/*      SPRUCC (0x200200a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLCLM9
*/


#define VISACTLCLM9_NPK_TPH_REG 0x1E1200A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaclmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLCLM9_NPK_TPH_STRUCT;

/* VISACTLULM0_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20022814)                                                      */
/*      SPRB0 (0x20022814)                                                      */
/*      SPRHBM (0x20022814)                                                     */
/*      SPRC0 (0x20022814)                                                      */
/*      SPRMCC (0x20022814)                                                     */
/*      SPRUCC (0x20022814)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLULM0
*/


#define VISACTLULM0_NPK_TPH_REG 0x1E122814

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaulmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLULM0_NPK_TPH_STRUCT;

/* VISACTLULM1_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20022818)                                                      */
/*      SPRB0 (0x20022818)                                                      */
/*      SPRHBM (0x20022818)                                                     */
/*      SPRC0 (0x20022818)                                                      */
/*      SPRMCC (0x20022818)                                                     */
/*      SPRUCC (0x20022818)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLULM1
*/


#define VISACTLULM1_NPK_TPH_REG 0x1E122818

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaulmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLULM1_NPK_TPH_STRUCT;

/* VISACTLULM2_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x2002281c)                                                      */
/*      SPRB0 (0x2002281c)                                                      */
/*      SPRHBM (0x2002281c)                                                     */
/*      SPRC0 (0x2002281c)                                                      */
/*      SPRMCC (0x2002281c)                                                     */
/*      SPRUCC (0x2002281c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLULM2
*/


#define VISACTLULM2_NPK_TPH_REG 0x1E12281C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaulmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLULM2_NPK_TPH_STRUCT;

/* VISACTLULM3_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20022820)                                                      */
/*      SPRB0 (0x20022820)                                                      */
/*      SPRHBM (0x20022820)                                                     */
/*      SPRC0 (0x20022820)                                                      */
/*      SPRMCC (0x20022820)                                                     */
/*      SPRUCC (0x20022820)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLULM3
*/


#define VISACTLULM3_NPK_TPH_REG 0x1E122820

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaulmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLULM3_NPK_TPH_STRUCT;

/* VISACTLULM4_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20022824)                                                      */
/*      SPRB0 (0x20022824)                                                      */
/*      SPRHBM (0x20022824)                                                     */
/*      SPRC0 (0x20022824)                                                      */
/*      SPRMCC (0x20022824)                                                     */
/*      SPRUCC (0x20022824)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLULM4
*/


#define VISACTLULM4_NPK_TPH_REG 0x1E122824

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaulmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLULM4_NPK_TPH_STRUCT;

/* VISACTLULM5_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20022828)                                                      */
/*      SPRB0 (0x20022828)                                                      */
/*      SPRHBM (0x20022828)                                                     */
/*      SPRC0 (0x20022828)                                                      */
/*      SPRMCC (0x20022828)                                                     */
/*      SPRUCC (0x20022828)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLULM5
*/


#define VISACTLULM5_NPK_TPH_REG 0x1E122828

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaulmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLULM5_NPK_TPH_STRUCT;

/* VISACTLULM6_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x2002282c)                                                      */
/*      SPRB0 (0x2002282c)                                                      */
/*      SPRHBM (0x2002282c)                                                     */
/*      SPRC0 (0x2002282c)                                                      */
/*      SPRMCC (0x2002282c)                                                     */
/*      SPRUCC (0x2002282c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLULM6
*/


#define VISACTLULM6_NPK_TPH_REG 0x1E12282C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaulmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLULM6_NPK_TPH_STRUCT;

/* VISACTLULM7_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20022830)                                                      */
/*      SPRB0 (0x20022830)                                                      */
/*      SPRHBM (0x20022830)                                                     */
/*      SPRC0 (0x20022830)                                                      */
/*      SPRMCC (0x20022830)                                                     */
/*      SPRUCC (0x20022830)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLULM7
*/


#define VISACTLULM7_NPK_TPH_REG 0x1E122830

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaulmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLULM7_NPK_TPH_STRUCT;

/* VISACTLULM8_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20022834)                                                      */
/*      SPRB0 (0x20022834)                                                      */
/*      SPRHBM (0x20022834)                                                     */
/*      SPRC0 (0x20022834)                                                      */
/*      SPRMCC (0x20022834)                                                     */
/*      SPRUCC (0x20022834)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLULM8
*/


#define VISACTLULM8_NPK_TPH_REG 0x1E122834

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaulmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLULM8_NPK_TPH_STRUCT;

/* VISACTLULM9_NPK_TPH_REG supported on:                                        */
/*      SPRA0 (0x20022838)                                                      */
/*      SPRB0 (0x20022838)                                                      */
/*      SPRHBM (0x20022838)                                                     */
/*      SPRC0 (0x20022838)                                                      */
/*      SPRMCC (0x20022838)                                                     */
/*      SPRUCC (0x20022838)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_tph              */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_tph                */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_tph             */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_VRC_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_VRC_POLICY                                  */
/* VISACTLULM9
*/


#define VISACTLULM9_NPK_TPH_REG 0x1E122838

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 visaulmdata : 32;

                            /* Bits[31:0], Access Type=WO, default=0x00000000*/

                            /*
                               Dummy CLM register. Added in RAL only for IP
                               validation purposes
                            */

  } Bits;
  UINT32 Data;

} VISACTLULM9_NPK_TPH_STRUCT;
#endif /* _NPK_TPH_h */
