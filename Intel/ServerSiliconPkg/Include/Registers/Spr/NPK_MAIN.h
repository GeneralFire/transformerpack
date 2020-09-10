
/** @file
  NPK_MAIN.h

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
/* NPK_PCI_POLICY                                                               */
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
/*    | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | DFX_THIRDPARTY_SAI |                  */
/*    DFX_UNTRUSTED_SAI | DFX_INTEL_MANUFACTURING_SAI |                         */
/*    DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI                                    */
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
/* NPK_PCI_POLICY                                                               */
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
/*    | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | DFX_THIRDPARTY_SAI |                  */
/*    DFX_UNTRUSTED_SAI | DFX_INTEL_MANUFACTURING_SAI |                         */
/*    DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI                                    */
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
/* NPK_PCI_POLICY                                                               */
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
/*    | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | DFX_THIRDPARTY_SAI |                  */
/*    DFX_UNTRUSTED_SAI | DFX_INTEL_MANUFACTURING_SAI |                         */
/*    DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI                                    */
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
/* NPK_PCI_POLICY                                                               */
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
/*    | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | DFX_THIRDPARTY_SAI |                  */
/*    DFX_UNTRUSTED_SAI | DFX_INTEL_MANUFACTURING_SAI |                         */
/*    DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI                                    */
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
/* NPK_PCI_POLICY                                                               */
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
/*    | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | DFX_THIRDPARTY_SAI |                  */
/*    DFX_UNTRUSTED_SAI | DFX_INTEL_MANUFACTURING_SAI |                         */
/*    DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI                                    */
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
/* NPK_PCI_POLICY                                                               */
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
/*    | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | DFX_THIRDPARTY_SAI |                  */
/*    DFX_UNTRUSTED_SAI | DFX_INTEL_MANUFACTURING_SAI |                         */
/*    DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI                                    */
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */


#ifndef _NPK_MAIN_h
#define _NPK_MAIN_h
#include <Base.h>


/* CMD_NPK_MAIN_REG supported on:                                               */
/*      SPRA0 (0x20820004)                                                      */
/*      SPRB0 (0x20820004)                                                      */
/*      SPRHBM (0x20820004)                                                     */
/*      SPRC0 (0x20820004)                                                      */
/*      SPRMCC (0x20820004)                                                     */
/*      SPRUCC (0x20820004)                                                     */
/* Register default value on SPRA0: 0x00100000                                  */
/* Register default value on SPRB0: 0x00100000                                  */
/* Register default value on SPRHBM: 0x00100000                                 */
/* Register default value on SPRC0: 0x00100000                                  */
/* Register default value on SPRMCC: 0x00100000                                 */
/* Register default value on SPRUCC: 0x00100000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg      */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg      */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg     */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg      */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_ttc_pci_cfg        */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg     */
/* Struct generated from SPRA0 BDF: 8_4_0                                       */
/* SPRA0 Security PolicyGroup: NPK_PCI_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_PCI_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_PCI_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_PCI_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_PCI_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_PCI_POLICY                                  */
/* Command Register
*/


#define CMD_NPK_MAIN_REG 0x1E020004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iose : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Memory Space Enable: When set, Memory Space
                               Decoding is enabled and memory transactions
                               targeting the device are accepted Note: The MSE
                               has to be set to accept any memory transaction
                               on the primary interface targeting any of North
                               Peak s BARs including its FW_BAR
                            */
    UINT32 bme : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bus Master Enable: When set enables the ability
                               to issue Memory or IO requests, including MSI.
                            */
    UINT32 rsvd_3 : 3;

                            /* Bits[5:3], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 pere : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Parity Error Response Enable */
    UINT32 rsvd_2 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 serren : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               System Error Enable: Setting this bit enables
                               the generation of System Error message, when
                               required through sideband interface
                            */
    UINT32 rsvd_1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 intdis : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Interrupt Disable: Disables the function to
                               generate INTx interrupt. A value of 0 enables
                               the function to generate INTA messages on IOSF
                               sideband. Note: this bit has no effect on MSI
                               generation.
                            */
    UINT32 rsvd : 5;

                            /* Bits[15:11], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 rsvd_16 : 3;

                            /* Bits[18:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 instat : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Status: Reflects the state of the
                               interrupt pin at the input of the enable/disable
                               circuit. When the interrupt is asserted, and
                               cleared when the interrupt is cleared
                               (independent of the state of Interrupt Disable
                               bit in command register. This bit is only
                               associated with the INTx messages and has no
                               meaning if the device is using MSI
                            */
    UINT32 clist : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000001*/

                            /*
                               Capabilities List: Indicates the controller
                               contains a capabilities pointer list and the
                               capability pointer register is implemented at
                               offset 0x40 in the configuration space
                            */
    UINT32 rsvd_21 : 3;

                            /* Bits[23:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mdpe : 1;

                            /* Bits[24:24], Access Type=RW/1C, default=0x00000000*/

                            /* Master Data Parity Error */
    UINT32 rsvd_25 : 2;

                            /* Bits[26:25], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sta : 1;

                            /* Bits[27:27], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Signaled Target Abort Status: Set by the device
                               when aborting a request that violates the device
                               programming model. When SERR Enable is set SERR
                               message will be send over sideband
                            */
    UINT32 rta : 1;

                            /* Bits[28:28], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Received Target Abort Status: This bit is set
                               when device receives a Completion transaction
                               with Completer Abort completion status. No error
                               will be reported
                            */
    UINT32 rma : 1;

                            /* Bits[29:29], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Received Master Abort Status: This bit is set
                               when device receives a Completion transaction
                               with Unsupported Request completion status. No
                               error will be reported
                            */
    UINT32 sse : 1;

                            /* Bits[30:30], Access Type=RW/1C, default=0x00000000*/

                            /*
                               Signaled System Error: This bit is set when the
                               device has detected an un-correctable error and
                               reported it via SERR message over sideband. This
                               requires SERR Enable bit to be set in Command
                               register.
                            */
    UINT32 dpe : 1;

                            /* Bits[31:31], Access Type=RW/1C, default=0x00000000*/

                            /* Detected Parity Error */

  } Bits;
  UINT32 Data;

} CMD_NPK_MAIN_STRUCT;



/* MTB_LBAR_NPK_MAIN_REG supported on:                                          */
/*      SPRA0 (0x20820010)                                                      */
/*      SPRB0 (0x20820010)                                                      */
/*      SPRHBM (0x20820010)                                                     */
/*      SPRC0 (0x20820010)                                                      */
/*      SPRMCC (0x20820010)                                                     */
/*      SPRUCC (0x20820010)                                                     */
/* Register default value on SPRA0: 0x00000004                                  */
/* Register default value on SPRB0: 0x00000004                                  */
/* Register default value on SPRHBM: 0x00000004                                 */
/* Register default value on SPRC0: 0x00000004                                  */
/* Register default value on SPRMCC: 0x00000004                                 */
/* Register default value on SPRUCC: 0x00000004                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg      */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg      */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg     */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg      */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_ttc_pci_cfg        */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg     */
/* Struct generated from SPRA0 BDF: 8_4_0                                       */
/* SPRA0 Security PolicyGroup: NPK_PCI_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_PCI_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_PCI_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_PCI_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_PCI_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_PCI_POLICY                                  */
/* MSC Trace Buffer Lower BAR
*/


#define MTB_LBAR_NPK_MAIN_REG 0x1E020010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 spty : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Space Type: Value of 0 indicates the BAR is
                               located in memory space
                            */
    UINT32 adrng : 2;

                            /* Bits[2:1], Access Type=RO, default=0x00000002*/

                            /*
                               Address Range: Value of 0x2 indicates that the
                               BAR is located anywhere system memory space
                               (i.e. 64-bit addressing)
                            */
    UINT32 pf : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Prefetchable: Value of 0 indicates the BAR
                               cannot be prefetched
                            */
    UINT32 rsvd : 16;

                            /* Bits[19:4], Access Type=RO, default=0x00000000*/

                            /*
                               Hardwired to 0 to indicate the memory space size
                               required by this CSR_MTB_BAR MMIO space is 1MB
                            */
    UINT32 addr : 12;

                            /* Bits[31:20], Access Type=RW, default=0x00000000*/

                            /*
                               CSR and MTB BAR (Lower). This register specifies
                               the lower 32 bits of the configurable base
                               address for CSRs (Configuration and Status
                               Registers) and MTB (Memory Trace Buffer). This
                               BAR is called BAR 0.
                            */

  } Bits;
  UINT32 Data;

} MTB_LBAR_NPK_MAIN_STRUCT;

/* MTB_UBAR_NPK_MAIN_REG supported on:                                          */
/*      SPRA0 (0x20820014)                                                      */
/*      SPRB0 (0x20820014)                                                      */
/*      SPRHBM (0x20820014)                                                     */
/*      SPRC0 (0x20820014)                                                      */
/*      SPRMCC (0x20820014)                                                     */
/*      SPRUCC (0x20820014)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg      */
/* SPRB0 Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg      */
/* SPRHBM Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg     */
/* SPRC0 Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg      */
/* SPRMCC Register File:    sprsp_top/northpeak_top_map/file_ttc_pci_cfg        */
/* SPRUCC Register File:    sprsp_top/northpeak_top_map[0]/file_ttc_pci_cfg     */
/* Struct generated from SPRA0 BDF: 8_4_0                                       */
/* SPRA0 Security PolicyGroup: NPK_PCI_POLICY                                   */
/* SPRB0 Security PolicyGroup: NPK_PCI_POLICY                                   */
/* SPRHBM Security PolicyGroup: NPK_PCI_POLICY                                  */
/* SPRC0 Security PolicyGroup: NPK_PCI_POLICY                                   */
/* SPRMCC Security PolicyGroup: NPK_PCI_POLICY                                  */
/* SPRUCC Security PolicyGroup: NPK_PCI_POLICY                                  */
/* MSC Trace Buffer Upper BAR
*/


#define MTB_UBAR_NPK_MAIN_REG 0x1E020014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               CSR and MTB BAR (Upper). This register specifies
                               the upper 32 bits of the configurable base
                               address for CSRs (Configuration and Status
                               Registers) and MTB (Memory Trace Buffer). This
                               BAR is called BAR 0.
                            */

  } Bits;
  UINT32 Data;

} MTB_UBAR_NPK_MAIN_STRUCT;




















#endif /* _NPK_MAIN_h */
