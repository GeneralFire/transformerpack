
/** @file
  IIO_M2IAL.h

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


#ifndef _IIO_M2IAL_h
#define _IIO_M2IAL_h
#include <Base.h>

/* VID_IIO_M2IAL_REG supported on:                                              */
/*      SPRA0 (0x10801000)                                                      */
/*      SPRB0 (0x10801000)                                                      */
/*      SPRHBM (0x10801000)                                                     */
/*      SPRC0 (0x10801000)                                                      */
/*      SPRMCC (0x10801000)                                                     */
/*      SPRUCC (0x10801000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Vendor ID
*/


#define VID_IIO_M2IAL_REG 0x22010000

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

} VID_IIO_M2IAL_STRUCT;

/* DID_IIO_M2IAL_REG supported on:                                              */
/*      SPRA0 (0x10801002)                                                      */
/*      SPRB0 (0x10801002)                                                      */
/*      SPRHBM (0x10801002)                                                     */
/*      SPRC0 (0x10801002)                                                      */
/*      SPRMCC (0x10801002)                                                     */
/*      SPRUCC (0x10801002)                                                     */
/* Register default value on SPRA0: 0x000009A4                                  */
/* Register default value on SPRB0: 0x000009A4                                  */
/* Register default value on SPRHBM: 0x000009A4                                 */
/* Register default value on SPRC0: 0x000009A4                                  */
/* Register default value on SPRMCC: 0x000009A4                                 */
/* Register default value on SPRUCC: 0x000009A4                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device ID
*/


#define DID_IIO_M2IAL_REG 0x22010002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x000009A4*/

                            /* Device ID assigned to the M2PCIe. */

  } Bits;
  UINT16 Data;

} DID_IIO_M2IAL_STRUCT;

/* PCICMD_IIO_M2IAL_REG supported on:                                           */
/*      SPRA0 (0x10801004)                                                      */
/*      SPRB0 (0x10801004)                                                      */
/*      SPRHBM (0x10801004)                                                     */
/*      SPRC0 (0x10801004)                                                      */
/*      SPRMCC (0x10801004)                                                     */
/*      SPRUCC (0x10801004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command
*/


#define PCICMD_IIO_M2IAL_REG 0x22010004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 iose : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               IO Space Enable[br] Hardwired to 0 since these
                               devices don't decode any IO BARs
                            */
    UINT16 mse : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Space Enable[br] Hardwired to 0 since
                               these devices don't decode any memory BARs
                            */
    UINT16 bme : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Bus Master Enable[br] Hardwired to 0 since these
                               devices don't generate any transactions
                            */
    UINT16 sce : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Special Cycle Enable[br] Not applicable to
                               internal devices. Hardwired to 0.
                            */
    UINT16 mwie : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Write and Invalidate Enable[br] Not
                               applicable to internal devices. Hardwired to 0.
                            */
    UINT16 vga_palette_snoop_enable : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to internal devices. Hardwired to
                               0.
                            */
    UINT16 perre : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Parity Error Response[br] This bit has no impact
                               on error reporting from these devices
                            */
    UINT16 idsel_stepping_wait_cycle_control : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to internal IIO devices.
                               Hardwired to 0.
                            */
    UINT16 serre : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               SERR Enable[br] This bit has no impact on error
                               reporting from these devices
                            */
    UINT16 fast_back_to_back_enable : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to PCI Express and is hardwired
                               to 0
                            */
    UINT16 interrupt_disable : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /* N/A for these devices */
    UINT16 rsvd : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PCICMD_IIO_M2IAL_STRUCT;

/* PCISTS_IIO_M2IAL_REG supported on:                                           */
/*      SPRA0 (0x10801006)                                                      */
/*      SPRB0 (0x10801006)                                                      */
/*      SPRHBM (0x10801006)                                                     */
/*      SPRC0 (0x10801006)                                                      */
/*      SPRMCC (0x10801006)                                                     */
/*      SPRUCC (0x10801006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status
*/


#define PCISTS_IIO_M2IAL_REG 0x22010006

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
                               Indicates a pending INTx interrupt.[br]
                               Hardwired to 0
                            */
    UINT16 capabilities_list : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               This bit indicates the presence of a
                               capabilities list structure.
                            */
    UINT16 pci66mhz_capable : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI Express. Hardwired to 0. */
    UINT16 rsvd_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 fast_back_to_back : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI Express. Hardwired to 0. */
    UINT16 mdpe : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /* Master Data Parity Error[br] Hardwired to 0 */
    UINT16 devsel_timing : 2;

                            /* Bits[10:9], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI Express. Hardwired to 0. */
    UINT16 sta : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /* Signalled Target Abort[br] Hardwired to 0 */
    UINT16 rta : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /* Received Target Abort[br] Hardwired to 0 */
    UINT16 rma : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /* Received Master Abort[br] Hardwired to 0 */
    UINT16 sse : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* Signalled System Error[br] Hardwired to 0 */
    UINT16 dpe : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* Detected Parity Error[br] Hardwired to 0 */

  } Bits;
  UINT16 Data;

} PCISTS_IIO_M2IAL_STRUCT;

/* RID_CCR_IIO_M2IAL_REG supported on:                                          */
/*      SPRA0 (0x20801008)                                                      */
/*      SPRB0 (0x20801008)                                                      */
/*      SPRHBM (0x20801008)                                                     */
/*      SPRC0 (0x20801008)                                                      */
/*      SPRMCC (0x20801008)                                                     */
/*      SPRUCC (0x20801008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_IIO_M2IAL_REG 0x22020008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 revision_id : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Reflects the M2PCIe Revision ID after reset.
                               MS2IOSF loads this value from a fuse.
                            */
    UINT32 interface : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               Register Level Programming Interface. Set to 0
                               for all non-APIC devices.
                            */
    UINT32 sub_class : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000080*/

                            /* Set to 80h for Generic Device */
    UINT32 base_class : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000008*/

                            /* Set to 08h for Generic Device */

  } Bits;
  UINT32 Data;

} RID_CCR_IIO_M2IAL_STRUCT;

/* CLSR_IIO_M2IAL_REG supported on:                                             */
/*      SPRA0 (0x80100c)                                                        */
/*      SPRB0 (0x80100c)                                                        */
/*      SPRHBM (0x80100c)                                                       */
/*      SPRC0 (0x80100c)                                                        */
/*      SPRMCC (0x80100c)                                                       */
/*      SPRUCC (0x80100c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Cacheline Size Register
*/


#define CLSR_IIO_M2IAL_REG 0x2200000C

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

} CLSR_IIO_M2IAL_STRUCT;

/* HDR_IIO_M2IAL_REG supported on:                                              */
/*      SPRA0 (0x80100e)                                                        */
/*      SPRB0 (0x80100e)                                                        */
/*      SPRHBM (0x80100e)                                                       */
/*      SPRC0 (0x80100e)                                                        */
/*      SPRMCC (0x80100e)                                                       */
/*      SPRUCC (0x80100e)                                                       */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header Type
*/


#define HDR_IIO_M2IAL_REG 0x2200000E

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
                               all these devices. The default is 00h,
                               indicating a 'endpoint device'.
                            */
    UINT8 multi_function_device : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               Set to 1b to indicate functions 1-7 may exist
                               for the device
                            */

  } Bits;
  UINT8 Data;

} HDR_IIO_M2IAL_STRUCT;

/* SVID_IIO_M2IAL_REG supported on:                                             */
/*      SPRA0 (0x1080102c)                                                      */
/*      SPRB0 (0x1080102c)                                                      */
/*      SPRHBM (0x1080102c)                                                     */
/*      SPRC0 (0x1080102c)                                                      */
/*      SPRMCC (0x1080102c)                                                     */
/*      SPRUCC (0x1080102c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Subsystem Vendor ID
*/


#define SVID_IIO_M2IAL_REG 0x2201002C

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

} SVID_IIO_M2IAL_STRUCT;

/* SID_IIO_M2IAL_REG supported on:                                              */
/*      SPRA0 (0x1080102e)                                                      */
/*      SPRB0 (0x1080102e)                                                      */
/*      SPRHBM (0x1080102e)                                                     */
/*      SPRC0 (0x1080102e)                                                      */
/*      SPRMCC (0x1080102e)                                                     */
/*      SPRUCC (0x1080102e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Subsystem ID
*/


#define SID_IIO_M2IAL_REG 0x2201002E

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

} SID_IIO_M2IAL_STRUCT;

/* CAPPTR_IIO_M2IAL_REG supported on:                                           */
/*      SPRA0 (0x801034)                                                        */
/*      SPRB0 (0x801034)                                                        */
/*      SPRHBM (0x801034)                                                       */
/*      SPRC0 (0x801034)                                                        */
/*      SPRMCC (0x801034)                                                       */
/*      SPRUCC (0x801034)                                                       */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Capability Pointer
*/


#define CAPPTR_IIO_M2IAL_REG 0x22000034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 capability_pointer : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000040*/

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

} CAPPTR_IIO_M2IAL_STRUCT;

/* INTL_IIO_M2IAL_REG supported on:                                             */
/*      SPRA0 (0x80103c)                                                        */
/*      SPRB0 (0x80103c)                                                        */
/*      SPRHBM (0x80103c)                                                       */
/*      SPRC0 (0x80103c)                                                        */
/*      SPRMCC (0x80103c)                                                       */
/*      SPRUCC (0x80103c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Interrupt Line Register
*/


#define INTL_IIO_M2IAL_REG 0x2200003C

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

} INTL_IIO_M2IAL_STRUCT;

/* INTPIN_IIO_M2IAL_REG supported on:                                           */
/*      SPRA0 (0x80103d)                                                        */
/*      SPRB0 (0x80103d)                                                        */
/*      SPRHBM (0x80103d)                                                       */
/*      SPRC0 (0x80103d)                                                        */
/*      SPRMCC (0x80103d)                                                       */
/*      SPRUCC (0x80103d)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Interrupt Pin Register
*/


#define INTPIN_IIO_M2IAL_REG 0x2200003D

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

} INTPIN_IIO_M2IAL_STRUCT;

/* PXPCAP_IIO_M2IAL_REG supported on:                                           */
/*      SPRA0 (0x20801040)                                                      */
/*      SPRB0 (0x20801040)                                                      */
/*      SPRHBM (0x20801040)                                                     */
/*      SPRC0 (0x20801040)                                                      */
/*      SPRMCC (0x20801040)                                                     */
/*      SPRUCC (0x20801040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_IIO_M2IAL_REG 0x22020040

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

} PXPCAP_IIO_M2IAL_STRUCT;

/* PXPENHCAP_IIO_M2IAL_REG supported on:                                        */
/*      SPRA0 (0x20801100)                                                      */
/*      SPRB0 (0x20801100)                                                      */
/*      SPRHBM (0x20801100)                                                     */
/*      SPRC0 (0x20801100)                                                      */
/*      SPRMCC (0x20801100)                                                     */
/*      SPRUCC (0x20801100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Enhanced Capability
*/


#define PXPENHCAP_IIO_M2IAL_REG 0x22020100

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

} PXPENHCAP_IIO_M2IAL_STRUCT;

/* UPI_RT_IIO_M2IAL_REG supported on:                                           */
/*      SPRA0 (0x20801140)                                                      */
/*      SPRB0 (0x20801140)                                                      */
/*      SPRHBM (0x20801140)                                                     */
/*      SPRC0 (0x20801140)                                                      */
/*      SPRMCC (0x20801140)                                                     */
/*      SPRUCC (0x20801140)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Intel UPI Routing Table
*/


#define UPI_RT_IIO_M2IAL_REG 0x22020140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 upiroutingtable : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               This field has two bits per NodeID for
                               supporting 3 UPI links and 8 socket systems[br]
                               Bits [1:0] correspond to NodeID0[br] Bits [3:2]
                               correspond to NodeID1[br] ...and so on[br] Value
                               of 0b00 means UPI 0[br] Value of 0b01 means UPI
                               1[br] Value of 0b10 means UPI 2
                            */
    UINT32 upiinterleavemode : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /*
                               2 bit interleave mode programmed by BIOS, based
                               on topology. Interleave Mode 0: QPI_RT looked up
                               using destination node id [legacy behavior, for
                               all topologies with single UPI connection
                               between node-ids, also serves as defeature]
                               Interleave Mode 1: Intended for 2S-2L. BIOS is
                               expected to program the UPI port information in
                               the correct index positions of 0, 1. Interleave
                               Mode 2: Intended for 2S-3L. BIOS is expected to
                               program the UPI port information in the correct
                               index positions of 0,1,2.
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UPI_RT_IIO_M2IAL_STRUCT;

/* IIO_BW_COUNTER_IIO_M2IAL_REG supported on:                                   */
/*      SPRA0 (0x20801144)                                                      */
/*      SPRB0 (0x20801144)                                                      */
/*      SPRHBM (0x20801144)                                                     */
/*      SPRC0 (0x20801144)                                                      */
/*      SPRMCC (0x20801144)                                                     */
/*      SPRUCC (0x20801144)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* IIO Bandwidth Counter
*/


#define IIO_BW_COUNTER_IIO_M2IAL_REG 0x22020144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iio_bandwidth_counter : 30;

                            /* Bits[29:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Free running counter that increments on each AD
                               request sent to the ring. Pcode uses this for
                               power metering and also for uncore P state
                               related decisions. Pcode can clear this counter
                               by writing a 1 to all bits in this field, at
                               which time the counter starts from 0.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IIO_BW_COUNTER_IIO_M2IAL_STRUCT;

/* R2P2PINITSHCRED0_IIO_M2IAL_REG supported on:                                 */
/*      SPRA0 (0x20801160)                                                      */
/*      SPRB0 (0x20801160)                                                      */
/*      SPRHBM (0x20801160)                                                     */
/*      SPRC0 (0x20801160)                                                      */
/*      SPRMCC (0x20801160)                                                     */
/*      SPRUCC (0x20801160)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Initial P2P shared credit count
*/


#define R2P2PINITSHCRED0_IIO_M2IAL_REG 0x22020160

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent0_initcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */
    UINT32 meshagent1_initcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */
    UINT32 meshagent2_initcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */
    UINT32 meshagent3_initcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */

  } Bits;
  UINT32 Data;

} R2P2PINITSHCRED0_IIO_M2IAL_STRUCT;

/* R2P2PINITSHCRED1_IIO_M2IAL_REG supported on:                                 */
/*      SPRA0 (0x20801164)                                                      */
/*      SPRB0 (0x20801164)                                                      */
/*      SPRHBM (0x20801164)                                                     */
/*      SPRC0 (0x20801164)                                                      */
/*      SPRMCC (0x20801164)                                                     */
/*      SPRUCC (0x20801164)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Initial P2P shared credit count
*/


#define R2P2PINITSHCRED1_IIO_M2IAL_REG 0x22020164

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent4_initcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */
    UINT32 meshagent5_initcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */
    UINT32 meshagent6_initcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */
    UINT32 meshagent7_initcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */

  } Bits;
  UINT32 Data;

} R2P2PINITSHCRED1_IIO_M2IAL_STRUCT;

/* R2P2PINITSHCRED2_IIO_M2IAL_REG supported on:                                 */
/*      SPRA0 (0x20801168)                                                      */
/*      SPRB0 (0x20801168)                                                      */
/*      SPRHBM (0x20801168)                                                     */
/*      SPRC0 (0x20801168)                                                      */
/*      SPRMCC (0x20801168)                                                     */
/*      SPRUCC (0x20801168)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Initial P2P shared credit count
*/


#define R2P2PINITSHCRED2_IIO_M2IAL_REG 0x22020168

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent8_initcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */
    UINT32 meshagent9_initcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */
    UINT32 meshagent10_initcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */
    UINT32 meshagent11_initcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */

  } Bits;
  UINT32 Data;

} R2P2PINITSHCRED2_IIO_M2IAL_STRUCT;

/* R2P2PINITSHCRED3_IIO_M2IAL_REG supported on:                                 */
/*      SPRA0 (0x2080116c)                                                      */
/*      SPRB0 (0x2080116c)                                                      */
/*      SPRHBM (0x2080116c)                                                     */
/*      SPRC0 (0x2080116c)                                                      */
/*      SPRMCC (0x2080116c)                                                     */
/*      SPRUCC (0x2080116c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Initial P2P shared credit count
*/


#define R2P2PINITSHCRED3_IIO_M2IAL_REG 0x2202016C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent12_initcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */
    UINT32 meshagent13_initcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */
    UINT32 meshagent14_initcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */
    UINT32 meshagent15_initcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               The number written to this register will cause
                               an equal number of credits to be placed on the
                               shared credit ring for the given agent (which
                               may not be the same agent as where this register
                               resides).
                            */

  } Bits;
  UINT32 Data;

} R2P2PINITSHCRED3_IIO_M2IAL_STRUCT;



/* R2DEFEATURE_IIO_M2IAL_REG supported on:                                      */
/*      SPRA0 (0x20801178)                                                      */
/*      SPRB0 (0x20801178)                                                      */
/*      SPRHBM (0x20801178)                                                     */
/*      SPRC0 (0x20801178)                                                      */
/*      SPRMCC (0x20801178)                                                     */
/*      SPRUCC (0x20801178)                                                     */
/* Register default value on SPRA0: 0x00000020                                  */
/* Register default value on SPRB0: 0x00000020                                  */
/* Register default value on SPRHBM: 0x00000020                                 */
/* Register default value on SPRC0: 0x00000020                                  */
/* Register default value on SPRMCC: 0x00000020                                 */
/* Register default value on SPRUCC: 0x00000020                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe defeature register
*/


#define R2DEFEATURE_IIO_M2IAL_REG 0x22020178

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 p2p_sharedcredring_drs_def : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, this bit disables MS2IOSF DRS
                               transactions from using the shared P2P credit
                               ring. Instead, MS2IOSF will use dedicated DRS
                               credits towards peer agents as configured in the
                               R2P2PDRSDEDCRED0/1 registers
                            */
    UINT32 p2p_sharedcredring_ncb_def : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, this bit disables MS2IOSF NCB
                               transactions from using the shared P2P credit
                               ring. Instead, MS2IOSF will use dedicated NCB
                               credits towards peer agents as configured in the
                               R2P2PNCBDEDCRED0/1 registers
                            */
    UINT32 p2p_sharedcredring_ncs_def : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, this bit disables MS2IOSF NCS
                               transactions from using the shared P2P credit
                               ring. Instead, MS2IOSF will use dedicated NCS
                               credits towards peer agents as configured in the
                               R2P2PNCSDEDCRED0/1 registers
                            */
    UINT32 bl_egr_chastripe_def : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, this bit disables MS2IOSF CHA
                               transactions sending BL transactions on both CMS
                               Port0/1. When the bit is disabled the traffic
                               will be sent on only port0.
                            */
    UINT32 bl_egr_p2p_postedro_def : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, this bit disables MS2IOSF P2P Posted
                               RO transactions sending BL transactions on both
                               CMS ADD Port0/1. When the bit is disabled the
                               traffic will be sent on only port0.
                            */
    UINT32 bl_egr_p2p_nonpostedro_def : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /*
                               When set, this bit disables MS2IOSF P2P
                               NonPosted RO transactions sending BL
                               transactions on both CMS drop Port0/1. When the
                               bit is disabled the traffic will be sent on only
                               port0.
                            */
    UINT32 bl_egr_p2p_cmplro_def : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, this bit disables MS2IOSF P2P
                               Completion RO transactions sending BL
                               transactions on both CMS drop Port0/1. When the
                               bit is disabled the traffic will be sent on only
                               port0.
                            */
    UINT32 spare_defeature : 25;

                            /* Bits[31:7], Access Type=RW/P, default=0x00000000*/

                            /* Spare Defeature */

  } Bits;
  UINT32 Data;

} R2DEFEATURE_IIO_M2IAL_STRUCT;

/* R2PRQINITCRED0_IIO_M2IAL_REG supported on:                                   */
/*      SPRA0 (0x20801180)                                                      */
/*      SPRB0 (0x20801180)                                                      */
/*      SPRHBM (0x20801180)                                                     */
/*      SPRC0 (0x20801180)                                                      */
/*      SPRMCC (0x20801180)                                                     */
/*      SPRUCC (0x20801180)                                                     */
/* Register default value on SPRA0: 0x33333333                                  */
/* Register default value on SPRB0: 0x33333333                                  */
/* Register default value on SPRHBM: 0x33333333                                 */
/* Register default value on SPRC0: 0x33333333                                  */
/* Register default value on SPRMCC: 0x33333333                                 */
/* Register default value on SPRUCC: 0x33333333                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* PRQ inital credit to each CHA. bit[31:0] maps to CHA[7:0](if applicable), 4-bit per CHA. [3:0] for CHA0 as so on
*/


#define R2PRQINITCRED0_IIO_M2IAL_REG 0x22020180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cboprqinitialcred_07_00 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x33333333*/

                            /* Cboprqinitialcred 07 00 */

  } Bits;
  UINT32 Data;

} R2PRQINITCRED0_IIO_M2IAL_STRUCT;

/* R2PRQINITCRED1_IIO_M2IAL_REG supported on:                                   */
/*      SPRA0 (0x20801184)                                                      */
/*      SPRB0 (0x20801184)                                                      */
/*      SPRHBM (0x20801184)                                                     */
/*      SPRC0 (0x20801184)                                                      */
/*      SPRMCC (0x20801184)                                                     */
/*      SPRUCC (0x20801184)                                                     */
/* Register default value on SPRA0: 0x33333333                                  */
/* Register default value on SPRB0: 0x33333333                                  */
/* Register default value on SPRHBM: 0x33333333                                 */
/* Register default value on SPRC0: 0x33333333                                  */
/* Register default value on SPRMCC: 0x33333333                                 */
/* Register default value on SPRUCC: 0x33333333                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* PRQ inital credit to each CHA. bit[31:0] maps to CHA[15:8](if applicable), 4-bit per CHA. [3:0] for CHA8 as so on
*/


#define R2PRQINITCRED1_IIO_M2IAL_REG 0x22020184

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cboprqinitialcred_15_08 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x33333333*/

                            /* Cboprqinitialcred 15 08 */

  } Bits;
  UINT32 Data;

} R2PRQINITCRED1_IIO_M2IAL_STRUCT;

/* R2PRQINITCRED2_IIO_M2IAL_REG supported on:                                   */
/*      SPRA0 (0x20801188)                                                      */
/*      SPRB0 (0x20801188)                                                      */
/*      SPRHBM (0x20801188)                                                     */
/*      SPRC0 (0x20801188)                                                      */
/*      SPRMCC (0x20801188)                                                     */
/*      SPRUCC (0x20801188)                                                     */
/* Register default value on SPRA0: 0x33333333                                  */
/* Register default value on SPRB0: 0x33333333                                  */
/* Register default value on SPRHBM: 0x33333333                                 */
/* Register default value on SPRC0: 0x33333333                                  */
/* Register default value on SPRMCC: 0x33333333                                 */
/* Register default value on SPRUCC: 0x33333333                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* PRQ inital credit to each CHA. bit[31:0] maps to CHA[23:16](if applicable), 4-bit per CHA. [3:0] for CHA16 as so on
*/


#define R2PRQINITCRED2_IIO_M2IAL_REG 0x22020188

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cboprqinitialcred_23_16 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x33333333*/

                            /* Cboprqinitialcred 23 16 */

  } Bits;
  UINT32 Data;

} R2PRQINITCRED2_IIO_M2IAL_STRUCT;

/* R2PRQINITCRED3_IIO_M2IAL_REG supported on:                                   */
/*      SPRA0 (0x2080118c)                                                      */
/*      SPRB0 (0x2080118c)                                                      */
/*      SPRHBM (0x2080118c)                                                     */
/*      SPRC0 (0x2080118c)                                                      */
/*      SPRMCC (0x2080118c)                                                     */
/*      SPRUCC (0x2080118c)                                                     */
/* Register default value on SPRA0: 0x33333333                                  */
/* Register default value on SPRB0: 0x33333333                                  */
/* Register default value on SPRHBM: 0x33333333                                 */
/* Register default value on SPRC0: 0x33333333                                  */
/* Register default value on SPRMCC: 0x33333333                                 */
/* Register default value on SPRUCC: 0x33333333                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* PRQ inital credit to each CHA. bit[31:0] maps to CHA[31:24](if applicable), 4-bit per CHA. [3:0] for CHA24 as so on
*/


#define R2PRQINITCRED3_IIO_M2IAL_REG 0x2202018C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cboprqinitialcred_31_24 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x33333333*/

                            /* Cboprqinitialcred 31 24 */

  } Bits;
  UINT32 Data;

} R2PRQINITCRED3_IIO_M2IAL_STRUCT;

/* R2PRQINITCRED4_IIO_M2IAL_REG supported on:                                   */
/*      SPRA0 (0x20801190)                                                      */
/*      SPRB0 (0x20801190)                                                      */
/*      SPRHBM (0x20801190)                                                     */
/*      SPRC0 (0x20801190)                                                      */
/*      SPRMCC (0x20801190)                                                     */
/*      SPRUCC (0x20801190)                                                     */
/* Register default value on SPRA0: 0x33333333                                  */
/* Register default value on SPRB0: 0x33333333                                  */
/* Register default value on SPRHBM: 0x33333333                                 */
/* Register default value on SPRC0: 0x33333333                                  */
/* Register default value on SPRMCC: 0x33333333                                 */
/* Register default value on SPRUCC: 0x33333333                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* PRQ inital credit to each CHA. bit[31:0] maps to CHA[39:32](if applicable), 4-bit per CHA. [3:0] for CHA32 as so on
*/


#define R2PRQINITCRED4_IIO_M2IAL_REG 0x22020190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cboprqinitialcred_39_32 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x33333333*/

                            /* Cboprqinitialcred 39 32 */

  } Bits;
  UINT32 Data;

} R2PRQINITCRED4_IIO_M2IAL_STRUCT;

/* R2PRQINITCRED5_IIO_M2IAL_REG supported on:                                   */
/*      SPRA0 (0x20801194)                                                      */
/*      SPRB0 (0x20801194)                                                      */
/*      SPRHBM (0x20801194)                                                     */
/*      SPRC0 (0x20801194)                                                      */
/*      SPRMCC (0x20801194)                                                     */
/*      SPRUCC (0x20801194)                                                     */
/* Register default value on SPRA0: 0x33333333                                  */
/* Register default value on SPRB0: 0x33333333                                  */
/* Register default value on SPRHBM: 0x33333333                                 */
/* Register default value on SPRC0: 0x33333333                                  */
/* Register default value on SPRMCC: 0x33333333                                 */
/* Register default value on SPRUCC: 0x33333333                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* PRQ inital credit to each CHA. bit[31:0] maps to CHA[47:40](if applicable), 4-bit per CHA. [3:0] for CHA40 as so on
*/


#define R2PRQINITCRED5_IIO_M2IAL_REG 0x22020194

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cboprqinitialcred_47_40 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x33333333*/

                            /* Cboprqinitialcred 47 40 */

  } Bits;
  UINT32 Data;

} R2PRQINITCRED5_IIO_M2IAL_STRUCT;

/* R2PRQINITCRED6_IIO_M2IAL_REG supported on:                                   */
/*      SPRA0 (0x20801198)                                                      */
/*      SPRB0 (0x20801198)                                                      */
/*      SPRHBM (0x20801198)                                                     */
/*      SPRC0 (0x20801198)                                                      */
/*      SPRMCC (0x20801198)                                                     */
/*      SPRUCC (0x20801198)                                                     */
/* Register default value on SPRA0: 0x33333333                                  */
/* Register default value on SPRB0: 0x33333333                                  */
/* Register default value on SPRHBM: 0x33333333                                 */
/* Register default value on SPRC0: 0x33333333                                  */
/* Register default value on SPRMCC: 0x33333333                                 */
/* Register default value on SPRUCC: 0x33333333                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* PRQ inital credit to each CHA. bit[31:0] maps to CHA[55:48](if applicable), 4-bit per CHA. [3:0] for CHA48 as so on
*/


#define R2PRQINITCRED6_IIO_M2IAL_REG 0x22020198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cboprqinitialcred_55_48 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x33333333*/

                            /* Cboprqinitialcred 55 48 */

  } Bits;
  UINT32 Data;

} R2PRQINITCRED6_IIO_M2IAL_STRUCT;

/* R2PRQINITCRED7_IIO_M2IAL_REG supported on:                                   */
/*      SPRA0 (0x2080119c)                                                      */
/*      SPRB0 (0x2080119c)                                                      */
/*      SPRHBM (0x2080119c)                                                     */
/*      SPRC0 (0x2080119c)                                                      */
/*      SPRMCC (0x2080119c)                                                     */
/*      SPRUCC (0x2080119c)                                                     */
/* Register default value on SPRA0: 0x33333333                                  */
/* Register default value on SPRB0: 0x33333333                                  */
/* Register default value on SPRHBM: 0x33333333                                 */
/* Register default value on SPRC0: 0x33333333                                  */
/* Register default value on SPRMCC: 0x33333333                                 */
/* Register default value on SPRUCC: 0x33333333                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* PRQ inital credit to each CHA. bit[31:0] maps to CHA[63:56](if applicable), 4-bit per CHA. [3:0] for CHA56 as so on
*/


#define R2PRQINITCRED7_IIO_M2IAL_REG 0x2202019C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cboprqinitialcred_63_56 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x33333333*/

                            /* Cboprqinitialcred 63 56 */

  } Bits;
  UINT32 Data;

} R2PRQINITCRED7_IIO_M2IAL_STRUCT;

/* R2NDRTOKTIINITCRED_IIO_M2IAL_REG supported on:                               */
/*      SPRA0 (0x208011a4)                                                      */
/*      SPRB0 (0x208011a4)                                                      */
/*      SPRHBM (0x208011a4)                                                     */
/*      SPRC0 (0x208011a4)                                                      */
/*      SPRMCC (0x208011a4)                                                     */
/*      SPRUCC (0x208011a4)                                                     */
/* Register default value on SPRA0: 0x01010101                                  */
/* Register default value on SPRB0: 0x01010101                                  */
/* Register default value on SPRHBM: 0x01010101                                 */
/* Register default value on SPRC0: 0x01010101                                  */
/* Register default value on SPRMCC: 0x01010101                                 */
/* Register default value on SPRUCC: 0x01010101                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* NDR credits allocated to this destination for each UPI Agent. 8-bit per agent: [br]
[7:0]: Intel UPI 0 [br]
[15:8]: Intel UPI 1 [br]
[23:16]: Intel UPI 2 [br]
[31:24]: Intel UPI 3
*/


#define R2NDRTOKTIINITCRED_IIO_M2IAL_REG 0x220201A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ndrdstktiinitialcrd : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x01010101*/

                            /* Ndrdstktiinitialcrd */

  } Bits;
  UINT32 Data;

} R2NDRTOKTIINITCRED_IIO_M2IAL_STRUCT;

/* R2IIO2UBOXINITCRED_IIO_M2IAL_REG supported on:                               */
/*      SPRA0 (0x208011a8)                                                      */
/*      SPRB0 (0x208011a8)                                                      */
/*      SPRHBM (0x208011a8)                                                     */
/*      SPRC0 (0x208011a8)                                                      */
/*      SPRMCC (0x208011a8)                                                     */
/*      SPRUCC (0x208011a8)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* NDR credits allocated to this destination for Ubox.
*/


#define R2IIO2UBOXINITCRED_IIO_M2IAL_REG 0x220201A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vn0ncbiio2ubxinitialcred : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000001*/

                            /* Initial credit for request from MS2IOSF to Ubox */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2IIO2UBOXINITCRED_IIO_M2IAL_STRUCT;


/* UFS_INGRESS_IIO_M2IAL_REG supported on:                                      */
/*      SPRA0 (0x208011e0)                                                      */
/*      SPRB0 (0x208011e0)                                                      */
/*      SPRHBM (0x208011e0)                                                     */
/*      SPRC0 (0x208011e0)                                                      */
/*      SPRMCC (0x208011e0)                                                     */
/*      SPRUCC (0x208011e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MS2IOSF UFS BL Ingress Count
*/


#define UFS_INGRESS_IIO_M2IAL_REG 0x220201E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 blingresscount : 32;

                            /* Bits[31:0], Access Type=RO/C/V, default=0x00000000*/

                            /* Blingresscount */

  } Bits;
  UINT32 Data;

} UFS_INGRESS_IIO_M2IAL_STRUCT;

/* UFS_INGRESS_LSBS_IIO_M2IAL_REG supported on:                                 */
/*      SPRA0 (0x208011e4)                                                      */
/*      SPRB0 (0x208011e4)                                                      */
/*      SPRHBM (0x208011e4)                                                     */
/*      SPRC0 (0x208011e4)                                                      */
/*      SPRMCC (0x208011e4)                                                     */
/*      SPRUCC (0x208011e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MS2IOSF UFS BL Ingress Count LSBs
*/


#define UFS_INGRESS_LSBS_IIO_M2IAL_REG 0x220201E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 blingresscountlsbs : 8;

                            /* Bits[7:0], Access Type=RO/C/V, default=0x00000000*/

                            /* Blingresscountlsbs */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UFS_INGRESS_LSBS_IIO_M2IAL_STRUCT;

/* UFS_EGRESS_IIO_M2IAL_REG supported on:                                       */
/*      SPRA0 (0x208011e8)                                                      */
/*      SPRB0 (0x208011e8)                                                      */
/*      SPRHBM (0x208011e8)                                                     */
/*      SPRC0 (0x208011e8)                                                      */
/*      SPRMCC (0x208011e8)                                                     */
/*      SPRUCC (0x208011e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MS2IOSF UFS BL Egress Count
*/


#define UFS_EGRESS_IIO_M2IAL_REG 0x220201E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 blegresscount : 32;

                            /* Bits[31:0], Access Type=RO/C/V, default=0x00000000*/

                            /* Blegresscount */

  } Bits;
  UINT32 Data;

} UFS_EGRESS_IIO_M2IAL_STRUCT;

/* UFS_EGRESS_LSBS_IIO_M2IAL_REG supported on:                                  */
/*      SPRA0 (0x208011ec)                                                      */
/*      SPRB0 (0x208011ec)                                                      */
/*      SPRHBM (0x208011ec)                                                     */
/*      SPRC0 (0x208011ec)                                                      */
/*      SPRMCC (0x208011ec)                                                     */
/*      SPRUCC (0x208011ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* MS2IOSF UFS BL Egress Count LSBs
*/


#define UFS_EGRESS_LSBS_IIO_M2IAL_REG 0x220201EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 blegresscountlsbs : 8;

                            /* Bits[7:0], Access Type=RO/C/V, default=0x00000000*/

                            /* Blegresscountlsbs */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UFS_EGRESS_LSBS_IIO_M2IAL_STRUCT;

/* R2LBWPRQINITCRED0_IIO_M2IAL_REG supported on:                                */
/*      SPRA0 (0x20801200)                                                      */
/*      SPRB0 (0x20801200)                                                      */
/*      SPRHBM (0x20801200)                                                     */
/*      SPRC0 (0x20801200)                                                      */
/*      SPRMCC (0x20801200)                                                     */
/*      SPRUCC (0x20801200)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Low BW PRQ inital credit to each CHA. Bit[31:0] maps to CHA[7:0] (if applicable), with 4 bits per CHA: [3:0] for CHA 0, [7:4] for CHA 1, and so on.
*/


#define R2LBWPRQINITCRED0_IIO_M2IAL_REG 0x22020200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lowbwcboprqinitialcred_07_00 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x11111111*/

                            /* Lowbwcboprqinitialcred 07 00 */

  } Bits;
  UINT32 Data;

} R2LBWPRQINITCRED0_IIO_M2IAL_STRUCT;

/* R2LBWPRQINITCRED1_IIO_M2IAL_REG supported on:                                */
/*      SPRA0 (0x20801204)                                                      */
/*      SPRB0 (0x20801204)                                                      */
/*      SPRHBM (0x20801204)                                                     */
/*      SPRC0 (0x20801204)                                                      */
/*      SPRMCC (0x20801204)                                                     */
/*      SPRUCC (0x20801204)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Low BW PRQ inital credit to each CHA. Bit[31:0] maps to CHA[15:8] (if applicable), with 4 bits per CHA: [3:0] for CHA 8, [7:4] for CHA 9, and so on.
*/


#define R2LBWPRQINITCRED1_IIO_M2IAL_REG 0x22020204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lowbwcboprqinitialcred_15_08 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x11111111*/

                            /* Lowbwcboprqinitialcred 15 08 */

  } Bits;
  UINT32 Data;

} R2LBWPRQINITCRED1_IIO_M2IAL_STRUCT;

/* R2LBWPRQINITCRED2_IIO_M2IAL_REG supported on:                                */
/*      SPRA0 (0x20801208)                                                      */
/*      SPRB0 (0x20801208)                                                      */
/*      SPRHBM (0x20801208)                                                     */
/*      SPRC0 (0x20801208)                                                      */
/*      SPRMCC (0x20801208)                                                     */
/*      SPRUCC (0x20801208)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Low BW PRQ inital credit to each CHA. Bit[31:0] maps to CHA[23:16] (if applicable), with 4 bits per CHA: [3:0] for CHA 16, [7:4] for CHA 17, and so on.
*/


#define R2LBWPRQINITCRED2_IIO_M2IAL_REG 0x22020208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lowbwcboprqinitialcred_23_16 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x11111111*/

                            /* Lowbwcboprqinitialcred 23 16 */

  } Bits;
  UINT32 Data;

} R2LBWPRQINITCRED2_IIO_M2IAL_STRUCT;

/* R2LBWPRQINITCRED3_IIO_M2IAL_REG supported on:                                */
/*      SPRA0 (0x2080120c)                                                      */
/*      SPRB0 (0x2080120c)                                                      */
/*      SPRHBM (0x2080120c)                                                     */
/*      SPRC0 (0x2080120c)                                                      */
/*      SPRMCC (0x2080120c)                                                     */
/*      SPRUCC (0x2080120c)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Low BW PRQ inital credit to each CHA. Bit[31:0] maps to CHA[31:24] (if applicable), with 4 bits per CHA: [3:0] for CHA 24, [7:4] for CHA 25, and so on.
*/


#define R2LBWPRQINITCRED3_IIO_M2IAL_REG 0x2202020C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lowbwcboprqinitialcred_31_24 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x11111111*/

                            /* Lowbwcboprqinitialcred 31 24 */

  } Bits;
  UINT32 Data;

} R2LBWPRQINITCRED3_IIO_M2IAL_STRUCT;

/* R2LBWPRQINITCRED4_IIO_M2IAL_REG supported on:                                */
/*      SPRA0 (0x20801210)                                                      */
/*      SPRB0 (0x20801210)                                                      */
/*      SPRHBM (0x20801210)                                                     */
/*      SPRC0 (0x20801210)                                                      */
/*      SPRMCC (0x20801210)                                                     */
/*      SPRUCC (0x20801210)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Low BW PRQ inital credit to each CHA. Bit[31:0] maps to CHA[39:32] (if applicable), with 4 bits per CHA: [3:0] for CHA 32, [7:4] for CHA 33, and so on.
*/


#define R2LBWPRQINITCRED4_IIO_M2IAL_REG 0x22020210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lowbwcboprqinitialcred_39_32 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x11111111*/

                            /* Lowbwcboprqinitialcred 39 32 */

  } Bits;
  UINT32 Data;

} R2LBWPRQINITCRED4_IIO_M2IAL_STRUCT;

/* R2LBWPRQINITCRED5_IIO_M2IAL_REG supported on:                                */
/*      SPRA0 (0x20801214)                                                      */
/*      SPRB0 (0x20801214)                                                      */
/*      SPRHBM (0x20801214)                                                     */
/*      SPRC0 (0x20801214)                                                      */
/*      SPRMCC (0x20801214)                                                     */
/*      SPRUCC (0x20801214)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Low BW PRQ inital credit to each CHA. Bit[31:0] maps to CHA[47:40] (if applicable), with 4 bits per CHA: [3:0] for CHA 40, [7:4] for CHA 41, and so on.
*/


#define R2LBWPRQINITCRED5_IIO_M2IAL_REG 0x22020214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lowbwcboprqinitialcred_47_40 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x11111111*/

                            /* Lowbwcboprqinitialcred 47 40 */

  } Bits;
  UINT32 Data;

} R2LBWPRQINITCRED5_IIO_M2IAL_STRUCT;

/* R2LBWPRQINITCRED6_IIO_M2IAL_REG supported on:                                */
/*      SPRA0 (0x20801218)                                                      */
/*      SPRB0 (0x20801218)                                                      */
/*      SPRHBM (0x20801218)                                                     */
/*      SPRC0 (0x20801218)                                                      */
/*      SPRMCC (0x20801218)                                                     */
/*      SPRUCC (0x20801218)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Low BW PRQ inital credit to each CHA. Bit[31:0] maps to CHA[55:48] (if applicable), with 4 bits per CHA: [3:0] for CHA 48, [7:4] for CHA 49, and so on.
*/


#define R2LBWPRQINITCRED6_IIO_M2IAL_REG 0x22020218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lowbwcboprqinitialcred_55_48 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x11111111*/

                            /* Lowbwcboprqinitialcred 55 48 */

  } Bits;
  UINT32 Data;

} R2LBWPRQINITCRED6_IIO_M2IAL_STRUCT;

/* R2LBWPRQINITCRED7_IIO_M2IAL_REG supported on:                                */
/*      SPRA0 (0x2080121c)                                                      */
/*      SPRB0 (0x2080121c)                                                      */
/*      SPRHBM (0x2080121c)                                                     */
/*      SPRC0 (0x2080121c)                                                      */
/*      SPRMCC (0x2080121c)                                                     */
/*      SPRUCC (0x2080121c)                                                     */
/* Register default value on SPRA0: 0x11111111                                  */
/* Register default value on SPRB0: 0x11111111                                  */
/* Register default value on SPRHBM: 0x11111111                                 */
/* Register default value on SPRC0: 0x11111111                                  */
/* Register default value on SPRMCC: 0x11111111                                 */
/* Register default value on SPRUCC: 0x11111111                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Low BW PRQ inital credit to each CHA. Bit[31:0] maps to CHA[63:56] (if applicable), with 4 bits per CHA: [3:0] for CHA 56, [7:4] for CHA 57, and so on.
*/


#define R2LBWPRQINITCRED7_IIO_M2IAL_REG 0x2202021C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lowbwcboprqinitialcred_63_56 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x11111111*/

                            /* Lowbwcboprqinitialcred 63 56 */

  } Bits;
  UINT32 Data;

} R2LBWPRQINITCRED7_IIO_M2IAL_STRUCT;

/* R2LBWPRQCFG_IIO_M2IAL_REG supported on:                                      */
/*      SPRA0 (0x20801220)                                                      */
/*      SPRB0 (0x20801220)                                                      */
/*      SPRHBM (0x20801220)                                                     */
/*      SPRC0 (0x20801220)                                                      */
/*      SPRMCC (0x20801220)                                                     */
/*      SPRUCC (0x20801220)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Contains defeature for DDRT credit feature. 
*/


#define R2LBWPRQCFG_IIO_M2IAL_REG 0x22020220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ddrt_defeature : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               Defeature for DDRT credit feature. If this is
                               set, all PRQ credits will get loaded in the DDR4
                               credit counters, DDRT counters will be disabled
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2LBWPRQCFG_IIO_M2IAL_STRUCT;

/* R22LMDDRTCFG_IIO_M2IAL_REG supported on:                                     */
/*      SPRA0 (0x20801248)                                                      */
/*      SPRB0 (0x20801248)                                                      */
/*      SPRHBM (0x20801248)                                                     */
/*      SPRC0 (0x20801248)                                                      */
/*      SPRMCC (0x20801248)                                                     */
/*      SPRUCC (0x20801248)                                                     */
/* Register default value on SPRA0: 0x00010000                                  */
/* Register default value on SPRB0: 0x00010000                                  */
/* Register default value on SPRHBM: 0x00010000                                 */
/* Register default value on SPRC0: 0x00010000                                  */
/* Register default value on SPRMCC: 0x00010000                                 */
/* Register default value on SPRUCC: 0x00010000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Contains 2LM DDRT Count thresholds and saturation value.
*/


#define R22LMDDRTCFG_IIO_M2IAL_REG 0x22020248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 r22lmddrtth : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Compare this threshold with 2LM DDRT Count for
                               2LM QoS feature.
                            */
    UINT32 r22lmddrtloth : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Clear DDRT distress flag when 2LM DDRT Count <
                               2LMDDRTLoTh.
                            */
    UINT32 r22lmthdisabled : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000001*/

                            /* Disable 2LM throttling. */
    UINT32 r22lmddrtcleardistressabvth : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, clear distress even if DDRT Count is
                               above threshold and non-distress response is
                               received.
                            */
    UINT32 r22lmdistresswiredisable : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* Disable 2LM throttling. */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R22LMDDRTCFG_IIO_M2IAL_STRUCT;

/* R22LMDDRTCOUNTERSCFG_IIO_M2IAL_REG supported on:                             */
/*      SPRA0 (0x2080124c)                                                      */
/*      SPRB0 (0x2080124c)                                                      */
/*      SPRHBM (0x2080124c)                                                     */
/*      SPRC0 (0x2080124c)                                                      */
/*      SPRMCC (0x2080124c)                                                     */
/*      SPRUCC (0x2080124c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Contains 2LM DDRT Counter saturation value and 2LM Leaky Bucket Time Window Counter initial value.
*/


#define R22LMDDRTCOUNTERSCFG_IIO_M2IAL_REG 0x2202024C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 r22lmddrtcountsatvalue : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Saturating max counter value for 2LM DDRT
                               counter.
                            */
    UINT32 r22lmtimewindow : 16;

                            /* Bits[31:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Initial value for 2LM Leaky Bucket Time Window
                               Counter.
                            */

  } Bits;
  UINT32 Data;

} R22LMDDRTCOUNTERSCFG_IIO_M2IAL_STRUCT;

/* R2GLERRCFG_IIO_M2IAL_REG supported on:                                       */
/*      SPRA0 (0x20801250)                                                      */
/*      SPRB0 (0x20801250)                                                      */
/*      SPRHBM (0x20801250)                                                     */
/*      SPRC0 (0x20801250)                                                      */
/*      SPRMCC (0x20801250)                                                     */
/*      SPRUCC (0x20801250)                                                     */
/* Register default value on SPRA0: 0x00006780                                  */
/* Register default value on SPRB0: 0x00006780                                  */
/* Register default value on SPRHBM: 0x00006780                                 */
/* Register default value on SPRC0: 0x00006780                                  */
/* Register default value on SPRMCC: 0x00006780                                 */
/* Register default value on SPRUCC: 0x00006780                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe global viral/fatal error configuration
*/


#define R2GLERRCFG_IIO_M2IAL_REG 0x22020250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 3;

                            /* Bits[2:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 resetglobalfatalerror : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Set 1 to force clear global fatal error status. */
    UINT32 resetglobalviral : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Set 1 to force clear global viral status. */
    UINT32 maskexternviralinin : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* Set 1 to block external Viral signals from Mesh */
    UINT32 maskexternfatalerrorin : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               Set 1 to block external fatal error signals from
                               Mesh
                            */
    UINT32 maskiiofatalerrorin : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /*
                               Set 1 to block fatal error status from MS2IOSF
                               to global fatal status
                            */
    UINT32 maskialfatalerrorin : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               Set 1 to block viral error status from
                               IAL/M2IOSF to global viral status
                            */
    UINT32 maskiioviralout : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000001*/

                            /*
                               Set 1 to block Viral status from global viral
                               status to MS2IOSF
                            */
    UINT32 maskiioviralin : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000001*/

                            /*
                               Set 1 to block Viral status from MS2IOSF to
                               global viral status
                            */
    UINT32 rsvd_11 : 2;

                            /* Bits[12:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 maskr2fatalerror : 2;

                            /* Bits[14:13], Access Type=RW/P, default=0x00000003*/

                            /*
                               Set bit[0] to 1 to block Ingress M2PCIE Error
                               Logging[br] Set bit[1] to 1 to block Egress
                               M2PCIE Error Logging
                            */
    UINT32 localerrorstatus : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* Read only R2PCIe error status */
    UINT32 viralstatustoiio : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* Read only to MS2IOSF viral status */
    UINT32 viralstatusfromiio : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /* Read only from MS2IOSF viral status */
    UINT32 fatalstatusfromiio : 1;

                            /* Bits[18:18], Access Type=RO/V, default=0x00000000*/

                            /* Read only from MS2IOSF fatal status */
    UINT32 viralstatusfromial : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /* Read only from MS2IOSF viral status */
    UINT32 rsvd_20 : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2GLERRCFG_IIO_M2IAL_STRUCT;

/* R2PINGERRLOG0_IIO_M2IAL_REG supported on:                                    */
/*      SPRA0 (0x20801260)                                                      */
/*      SPRB0 (0x20801260)                                                      */
/*      SPRHBM (0x20801260)                                                     */
/*      SPRC0 (0x20801260)                                                      */
/*      SPRMCC (0x20801260)                                                     */
/*      SPRUCC (0x20801260)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe Ingress Error Log
*/


#define R2PINGERRLOG0_IIO_M2IAL_REG 0x22020260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ak0bgfcrdoverflow : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK BGF 0 BGF credit overflow */
    UINT32 ak1bgfcrdoverflow : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK BGF 1 BGF credit overflow */
    UINT32 blbgfcrdoverflow : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL BGF credit overflow */
    UINT32 rsvd : 4;

                            /* Bits[6:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 iioidicrdoverflow : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* MS2IOSF IDI credit overflow */
    UINT32 iioncbcrdoverflow : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* MS2IOSF NCB credit overflow */
    UINT32 iioncscrdoverflow : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* MS2IOSF NCS credit overflow */
    UINT32 parerring0 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Ingress BL data parity error - Agent 0 */
    UINT32 parerring0injdone : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicating parity error injection is done. S/W
                               needs to clear this bit if it wants to inject
                               another error.
                            */
    UINT32 rsvd_12 : 2;

                            /* Bits[13:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ivbgfcrdoverflow : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IV BGF credit overflow */
    UINT32 ivsnpcrdoverflow : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IV Snoop credit overflow */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2PINGERRLOG0_IIO_M2IAL_STRUCT;

/* R2EGRERRLOG_IIO_M2IAL_REG supported on:                                      */
/*      SPRA0 (0x20801264)                                                      */
/*      SPRB0 (0x20801264)                                                      */
/*      SPRHBM (0x20801264)                                                     */
/*      SPRC0 (0x20801264)                                                      */
/*      SPRMCC (0x20801264)                                                     */
/*      SPRUCC (0x20801264)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe Egress Error Log
*/


#define R2EGRERRLOG_IIO_M2IAL_REG 0x22020264

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 18;

                            /* Bits[17:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 akegress0_write_to_valid_entry : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK Egress Agent 0 write to occupied entry */
    UINT32 akegress1_write_to_valid_entry : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK Egress Agent 1 write to occupied entry */
    UINT32 blegress0_write_to_valid_entry : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Egress Agent 0 write to occupied entry */
    UINT32 blegress1_write_to_valid_entry : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Egress Agent 1 write to occupied entry */
    UINT32 adegress0_write_to_valid_entry : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Egress Agent 0 write to occupied entry */
    UINT32 adegress1_write_to_valid_entry : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Egress Agent 1 write to occupied entry */
    UINT32 akegress0_overflow : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK Egress Agent 0 buffer overflow */
    UINT32 akegress1_overflow : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AK Egress Agent 1 buffer overflow */
    UINT32 blegress0_overflow : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Egress Agent 0 buffer overflow */
    UINT32 blegress1_overflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* BL Egress Agent 1 buffer overflow */
    UINT32 adegress0_overflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Egress Agent 0 buffer overflow */
    UINT32 adegress1_overflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* AD Egress Agent 1 buffer overflow */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2EGRERRLOG_IIO_M2IAL_STRUCT;

/* R2EGRPRQERRLOG0_IIO_M2IAL_REG supported on:                                  */
/*      SPRA0 (0x20801270)                                                      */
/*      SPRB0 (0x20801270)                                                      */
/*      SPRHBM (0x20801270)                                                     */
/*      SPRC0 (0x20801270)                                                      */
/*      SPRMCC (0x20801270)                                                     */
/*      SPRUCC (0x20801270)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* PRQ Credit Overflow Error for each CHA. Bit[31:0] maps to error for CHA[31:0] (if applicable).
*/


#define R2EGRPRQERRLOG0_IIO_M2IAL_REG 0x22020270

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cboprqcrdoverflow_31_00_00 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 0 */
    UINT32 cboprqcrdoverflow_31_00_01 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 1 */
    UINT32 cboprqcrdoverflow_31_00_02 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 2 */
    UINT32 cboprqcrdoverflow_31_00_03 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 3 */
    UINT32 cboprqcrdoverflow_31_00_04 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 4 */
    UINT32 cboprqcrdoverflow_31_00_05 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 5 */
    UINT32 cboprqcrdoverflow_31_00_06 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 6 */
    UINT32 cboprqcrdoverflow_31_00_07 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 7 */
    UINT32 cboprqcrdoverflow_31_00_08 : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 8 */
    UINT32 cboprqcrdoverflow_31_00_09 : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 9 */
    UINT32 cboprqcrdoverflow_31_00_10 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 10 */
    UINT32 cboprqcrdoverflow_31_00_11 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 11 */
    UINT32 cboprqcrdoverflow_31_00_12 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 12 */
    UINT32 cboprqcrdoverflow_31_00_13 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 13 */
    UINT32 cboprqcrdoverflow_31_00_14 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 14 */
    UINT32 cboprqcrdoverflow_31_00_15 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 15 */
    UINT32 cboprqcrdoverflow_31_00_16 : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 16 */
    UINT32 cboprqcrdoverflow_31_00_17 : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 17 */
    UINT32 cboprqcrdoverflow_31_00_18 : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 18 */
    UINT32 cboprqcrdoverflow_31_00_19 : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 19 */
    UINT32 cboprqcrdoverflow_31_00_20 : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 20 */
    UINT32 cboprqcrdoverflow_31_00_21 : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 21 */
    UINT32 cboprqcrdoverflow_31_00_22 : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 22 */
    UINT32 cboprqcrdoverflow_31_00_23 : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 23 */
    UINT32 cboprqcrdoverflow_31_00_24 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 24 */
    UINT32 cboprqcrdoverflow_31_00_25 : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 25 */
    UINT32 cboprqcrdoverflow_31_00_26 : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 26 */
    UINT32 cboprqcrdoverflow_31_00_27 : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 27 */
    UINT32 cboprqcrdoverflow_31_00_28 : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 28 */
    UINT32 cboprqcrdoverflow_31_00_29 : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 29 */
    UINT32 cboprqcrdoverflow_31_00_30 : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 30 */
    UINT32 cboprqcrdoverflow_31_00_31 : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 31 */

  } Bits;
  UINT32 Data;

} R2EGRPRQERRLOG0_IIO_M2IAL_STRUCT;

/* R2EGRPRQERRLOG1_IIO_M2IAL_REG supported on:                                  */
/*      SPRA0 (0x20801274)                                                      */
/*      SPRB0 (0x20801274)                                                      */
/*      SPRHBM (0x20801274)                                                     */
/*      SPRC0 (0x20801274)                                                      */
/*      SPRMCC (0x20801274)                                                     */
/*      SPRUCC (0x20801274)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* PRQ Credit Overflow Error for each CHA. Bit[31:0] maps to error for CHA[63:32] (if applicable).
*/


#define R2EGRPRQERRLOG1_IIO_M2IAL_REG 0x22020274

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cboprqcrdoverflow_63_32_00 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 32 */
    UINT32 cboprqcrdoverflow_63_32_01 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 33 */
    UINT32 cboprqcrdoverflow_63_32_02 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 34 */
    UINT32 cboprqcrdoverflow_63_32_03 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 35 */
    UINT32 cboprqcrdoverflow_63_32_04 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 36 */
    UINT32 cboprqcrdoverflow_63_32_05 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 37 */
    UINT32 cboprqcrdoverflow_63_32_06 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 38 */
    UINT32 cboprqcrdoverflow_63_32_07 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 39 */
    UINT32 cboprqcrdoverflow_63_32_08 : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 40 */
    UINT32 cboprqcrdoverflow_63_32_09 : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 41 */
    UINT32 cboprqcrdoverflow_63_32_10 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 42 */
    UINT32 cboprqcrdoverflow_63_32_11 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 43 */
    UINT32 cboprqcrdoverflow_63_32_12 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 44 */
    UINT32 cboprqcrdoverflow_63_32_13 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 45 */
    UINT32 cboprqcrdoverflow_63_32_14 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 46 */
    UINT32 cboprqcrdoverflow_63_32_15 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 47 */
    UINT32 cboprqcrdoverflow_63_32_16 : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 48 */
    UINT32 cboprqcrdoverflow_63_32_17 : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 49 */
    UINT32 cboprqcrdoverflow_63_32_18 : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 50 */
    UINT32 cboprqcrdoverflow_63_32_19 : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 51 */
    UINT32 cboprqcrdoverflow_63_32_20 : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 52 */
    UINT32 cboprqcrdoverflow_63_32_21 : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 53 */
    UINT32 cboprqcrdoverflow_63_32_22 : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 54 */
    UINT32 cboprqcrdoverflow_63_32_23 : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 55 */
    UINT32 cboprqcrdoverflow_63_32_24 : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 56 */
    UINT32 cboprqcrdoverflow_63_32_25 : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 57 */
    UINT32 cboprqcrdoverflow_63_32_26 : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 58 */
    UINT32 cboprqcrdoverflow_63_32_27 : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 59 */
    UINT32 cboprqcrdoverflow_63_32_28 : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 60 */
    UINT32 cboprqcrdoverflow_63_32_29 : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 61 */
    UINT32 cboprqcrdoverflow_63_32_30 : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 62 */
    UINT32 cboprqcrdoverflow_63_32_31 : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* PRQ credit overflow from CHA 63 */

  } Bits;
  UINT32 Data;

} R2EGRPRQERRLOG1_IIO_M2IAL_STRUCT;

/* R2PINGERRMSK0_IIO_M2IAL_REG supported on:                                    */
/*      SPRA0 (0x20801280)                                                      */
/*      SPRB0 (0x20801280)                                                      */
/*      SPRHBM (0x20801280)                                                     */
/*      SPRC0 (0x20801280)                                                      */
/*      SPRMCC (0x20801280)                                                     */
/*      SPRUCC (0x20801280)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe Ingress Error Mask
*/


#define R2PINGERRMSK0_IIO_M2IAL_REG 0x22020280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ak0bgfcrderrmsk : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Mask AK 0 BGF credit overflow error */
    UINT32 ak1bgfcrderrmsk : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* Mask AK 1 BGF credit overflow error */
    UINT32 blbgfcrderrmsk : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* Mask BL BGF credit overflow error */
    UINT32 rsvd : 4;

                            /* Bits[6:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 iioidicrderrmsk : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* Mask MS2IOSF IDI credit overflow error */
    UINT32 iioncbcrderrmsk : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* Mask MS2IOSF NCB credit overflow error */
    UINT32 iioncscrderrmsk : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* Mask MS2IOSF NCS credit overflow error */
    UINT32 parerring0msk : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* Mask BL ingress data parity error - Agent 0 */
    UINT32 rsvd_11 : 3;

                            /* Bits[13:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ivbgfcrderrmsk : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* IV BGF credit overflow error */
    UINT32 ivsnpcrderrmsk : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* IV Snoop credit overflow error */
    UINT32 ingparerrinjentry : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               8b11111111: Par error injection happens to
                               next[br] Ingress Write; otherwise, Inject error
                               the the ingress entry[br] specified by this
                               field
                            */
    UINT32 ingparerrinj : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               When set, one single parity error injection will
                               happen to an ingress entry write sepcified by
                               R2PINGERRMSK0.IngParErrInjEntry. Once injection
                               is done, R2PINGERRLOG0.ParErrIng0InjDone will be
                               set. SW needs to clear both bits if it wants to
                               do another injection.
                            */
    UINT32 ingparerrinjarray : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Inject parity error into RF for CHA traffic
                               1: Inject parity error into RF for non-CHA
                               traffic
                            */
    UINT32 rsvd_26 : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2PINGERRMSK0_IIO_M2IAL_STRUCT;

/* R2EGRERRMSK_IIO_M2IAL_REG supported on:                                      */
/*      SPRA0 (0x20801288)                                                      */
/*      SPRB0 (0x20801288)                                                      */
/*      SPRHBM (0x20801288)                                                     */
/*      SPRC0 (0x20801288)                                                      */
/*      SPRMCC (0x20801288)                                                     */
/*      SPRUCC (0x20801288)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* R2PCIe Egress Error Mask
*/


#define R2EGRERRMSK_IIO_M2IAL_REG 0x22020288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 18;

                            /* Bits[17:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 akegress0_write_to_valid_entry : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask AK Egress Agent 0 write to occupied entry
                               error
                            */
    UINT32 akegress1_write_to_valid_entry : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask AK Egress Agent 1 write to occupied entry
                               error
                            */
    UINT32 blegress0_write_to_valid_entry : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask BL Egress Agent 0 write to occupied entry
                               error
                            */
    UINT32 blegress1_write_to_valid_entry : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask BL Egress Agent 1 write to occupied entry
                               error
                            */
    UINT32 adegress0_write_to_valid_entry : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask AD Egress Agent 0 write to occupied entry
                               error
                            */
    UINT32 adegress1_write_to_valid_entry : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask AD Egress Agent 0 write to occupied entry
                               error
                            */
    UINT32 akegress0_overflow : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* Mask AK Egress Agent 0 overflow error */
    UINT32 akegress1_overflow : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /* Mask AK Egress Agent 1 overflow error */
    UINT32 blegress0_overflow : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /* Mask BL Egress Agent 0 overflow error */
    UINT32 blegress1_overflow : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /* Mask AK Egress Agent 1 overflow error */
    UINT32 adegress0_overflow : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /* Mask AD Egress Agent 0 overflow error */
    UINT32 adegress1_overflow : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /* Mask AD Egress Agent 1 overflow error */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2EGRERRMSK_IIO_M2IAL_STRUCT;

/* R2EGRPRQERRMSK0_IIO_M2IAL_REG supported on:                                  */
/*      SPRA0 (0x20801290)                                                      */
/*      SPRB0 (0x20801290)                                                      */
/*      SPRHBM (0x20801290)                                                     */
/*      SPRC0 (0x20801290)                                                      */
/*      SPRMCC (0x20801290)                                                     */
/*      SPRUCC (0x20801290)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* PRQ Credit Error Mask for each CHA. Bit[31:0] maps to error for CHA[31:0] (if applicable).
*/


#define R2EGRPRQERRMSK0_IIO_M2IAL_REG 0x22020290

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cboprqcrdoverflowmask_31_00 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* Cboprqcrdoverflowmask 31 00 */

  } Bits;
  UINT32 Data;

} R2EGRPRQERRMSK0_IIO_M2IAL_STRUCT;




#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
/* R2IVIGRCFG_IIO_M2IAL_REG supported on:                                       */
/*      SPRA0 (0x208012c8)                                                      */
/*      SPRB0 (0x208012c8)                                                      */
/*      SPRHBM (0x208012c8)                                                     */
/* Register default value on SPRA0: 0x0000000F                                  */
/* Register default value on SPRB0: 0x0000000F                                  */
/* Register default value on SPRHBM: 0x0000000F                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* IV Ingress configuration register for PCIE Gen5 perf
*/


#define R2IVIGRCFG_IIO_M2IAL_REG 0x220202C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ivigrinitialcred : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x0000000F*/

                            /*
                               This is the number of entries enabled in the IV
                               Ingress (0-16). Default 8 entries are enabled.
                               Credit return counter will return these many
                               credits to CMS on IV Drop, only if IV Ingress is
                               enabled (byp=0) Any change to this register must
                               be done only under quiesce conditions followed
                               by pulsing the hw_init
                            */
    UINT32 rsvd : 4;

                            /* Bits[7:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ivigrbypen : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enables bypassing the IV Ingress. 0= IV Ingress
                               queue enabled. 1= IV Ingress queue disabled
                               (bypassed) Any change to this register must be
                               done only under quiesce conditions followed by
                               pulsing the hw_init
                            */
    UINT32 rsvd_9 : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} R2IVIGRCFG_IIO_M2IAL_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

/* M2IOSF_SCRATCHPAD_1_0_1_IIO_M2IAL_REG supported on:                          */
/*      SPRA0 (0x208013fc)                                                      */
/*      SPRB0 (0x208013fc)                                                      */
/*      SPRHBM (0x208013fc)                                                     */
/*      SPRC0 (0x208013fc)                                                      */
/*      SPRMCC (0x208013fc)                                                     */
/*      SPRUCC (0x208013fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0     */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_R2PCIE_F0    */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_R2PCIE_F0    */
/* Struct generated from SPRA0 BDF: 8_0_1                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Read-write bits used for validation and debug.
*/


#define M2IOSF_SCRATCHPAD_1_0_1_IIO_M2IAL_REG 0x220203FC

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

} M2IOSF_SCRATCHPAD_1_0_1_IIO_M2IAL_STRUCT;
#endif /* _IIO_M2IAL_h */
