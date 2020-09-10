
/** @file
  IIO_VMD.h

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


#ifndef _IIO_VMD_h
#define _IIO_VMD_h
#include <Base.h>

/* VID_IIO_VMD_REG supported on:                                                */
/*      SPRA0 (0x10805000)                                                      */
/*      SPRB0 (0x10805000)                                                      */
/*      SPRHBM (0x10805000)                                                     */
/*      SPRC0 (0x10805000)                                                      */
/*      SPRMCC (0x10805000)                                                     */
/*      SPRUCC (0x10805000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Vendor ID
*/


#define VID_IIO_VMD_REG 0x11410000

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

} VID_IIO_VMD_STRUCT;

/* DID_IIO_VMD_REG supported on:                                                */
/*      SPRA0 (0x10805002)                                                      */
/*      SPRB0 (0x10805002)                                                      */
/*      SPRHBM (0x10805002)                                                     */
/*      SPRC0 (0x10805002)                                                      */
/*      SPRMCC (0x10805002)                                                     */
/*      SPRUCC (0x10805002)                                                     */
/* Register default value on SPRA0: 0x000028C0                                  */
/* Register default value on SPRB0: 0x000028C0                                  */
/* Register default value on SPRHBM: 0x000028C0                                 */
/* Register default value on SPRC0: 0x000028C0                                  */
/* Register default value on SPRMCC: 0x000028C0                                 */
/* Register default value on SPRUCC: 0x000028C0                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device ID
*/


#define DID_IIO_VMD_REG 0x11410002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x000028C0*/

                            /*
                               The value in this register specifies the Device
                               ID for the Volume Management Device. This value
                               is the same for all instances of the VMD.[br]
                               Volume Management Device v2.0 for Server uses
                               Device ID values 0x28C0 to 0x28CF selectable by
                               the VMCONFIG.DEVID_SELECT register field.
                            */

  } Bits;
  UINT16 Data;

} DID_IIO_VMD_STRUCT;

/* PCICMD_IIO_VMD_REG supported on:                                             */
/*      SPRA0 (0x10805004)                                                      */
/*      SPRB0 (0x10805004)                                                      */
/*      SPRHBM (0x10805004)                                                     */
/*      SPRC0 (0x10805004)                                                      */
/*      SPRMCC (0x10805004)                                                     */
/*      SPRUCC (0x10805004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command
*/


#define PCICMD_IIO_VMD_REG 0x11410004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 iose : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Not supported by VMD. VMD driver must not enable
                               I/O regions in VMD-Owned Root Port or Endpoint
                               devices.
                            */
    UINT16 mse : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Virtually, this bit is meant to enable the VMD
                               memory BARs. In hardware, this bit will enable
                               CFGBAR and the MEMBAR2 MSI-X table. It has no
                               effect on MEMBAR1 and the rest of MEMBAR2 (which
                               are decoded by the VMD-Owned Root Ports).[br]
                               The VMD driver reads this bit to decide how to
                               set corresponding MSE bits in the VMD-Owned Root
                               Ports and/or Endpoint devices.[br] 1: If this
                               bit is set, CFGBAR and MSI-X tables are enabled
                               for access. The VMD driver may enable VMD-Owned
                               Root Port and Endpoint device BAR regions.[br]
                               0: If this bit is clear, CFGBAR and MSI-X tables
                               are disabled and inaccessible. The VMD driver
                               must disable VMD-Owned Root Port and Endpoint
                               device BAR regions.[br] Notes:[br] - A write to
                               this register will trigger an interrupt to the
                               VMD driver using the MSI table entry 0.
                            */
    UINT16 bme : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Virtually, this bit is meant to enable the VMD
                               to master requests to the system. This bit
                               prevents VMD from mastering interrupts
                               (generated internally or forwarded from VMD-
                               owned devices) but it does not prevent VMD-owned
                               devices from mastering memory requests. [br] The
                               VMD driver reads this bit to decide how to set
                               corresponding BME bits in the VMD-Owned Root
                               Ports and Endpoint devices.[br] 1: If this bit
                               is set, the VMD driver may allow VMD-Owned Root
                               Ports and Endpoint devices to master requests to
                               the Root Complex.[br] 0: If this bit is clear,
                               the VMD driver must prevent VMD-Owned Root Ports
                               and Endpoint devices from mastering requests to
                               the Root Complex.[br] Notes:[br] - A write to
                               this register will trigger an interrupt to the
                               VMD driver using the MSI table entry 0, unless
                               the write changes the value of BME to 0.
                            */
    UINT16 sce : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to DMI/PCI Express devices.
                               Hardwired to 0
                            */
    UINT16 mwie : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to internal MS2IOSF devices.
                               Hardwired to 0.
                            */
    UINT16 vga_palette_snoop_enable : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to internal MS2IOSF devices.
                               Hardwired to 0.
                            */
    UINT16 perre : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Not supported for VMD. VMD-Owned Root Ports
                               still report parity errors separately.
                            */
    UINT16 idsel_stepping_wait_cycle_control : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to internal MS2IOSF devices.
                               Hardwired to 0.
                            */
    UINT16 serre : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Not supported for VMD. VMD-Owned Root Ports may
                               be programmed by the VMD driver to signal a
                               system error.
                            */
    UINT16 fast_back_to_back_enable : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to PCI Express and is hardwired
                               to 0
                            */
    UINT16 interrupt_disable : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               VMD does not support the generation of INTx, but
                               INTx from VMD-owned devices are not blocked. The
                               VMD driver must prevent the enabling of INTx
                               generation in VMD owned devices by utilizing MSI
                               only. This bit has no effect in hardware.[br] 1:
                               INTx Legacy Interrupt generation is disabled[br]
                               0: INTx Legacy Interrupt generation is
                               enabled[br] Notes:[br] - INTx message received
                               from VMD-owned Root Ports will be routed to the
                               system using the same rules defined in the Root
                               Ports as though they were not VMD-owned. If the
                               VMD driver expects INTx, then the INTPIN
                               registers in the VMD-owned Root Ports and
                               Switches must be programmed by the VMD
                               driver.[br] - A write to this register will
                               trigger an interrupt to the VMD driver using the
                               MSI table entry 0.
                            */
    UINT16 rsvd : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PCICMD_IIO_VMD_STRUCT;

/* PCISTS_IIO_VMD_REG supported on:                                             */
/*      SPRA0 (0x10805006)                                                      */
/*      SPRB0 (0x10805006)                                                      */
/*      SPRHBM (0x10805006)                                                     */
/*      SPRC0 (0x10805006)                                                      */
/*      SPRMCC (0x10805006)                                                     */
/*      SPRUCC (0x10805006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status
*/


#define PCISTS_IIO_VMD_REG 0x11410006

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
                               Indicates a pending INTx interrupt.[br] Not used
                               by VMD.
                            */
    UINT16 capabilities_list : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               This bit indicates the presence of a
                               capabilities list structure.
                            */
    UINT16 pci66mhz_capable : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /* Not applicable to VMD. Hardwired to 0. */
    UINT16 rsvd_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 fast_back_to_back : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* Not applicable to VMD. Hardwired to 0. */
    UINT16 mdpe : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /* Master Data Parity Error[br] Not used by VMD. */
    UINT16 devsel_timing : 2;

                            /* Bits[10:9], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI Express. Hardwired to 0. */
    UINT16 sta : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /* Signaled Target Abort[br] Not used by VMD. */
    UINT16 rta : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /* Received Target Abort[br] Not used by VMD. */
    UINT16 rma : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /* Received Master Abort[br] Not used by VMD. */
    UINT16 sse : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* Signaled System Error[br] Not used by VMD. */
    UINT16 dpe : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* Detected Parity Error[br] Not used by VMD. */

  } Bits;
  UINT16 Data;

} PCISTS_IIO_VMD_STRUCT;

/* RID_CCR_IIO_VMD_REG supported on:                                            */
/*      SPRA0 (0x20805008)                                                      */
/*      SPRB0 (0x20805008)                                                      */
/*      SPRHBM (0x20805008)                                                     */
/*      SPRC0 (0x20805008)                                                      */
/*      SPRMCC (0x20805008)                                                     */
/*      SPRUCC (0x20805008)                                                     */
/* Register default value on SPRA0: 0x01040000                                  */
/* Register default value on SPRB0: 0x01040000                                  */
/* Register default value on SPRHBM: 0x01040000                                 */
/* Register default value on SPRC0: 0x01040000                                  */
/* Register default value on SPRMCC: 0x01040000                                 */
/* Register default value on SPRUCC: 0x01040000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI header Revision ID register and Class Code register
*/


#define RID_CCR_IIO_VMD_REG 0x11420008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 revision_id : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Reflects the Revision ID for VMD steppings. Does
                               not reflect the VMD version. Determined by
                               MS2IOSF metal options.
                            */
    UINT32 interface : 8;

                            /* Bits[15:8], Access Type=RW/L, default=0x00000000*/

                            /* VMD can be any value.[br] Lock: VMLOCK.VMDLOCK */
    UINT32 sub_class : 8;

                            /* Bits[23:16], Access Type=RW/L, default=0x00000004*/

                            /*
                               VMD can be represented with any Sub Class.
                               Initial Sub Class is RAID device.[br] Lock:
                               VMLOCK.VMDLOCK
                            */
    UINT32 base_class : 8;

                            /* Bits[31:24], Access Type=RW/L, default=0x00000001*/

                            /*
                               VMD can represent itself as any Base Class.
                               Initial Base Class is Mass Storage Device.[br]
                               Lock: VMLOCK.VMDLOCK
                            */

  } Bits;
  UINT32 Data;

} RID_CCR_IIO_VMD_STRUCT;

/* CLSR_IIO_VMD_REG supported on:                                               */
/*      SPRA0 (0x80500c)                                                        */
/*      SPRB0 (0x80500c)                                                        */
/*      SPRHBM (0x80500c)                                                       */
/*      SPRC0 (0x80500c)                                                        */
/*      SPRMCC (0x80500c)                                                       */
/*      SPRUCC (0x80500c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Cacheline Size Register
*/


#define CLSR_IIO_VMD_REG 0x1140000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cacheline_size : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               This register is set as RW for compatibility
                               reasons only. Cacheline size is 64B.
                            */

  } Bits;
  UINT8 Data;

} CLSR_IIO_VMD_STRUCT;

/* HDR_IIO_VMD_REG supported on:                                                */
/*      SPRA0 (0x80500e)                                                        */
/*      SPRB0 (0x80500e)                                                        */
/*      SPRHBM (0x80500e)                                                       */
/*      SPRC0 (0x80500e)                                                        */
/*      SPRMCC (0x80500e)                                                       */
/*      SPRUCC (0x80500e)                                                       */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header Type
*/


#define HDR_IIO_VMD_REG 0x1140000E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 configuration_layout : 7;

                            /* Bits[6:0], Access Type=RO, default=0x00000000*/

                            /*
                               This field identifies the format of the
                               configuration header layout. It is Type 0 for
                               all this device. The default is 00h, indicating
                               a 'endpoint device'.
                            */
    UINT8 multi_function_device : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               Set to 1b to indicate functions 1-7 may exist
                               for the device
                            */

  } Bits;
  UINT8 Data;

} HDR_IIO_VMD_STRUCT;

/* CFGBAR_N0_IIO_VMD_REG supported on:                                          */
/*      SPRA0 (0x20805010)                                                      */
/*      SPRB0 (0x20805010)                                                      */
/*      SPRHBM (0x20805010)                                                     */
/*      SPRC0 (0x20805010)                                                      */
/*      SPRMCC (0x20805010)                                                     */
/*      SPRUCC (0x20805010)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VMD Configuration Base Address
*/


#define CFGBAR_N0_IIO_VMD_REG 0x11420010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 memory_space_indicator : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* BAR resource is memory (as opposed to I/O). */
    UINT32 type : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000002*/

                            /*
                               Memory type claimed by this BAR is 64-bit
                               addressable[br] Lock: VMLOCK.VMDLOCK
                            */
    UINT32 prefetchable : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000001*/

                            /*
                               BAR points to Prefetchable memory.[br] Lock:
                               VMLOCK.VMDLOCK
                            */
    UINT32 rsvd : 16;

                            /* Bits[19:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 memory_base_address : 12;

                            /* Bits[31:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the location of the CFGBAR in memory space.
                               The size is programmed in CFGBARSZ by BIOS.
                               CFGBARSZ specifies the lowest order address bit
                               that is writeable. The minimum granularity is
                               1MB.[br] If CFGBAR.Type = 10b, then bits 63:32
                               are writeable.[br] If CFGBAR.Type = 01b, then
                               bits 63:32 are read-only.[br] If CFGBARSZ = 0,
                               then all address bits are read-only.
                            */

  } Bits;
  UINT32 Data;

} CFGBAR_N0_IIO_VMD_STRUCT;

/* CFGBAR_N1_IIO_VMD_REG supported on:                                          */
/*      SPRA0 (0x20805014)                                                      */
/*      SPRB0 (0x20805014)                                                      */
/*      SPRHBM (0x20805014)                                                     */
/*      SPRC0 (0x20805014)                                                      */
/*      SPRMCC (0x20805014)                                                     */
/*      SPRUCC (0x20805014)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VMD Configuration Base Address
*/


#define CFGBAR_N1_IIO_VMD_REG 0x11420014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 memory_base_address : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the location of the CFGBAR in memory space.
                               The size is programmed in CFGBARSZ by BIOS.
                               CFGBARSZ specifies the lowest order address bit
                               that is writeable. The minimum granularity is
                               1MB.[br] If CFGBAR.Type = 10b, then bits 63:32
                               are writeable.[br] If CFGBAR.Type = 01b, then
                               bits 63:32 are read-only.[br] If CFGBARSZ = 0,
                               then all address bits are read-only.
                            */

  } Bits;
  UINT32 Data;

} CFGBAR_N1_IIO_VMD_STRUCT;

/* MEMBAR1_N0_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x20805018)                                                      */
/*      SPRB0 (0x20805018)                                                      */
/*      SPRHBM (0x20805018)                                                     */
/*      SPRC0 (0x20805018)                                                      */
/*      SPRMCC (0x20805018)                                                     */
/*      SPRUCC (0x20805018)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VMD Memory Base Address Range 1
*/


#define MEMBAR1_N0_IIO_VMD_REG 0x11420018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 memory_space_indicator : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* BAR resource is memory (as opposed to I/O). */
    UINT32 type : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000002*/

                            /*
                               Memory type claimed by this BAR is 64-bit
                               addressable[br] Lock: VMLOCK.VMDLOCK
                            */
    UINT32 prefetchable : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000001*/

                            /*
                               BAR points to Prefetchable memory.[br] Lock:
                               VMLOCK.VMDLOCK
                            */
    UINT32 rsvd : 8;

                            /* Bits[11:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 memory_base_address : 20;

                            /* Bits[31:12], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the location of the MEMBAR1 in memory
                               space. The size is programmed in MEMBAR1SZ by
                               BIOS. MEMBAR1SZ specifies the lowest order
                               address bit that is writeable. The minimum
                               granularity is 4kB.[br] If MEMBAR1.Type = 10b,
                               then bits 63:32 are writeable.[br] If
                               MEMBAR1.Type = 01b, then bits 63:32 are read-
                               only.[br] If MEMBAR1SZ = 0, then all address
                               bits are read-only.[br] If MEMBAR1SZ < 12, then
                               the effective value is 12.
                            */

  } Bits;
  UINT32 Data;

} MEMBAR1_N0_IIO_VMD_STRUCT;

/* MEMBAR1_N1_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x2080501c)                                                      */
/*      SPRB0 (0x2080501c)                                                      */
/*      SPRHBM (0x2080501c)                                                     */
/*      SPRC0 (0x2080501c)                                                      */
/*      SPRMCC (0x2080501c)                                                     */
/*      SPRUCC (0x2080501c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VMD Memory Base Address Range 1
*/


#define MEMBAR1_N1_IIO_VMD_REG 0x1142001C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 memory_base_address : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the location of the MEMBAR1 in memory
                               space. The size is programmed in MEMBAR1SZ by
                               BIOS. MEMBAR1SZ specifies the lowest order
                               address bit that is writeable. The minimum
                               granularity is 4kB.[br] If MEMBAR1.Type = 10b,
                               then bits 63:32 are writeable.[br] If
                               MEMBAR1.Type = 01b, then bits 63:32 are read-
                               only.[br] If MEMBAR1SZ = 0, then all address
                               bits are read-only.[br] If MEMBAR1SZ < 12, then
                               the effective value is 12.
                            */

  } Bits;
  UINT32 Data;

} MEMBAR1_N1_IIO_VMD_STRUCT;

/* MEMBAR2_N0_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x20805020)                                                      */
/*      SPRB0 (0x20805020)                                                      */
/*      SPRHBM (0x20805020)                                                     */
/*      SPRC0 (0x20805020)                                                      */
/*      SPRMCC (0x20805020)                                                     */
/*      SPRUCC (0x20805020)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VMD Memory Base Address Range 2
*/


#define MEMBAR2_N0_IIO_VMD_REG 0x11420020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 memory_space_indicator : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* BAR resource is memory (as opposed to I/O). */
    UINT32 type : 2;

                            /* Bits[2:1], Access Type=RW/L, default=0x00000002*/

                            /*
                               Memory type claimed by this BAR is 64-bit
                               addressable[br] Lock: VMLOCK.VMDLOCK
                            */
    UINT32 prefetchable : 1;

                            /* Bits[3:3], Access Type=RW/L, default=0x00000001*/

                            /*
                               BAR points to Prefetchable memory.[br] Lock:
                               VMLOCK.VMDLOCK
                            */
    UINT32 rsvd : 10;

                            /* Bits[13:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 memory_base_address : 18;

                            /* Bits[31:14], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the location of the MEMBAR2 in memory
                               space. The size is programmed in MEMBAR2SZ by
                               BIOS. MEMBAR2SZ specifies the lowest order
                               address bit that is writeable. The minimum
                               granularity is 16kB.[br] If MEMBAR2.Type = 10b,
                               then bits 63:32 are writeable.[br] If
                               MEMBAR2.Type = 01b, then bits 63:32 are read-
                               only.[br] If MEMBAR2SZ = 0, then all address
                               bits are read-only.[br] If MEMBAR2SZ < 14, then
                               the effective value is 14.
                            */

  } Bits;
  UINT32 Data;

} MEMBAR2_N0_IIO_VMD_STRUCT;

/* MEMBAR2_N1_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x20805024)                                                      */
/*      SPRB0 (0x20805024)                                                      */
/*      SPRHBM (0x20805024)                                                     */
/*      SPRC0 (0x20805024)                                                      */
/*      SPRMCC (0x20805024)                                                     */
/*      SPRUCC (0x20805024)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VMD Memory Base Address Range 2
*/


#define MEMBAR2_N1_IIO_VMD_REG 0x11420024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 memory_base_address : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the location of the MEMBAR2 in memory
                               space. The size is programmed in MEMBAR2SZ by
                               BIOS. MEMBAR2SZ specifies the lowest order
                               address bit that is writeable. The minimum
                               granularity is 16kB.[br] If MEMBAR2.Type = 10b,
                               then bits 63:32 are writeable.[br] If
                               MEMBAR2.Type = 01b, then bits 63:32 are read-
                               only.[br] If MEMBAR2SZ = 0, then all address
                               bits are read-only.[br] If MEMBAR2SZ < 14, then
                               the effective value is 14.
                            */

  } Bits;
  UINT32 Data;

} MEMBAR2_N1_IIO_VMD_STRUCT;

/* SVID_IIO_VMD_REG supported on:                                               */
/*      SPRA0 (0x1080502c)                                                      */
/*      SPRB0 (0x1080502c)                                                      */
/*      SPRHBM (0x1080502c)                                                     */
/*      SPRC0 (0x1080502c)                                                      */
/*      SPRMCC (0x1080502c)                                                     */
/*      SPRUCC (0x1080502c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Subsystem Vendor ID
*/


#define SVID_IIO_VMD_REG 0x1141002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 subsystem_vendor_id : 16;

                            /* Bits[15:0], Access Type=RW/O, default=0x00008086*/

                            /*
                               The default value specifies Intel but can be set
                               to any value once after reset.
                            */

  } Bits;
  UINT16 Data;

} SVID_IIO_VMD_STRUCT;

/* SID_IIO_VMD_REG supported on:                                                */
/*      SPRA0 (0x1080502e)                                                      */
/*      SPRB0 (0x1080502e)                                                      */
/*      SPRHBM (0x1080502e)                                                     */
/*      SPRC0 (0x1080502e)                                                      */
/*      SPRMCC (0x1080502e)                                                     */
/*      SPRUCC (0x1080502e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Subsystem ID
*/


#define SID_IIO_VMD_REG 0x1141002E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 subsystem_id : 16;

                            /* Bits[15:0], Access Type=RW/O, default=0x00000000*/

                            /*
                               The default value specifies Intel but can be set
                               to any value once after reset.
                            */

  } Bits;
  UINT16 Data;

} SID_IIO_VMD_STRUCT;

/* CAPPTR_IIO_VMD_REG supported on:                                             */
/*      SPRA0 (0x805034)                                                        */
/*      SPRB0 (0x805034)                                                        */
/*      SPRHBM (0x805034)                                                       */
/*      SPRC0 (0x805034)                                                        */
/*      SPRMCC (0x805034)                                                       */
/*      SPRUCC (0x805034)                                                       */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Capability Pointer
*/


#define CAPPTR_IIO_VMD_REG 0x11400034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 capability_pointer : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000080*/

                            /*
                               Points to the first capability structure for the
                               device which is the PCIe capability (for devices
                               that support 4kB extended configuration
                               space).[br] A value of zero indicates there are
                               no capability structures (and no extended
                               configuration space).
                            */

  } Bits;
  UINT8 Data;

} CAPPTR_IIO_VMD_STRUCT;

/* INTL_IIO_VMD_REG supported on:                                               */
/*      SPRA0 (0x80503c)                                                        */
/*      SPRB0 (0x80503c)                                                        */
/*      SPRHBM (0x80503c)                                                       */
/*      SPRC0 (0x80503c)                                                        */
/*      SPRMCC (0x80503c)                                                       */
/*      SPRUCC (0x80503c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Interrupt Line Register
*/


#define INTL_IIO_VMD_REG 0x1140003C

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

} INTL_IIO_VMD_STRUCT;

/* INTPIN_IIO_VMD_REG supported on:                                             */
/*      SPRA0 (0x80503d)                                                        */
/*      SPRB0 (0x80503d)                                                        */
/*      SPRHBM (0x80503d)                                                       */
/*      SPRC0 (0x80503d)                                                        */
/*      SPRMCC (0x80503d)                                                       */
/*      SPRUCC (0x80503d)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Interrupt Pin Register
*/


#define INTPIN_IIO_VMD_REG 0x1140003D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 intp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Pin[br] N/A since these devices do not
                               generate any interrupt on their own
                            */

  } Bits;
  UINT8 Data;

} INTPIN_IIO_VMD_STRUCT;

/* VMCAP_IIO_VMD_REG supported on:                                              */
/*      SPRA0 (0x20805040)                                                      */
/*      SPRB0 (0x20805040)                                                      */
/*      SPRHBM (0x20805040)                                                     */
/*      SPRC0 (0x20805040)                                                      */
/*      SPRMCC (0x20805040)                                                     */
/*      SPRUCC (0x20805040)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Volume Management Capability
*/


#define VMCAP_IIO_VMD_REG 0x11420040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bus_restrict_cap : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /*
                               0: Indicates that hardware supports a separate
                               set of bus encodings for VMD. Bus restrictions
                               are not required.[br] 1: Indicates that bus
                               restrictions are required. BIOS must program
                               VMCONFIG.BUS_RESTRICTIONS field. VMD may only
                               use 128 bus numbers unless VMD owns all devices
                               in its ACPI domain.[br] VMD driver reads this
                               field to determine if bus number restrictions
                               are required. When set, VMD driver must also
                               read VMCONFIG.BUS_RESTRICTIONS field to
                               determine what range of bus numbers to use for
                               Secondary and Subbordinate bus ranges.
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} VMCAP_IIO_VMD_STRUCT;

/* VMCONFIG_IIO_VMD_REG supported on:                                           */
/*      SPRA0 (0x20805044)                                                      */
/*      SPRB0 (0x20805044)                                                      */
/*      SPRHBM (0x20805044)                                                     */
/*      SPRC0 (0x20805044)                                                      */
/*      SPRMCC (0x20805044)                                                     */
/*      SPRUCC (0x20805044)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Volume Management Configuration
*/


#define VMCONFIG_IIO_VMD_REG 0x11420044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 msi_rmp_dis : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               MSI Remapping Disable - When set, causes MSI
                               transactions to bypass the remap indexing that
                               triggers a native VMD MSI-X Table Entries. The
                               original MSI will be set up without indexing,
                               however, the Requestor ID will be changed to
                               match the VMD before being translated by
                               VTd.[br] This bit is programmed by the OS or by
                               the VMD Driver.
                            */
    UINT32 rsvd_2 : 2;

                            /* Bits[3:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 devid_select : 4;

                            /* Bits[7:4], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Selects the Device ID that appears in the Device
                               ID register. Values are device specific. Only
                               BIOS may program this register field.[br] Volume
                               Management Device v2.0 for Server uses Device ID
                               values 0x28C0 to 0x28CF.[br] Lock:
                               VMLOCK.VMDLOCK
                            */
    UINT32 bus_restrictions : 2;

                            /* Bits[9:8], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               00: VMD-Owned Root Port and PCI Switch Bridges
                               must use 1-127 for secondary/subordinate bus
                               numbers (the first 1MB of CFGBAR maps to the bus
                               number programmed in ROOTBUS.RootBusS).[br] 01:
                               VMD-Owned Root Port and PCI Switch Bridges must
                               use 129-255 for secondary/subordinate bus
                               numbers (the first 1MB of CFGBAR maps to the bus
                               number programmed in ROOTBUS.RootBusS).[br] 10:
                               reserved[br] 11: reserved[br] This field is read
                               only when VMCAP.BUS_RESTRICT_CAP bit is
                               clear.[br] BIOS programs this field to a value
                               that ensures the OS visible busses in the ACPI
                               domain where this VMD resides do not conflict
                               with bus number encodings assigned to VMD.[br]
                               VMD driver reads this field to select the
                               appropriate range of bus numbers to use in
                               Secondary and Subbordinate Bus registers of PCIe
                               Root Port and Switch Bridges.[br] VMD driver
                               does not use this field to determine the primary
                               bus number for PCIe Root Port and Switch Bridges
                               owned by VMD. The primary bus number must be the
                               same as the Root Bus number that the VMD Root
                               Complext Integrated Endpoint resides on.[br]
                               Lock: VMLOCK.VMDLOCK
                            */
    UINT32 rsvd_10 : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} VMCONFIG_IIO_VMD_STRUCT;

/* VMASSIGN_IIO_VMD_REG supported on:                                           */
/*      SPRA0 (0x20805048)                                                      */
/*      SPRB0 (0x20805048)                                                      */
/*      SPRHBM (0x20805048)                                                     */
/*      SPRC0 (0x20805048)                                                      */
/*      SPRMCC (0x20805048)                                                     */
/*      SPRUCC (0x20805048)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Volume Management Assignment
*/


#define VMASSIGN_IIO_VMD_REG 0x11420048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 port_assign : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on the
                               same bus number as VMD. For each bit set, the
                               associated device will be assigned to the VMD
                               and hidden from the OS.[br] Lock: VMLOCK.VMDLOCK
                            */

  } Bits;
  UINT32 Data;

} VMASSIGN_IIO_VMD_STRUCT;

/* CFGBARSZ_IIO_VMD_REG supported on:                                           */
/*      SPRA0 (0x80504c)                                                        */
/*      SPRB0 (0x80504c)                                                        */
/*      SPRHBM (0x80504c)                                                       */
/*      SPRC0 (0x80504c)                                                        */
/*      SPRMCC (0x80504c)                                                       */
/*      SPRUCC (0x80504c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VMD Configuration BAR Size
*/


#define CFGBARSZ_IIO_VMD_REG 0x1140004C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 size : 8;

                            /* Bits[7:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Value indicating the size of CFGBAR. This value
                               is written by BIOS prior to enumeration. The
                               value indicates the lowest order address bit
                               that is writeable in CFGBAR. The only legal
                               settings are 20 to 28 (decimal), representing
                               BAR sizes of 1MB through 256MB.[br] If CFGBARSZ
                               = 0, then all address bits are read-only.[br] If
                               CFGBARSZ < 20, then the effective value is
                               20.[br] If CFGBARSZ > 28, then the effective
                               value is 28.[br] Lock: VMLOCK.VMDLOCK
                            */

  } Bits;
  UINT8 Data;

} CFGBARSZ_IIO_VMD_STRUCT;

/* MEMBAR1SZ_IIO_VMD_REG supported on:                                          */
/*      SPRA0 (0x80504d)                                                        */
/*      SPRB0 (0x80504d)                                                        */
/*      SPRHBM (0x80504d)                                                       */
/*      SPRC0 (0x80504d)                                                        */
/*      SPRMCC (0x80504d)                                                       */
/*      SPRUCC (0x80504d)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VMD Memory BAR 1 Size
*/


#define MEMBAR1SZ_IIO_VMD_REG 0x1140004D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 size : 8;

                            /* Bits[7:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Value indicating the size of MEMBAR1SZ. This
                               value is written by BIOS prior to enumeration.
                               The value indicates the lowest order address bit
                               that is writeable in MEMBAR1SZ. The only legal
                               settings are 12 to 47 (decimal), representing
                               BAR sizes of 4kB through 256TB.[br] If MEMBAR1SZ
                               = 0, then all address bits are read-only.[br] If
                               MEMBAR1SZ < 12, then the effective value is
                               12.[br] If MEMBAR1SZ > 47, then the effective
                               value is 47.[br] Lock: VMLOCK.VMDLOCK
                            */

  } Bits;
  UINT8 Data;

} MEMBAR1SZ_IIO_VMD_STRUCT;

/* MEMBAR2SZ_IIO_VMD_REG supported on:                                          */
/*      SPRA0 (0x80504e)                                                        */
/*      SPRB0 (0x80504e)                                                        */
/*      SPRHBM (0x80504e)                                                       */
/*      SPRC0 (0x80504e)                                                        */
/*      SPRMCC (0x80504e)                                                       */
/*      SPRUCC (0x80504e)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VMD Memory BAR 2 Size
*/


#define MEMBAR2SZ_IIO_VMD_REG 0x1140004E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 size : 8;

                            /* Bits[7:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Value indicating the size of MEMBAR2SZ. This
                               value is written by BIOS prior to enumeration.
                               The value indicates the lowest order address bit
                               that is writeable in MEMBAR2SZ. The only legal
                               settings are 14 to 47 (decimal), representing
                               BAR sizes of 16KB through 256TB.[br] If
                               MEMBAR1SZ = 0, then all address bits are read-
                               only.[br] If MEMBAR2SZ < 14, then the effective
                               value is 14.[br] If MEMBAR2SZ > 47, then the
                               effective value is 47.[br] Lock: VMLOCK.VMDLOCK
                            */

  } Bits;
  UINT8 Data;

} MEMBAR2SZ_IIO_VMD_STRUCT;

/* ROOTBUS_IIO_VMD_REG supported on:                                            */
/*      SPRA0 (0x80504f)                                                        */
/*      SPRB0 (0x80504f)                                                        */
/*      SPRHBM (0x80504f)                                                       */
/*      SPRC0 (0x80504f)                                                        */
/*      SPRMCC (0x80504f)                                                       */
/*      SPRUCC (0x80504f)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VMD RootBus
*/


#define ROOTBUS_IIO_VMD_REG 0x1140004F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 rootbus : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               The value of the RootBus where VMD can take
                               ownership of devices and functions through the
                               VMASSIGN and FNASSIGN registers.
                            */

  } Bits;
  UINT8 Data;

} ROOTBUS_IIO_VMD_STRUCT;

/* FN0_ASSIGN_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x20805050)                                                      */
/*      SPRB0 (0x20805050)                                                      */
/*      SPRHBM (0x20805050)                                                     */
/*      SPRC0 (0x20805050)                                                      */
/*      SPRMCC (0x20805050)                                                     */
/*      SPRUCC (0x20805050)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Volume Management Function 0 Assignment
*/


#define FN0_ASSIGN_IIO_VMD_REG 0x11420050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 device : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on the
                               same bus number as VMD (RootBus0). For each bit
                               set, the associated device function 0 will be
                               assigned to the VMD and hidden from the OS.[br]
                               Each bit may be Read-Write for BIOS or Read Only
                               based on the specific VMD implementation. To
                               discover which bits are writeable, BIOS may
                               write ones to all bits and read the value
                               back.[br] If a bit in this register is set for a
                               device, then a dummy function will appear at
                               function 0 in its place in OS visible
                               configuration space.[br] Lock: VMLOCK.VMDLOCK
                            */

  } Bits;
  UINT32 Data;

} FN0_ASSIGN_IIO_VMD_STRUCT;

/* FN1_ASSIGN_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x20805054)                                                      */
/*      SPRB0 (0x20805054)                                                      */
/*      SPRHBM (0x20805054)                                                     */
/*      SPRC0 (0x20805054)                                                      */
/*      SPRMCC (0x20805054)                                                     */
/*      SPRUCC (0x20805054)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Volume Management Function 1 Assignment
*/


#define FN1_ASSIGN_IIO_VMD_REG 0x11420054

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 device : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on the
                               same bus number as VMD (RootBus0). For each bit
                               set, the associated device function 1 will be
                               assigned to the VMD and hidden from the OS.[br]
                               Each bit may be Read-Write for BIOS or Read Only
                               based on the specific VMD implementation. To
                               discover which bits are writeable, BIOS may
                               write ones to all bits and read the value
                               back.[br] If a bit in this register is set for a
                               device, and the corresponding bit for function 0
                               is not set in FN0_ASSIGN, then a dummy function
                               will appear at function 0 for the device in
                               CFGBAR configuration space.[br] Lock:
                               VMLOCK.VMDLOCK
                            */

  } Bits;
  UINT32 Data;

} FN1_ASSIGN_IIO_VMD_STRUCT;

/* FN2_ASSIGN_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x20805058)                                                      */
/*      SPRB0 (0x20805058)                                                      */
/*      SPRHBM (0x20805058)                                                     */
/*      SPRC0 (0x20805058)                                                      */
/*      SPRMCC (0x20805058)                                                     */
/*      SPRUCC (0x20805058)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Volume Management Function 2 Assignment
*/


#define FN2_ASSIGN_IIO_VMD_REG 0x11420058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 device : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on the
                               same bus number as VMD (RootBus0). For each bit
                               set, the associated device function 2 will be
                               assigned to the VMD and hidden from the OS.[br]
                               Each bit may be Read-Write for BIOS or Read Only
                               based on the specific VMD implementation. To
                               discover which bits are writeable, BIOS may
                               write ones to all bits and read the value
                               back.[br] If a bit in this register is set for a
                               device, and the corresponding bit for function 0
                               is not set in FN0_ASSIGN, then a dummy function
                               will appear at function 0 for the device in
                               CFGBAR configuration space.[br] Lock:
                               VMLOCK.VMDLOCK
                            */

  } Bits;
  UINT32 Data;

} FN2_ASSIGN_IIO_VMD_STRUCT;

/* FN3_ASSIGN_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x2080505c)                                                      */
/*      SPRB0 (0x2080505c)                                                      */
/*      SPRHBM (0x2080505c)                                                     */
/*      SPRC0 (0x2080505c)                                                      */
/*      SPRMCC (0x2080505c)                                                     */
/*      SPRUCC (0x2080505c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Volume Management Function 3 Assignment
*/


#define FN3_ASSIGN_IIO_VMD_REG 0x1142005C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 device : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on the
                               same bus number as VMD (RootBus0). For each bit
                               set, the associated device function 3 will be
                               assigned to the VMD and hidden from the OS.[br]
                               Each bit may be Read-Write for BIOS or Read Only
                               based on the specific VMD implementation. To
                               discover which bits are writeable, BIOS may
                               write ones to all bits and read the value
                               back.[br] If a bit in this register is set for a
                               device, and the corresponding bit for function 0
                               is not set in FN0_ASSIGN, then a dummy function
                               will appear at function 0 for the device in
                               CFGBAR configuration space.[br] Lock:
                               VMLOCK.VMDLOCK
                            */

  } Bits;
  UINT32 Data;

} FN3_ASSIGN_IIO_VMD_STRUCT;

/* FN4_ASSIGN_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x20805060)                                                      */
/*      SPRB0 (0x20805060)                                                      */
/*      SPRHBM (0x20805060)                                                     */
/*      SPRC0 (0x20805060)                                                      */
/*      SPRMCC (0x20805060)                                                     */
/*      SPRUCC (0x20805060)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Volume Management Function 4 Assignment
*/


#define FN4_ASSIGN_IIO_VMD_REG 0x11420060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 device : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on the
                               same bus number as VMD (RootBus0). For each bit
                               set, the associated device function 4 will be
                               assigned to the VMD and hidden from the OS.[br]
                               Each bit may be Read-Write for BIOS or Read Only
                               based on the specific VMD implementation. To
                               discover which bits are writeable, BIOS may
                               write ones to all bits and read the value
                               back.[br] If a bit in this register is set for a
                               device, and the corresponding bit for function 0
                               is not set in FN0_ASSIGN, then a dummy function
                               will appear at function 0 for the device in
                               CFGBAR configuration space.[br] Lock:
                               VMLOCK.VMDLOCK
                            */

  } Bits;
  UINT32 Data;

} FN4_ASSIGN_IIO_VMD_STRUCT;

/* FN5_ASSIGN_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x20805064)                                                      */
/*      SPRB0 (0x20805064)                                                      */
/*      SPRHBM (0x20805064)                                                     */
/*      SPRC0 (0x20805064)                                                      */
/*      SPRMCC (0x20805064)                                                     */
/*      SPRUCC (0x20805064)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Volume Management Function 5 Assignment
*/


#define FN5_ASSIGN_IIO_VMD_REG 0x11420064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 device : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on the
                               same bus number as VMD (RootBus0). For each bit
                               set, the associated device function 5 will be
                               assigned to the VMD and hidden from the OS.[br]
                               Each bit may be Read-Write for BIOS or Read Only
                               based on the specific VMD implementation. To
                               discover which bits are writeable, BIOS may
                               write ones to all bits and read the value
                               back.[br] If a bit in this register is set for a
                               device, and the corresponding bit for function 0
                               is not set in FN0_ASSIGN, then a dummy function
                               will appear at function 0 for the device in
                               CFGBAR configuration space.[br] Lock:
                               VMLOCK.VMDLOCK
                            */

  } Bits;
  UINT32 Data;

} FN5_ASSIGN_IIO_VMD_STRUCT;

/* FN6_ASSIGN_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x20805068)                                                      */
/*      SPRB0 (0x20805068)                                                      */
/*      SPRHBM (0x20805068)                                                     */
/*      SPRC0 (0x20805068)                                                      */
/*      SPRMCC (0x20805068)                                                     */
/*      SPRUCC (0x20805068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Volume Management Function 6 Assignment
*/


#define FN6_ASSIGN_IIO_VMD_REG 0x11420068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 device : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on the
                               same bus number as VMD (RootBus0). For each bit
                               set, the associated device function 6 will be
                               assigned to the VMD and hidden from the OS.[br]
                               Each bit may be Read-Write for BIOS or Read Only
                               based on the specific VMD implementation. To
                               discover which bits are writeable, BIOS may
                               write ones to all bits and read the value
                               back.[br] If a bit in this register is set for a
                               device, and the corresponding bit for function 0
                               is not set in FN0_ASSIGN, then a dummy function
                               will appear at function 0 for the device in
                               CFGBAR configuration space.[br] Lock:
                               VMLOCK.VMDLOCK
                            */

  } Bits;
  UINT32 Data;

} FN6_ASSIGN_IIO_VMD_STRUCT;

/* FN7_ASSIGN_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x2080506c)                                                      */
/*      SPRB0 (0x2080506c)                                                      */
/*      SPRHBM (0x2080506c)                                                     */
/*      SPRC0 (0x2080506c)                                                      */
/*      SPRMCC (0x2080506c)                                                     */
/*      SPRUCC (0x2080506c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Volume Management Function 7 Assignment
*/


#define FN7_ASSIGN_IIO_VMD_REG 0x1142006C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 device : 32;

                            /* Bits[31:0], Access Type=RW/P/L, default=0x00000000*/

                            /*
                               Each bit corresponds to a device number on the
                               same bus number as VMD (RootBus0). For each bit
                               set, the associated device function 7 will be
                               assigned to the VMD and hidden from the OS.[br]
                               Each bit may be Read-Write for BIOS or Read Only
                               based on the specific VMD implementation. To
                               discover which bits are writeable, BIOS may
                               write ones to all bits and read the value
                               back.[br] If a bit in this register is set for a
                               device, and the corresponding bit for function 0
                               is not set in FN0_ASSIGN, then a dummy function
                               will appear at function 0 for the device in
                               CFGBAR configuration space.[br] Lock:
                               VMLOCK.VMDLOCK
                            */

  } Bits;
  UINT32 Data;

} FN7_ASSIGN_IIO_VMD_STRUCT;

/* VMLOCK_IIO_VMD_REG supported on:                                             */
/*      SPRA0 (0x20805070)                                                      */
/*      SPRB0 (0x20805070)                                                      */
/*      SPRHBM (0x20805070)                                                     */
/*      SPRC0 (0x20805070)                                                      */
/*      SPRMCC (0x20805070)                                                     */
/*      SPRUCC (0x20805070)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Volume Management Lock
*/


#define VMLOCK_IIO_VMD_REG 0x11420070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vmdlock : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Locks registers in the Volume Management Device.
                               This must be set by BIOS after programming VMD
                               registers.
                            */
    UINT32 membar_shadow_enable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enables shadowing of MEMBAR1 and MEMBAR2 in
                               configuration space into the memory-mapped
                               shadow registers in MEMBAR2
                            */
    UINT32 vmd_reqid_sub_disable : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               By default, VMD requester ID substitution is
                               enbled. When set to 1, it will be disabled. This
                               is for SDV purposes.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} VMLOCK_IIO_VMD_STRUCT;

/* DEVCON_IIO_VMD_REG supported on:                                             */
/*      SPRA0 (0x10805078)                                                      */
/*      SPRB0 (0x10805078)                                                      */
/*      SPRHBM (0x10805078)                                                     */
/*      SPRC0 (0x10805078)                                                      */
/*      SPRMCC (0x10805078)                                                     */
/*      SPRUCC (0x10805078)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device Control register for VMD (modeled after PCIe Device Control Register)[br]
The VMD driver or BIOS must program this register with the lowest MPS value of all devices in the PCIe hierarchy owned by this VMD
*/


#define DEVCON_IIO_VMD_REG 0x11410078

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 rsvd : 5;

                            /* Bits[4:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 max_payload_size : 3;

                            /* Bits[7:5], Access Type=RW, default=0x00000000*/

                            /*
                               Indicates the largest common packet size
                               supported by the devices owned by this VMD. The
                               driver or BIOS should set this field to the
                               lowest MPS value of all devices owned by this
                               VMD instance.[br] Defined encodings for this
                               field are:[br] 000b 128 bytes max payload
                               size[br] 001b 256 bytes max payload size[br]
                               010b 512 bytes max payload size[br] 011b 1024
                               bytes max payload size[br] 100b 2048 bytes max
                               payload size[br] 101b 4096 bytes max payload
                               size[br] 110b Reserved[br] 111b 64 address
                               aligned bytes max payload size
                            */
    UINT16 rsvd_8 : 8;

                            /* Bits[15:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} DEVCON_IIO_VMD_STRUCT;

/* MSIXCAPID_IIO_VMD_REG supported on:                                          */
/*      SPRA0 (0x805080)                                                        */
/*      SPRB0 (0x805080)                                                        */
/*      SPRHBM (0x805080)                                                       */
/*      SPRC0 (0x805080)                                                        */
/*      SPRMCC (0x805080)                                                       */
/*      SPRUCC (0x805080)                                                       */
/* Register default value on SPRA0: 0x00000011                                  */
/* Register default value on SPRB0: 0x00000011                                  */
/* Register default value on SPRHBM: 0x00000011                                 */
/* Register default value on SPRC0: 0x00000011                                  */
/* Register default value on SPRMCC: 0x00000011                                 */
/* Register default value on SPRUCC: 0x00000011                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSI-X Capability ID
*/


#define MSIXCAPID_IIO_VMD_REG 0x11400080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 capability_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000011*/

                            /* Assigned by PCI-SIG for MSI-X. */

  } Bits;
  UINT8 Data;

} MSIXCAPID_IIO_VMD_STRUCT;

/* MSIXNXTPTR_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x805081)                                                        */
/*      SPRB0 (0x805081)                                                        */
/*      SPRHBM (0x805081)                                                       */
/*      SPRC0 (0x805081)                                                        */
/*      SPRMCC (0x805081)                                                       */
/*      SPRUCC (0x805081)                                                       */
/* Register default value on SPRA0: 0x00000090                                  */
/* Register default value on SPRB0: 0x00000090                                  */
/* Register default value on SPRHBM: 0x00000090                                 */
/* Register default value on SPRC0: 0x00000090                                  */
/* Register default value on SPRMCC: 0x00000090                                 */
/* Register default value on SPRUCC: 0x00000090                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSI-X Next Pointer
*/


#define MSIXNXTPTR_IIO_VMD_REG 0x11400081

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 next_ptr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000090*/

                            /*
                               This field is set to 90h for the next capability
                               list (Power Management capability structure) in
                               the chain.
                            */

  } Bits;
  UINT8 Data;

} MSIXNXTPTR_IIO_VMD_STRUCT;

/* MSIXMSGCTL_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x10805082)                                                      */
/*      SPRB0 (0x10805082)                                                      */
/*      SPRHBM (0x10805082)                                                     */
/*      SPRC0 (0x10805082)                                                      */
/*      SPRMCC (0x10805082)                                                     */
/*      SPRUCC (0x10805082)                                                     */
/* Register default value on SPRA0: 0x0000003F                                  */
/* Register default value on SPRB0: 0x0000003F                                  */
/* Register default value on SPRHBM: 0x0000003F                                 */
/* Register default value on SPRC0: 0x0000003F                                  */
/* Register default value on SPRMCC: 0x0000003F                                 */
/* Register default value on SPRUCC: 0x0000003F                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSI-X Message Control
*/


#define MSIXMSGCTL_IIO_VMD_REG 0x11410082

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 table_size : 11;

                            /* Bits[10:0], Access Type=RO, default=0x0000003F*/

                            /*
                               System software reads this field to determine
                               the MSI-X Table Size N, which is encoded as
                               N-1.[br] VMD table size is 64, encoded as a
                               value of 03Fh.[br]
                            */
    UINT16 rsvd : 3;

                            /* Bits[13:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 function_mask : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               1: all the vectors associated with the VMD are
                               masked, regardless of the per vector mask bit
                               state.[br] 0: each vector's mask bit determines
                               whether the vector is masked or not.[br]
                               Note:[br] - Since VMD shares the MSI-X table
                               with the NTB iEP, it is required to disable the
                               MSI-X table at the sources for VMD and for NTB.
                               For VMD, the source of interrupts is from MSIs
                               generated by VMD-Owned devices. For NTB, the
                               source of interrupts is from doorbell registers.
                            */
    UINT16 msi_x_enable : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Software uses this bit to select between INTx or
                               MSI or MSI-X method for signaling interrupts
                               from the VMD[br] 0: VMD is prohibited from using
                               MSI-X to request service[br] 1: MSI-X method is
                               chosen for VMD interrupts[br] Note:[br] - Since
                               VMD shares the MSI-X table with the NTB iEP, it
                               is required to disable the MSI-X table at the
                               sources for VMD and for NTB. For VMD, the source
                               of interrupts is from MSIs generated by VMD-
                               Owned devices. For NTB, the source of interrupts
                               is from doorbell registers.
                            */

  } Bits;
  UINT16 Data;

} MSIXMSGCTL_IIO_VMD_STRUCT;

/* TABLEOFF_BIR_IIO_VMD_REG supported on:                                       */
/*      SPRA0 (0x20805084)                                                      */
/*      SPRB0 (0x20805084)                                                      */
/*      SPRHBM (0x20805084)                                                     */
/*      SPRC0 (0x20805084)                                                      */
/*      SPRMCC (0x20805084)                                                     */
/*      SPRUCC (0x20805084)                                                     */
/* Register default value on SPRA0: 0x00000004                                  */
/* Register default value on SPRB0: 0x00000004                                  */
/* Register default value on SPRHBM: 0x00000004                                 */
/* Register default value on SPRC0: 0x00000004                                  */
/* Register default value on SPRMCC: 0x00000004                                 */
/* Register default value on SPRUCC: 0x00000004                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSI-X Table Offset and BAR Indicator
*/


#define TABLEOFF_BIR_IIO_VMD_REG 0x11420084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 table_bir : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000004*/

                            /*
                               Indicates which one of a function's Base Address
                               registers, located beginning at 10h in
                               Configuration Space, is used to map the
                               function's MSI-X Table intoMemory Space.[br] BIR
                               Value Base Address register[br] 0: 10h[br] 1:
                               14h[br] 2: 18h[br] 3: 1Ch[br] 4: 20h[br] 5:
                               24h[br] 6: Reserved[br] 7: Reserved[br] For a
                               64-bit Base Address register, the Table BIR
                               indicates the lower DWORD.
                            */
    UINT32 table_offset : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00000000*/

                            /*
                               MSI-X Table Structure is at offset 0 from the
                               MEMBASE2 address.
                            */

  } Bits;
  UINT32 Data;

} TABLEOFF_BIR_IIO_VMD_STRUCT;

/* PBAOFF_BIR_IIO_VMD_REG supported on:                                         */
/*      SPRA0 (0x20805088)                                                      */
/*      SPRB0 (0x20805088)                                                      */
/*      SPRHBM (0x20805088)                                                     */
/*      SPRC0 (0x20805088)                                                      */
/*      SPRMCC (0x20805088)                                                     */
/*      SPRUCC (0x20805088)                                                     */
/* Register default value on SPRA0: 0x00001004                                  */
/* Register default value on SPRB0: 0x00001004                                  */
/* Register default value on SPRHBM: 0x00001004                                 */
/* Register default value on SPRC0: 0x00001004                                  */
/* Register default value on SPRMCC: 0x00001004                                 */
/* Register default value on SPRUCC: 0x00001004                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSI-X Pending Array Offset and BAR Indicator
*/


#define PBAOFF_BIR_IIO_VMD_REG 0x11420088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pba_bir : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000004*/

                            /*
                               Indicates which one of a function's Base Address
                               registers, located beginning at 10h in
                               Configuration Space, is used to map the
                               function's MSI-X Table into Memory Space.[br]
                               BIR Value Base Address register[br] 0: 10h[br]
                               1: 14h[br] 2: 18h[br] 3: 1Ch[br] 4: 20h[br] 5:
                               24h[br] 6: Reserved[br] 7: Reserved[br] For a
                               64-bit Base Address register, the Table BIR
                               indicates the lower DWORD.
                            */
    UINT32 table_offset : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00000200*/

                            /*
                               MSI-X PBA Structure is at offset 4KB from the
                               MEMBAR2 address.
                            */

  } Bits;
  UINT32 Data;

} PBAOFF_BIR_IIO_VMD_STRUCT;

/* PXPCAPID_IIO_VMD_REG supported on:                                           */
/*      SPRA0 (0x805090)                                                        */
/*      SPRB0 (0x805090)                                                        */
/*      SPRHBM (0x805090)                                                       */
/*      SPRC0 (0x805090)                                                        */
/*      SPRMCC (0x805090)                                                       */
/*      SPRUCC (0x805090)                                                       */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability Identity
*/


#define PXPCAPID_IIO_VMD_REG 0x11400090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 capability_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000010*/

                            /*
                               Identifies the PCI Express capability assigned
                               by PCI-SIG.
                            */

  } Bits;
  UINT8 Data;

} PXPCAPID_IIO_VMD_STRUCT;

/* PXPNXTPTR_IIO_VMD_REG supported on:                                          */
/*      SPRA0 (0x805091)                                                        */
/*      SPRB0 (0x805091)                                                        */
/*      SPRHBM (0x805091)                                                       */
/*      SPRC0 (0x805091)                                                        */
/*      SPRMCC (0x805091)                                                       */
/*      SPRUCC (0x805091)                                                       */
/* Register default value on SPRA0: 0x000000E0                                  */
/* Register default value on SPRB0: 0x000000E0                                  */
/* Register default value on SPRHBM: 0x000000E0                                 */
/* Register default value on SPRC0: 0x000000E0                                  */
/* Register default value on SPRMCC: 0x000000E0                                 */
/* Register default value on SPRUCC: 0x000000E0                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Next Pointer
*/


#define PXPNXTPTR_IIO_VMD_REG 0x11400091

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 next_ptr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x000000E0*/

                            /*
                               Pointer to the next capability. This field is
                               set to the PCI PM capability.
                            */

  } Bits;
  UINT8 Data;

} PXPNXTPTR_IIO_VMD_STRUCT;

/* PXPCAP_IIO_VMD_REG supported on:                                             */
/*      SPRA0 (0x10805092)                                                      */
/*      SPRB0 (0x10805092)                                                      */
/*      SPRHBM (0x10805092)                                                     */
/*      SPRC0 (0x10805092)                                                      */
/*      SPRMCC (0x10805092)                                                     */
/*      SPRUCC (0x10805092)                                                     */
/* Register default value on SPRA0: 0x00000092                                  */
/* Register default value on SPRB0: 0x00000092                                  */
/* Register default value on SPRHBM: 0x00000092                                 */
/* Register default value on SPRC0: 0x00000092                                  */
/* Register default value on SPRMCC: 0x00000092                                 */
/* Register default value on SPRUCC: 0x00000092                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_IIO_VMD_REG 0x11410092

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

                            /* Bits[7:4], Access Type=RO, default=0x00000009*/

                            /*
                               This field identifies the type of device. It is
                               set to 9h indicating Root Complex Integrated
                               Endpoint for VMD.
                            */
    UINT16 slot_implemented : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Does not apply to Root Complex Integrated
                               Endpoints
                            */
    UINT16 interrupt_message_number : 5;

                            /* Bits[13:9], Access Type=RO, default=0x00000000*/

                            /*
                               Not valid for this device, since the device does
                               not generate interrupts
                            */
    UINT16 rsvd : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PXPCAP_IIO_VMD_STRUCT;

/* PMCAP_IIO_VMD_REG supported on:                                              */
/*      SPRA0 (0x208050e0)                                                      */
/*      SPRB0 (0x208050e0)                                                      */
/*      SPRHBM (0x208050e0)                                                     */
/*      SPRC0 (0x208050e0)                                                      */
/*      SPRMCC (0x208050e0)                                                     */
/*      SPRUCC (0x208050e0)                                                     */
/* Register default value on SPRA0: 0x00030001                                  */
/* Register default value on SPRB0: 0x00030001                                  */
/* Register default value on SPRHBM: 0x00030001                                 */
/* Register default value on SPRC0: 0x00030001                                  */
/* Register default value on SPRMCC: 0x00030001                                 */
/* Register default value on SPRUCC: 0x00030001                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Power Management Capability
*/


#define PMCAP_IIO_VMD_REG 0x114200E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 capability_id : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000001*/

                            /*
                               Provides the PM capability ID assigned by PCI-
                               SIG.
                            */
    UINT32 next_capability_pointer : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               This is the last capability in the chain and
                               hence set to 0.
                            */
    UINT32 version : 3;

                            /* Bits[18:16], Access Type=RO, default=0x00000003*/

                            /*
                               This field is set to 3h (PM 1.2 compliant) as
                               version number for all PCI Express ports.
                            */
    UINT32 pme_clock : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /* Not applicable. */
    UINT32 rsvd : 1;

                            /* Bits[20:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 device_specific_initialization : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /* Not applicable. */
    UINT32 aux_current : 3;

                            /* Bits[24:22], Access Type=RO, default=0x00000000*/

                            /* Not applicable. */
    UINT32 d1_support : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /* VMD does not support power management state D1. */
    UINT32 d2_support : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /* VMD does not support power management state D2. */
    UINT32 pme_support : 5;

                            /* Bits[31:27], Access Type=RO, default=0x00000000*/

                            /*
                               The Volume Management Device does not support
                               generating PME#.
                            */

  } Bits;
  UINT32 Data;

} PMCAP_IIO_VMD_STRUCT;

/* PMCSR_IIO_VMD_REG supported on:                                              */
/*      SPRA0 (0x208050e4)                                                      */
/*      SPRB0 (0x208050e4)                                                      */
/*      SPRHBM (0x208050e4)                                                     */
/*      SPRC0 (0x208050e4)                                                      */
/*      SPRMCC (0x208050e4)                                                     */
/*      SPRUCC (0x208050e4)                                                     */
/* Register default value on SPRA0: 0x00000008                                  */
/* Register default value on SPRB0: 0x00000008                                  */
/* Register default value on SPRHBM: 0x00000008                                 */
/* Register default value on SPRC0: 0x00000008                                  */
/* Register default value on SPRMCC: 0x00000008                                 */
/* Register default value on SPRUCC: 0x00000008                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Power Management Control and Status Register
*/


#define PMCSR_IIO_VMD_REG 0x114200E4

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
                               new power state as well.[br] 00: D0[br] 01: D1
                               (not supported by VMD)[br] 10: D2 (not supported
                               by VMD)[br] 11: D3_hot[br] If Software tries to
                               write 01 or 10 to this field, the power state
                               does not change from the existing power state
                               (which is either D0 or D3hot) and nor do these
                               bits 1:0 change value.[br] In the D3hot state,
                               VMD will not respond to memory/IO transactions,
                               which is equivalent to MSE bit being clear.
                               Also, VMD will not generate any memory
                               transactions as initiator. There is no hardware
                               to enforce this. The VMD driver will enforce
                               this by programming all of the VMD-Owned devices
                               to D3hot when the OS writes 11b to this
                               field.[br] Notes: A write to this register will
                               trigger an interrupt to the VMD driver using the
                               MSI table entry 0.
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 no_soft_reset : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Indicates VMD does not reset its registers when
                               transitioning from D3hot to D0.
                            */
    UINT32 rsvd_4 : 4;

                            /* Bits[7:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pme_enable : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /* Not supported. */
    UINT32 data_select : 4;

                            /* Bits[12:9], Access Type=RO, default=0x00000000*/

                            /* Not relevant for MS2IOSF */
    UINT32 data_scale : 2;

                            /* Bits[14:13], Access Type=RO, default=0x00000000*/

                            /* Not relevant for MS2IOSF */
    UINT32 pme_status : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* Not supported. */
    UINT32 rsvd_16 : 6;

                            /* Bits[21:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 b2_b3_support : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               This field is hardwired to 0h as it does not
                               apply to PCI Express.
                            */
    UINT32 bus_power_clock_control_enable : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /*
                               This field is hardwired to 0h as it does not
                               apply to PCI Express.
                            */
    UINT32 data : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Not relevant for MS2IOSF */

  } Bits;
  UINT32 Data;

} PMCSR_IIO_VMD_STRUCT;

/* PXPEXTCAP_IIO_VMD_REG supported on:                                          */
/*      SPRA0 (0x20805100)                                                      */
/*      SPRB0 (0x20805100)                                                      */
/*      SPRHBM (0x20805100)                                                     */
/*      SPRC0 (0x20805100)                                                      */
/*      SPRMCC (0x20805100)                                                     */
/*      SPRUCC (0x20805100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This field implements the Extended Capability Header required by section 7.9.1 in the PCIe 3.0 spec.
*/


#define PXPEXTCAP_IIO_VMD_REG 0x11420100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extended_capability_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /*
                               Set to zero to indicate that there is no
                               capability at this location.
                            */
    UINT32 capability_version : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000000*/

                            /*
                               Set to zero to indicate that there is no
                               capability at this location.
                            */
    UINT32 next_capability_offset : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000000*/

                            /*
                               Pointer to the next capability in extended
                               configuration space. Set to zero to indicate
                               that there are no more capability structures.
                            */

  } Bits;
  UINT32 Data;

} PXPEXTCAP_IIO_VMD_STRUCT;

/* VDMPROC_CMD_IIO_VMD_REG supported on:                                        */
/*      SPRA0 (0x10805140)                                                      */
/*      SPRB0 (0x10805140)                                                      */
/*      SPRHBM (0x10805140)                                                     */
/*      SPRC0 (0x10805140)                                                      */
/*      SPRMCC (0x10805140)                                                     */
/*      SPRUCC (0x10805140)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VDM Processing CMD register
*/


#define VDMPROC_CMD_IIO_VMD_REG 0x11410140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 reset_vdm_drop_count : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Reset OOBMSM->VMD endpoint VDM MCTP drop count
                               maintained by HW
                            */
    UINT16 reset_mctp_drop_count : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Reset VMD endpoint -> OOBMSM MCTP drop count
                               maintained by HW
                            */
    UINT16 rsvd : 14;

                            /* Bits[15:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} VDMPROC_CMD_IIO_VMD_STRUCT;

/* VDMPROC_STS_IIO_VMD_REG supported on:                                        */
/*      SPRA0 (0x10805142)                                                      */
/*      SPRB0 (0x10805142)                                                      */
/*      SPRHBM (0x10805142)                                                     */
/*      SPRC0 (0x10805142)                                                      */
/*      SPRMCC (0x10805142)                                                     */
/*      SPRUCC (0x10805142)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VDM Processing Status register
*/


#define VDMPROC_STS_IIO_VMD_REG 0x11410142

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 vmd_cfg_vdm_poison : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /* Poison for VMD CFG VDM from OOBMSM detected */
    UINT16 vmd_cfg_vdm_parerr : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Parity error for VMD CFG VDM from OOBMSM
                               detected
                            */
    UINT16 vmd_rx_mailbox_poison : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /* Poison for RX VMD MailBox message detected */
    UINT16 vmd_rx_mailbox_parerr : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /* Parity error for RX VMD MailBox message detected */
    UINT16 vmd_mctp_vdmseq0_poison : 1;

                            /* Bits[4:4], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Poison for VMD MCTP VDM Seq0 message from OOBMSM
                               detected
                            */
    UINT16 vmd_mctp_vdmseq0_parerr : 1;

                            /* Bits[5:5], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Parity error for VMD MCTP VDM Seq0 message from
                               OOBMSM detected
                            */
    UINT16 vdm_mctp_seq_error : 1;

                            /* Bits[6:6], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Signal Sequence error in VDM MCTP packet
                               received from OOBMSM
                            */
    UINT16 rsvd : 9;

                            /* Bits[15:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} VDMPROC_STS_IIO_VMD_STRUCT;

/* VDMPROC_STS2_IIO_VMD_REG supported on:                                       */
/*      SPRA0 (0x20805144)                                                      */
/*      SPRB0 (0x20805144)                                                      */
/*      SPRHBM (0x20805144)                                                     */
/*      SPRC0 (0x20805144)                                                      */
/*      SPRMCC (0x20805144)                                                     */
/*      SPRUCC (0x20805144)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VDM Processing Status register
*/


#define VDMPROC_STS2_IIO_VMD_REG 0x11420144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vdm_drop_count : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Count of OOBMSM->VDM endpoint MCTP VDM packets
                               dropped by the VMD. (bit[15] is overflow bit)
                            */
    UINT32 mctp_drop_count : 16;

                            /* Bits[31:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               Count of VDM endpoint -> OOBMSM MCTP packets
                               dropped by the VMD (bit[15] is overflow bit)
                            */

  } Bits;
  UINT32 Data;

} VDMPROC_STS2_IIO_VMD_STRUCT;

/* M2IOSF_SCRATCHPAD_IIO_VMD_REG supported on:                                  */
/*      SPRA0 (0x208053fc)                                                      */
/*      SPRB0 (0x208053fc)                                                      */
/*      SPRHBM (0x208053fc)                                                     */
/*      SPRC0 (0x208053fc)                                                      */
/*      SPRMCC (0x208053fc)                                                     */
/*      SPRUCC (0x208053fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_5_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_5                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Read-write bits used for validation and debug.
*/


#define M2IOSF_SCRATCHPAD_IIO_VMD_REG 0x114203FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 scratchpad : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Read-write bits used for validation and debug. */

  } Bits;
  UINT32 Data;

} M2IOSF_SCRATCHPAD_IIO_VMD_STRUCT;
#endif /* _IIO_VMD_h */
