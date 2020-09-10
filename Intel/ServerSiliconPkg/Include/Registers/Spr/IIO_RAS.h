
/** @file
  IIO_RAS.h

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


#ifndef _IIO_RAS_h
#define _IIO_RAS_h
#include <Base.h>

/* VID_IIO_RAS_REG supported on:                                                */
/*      SPRA0 (0x10802000)                                                      */
/*      SPRB0 (0x10802000)                                                      */
/*      SPRHBM (0x10802000)                                                     */
/*      SPRC0 (0x10802000)                                                      */
/*      SPRMCC (0x10802000)                                                     */
/*      SPRUCC (0x10802000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Vendor ID
*/


#define VID_IIO_RAS_REG 0x11210000

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

} VID_IIO_RAS_STRUCT;

/* DID_IIO_RAS_REG supported on:                                                */
/*      SPRA0 (0x10802002)                                                      */
/*      SPRB0 (0x10802002)                                                      */
/*      SPRHBM (0x10802002)                                                     */
/*      SPRC0 (0x10802002)                                                      */
/*      SPRMCC (0x10802002)                                                     */
/*      SPRUCC (0x10802002)                                                     */
/* Register default value on SPRA0: 0x000009A3                                  */
/* Register default value on SPRB0: 0x000009A3                                  */
/* Register default value on SPRHBM: 0x000009A3                                 */
/* Register default value on SPRC0: 0x000009A3                                  */
/* Register default value on SPRMCC: 0x000009A3                                 */
/* Register default value on SPRUCC: 0x000009A3                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device ID
*/


#define DID_IIO_RAS_REG 0x11210002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x000009A3*/

                            /* Device ID for the MS2IOSF RAS Registers */

  } Bits;
  UINT16 Data;

} DID_IIO_RAS_STRUCT;

/* PCICMD_IIO_RAS_REG supported on:                                             */
/*      SPRA0 (0x10802004)                                                      */
/*      SPRB0 (0x10802004)                                                      */
/*      SPRHBM (0x10802004)                                                     */
/*      SPRC0 (0x10802004)                                                      */
/*      SPRMCC (0x10802004)                                                     */
/*      SPRUCC (0x10802004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command
*/


#define PCICMD_IIO_RAS_REG 0x11210004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 iose : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Hardwired to 0 since these devices don't decode
                               any IO BARs
                            */
    UINT16 mse : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Hardwired to 0 since these devices don't decode
                               any memory BARs
                            */
    UINT16 bme : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Hardwired to 0 since these devices don't
                               generate any transactions
                            */
    UINT16 sce : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to internal devices. Hardwired to
                               0.
                            */
    UINT16 mwie : 1;

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
    UINT16 perre : 1;

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
    UINT16 serre : 1;

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
    UINT16 interrupt_disable : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /* N/A for these devices */
    UINT16 rsvd : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PCICMD_IIO_RAS_STRUCT;

/* PCISTS_IIO_RAS_REG supported on:                                             */
/*      SPRA0 (0x10802006)                                                      */
/*      SPRB0 (0x10802006)                                                      */
/*      SPRHBM (0x10802006)                                                     */
/*      SPRC0 (0x10802006)                                                      */
/*      SPRMCC (0x10802006)                                                     */
/*      SPRUCC (0x10802006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status
*/


#define PCISTS_IIO_RAS_REG 0x11210006

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

                            /* Hardwired to 0 */
    UINT16 capabilities_list : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               This bit indicates the presence of a
                               capabilities list structure
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

                            /* Hardwired to 0 */
    UINT16 devsel_timing : 2;

                            /* Bits[10:9], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI Express. Hardwired to 0. */
    UINT16 sta : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 rta : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 rma : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 sse : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */
    UINT16 dpe : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 0 */

  } Bits;
  UINT16 Data;

} PCISTS_IIO_RAS_STRUCT;

/* RID_CCR_IIO_RAS_REG supported on:                                            */
/*      SPRA0 (0x20802008)                                                      */
/*      SPRB0 (0x20802008)                                                      */
/*      SPRHBM (0x20802008)                                                     */
/*      SPRC0 (0x20802008)                                                      */
/*      SPRMCC (0x20802008)                                                     */
/*      SPRUCC (0x20802008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_IIO_RAS_REG 0x11220008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 revision_id : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Reflects the Uncore Revision ID after reset.
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

} RID_CCR_IIO_RAS_STRUCT;

/* CLSR_IIO_RAS_REG supported on:                                               */
/*      SPRA0 (0x80200c)                                                        */
/*      SPRB0 (0x80200c)                                                        */
/*      SPRHBM (0x80200c)                                                       */
/*      SPRC0 (0x80200c)                                                        */
/*      SPRMCC (0x80200c)                                                       */
/*      SPRUCC (0x80200c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Cacheline Size Register
*/


#define CLSR_IIO_RAS_REG 0x1120000C

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

} CLSR_IIO_RAS_STRUCT;

/* HDR_IIO_RAS_REG supported on:                                                */
/*      SPRA0 (0x80200e)                                                        */
/*      SPRB0 (0x80200e)                                                        */
/*      SPRHBM (0x80200e)                                                       */
/*      SPRC0 (0x80200e)                                                        */
/*      SPRMCC (0x80200e)                                                       */
/*      SPRUCC (0x80200e)                                                       */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header Type
*/


#define HDR_IIO_RAS_REG 0x1120000E

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

} HDR_IIO_RAS_STRUCT;

/* SVID_IIO_RAS_REG supported on:                                               */
/*      SPRA0 (0x1080202c)                                                      */
/*      SPRB0 (0x1080202c)                                                      */
/*      SPRHBM (0x1080202c)                                                     */
/*      SPRC0 (0x1080202c)                                                      */
/*      SPRMCC (0x1080202c)                                                     */
/*      SPRUCC (0x1080202c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Subsystem Vendor ID
*/


#define SVID_IIO_RAS_REG 0x1121002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 subsystem_vendor_id : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00008086*/

                            /*
                               The default value specifies Intel but can be set
                               to any value once after reset.
                            */

  } Bits;
  UINT16 Data;

} SVID_IIO_RAS_STRUCT;

/* SID_IIO_RAS_REG supported on:                                                */
/*      SPRA0 (0x1080202e)                                                      */
/*      SPRB0 (0x1080202e)                                                      */
/*      SPRHBM (0x1080202e)                                                     */
/*      SPRC0 (0x1080202e)                                                      */
/*      SPRMCC (0x1080202e)                                                     */
/*      SPRUCC (0x1080202e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: BIOS_W                                           */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* Subsystem ID
*/


#define SID_IIO_RAS_REG 0x1121002E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 subsystem_id : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               The default value specifies Intel but can be set
                               to any value once after reset.
                            */

  } Bits;
  UINT16 Data;

} SID_IIO_RAS_STRUCT;

/* CAPPTR_IIO_RAS_REG supported on:                                             */
/*      SPRA0 (0x802034)                                                        */
/*      SPRB0 (0x802034)                                                        */
/*      SPRHBM (0x802034)                                                       */
/*      SPRC0 (0x802034)                                                        */
/*      SPRMCC (0x802034)                                                       */
/*      SPRUCC (0x802034)                                                       */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Capability Pointer
*/


#define CAPPTR_IIO_RAS_REG 0x11200034

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

} CAPPTR_IIO_RAS_STRUCT;

/* INTL_IIO_RAS_REG supported on:                                               */
/*      SPRA0 (0x80203c)                                                        */
/*      SPRB0 (0x80203c)                                                        */
/*      SPRHBM (0x80203c)                                                       */
/*      SPRC0 (0x80203c)                                                        */
/*      SPRMCC (0x80203c)                                                       */
/*      SPRUCC (0x80203c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Interrupt Line Register
*/


#define INTL_IIO_RAS_REG 0x1120003C

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

} INTL_IIO_RAS_STRUCT;

/* INTPIN_IIO_RAS_REG supported on:                                             */
/*      SPRA0 (0x80203d)                                                        */
/*      SPRB0 (0x80203d)                                                        */
/*      SPRHBM (0x80203d)                                                       */
/*      SPRC0 (0x80203d)                                                        */
/*      SPRMCC (0x80203d)                                                       */
/*      SPRUCC (0x80203d)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Interrupt Pin Register
*/


#define INTPIN_IIO_RAS_REG 0x1120003D

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

} INTPIN_IIO_RAS_STRUCT;

/* PXPCAP_IIO_RAS_REG supported on:                                             */
/*      SPRA0 (0x20802040)                                                      */
/*      SPRB0 (0x20802040)                                                      */
/*      SPRHBM (0x20802040)                                                     */
/*      SPRC0 (0x20802040)                                                      */
/*      SPRMCC (0x20802040)                                                     */
/*      SPRUCC (0x20802040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_IIO_RAS_REG 0x11220040

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
                               Pointer to the next capability. A value of zero
                               indicates this is the last capability.
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

                            /* N/A for integrated endpoints */
    UINT32 interrupt_message_number : 5;

                            /* Bits[29:25], Access Type=RO, default=0x00000000*/

                            /* N/A for this device */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PXPCAP_IIO_RAS_STRUCT;

/* VIRAL_IIO_RAS_REG supported on:                                              */
/*      SPRA0 (0x208020a0)                                                      */
/*      SPRB0 (0x208020a0)                                                      */
/*      SPRHBM (0x208020a0)                                                     */
/*      SPRC0 (0x208020a0)                                                      */
/*      SPRMCC (0x208020a0)                                                     */
/*      SPRUCC (0x208020a0)                                                     */
/* Register default value on SPRA0: 0x00000004                                  */
/* Register default value on SPRB0: 0x00000004                                  */
/* Register default value on SPRHBM: 0x00000004                                 */
/* Register default value on SPRC0: 0x00000004                                  */
/* Register default value on SPRMCC: 0x00000004                                 */
/* Register default value on SPRUCC: 0x00000004                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register provides the option to generate viral alert upon the detection of a fatal error.
*/


#define VIRAL_IIO_RAS_REG 0x112200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 iio_fatal_viral_alert_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               0: Disable Viral Alert for error severity 2
                               errors in MS2IOSF. MS2IOSF will still go viral
                               if UPI or another MS2IOSF signals viral to this
                               MS2IOSF. To disable viral actions completely,
                               viral must be disabled in UPI and other MS2IOSF
                               instances. [br] 1: MS2IOSF goes viral when error
                               severity 2 is set in the system event status
                               register
                            */
    UINT32 iio_signal_global_fatal : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Enable MS2IOSF to signal Global Fatal for Fatal
                               Error. This will result in signalling CATERR#
                               when MS2IOSF detects an internal fatal error or
                               viral packet.[br] 0: Disable Viral Alert for
                               global viral indication[br] 1: MS2IOSF signals
                               fatal on Global Fatal chain when MS2IOSF detects
                               a fatal error
                            */
    UINT32 iio_global_viral_mask : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Mask global viral propagation 0: Do not mask
                               global viral propagation (MS2IOSF blocks packets
                               during viral) 1: Mask global viral propagation
                               (MS2IOSF behaves normally during viral, and does
                               not block)
                            */
    UINT32 generate_viral_alert : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Debug mode to make the MS2IOSF go viral in lieu
                               of an actual error condition.
                            */
    UINT32 rsvd : 26;

                            /* Bits[29:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 iio_viral_status : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates the MS2IOSF cluster had gone to viral.
                               This bit has no effect on hardware and does not
                               indicate the MS2IOSF is currently in the viral
                               state. This bit is persistent through warm reset
                               (sticky), even though the viral state is not.
                            */
    UINT32 iio_viral_state : 1;

                            /* Bits[31:31], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Indicates MS2IOSF is in a viral state. When set,
                               all outbound requests are aborted, all inbound
                               requests are completer aborted. If cleared by
                               software by setting a '1', the MS2IOSF cluster
                               will exit the viral state. This state bit is
                               cleared by warm reset.
                            */

  } Bits;
  UINT32 Data;

} VIRAL_IIO_RAS_STRUCT;

/* M2IOSFRASERR_IIO_RAS_REG supported on:                                       */
/*      SPRA0 (0x208020b0)                                                      */
/*      SPRB0 (0x208020b0)                                                      */
/*      SPRHBM (0x208020b0)                                                     */
/*      SPRC0 (0x208020b0)                                                      */
/*      SPRMCC (0x208020b0)                                                     */
/*      SPRUCC (0x208020b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Aggregates First/Next Errors from all M2IOSF FUBs, as reported to IEH
*/


#define M2IOSFRASERR_IIO_RAS_REG 0x112200B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ras_sts_first_err : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Status of First RAS Error Detected by miscblk,
                               as reported in Message to IEH[br] 0 : No Error
                               Logged[br] 1 : Error Logged
                            */
    UINT32 ras_sev_first_err : 2;

                            /* Bits[2:1], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Severity of First RAS Error Detected by miscblk,
                               as reported in Message to IEH
                            */
    UINT32 ras_func_first_err : 3;

                            /* Bits[5:3], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Function Number of First RAS Error Detected by
                               miscblk, as reported in Message to IEH
                            */
    UINT32 ras_sts_next_err : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Status of Next RAS Error Detected by miscblk, as
                               reported in Message to IEH[br] 0 : No Error
                               Logged[br] 1 : Error Logged
                            */
    UINT32 ras_sev_next_err : 2;

                            /* Bits[8:7], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Severity of Next RAS Error Detected by miscblk,
                               as reported in Message to IEH
                            */
    UINT32 ras_func_next_err : 3;

                            /* Bits[11:9], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Function Number of Next RAS Error Detected by
                               miscblk, as reported in Message to IEH
                            */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M2IOSFRASERR_IIO_RAS_STRUCT;

/* M2IOSFRASERRST_IIO_RAS_REG supported on:                                     */
/*      SPRA0 (0x208020b4)                                                      */
/*      SPRB0 (0x208020b4)                                                      */
/*      SPRHBM (0x208020b4)                                                     */
/*      SPRC0 (0x208020b4)                                                      */
/*      SPRMCC (0x208020b4)                                                     */
/*      SPRUCC (0x208020b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Status of errors from all M2IOSF FUBs, as reported to IEH
*/


#define M2IOSFRASERRST_IIO_RAS_REG 0x112200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 itc_sev0_err_st : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Status of ITC Severity 0 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 itc_sev1_err_st : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Status of ITC Severity 1 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 itc_sev2_err_st : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Status of ITC Severity 2 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 otc_sev0_err_st : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Status of OTC Severity 0 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 otc_sev1_err_st : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Status of OTC Severity 1 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 otc_sev2_err_st : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Status of OTC Severity 2 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 irp_sev0_err_st : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Status of IRP Severity 0 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 irp_sev1_err_st : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Status of IRP Severity 1 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 irp_sev2_err_st : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Status of IRP Severity 2 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 m2pcie_sev2_err_st : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Status of M2PCIE Severity 2 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 vt_sev0_err_st : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Status of VT Severity 0 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 vt_sev1_err_st : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Status of VT Severity 1 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 vt_sev2_err_st : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Status of VT Severity 2 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 rsvd : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M2IOSFRASERRST_IIO_RAS_STRUCT;

/* M2IOSFRASERRMSK_IIO_RAS_REG supported on:                                    */
/*      SPRA0 (0x208020b8)                                                      */
/*      SPRB0 (0x208020b8)                                                      */
/*      SPRHBM (0x208020b8)                                                     */
/*      SPRC0 (0x208020b8)                                                      */
/*      SPRMCC (0x208020b8)                                                     */
/*      SPRUCC (0x208020b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Mask of errors from all M2IOSF FUBs, as reported to IEH
*/


#define M2IOSFRASERRMSK_IIO_RAS_REG 0x112200B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 itc_sev0_err_msk : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask of ITC Severity 0 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 itc_sev1_err_msk : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask of ITC Severity 1 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 itc_sev2_err_msk : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask of ITC Severity 2 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 otc_sev0_err_msk : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask of OTC Severity 0 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 otc_sev1_err_msk : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask of OTC Severity 1 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 otc_sev2_err_msk : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask of OTC Severity 2 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 irp_sev0_err_msk : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask of IRP Severity 0 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 irp_sev1_err_msk : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask of IRP Severity 1 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 irp_sev2_err_msk : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask of IRP Severity 2 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 m2pcie_sev2_err_msk : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask of M2PCIE Severity 2 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 vt_sev0_err_msk : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask of VT Severity 0 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 vt_sev1_err_msk : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask of VT Severity 1 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 vt_sev2_err_msk : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask of VT Severity 2 Errors in M2IOSF RAS
                               Block[br] 0 : No Error Logged[br] 1 : Error
                               Logged
                            */
    UINT32 rsvd : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M2IOSFRASERRMSK_IIO_RAS_STRUCT;

/* PXPEXTCAP_IIO_RAS_REG supported on:                                          */
/*      SPRA0 (0x20802100)                                                      */
/*      SPRB0 (0x20802100)                                                      */
/*      SPRHBM (0x20802100)                                                     */
/*      SPRC0 (0x20802100)                                                      */
/*      SPRMCC (0x20802100)                                                     */
/*      SPRUCC (0x20802100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This field implements the Extended Capability Header required by section 7.9.1 in the PCIe 3.0 spec.
*/


#define PXPEXTCAP_IIO_RAS_REG 0x11220100

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

} PXPEXTCAP_IIO_RAS_STRUCT;

/* IRPPERRSV_N0_IIO_RAS_REG supported on:                                       */
/*      SPRA0 (0x20802180)                                                      */
/*      SPRB0 (0x20802180)                                                      */
/*      SPRHBM (0x20802180)                                                     */
/*      SPRC0 (0x20802180)                                                      */
/*      SPRMCC (0x20802180)                                                     */
/*      SPRUCC (0x20802180)                                                     */
/* Register default value on SPRA0: 0xAAAAAA54                                  */
/* Register default value on SPRB0: 0xAAAAAA54                                  */
/* Register default value on SPRHBM: 0xAAAAAA54                                 */
/* Register default value on SPRC0: 0xAAAAAA54                                  */
/* Register default value on SPRMCC: 0xAAAAAA54                                 */
/* Register default value on SPRUCC: 0xAAAAAA54                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* IRP Protocol Error Severity
*/


#define IRPPERRSV_N0_IIO_RAS_REG 0x11220180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wrcache_correcc_error : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST. This field
                               controls the severity.[br] 00: Error Severity
                               Level 0 (Correctable)[br] 01: Error Severity
                               Level 1 (Recoverable)[br] 10: Error Severity
                               Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 wrcache_uncecc_error : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST. This field
                               controls the severity[br] 00: Error Severity
                               Level 0 (Correctable)[br] 01: Error Severity
                               Level 1 (Recoverable)[br] 10: Error Severity
                               Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 protocol_rcvd_poison : 2;

                            /* Bits[5:4], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 csr_acc_32b_unaligned : 2;

                            /* Bits[7:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 protocol_rcvd_unexprsp : 2;

                            /* Bits[9:8], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 protocol_parity_error : 2;

                            /* Bits[11:10], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 p2p_hdr_rf_addr_par_error : 2;

                            /* Bits[13:12], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 faf_rf_addr_par_error : 2;

                            /* Bits[15:14], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 addrcam0_addr_par_error : 2;

                            /* Bits[17:16], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 addrcam1_addr_par_error : 2;

                            /* Bits[19:18], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 addrcam2_addr_par_error : 2;

                            /* Bits[21:20], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 addrcam3_addr_par_error : 2;

                            /* Bits[23:22], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 pf_timeout_err_cs0 : 2;

                            /* Bits[25:24], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 pf_timeout_err_cs1 : 2;

                            /* Bits[27:26], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 pf_timeout_err_cs2 : 2;

                            /* Bits[29:28], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 pf_timeout_err_cs3 : 2;

                            /* Bits[31:30], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */

  } Bits;
  UINT32 Data;

} IRPPERRSV_N0_IIO_RAS_STRUCT;

/* IRPPERRSV_N1_IIO_RAS_REG supported on:                                       */
/*      SPRA0 (0x20802184)                                                      */
/*      SPRB0 (0x20802184)                                                      */
/*      SPRHBM (0x20802184)                                                     */
/*      SPRC0 (0x20802184)                                                      */
/*      SPRMCC (0x20802184)                                                     */
/*      SPRUCC (0x20802184)                                                     */
/* Register default value on SPRA0: 0x0000000A                                  */
/* Register default value on SPRB0: 0x0000000A                                  */
/* Register default value on SPRHBM: 0x0000000A                                 */
/* Register default value on SPRC0: 0x0000000A                                  */
/* Register default value on SPRMCC: 0x0000000A                                 */
/* Register default value on SPRUCC: 0x0000000A                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* IRP Protocol Error Severity
*/


#define IRPPERRSV_N1_IIO_RAS_REG 0x11220184

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 protocol_qt_overflow_underflow : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 iommu_faf_rf_addr_par_error : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPPERRSV_N1_IIO_RAS_STRUCT;

/* IRPERRCTL_IIO_RAS_REG supported on:                                          */
/*      SPRA0 (0x20802188)                                                      */
/*      SPRB0 (0x20802188)                                                      */
/*      SPRHBM (0x20802188)                                                     */
/*      SPRC0 (0x20802188)                                                      */
/*      SPRMCC (0x20802188)                                                     */
/*      SPRUCC (0x20802188)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register enables the error status bit setting for a Coherent Interface detected error. Setting of the bit enables the setting of the corresponding error status bit in IRPERRST register. If the bit is cleared, the corresponding error status will not be set.[br]

*/


#define IRPERRCTL_IIO_RAS_REG 0x11220188

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wrcache_correcc_error : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 wrcache_uncecc_error : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 protocol_rcvd_poison : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 csr_acc_32b_unaligned : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 protocol_rcvd_unexprsp : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 protocol_parity_error : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 p2p_hdr_rf_addr_par_error : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 faf_rf_addr_par_error : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 addrcam0_addr_par_error : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 addrcam1_addr_par_error : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 addrcam2_addr_par_error : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 addrcam3_addr_par_error : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 pf_timeout_err_cs0 : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 pf_timeout_err_cs1 : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 pf_timeout_err_cs2 : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 pf_timeout_err_cs3 : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 protocol_qt_overflow_underflow : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 iommu_faf_rf_addr_par_error : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable error status logging for this
                               error[br] 1: Enable Error status logging for
                               this error[br] For error description, refer to
                               IRPERRST
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPERRCTL_IIO_RAS_STRUCT;

/* IRPRINGERRCTL_IIO_RAS_REG supported on:                                      */
/*      SPRA0 (0x2080218c)                                                      */
/*      SPRB0 (0x2080218c)                                                      */
/*      SPRHBM (0x2080218c)                                                     */
/*      SPRC0 (0x2080218c)                                                      */
/*      SPRMCC (0x2080218c)                                                     */
/*      SPRUCC (0x2080218c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register enables the error status bit setting for a Coherent Interface detected error. Setting of the bit enables the setting of the corresponding error status bit in IRPRINGERRST register. If the bit is cleared, the corresponding error status will not be set.[br]

*/


#define IRPRINGERRCTL_IIO_RAS_REG 0x1122018C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_parity_error : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 snp_addr_parity_error : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 rxdata1_parity_error : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 rxdata0_parity_error : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 blq_parity_error : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 bl_ecc_corrected_error : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 bl_ecc_uncorrectable_error : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 bl_cqid_parity_error : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 ak0_cqid_parity_error : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 ak1_cqid_parity_error : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 txq0_addr_par_error : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 txq1_addr_par_error : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 snpq_rf_addr_parity_error : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 unexpected_go_err_rcvd : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to IRPRINGERRST
                            */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPRINGERRCTL_IIO_RAS_STRUCT;

/* IRPRINGERRSV_IIO_RAS_REG supported on:                                       */
/*      SPRA0 (0x20802190)                                                      */
/*      SPRB0 (0x20802190)                                                      */
/*      SPRHBM (0x20802190)                                                     */
/*      SPRC0 (0x20802190)                                                      */
/*      SPRMCC (0x20802190)                                                     */
/*      SPRUCC (0x20802190)                                                     */
/* Register default value on SPRA0: 0x0AAAA2AA                                  */
/* Register default value on SPRB0: 0x0AAAA2AA                                  */
/* Register default value on SPRHBM: 0x0AAAA2AA                                 */
/* Register default value on SPRC0: 0x0AAAA2AA                                  */
/* Register default value on SPRMCC: 0x0AAAA2AA                                 */
/* Register default value on SPRUCC: 0x0AAAA2AA                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* IRP Ring Error Severity
*/


#define IRPRINGERRSV_IIO_RAS_REG 0x11220190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_parity_error : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 snp_addr_parity_error : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 rxdata1_parity_error : 2;

                            /* Bits[5:4], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 rxdata0_parity_error : 2;

                            /* Bits[7:6], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 blq_parity_error : 2;

                            /* Bits[9:8], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 bl_ecc_corrected_error : 2;

                            /* Bits[11:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 bl_ecc_uncorrectable_error : 2;

                            /* Bits[13:12], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 bl_cqid_parity_error : 2;

                            /* Bits[15:14], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 ak0_cqid_parity_error : 2;

                            /* Bits[17:16], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 ak1_cqid_parity_error : 2;

                            /* Bits[19:18], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 txq0_addr_par_error : 2;

                            /* Bits[21:20], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 txq1_addr_par_error : 2;

                            /* Bits[23:22], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 snpq_rf_addr_parity_error : 2;

                            /* Bits[25:24], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 unexpected_go_err_rcvd : 2;

                            /* Bits[27:26], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to IRPRINGERRST.[br] 00:
                               Error Severity Level 0 (Correctable)[br] 01:
                               Error Severity Level 1 (Recoverable)[br] 10:
                               Error Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPRINGERRSV_IIO_RAS_STRUCT;

/* IRPRINGERRST_IIO_RAS_REG supported on:                                       */
/*      SPRA0 (0x20802208)                                                      */
/*      SPRB0 (0x20802208)                                                      */
/*      SPRHBM (0x20802208)                                                     */
/*      SPRC0 (0x20802208)                                                      */
/*      SPRMCC (0x20802208)                                                     */
/*      SPRUCC (0x20802208)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register indicates the error detected by the Coherent Interface.
*/


#define IRPRINGERRST_IIO_RAS_REG 0x11220208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_parity_error : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Parity error detected in BL packet in write
                               cache
                            */
    UINT32 snp_addr_parity_error : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Address parity error detected in the inbound
                               rxiv (received IV packet from the mesh)
                            */
    UINT32 rxdata1_parity_error : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Parity error detected in the inbound rxdq/rxdata
                               buffer 32B chunk1
                            */
    UINT32 rxdata0_parity_error : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Parity error detected in the inbound rxdq/rxdata
                               buffer 32B chunk0
                            */
    UINT32 blq_parity_error : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error detected in the inbound BL queue */
    UINT32 bl_ecc_corrected_error : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates that a corrected (single bit) error
                               occurred during ECC check when IRP received the
                               BL from the mesh
                            */
    UINT32 bl_ecc_uncorrectable_error : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates an uncorrectable (double bit) error
                               occurred during ECC check when IRP received the
                               BL from the mesh
                            */
    UINT32 bl_cqid_parity_error : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Parity error detected on the CQID of a BL
                               packet. Valid only for C2P type BL packet.
                            */
    UINT32 ak0_cqid_parity_error : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error detected on the CQID of AK0 packet */
    UINT32 ak1_cqid_parity_error : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error detected on the CQID of AK1 packet */
    UINT32 txq0_addr_par_error : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Parity error detected in ADQ0 internal register
                               file (RF)
                            */
    UINT32 txq1_addr_par_error : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Parity error detected in ADQ1 internal register
                               file (RF)
                            */
    UINT32 snpq_rf_addr_parity_error : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Parity error detected in RX_SNPQ internal
                               register file (RF)
                            */
    UINT32 unexpected_go_err_rcvd : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Unexpected GO_ERR received from CHA -- MS2IOSF
                               Does Support Go_Err
                            */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPRINGERRST_IIO_RAS_STRUCT;

/* IRPRINGFFERRST_IIO_RAS_REG supported on:                                     */
/*      SPRA0 (0x20802210)                                                      */
/*      SPRB0 (0x20802210)                                                      */
/*      SPRHBM (0x20802210)                                                     */
/*      SPRC0 (0x20802210)                                                      */
/*      SPRMCC (0x20802210)                                                     */
/*      SPRUCC (0x20802210)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The error status log indicates which error is causing the report of the first fatal error event. When SW clears the error status bit in IRPRINGERRST, HW automatically clears the corresponding bit in this register. [br]

*/


#define IRPRINGFFERRST_IIO_RAS_REG 0x11220210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_parity_error : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 snp_addr_parity_error : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 rxdata1_parity_error : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 rxdata0_parity_error : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 blq_parity_error : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 bl_ecc_corrected_error : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 bl_ecc_uncorrectable_error : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 bl_cqid_parity_error : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 ak0_cqid_parity_error : 1;

                            /* Bits[8:8], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 ak1_cqid_parity_error : 1;

                            /* Bits[9:9], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 txq0_addr_par_error : 1;

                            /* Bits[10:10], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 txq1_addr_par_error : 1;

                            /* Bits[11:11], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 snpq_rf_addr_parity_error : 1;

                            /* Bits[12:12], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 unexpected_go_err_rcvd : 1;

                            /* Bits[13:13], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPRINGFFERRST_IIO_RAS_STRUCT;

/* IRPRINGFNERRST_IIO_RAS_REG supported on:                                     */
/*      SPRA0 (0x20802214)                                                      */
/*      SPRB0 (0x20802214)                                                      */
/*      SPRHBM (0x20802214)                                                     */
/*      SPRC0 (0x20802214)                                                      */
/*      SPRMCC (0x20802214)                                                     */
/*      SPRUCC (0x20802214)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The error status log indicates which error is causing the report of the next fatal error event (any event that is not the first). When SW clears the error status bit in IRPRINGERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define IRPRINGFNERRST_IIO_RAS_REG 0x11220214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_parity_error : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 snp_addr_parity_error : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 rxdata1_parity_error : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 rxdata0_parity_error : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 blq_parity_error : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 bl_ecc_corrected_error : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 bl_ecc_uncorrectable_error : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 bl_cqid_parity_error : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 ak0_cqid_parity_error : 1;

                            /* Bits[8:8], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 ak1_cqid_parity_error : 1;

                            /* Bits[9:9], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 txq0_addr_par_error : 1;

                            /* Bits[10:10], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 txq1_addr_par_error : 1;

                            /* Bits[11:11], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 snpq_rf_addr_parity_error : 1;

                            /* Bits[12:12], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 unexpected_go_err_rcvd : 1;

                            /* Bits[13:13], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPRINGFNERRST_IIO_RAS_STRUCT;

/* IRPRINGNFERRST_IIO_RAS_REG supported on:                                     */
/*      SPRA0 (0x20802218)                                                      */
/*      SPRB0 (0x20802218)                                                      */
/*      SPRHBM (0x20802218)                                                     */
/*      SPRC0 (0x20802218)                                                      */
/*      SPRMCC (0x20802218)                                                     */
/*      SPRUCC (0x20802218)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The error status log indicates which error is causing the report of the first fatal error event. When SW clears the error status bit in IRPRINGERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define IRPRINGNFERRST_IIO_RAS_REG 0x11220218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_parity_error : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 snp_addr_parity_error : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 rxdata1_parity_error : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 rxdata0_parity_error : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 blq_parity_error : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 bl_ecc_corrected_error : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 bl_ecc_uncorrectable_error : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 bl_cqid_parity_error : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 ak0_cqid_parity_error : 1;

                            /* Bits[8:8], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 ak1_cqid_parity_error : 1;

                            /* Bits[9:9], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 txq0_addr_par_error : 1;

                            /* Bits[10:10], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 txq1_addr_par_error : 1;

                            /* Bits[11:11], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 snpq_rf_addr_parity_error : 1;

                            /* Bits[12:12], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 unexpected_go_err_rcvd : 1;

                            /* Bits[13:13], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPRINGNFERRST_IIO_RAS_STRUCT;

/* IRPRINGNNERRST_IIO_RAS_REG supported on:                                     */
/*      SPRA0 (0x2080221c)                                                      */
/*      SPRB0 (0x2080221c)                                                      */
/*      SPRHBM (0x2080221c)                                                     */
/*      SPRC0 (0x2080221c)                                                      */
/*      SPRMCC (0x2080221c)                                                     */
/*      SPRUCC (0x2080221c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The error status log indicates which error is causing the report of the next fatal error event (any event that is not the first). When SW clears the error status bit in IRPRINGERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define IRPRINGNNERRST_IIO_RAS_REG 0x1122021C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_parity_error : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 snp_addr_parity_error : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 rxdata1_parity_error : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 rxdata0_parity_error : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 blq_parity_error : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 bl_ecc_corrected_error : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 bl_ecc_uncorrectable_error : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 bl_cqid_parity_error : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 ak0_cqid_parity_error : 1;

                            /* Bits[8:8], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 ak1_cqid_parity_error : 1;

                            /* Bits[9:9], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 txq0_addr_par_error : 1;

                            /* Bits[10:10], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 txq1_addr_par_error : 1;

                            /* Bits[11:11], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 snpq_rf_addr_parity_error : 1;

                            /* Bits[12:12], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 unexpected_go_err_rcvd : 1;

                            /* Bits[13:13], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPRINGERRST */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPRINGNNERRST_IIO_RAS_STRUCT;

/* IRPRINGERRCNTSEL_IIO_RAS_REG supported on:                                   */
/*      SPRA0 (0x20802220)                                                      */
/*      SPRB0 (0x20802220)                                                      */
/*      SPRHBM (0x20802220)                                                     */
/*      SPRC0 (0x20802220)                                                      */
/*      SPRMCC (0x20802220)                                                     */
/*      SPRUCC (0x20802220)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* IRP Ring Error Counter Select
*/


#define IRPRINGERRCNTSEL_IIO_RAS_REG 0x11220220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 irpringerrcntsel_bl_parity_err : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irpringerrcntsel_snp_addr_parity_err : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irpringerrcntsel_rxdata1_parity_err : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irpringerrcntsel_rxdata0_parity_err : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irpringerrcntsel_blq_parity_err : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irpringerrcntsel_bl_ecc_corrected_err : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irpringerrcntsel_bl_ecc_uncorrectable_err : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irpringerrcntsel_bl_cqid_parity_err : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irpringerrcntsel_ak0_cqid_parity_err : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irpringerrcntsel_ak1_cqid_parity_err : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irpringerrcntsel_txq0_addr_par_err : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irpringerrcntsel_txq1_addr_par_err : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irpringerrcntsel_snpq_rf_addr_parity_err : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irpringerrcntsel_unexpected_go_err_rcvd : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPRINGERRCNTSEL_IIO_RAS_STRUCT;

/* IRPRINGERRCNT_IIO_RAS_REG supported on:                                      */
/*      SPRA0 (0x20802224)                                                      */
/*      SPRB0 (0x20802224)                                                      */
/*      SPRHBM (0x20802224)                                                     */
/*      SPRC0 (0x20802224)                                                      */
/*      SPRMCC (0x20802224)                                                     */
/*      SPRUCC (0x20802224)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* IRP Ring Error Counter
*/


#define IRPRINGERRCNT_IIO_RAS_REG 0x11220224

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 errcnt : 7;

                            /* Bits[6:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error Accumulator (Counter)[br] This counter
                               accumulates errors that occur when the
                               associated error type is selected in the
                               IRPRINGERRCNTSEL register.[br] Notes:[br] - This
                               register is cleared by writing 7Fh.[br] -
                               Maximum counter available is 127d (7Fh)
                            */
    UINT32 errovf : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error Accumulator Overflow[br] 0: No overflow
                               occurred[br] 1: Error overflow. The error count
                               may not be valid.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPRINGERRCNT_IIO_RAS_STRUCT;

/* IRPRINGMISC_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x20802228)                                                      */
/*      SPRB0 (0x20802228)                                                      */
/*      SPRHBM (0x20802228)                                                     */
/*      SPRC0 (0x20802228)                                                      */
/*      SPRMCC (0x20802228)                                                     */
/*      SPRUCC (0x20802228)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register is a miscellaneous error log register, to be used for additional details about an error, such as the position of parity bits with error.
*/


#define IRPRINGMISC_IIO_RAS_REG 0x11220228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl_parity_bits_0 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates which parity bit from the mesh had the
                               error
                            */
    UINT32 bl_parity_bits_1 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates which parity bit from the mesh had the
                               error
                            */
    UINT32 bl_parity_bits_2 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates which parity bit from the mesh had the
                               error
                            */
    UINT32 bl_parity_bits_3 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates which parity bit from the mesh had the
                               error
                            */
    UINT32 bl_parity_bits_4 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Indicates which parity bit from the mesh had the
                               error (this bit used for byte enable parity)
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPRINGMISC_IIO_RAS_STRUCT;

/* IRPERRST_IIO_RAS_REG supported on:                                           */
/*      SPRA0 (0x20802230)                                                      */
/*      SPRB0 (0x20802230)                                                      */
/*      SPRHBM (0x20802230)                                                     */
/*      SPRC0 (0x20802230)                                                      */
/*      SPRMCC (0x20802230)                                                     */
/*      SPRUCC (0x20802230)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register indicates the errors detected by the Coherent Interface.[br]

*/


#define IRPERRST_IIO_RAS_REG 0x11220230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wrcache_correcc_error : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Write Cache Correctable ECC[br] A single bit ECC
                               error was detected and corrected within the
                               Write Cache
                            */
    UINT32 wrcache_uncecc_error : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Write Cache Un-correctable ECC[br] A double bit
                               ECC error was detected within the Write Cache
                            */
    UINT32 protocol_rcvd_poison : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Protocol Layer Received Poisoned Packet (C1)[br]
                               A poisoned packet has been received from the
                               Coherent Interface.[br] Trigger cases:[br] 1.
                               IRP receives BL from mesh with poison=1, and the
                               BL is associated with a request that is not a
                               memory read completion (e.g. outbound write
                               request, P2P completion)[br] 2. IRP receives a
                               BL from the mesh with poison=1, and the BL is
                               associated with a memory read completion
                               (triggers only when irpp0plsr0.poison_log_en=1
                               and iiomiscctrl.poisfen=1)
                            */
    UINT32 csr_acc_32b_unaligned : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* CSR access crossing 32-bit boundary (C3) */
    UINT32 protocol_rcvd_unexprsp : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Protocol Layer Received Unexpected
                               Response/Completion (D7)[br] A completion has
                               been received from the Coherent Interface that
                               was unexpected.
                            */
    UINT32 protocol_parity_error : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Protocol Parity Error (DB)[br] Originally used
                               for detecting parity error on coherent
                               interface, however, no parity checks exist. So
                               this logs parity errors on data from the MS2IOSF
                               switch on the inbound path.
                            */
    UINT32 p2p_hdr_rf_addr_par_error : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               IRP detected a parity error in the P2P header
                               register file (RF)
                            */
    UINT32 faf_rf_addr_par_error : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               IRP detected a parity error in the FAF (fire-
                               and-forget) register file (RF)
                            */
    UINT32 addrcam0_addr_par_error : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               IRP detected a parity error in the AddrCam0
                               register file (RF)
                            */
    UINT32 addrcam1_addr_par_error : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               IRP detected a parity error in the AddrCam1
                               register file (RF)
                            */
    UINT32 addrcam2_addr_par_error : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               IRP detected a parity error in the AddrCam2
                               register file (RF)
                            */
    UINT32 addrcam3_addr_par_error : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               IRP detected a parity error in the AddrCam3
                               register file (RF)
                            */
    UINT32 pf_timeout_err_cs0 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS0 */
    UINT32 pf_timeout_err_cs1 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS1 */
    UINT32 pf_timeout_err_cs2 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS2 */
    UINT32 pf_timeout_err_cs3 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS3 */
    UINT32 protocol_qt_overflow_underflow : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Protocol Queue/Table Overflow or Underflow (DA) */
    UINT32 iommu_faf_rf_addr_par_error : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               IRP detected a parity error in the iommu VFAF
                               (fire-and-forget) register file (RF)
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPERRST_IIO_RAS_STRUCT;

/* IRPFFERRST_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x20802238)                                                      */
/*      SPRB0 (0x20802238)                                                      */
/*      SPRHBM (0x20802238)                                                     */
/*      SPRC0 (0x20802238)                                                      */
/*      SPRMCC (0x20802238)                                                     */
/*      SPRUCC (0x20802238)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The error status log indicates which error is causing the report of the first fatal error event. When SW clears the error status bit in IRPERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define IRPFFERRST_IIO_RAS_REG 0x11220238

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wrcache_correcc_error : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 wrcache_uncecc_error : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 protocol_rcvd_poison : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 csr_acc_32b_unaligned : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 protocol_rcvd_unexprsp : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 protocol_parity_error : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 p2p_hdr_rf_addr_par_error : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 faf_rf_addr_par_error : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam0_addr_par_error : 1;

                            /* Bits[8:8], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam1_addr_par_error : 1;

                            /* Bits[9:9], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam2_addr_par_error : 1;

                            /* Bits[10:10], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam3_addr_par_error : 1;

                            /* Bits[11:11], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 pf_timeout_err_cs0 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS0 */
    UINT32 pf_timeout_err_cs1 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS1 */
    UINT32 pf_timeout_err_cs2 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS2 */
    UINT32 pf_timeout_err_cs3 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS3 */
    UINT32 protocol_qt_overflow_underflow : 1;

                            /* Bits[16:16], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 iommu_faf_rf_addr_par_error : 1;

                            /* Bits[17:17], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPFFERRST_IIO_RAS_STRUCT;

/* IRPFNERRST_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x2080223c)                                                      */
/*      SPRB0 (0x2080223c)                                                      */
/*      SPRHBM (0x2080223c)                                                     */
/*      SPRC0 (0x2080223c)                                                      */
/*      SPRMCC (0x2080223c)                                                     */
/*      SPRUCC (0x2080223c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The error status log indicates which error is causing the report of the next fatal error event (any event that is not the first). When SW clears the error status bit in IRPERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define IRPFNERRST_IIO_RAS_REG 0x1122023C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wrcache_correcc_error : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 wrcache_uncecc_error : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 protocol_rcvd_poison : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 csr_acc_32b_unaligned : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 protocol_rcvd_unexprsp : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 protocol_parity_error : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 p2p_hdr_rf_addr_par_error : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 faf_rf_addr_par_error : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam0_addr_par_error : 1;

                            /* Bits[8:8], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam1_addr_par_error : 1;

                            /* Bits[9:9], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam2_addr_par_error : 1;

                            /* Bits[10:10], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam3_addr_par_error : 1;

                            /* Bits[11:11], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 pf_timeout_err_cs0 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS0 */
    UINT32 pf_timeout_err_cs1 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS1 */
    UINT32 pf_timeout_err_cs2 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS2 */
    UINT32 pf_timeout_err_cs3 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS2 */
    UINT32 protocol_qt_overflow_underflow : 1;

                            /* Bits[16:16], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 iommu_faf_rf_addr_par_error : 1;

                            /* Bits[17:17], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPFNERRST_IIO_RAS_STRUCT;

/* IRPFFERRHD0_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x20802240)                                                      */
/*      SPRB0 (0x20802240)                                                      */
/*      SPRHBM (0x20802240)                                                     */
/*      SPRC0 (0x20802240)                                                      */
/*      SPRMCC (0x20802240)                                                     */
/*      SPRUCC (0x20802240)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Log DWORD 0 of the first fatal error in IRP
*/


#define IRPFFERRHD0_IIO_RAS_REG 0x11220240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Logs a given DWORD, denoted by the integer
                               number in the Register's name, of the header on
                               an error condition.
                            */

  } Bits;
  UINT32 Data;

} IRPFFERRHD0_IIO_RAS_STRUCT;

/* IRPFFERRHD1_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x20802244)                                                      */
/*      SPRB0 (0x20802244)                                                      */
/*      SPRHBM (0x20802244)                                                     */
/*      SPRC0 (0x20802244)                                                      */
/*      SPRMCC (0x20802244)                                                     */
/*      SPRUCC (0x20802244)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Log DWORD 1 of the first fatal error in IRP
*/


#define IRPFFERRHD1_IIO_RAS_REG 0x11220244

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Logs a given DWORD, denoted by the integer
                               number in the Register's name, of the header on
                               an error condition.
                            */

  } Bits;
  UINT32 Data;

} IRPFFERRHD1_IIO_RAS_STRUCT;

/* IRPFFERRHD2_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x20802248)                                                      */
/*      SPRB0 (0x20802248)                                                      */
/*      SPRHBM (0x20802248)                                                     */
/*      SPRC0 (0x20802248)                                                      */
/*      SPRMCC (0x20802248)                                                     */
/*      SPRUCC (0x20802248)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Log DWORD 2 of the first fatal error in IRP
*/


#define IRPFFERRHD2_IIO_RAS_REG 0x11220248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Logs a given DWORD, denoted by the integer
                               number in the Register's name, of the header on
                               an error condition.
                            */

  } Bits;
  UINT32 Data;

} IRPFFERRHD2_IIO_RAS_STRUCT;

/* IRPFFERRHD3_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x2080224c)                                                      */
/*      SPRB0 (0x2080224c)                                                      */
/*      SPRHBM (0x2080224c)                                                     */
/*      SPRC0 (0x2080224c)                                                      */
/*      SPRMCC (0x2080224c)                                                     */
/*      SPRUCC (0x2080224c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Log DWORD 3 of the first fatal error in IRP
*/


#define IRPFFERRHD3_IIO_RAS_REG 0x1122024C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Logs a given DWORD, denoted by the integer
                               number in the Register's name, of the header on
                               an error condition.
                            */

  } Bits;
  UINT32 Data;

} IRPFFERRHD3_IIO_RAS_STRUCT;

/* IRPFFERRHD4_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x20802250)                                                      */
/*      SPRB0 (0x20802250)                                                      */
/*      SPRHBM (0x20802250)                                                     */
/*      SPRC0 (0x20802250)                                                      */
/*      SPRMCC (0x20802250)                                                     */
/*      SPRUCC (0x20802250)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Log DWORD 4 of the first fatal error in IRP
*/


#define IRPFFERRHD4_IIO_RAS_REG 0x11220250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Logs a given DWORD, denoted by the integer
                               number in the Register's name, of the header on
                               an error condition.
                            */

  } Bits;
  UINT32 Data;

} IRPFFERRHD4_IIO_RAS_STRUCT;

/* IRPNFERRST_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x20802254)                                                      */
/*      SPRB0 (0x20802254)                                                      */
/*      SPRHBM (0x20802254)                                                     */
/*      SPRC0 (0x20802254)                                                      */
/*      SPRMCC (0x20802254)                                                     */
/*      SPRUCC (0x20802254)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The error status log indicates which error is causing the report of the first fatal error event. When SW clears the error status bit in IRPERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define IRPNFERRST_IIO_RAS_REG 0x11220254

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wrcache_correcc_error : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 wrcache_uncecc_error : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 protocol_rcvd_poison : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 csr_acc_32b_unaligned : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 protocol_rcvd_unexprsp : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 protocol_parity_error : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 p2p_hdr_rf_addr_par_error : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 faf_rf_addr_par_error : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam0_addr_par_error : 1;

                            /* Bits[8:8], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam1_addr_par_error : 1;

                            /* Bits[9:9], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam2_addr_par_error : 1;

                            /* Bits[10:10], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam3_addr_par_error : 1;

                            /* Bits[11:11], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 pf_timeout_err_cs0 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS0 */
    UINT32 pf_timeout_err_cs1 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS1 */
    UINT32 pf_timeout_err_cs2 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS2 */
    UINT32 pf_timeout_err_cs3 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS2 */
    UINT32 protocol_qt_overflow_underflow : 1;

                            /* Bits[16:16], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 iommu_faf_rf_addr_par_error : 1;

                            /* Bits[17:17], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPNFERRST_IIO_RAS_STRUCT;

/* IRPNNERRST_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x20802258)                                                      */
/*      SPRB0 (0x20802258)                                                      */
/*      SPRHBM (0x20802258)                                                     */
/*      SPRC0 (0x20802258)                                                      */
/*      SPRMCC (0x20802258)                                                     */
/*      SPRUCC (0x20802258)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The error status log indicates which error is causing the report of the next fatal error event (any event that is not the first). When SW clears the error status bit in IRPERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define IRPNNERRST_IIO_RAS_REG 0x11220258

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 wrcache_correcc_error : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 wrcache_uncecc_error : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 protocol_rcvd_poison : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 csr_acc_32b_unaligned : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 protocol_rcvd_unexprsp : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 protocol_parity_error : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 p2p_hdr_rf_addr_par_error : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 faf_rf_addr_par_error : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam0_addr_par_error : 1;

                            /* Bits[8:8], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam1_addr_par_error : 1;

                            /* Bits[9:9], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam2_addr_par_error : 1;

                            /* Bits[10:10], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 addrcam3_addr_par_error : 1;

                            /* Bits[11:11], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 pf_timeout_err_cs0 : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS0 */
    UINT32 pf_timeout_err_cs1 : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS1 */
    UINT32 pf_timeout_err_cs2 : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS2 */
    UINT32 pf_timeout_err_cs3 : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* IRP detected a prefetch timeout in CS3 */
    UINT32 protocol_qt_overflow_underflow : 1;

                            /* Bits[16:16], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 iommu_faf_rf_addr_par_error : 1;

                            /* Bits[17:17], Access Type=RO/V/P, default=0x00000000*/

                            /* For error description, refer to IRPERRST */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPNNERRST_IIO_RAS_STRUCT;

/* IRPFNERRHD0_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x2080225c)                                                      */
/*      SPRB0 (0x2080225c)                                                      */
/*      SPRHBM (0x2080225c)                                                     */
/*      SPRC0 (0x2080225c)                                                      */
/*      SPRMCC (0x2080225c)                                                     */
/*      SPRUCC (0x2080225c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Log DWORD 0 of the first non-fatal error in IRP
*/


#define IRPFNERRHD0_IIO_RAS_REG 0x1122025C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Logs a given DWORD, denoted by the integer
                               number in the Register's name, of the header on
                               an error condition.
                            */

  } Bits;
  UINT32 Data;

} IRPFNERRHD0_IIO_RAS_STRUCT;

/* IRPFNERRHD1_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x20802260)                                                      */
/*      SPRB0 (0x20802260)                                                      */
/*      SPRHBM (0x20802260)                                                     */
/*      SPRC0 (0x20802260)                                                      */
/*      SPRMCC (0x20802260)                                                     */
/*      SPRUCC (0x20802260)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Log DWORD 1 of the first non-fatal error in IRP
*/


#define IRPFNERRHD1_IIO_RAS_REG 0x11220260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Logs a given DWORD, denoted by the integer
                               number in the Register's name, of the header on
                               an error condition.
                            */

  } Bits;
  UINT32 Data;

} IRPFNERRHD1_IIO_RAS_STRUCT;

/* IRPFNERRHD2_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x20802264)                                                      */
/*      SPRB0 (0x20802264)                                                      */
/*      SPRHBM (0x20802264)                                                     */
/*      SPRC0 (0x20802264)                                                      */
/*      SPRMCC (0x20802264)                                                     */
/*      SPRUCC (0x20802264)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Log DWORD 2 of the first non-fatal error in IRP
*/


#define IRPFNERRHD2_IIO_RAS_REG 0x11220264

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Logs a given DWORD, denoted by the integer
                               number in the Register's name, of the header on
                               an error condition.
                            */

  } Bits;
  UINT32 Data;

} IRPFNERRHD2_IIO_RAS_STRUCT;

/* IRPFNERRHD3_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x20802268)                                                      */
/*      SPRB0 (0x20802268)                                                      */
/*      SPRHBM (0x20802268)                                                     */
/*      SPRC0 (0x20802268)                                                      */
/*      SPRMCC (0x20802268)                                                     */
/*      SPRUCC (0x20802268)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Log DWORD 3 of the first non-fatal error in IRP
*/


#define IRPFNERRHD3_IIO_RAS_REG 0x11220268

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Logs a given DWORD, denoted by the integer
                               number in the Register's name, of the header on
                               an error condition.
                            */

  } Bits;
  UINT32 Data;

} IRPFNERRHD3_IIO_RAS_STRUCT;

/* IRPFNERRHD4_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x2080226c)                                                      */
/*      SPRB0 (0x2080226c)                                                      */
/*      SPRHBM (0x2080226c)                                                     */
/*      SPRC0 (0x2080226c)                                                      */
/*      SPRMCC (0x2080226c)                                                     */
/*      SPRUCC (0x2080226c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Log DWORD 4 of the first non-fatal error in IRP
*/


#define IRPFNERRHD4_IIO_RAS_REG 0x1122026C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Logs a given DWORD, denoted by the integer
                               number in the Register's name, of the header on
                               an error condition.
                            */

  } Bits;
  UINT32 Data;

} IRPFNERRHD4_IIO_RAS_STRUCT;

/* IRPERRCNTSEL_IIO_RAS_REG supported on:                                       */
/*      SPRA0 (0x20802270)                                                      */
/*      SPRB0 (0x20802270)                                                      */
/*      SPRHBM (0x20802270)                                                     */
/*      SPRC0 (0x20802270)                                                      */
/*      SPRMCC (0x20802270)                                                     */
/*      SPRUCC (0x20802270)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* IRP Error Counter Select
*/


#define IRPERRCNTSEL_IIO_RAS_REG 0x11220270

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 irperrcntsel_wrcache_correcc_error : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_wrcache_uncecc_error : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_protocol_rcvd_poison : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_csr_acc_32b_unaligned : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_protocol_rcvd_unexprsp : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_protocol_parity_error : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_p2p_hdr_rf_addr_par_error : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_faf_rf_addr_par_error : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_addrcam0_addr_par_error : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_addrcam1_addr_par_error : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_addrcam2_addr_par_error : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_addrcam3_addr_par_error : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_pf_timeout_err_cs0 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_pf_timeout_err_cs1 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_pf_timeout_err_cs2 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_pf_timeout_err_cs3 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_protocol_qt_overflow_underflow : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 irperrcntsel_iommu_faf_rf_addr_par_error : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               0: Do not select this error type for error
                               counting[br] 1: Select this error type for error
                               counting
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPERRCNTSEL_IIO_RAS_STRUCT;

/* IRPERRCNT_IIO_RAS_REG supported on:                                          */
/*      SPRA0 (0x20802274)                                                      */
/*      SPRB0 (0x20802274)                                                      */
/*      SPRHBM (0x20802274)                                                     */
/*      SPRC0 (0x20802274)                                                      */
/*      SPRMCC (0x20802274)                                                     */
/*      SPRUCC (0x20802274)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* IRP Error Counter
*/


#define IRPERRCNT_IIO_RAS_REG 0x11220274

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 errcnt : 7;

                            /* Bits[6:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error Accumulator (Counter)[br] This counter
                               accumulates errors that occur when the
                               associated error type is selected in the
                               ERRCNTSEL register.[br] Notes:[br] - This
                               register is cleared by writing 7Fh.[br] -
                               Maximum counter available is 127d (7Fh)
                            */
    UINT32 errovf : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error Accumulator Overflow[br] 0: No overflow
                               occurred[br] 1: Error overflow. The error count
                               may not be valid.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPERRCNT_IIO_RAS_STRUCT;

/* ITCERRST_IIO_RAS_REG supported on:                                           */
/*      SPRA0 (0x20802278)                                                      */
/*      SPRB0 (0x20802278)                                                      */
/*      SPRHBM (0x20802278)                                                     */
/*      SPRC0 (0x20802278)                                                      */
/*      SPRMCC (0x20802278)                                                     */
/*      SPRUCC (0x20802278)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ITC Traffic Controller Error Status
*/


#define ITCERRST_IIO_RAS_REG 0x11220278

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 8;

                            /* Bits[7:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 itc_sb_of : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* ITC SB overflow */
    UINT32 itc_sb_uf : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* ITC SB underflow */
    UINT32 itc_iosf_cred_uf : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* ITC IOSF credit underflow */
    UINT32 itc_iosf_cred_of : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* ITC IRP credit overflow */
    UINT32 itc_enq_data_overflow : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               ITC received data from IOSF when the inbound
                               FIFO was already full
                            */
    UINT32 sw2iosf_mps_err : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               MS2IOSF received a request with data with length
                               greater MS2IOSF's IMPS setting
                            */
    UINT32 sw2iosf_req_buf_uf : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* SW2IOSF request buffer underflow error */
    UINT32 sw2iosf_req_buf_ov : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* SW2IOSF request buffer overflow error */
    UINT32 itc_irp_cred_uf : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* ITC IRP credit underflow */
    UINT32 itc_irp_cred_of : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* ITC IRP credit overflow */
    UINT32 itc_par_iosf_dat : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error in the incoming data from IOSF */
    UINT32 itc_par_hdr_rf : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error in the ITC hdr_q RF */
    UINT32 itc_vtmisc_hdr_rf : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error in the ITC vtd_misc_info RF */
    UINT32 itc_par_addr_rf : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error in the ITC addr_q RF */
    UINT32 itc_ecc_cor_rf : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* ECC corrected error in the ITC dat_dword RF */
    UINT32 itc_ecc_uncor_rf : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* ECC uncorrectable error in the ITC dat_dword RF */
    UINT32 itc_cabort : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               ITC completed a request with Completer Abort
                               status
                            */
    UINT32 itc_mabort : 1;

                            /* Bits[25:25], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               ITC completed a request with Unsupported Request
                               status
                            */
    UINT32 inb_unsuccessful_cmpl : 1;

                            /* Bits[26:26], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               MS2IOSF received an unsuccessful completion
                               (status != 0) on the IOSF interface
                            */
    UINT32 itc_enq_overflow : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               ITC received a header from IOSF when the inbound
                               FIFO was already full
                            */
    UINT32 itc_misc_prh_overflow : 1;

                            /* Bits[28:28], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               ITC received a misc channel prh header when a
                               prh_complete on that same channel was still
                               pending
                            */
    UINT32 itc_hw_assert : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               ITC triggered internal hardware assertion. Used
                               for a variety of fatal events, e.g. internal
                               pointer parity error.
                            */
    UINT32 itc_par_iosf_cmd : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error in the incoming command from IOSF */
    UINT32 itc_msgd_illegal_size : 1;

                            /* Bits[31:31], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Inbound message with data length greater than 16
                               DW
                            */

  } Bits;
  UINT32 Data;

} ITCERRST_IIO_RAS_STRUCT;

/* TCERRCNT_IIO_RAS_REG supported on:                                           */
/*      SPRA0 (0x2080227c)                                                      */
/*      SPRB0 (0x2080227c)                                                      */
/*      SPRHBM (0x2080227c)                                                     */
/*      SPRC0 (0x2080227c)                                                      */
/*      SPRMCC (0x2080227c)                                                     */
/*      SPRUCC (0x2080227c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Traffic Controller Error Counter
*/


#define TCERRCNT_IIO_RAS_REG 0x1122027C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc_errcnt : 7;

                            /* Bits[6:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error Accumulator (Counter)[br] This counter
                               accumulates errors that occur when the
                               associated error type is selected in the
                               ERRCNTSEL register.[br] Notes:[br] - This
                               register is cleared by writing 7Fh.[br] -
                               Maximum counter available is 127d (7Fh)
                            */
    UINT32 otc_errovf : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error Accumulator Overflow[br] 0: No overflow
                               occurred[br] 1: Error overflow. The error count
                               may not be valid.
                            */
    UINT32 itc_errcnt : 7;

                            /* Bits[14:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error Accumulator (Counter)[br] This counter
                               accumulates errors that occur when the
                               associated error type is selected in the
                               ERRCNTSEL register.[br] Notes:[br] - This
                               register is cleared by writing 7Fh.[br] -
                               Maximum counter available is 127d (7Fh)
                            */
    UINT32 itc_errovf : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error Accumulator Overflow[br] 0: No overflow
                               occurred[br] 1: Error overflow. The error count
                               may not be valid.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TCERRCNT_IIO_RAS_STRUCT;

/* ITCPAREINJCTL_IIO_RAS_REG supported on:                                      */
/*      SPRA0 (0x20802280)                                                      */
/*      SPRB0 (0x20802280)                                                      */
/*      SPRHBM (0x20802280)                                                     */
/*      SPRC0 (0x20802280)                                                      */
/*      SPRMCC (0x20802280)                                                     */
/*      SPRUCC (0x20802280)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* ITC Parity Error Injection Control Register
*/


#define ITCPAREINJCTL_IIO_RAS_REG 0x11220280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 injmode : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Inject single error until ERRINJST is
                               clear[br] 1: Inject error on every valid packet
                            */
    UINT32 rsvd : 23;

                            /* Bits[23:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bps : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Buffer-Path select[br] 000: AddrQ parity[br]
                               001: HdrQ parity[br] 010: Data uncorrected
                               ECC[br] 011: Data corrected ECC[br] 100: VTdQ
                               parity[br] 101: Address parity to IRP[br] 110:
                               Hardware assertions
                            */
    UINT32 errinjst : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error injected status[br] 0: No error was
                               injected[br] 1: An error was injected
                            */
    UINT32 pol : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Inject parity error on even address bits[br]
                               1: Inject parity error on odd address bits
                            */
    UINT32 eirfsel : 2;

                            /* Bits[30:29], Access Type=RW/P, default=0x00000000*/

                            /*
                               Error Injection Function Select[br] 00: Always
                               Enabled[br] 01: Select EINJ0 response
                               function.[br] 10: Select EINJ1 response
                               function.[br] 11: Reserved
                            */
    UINT32 eien : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               Error injection enable[br] 0: Disable error
                               injection[br] 1: Enable error injection
                            */

  } Bits;
  UINT32 Data;

} ITCPAREINJCTL_IIO_RAS_STRUCT;

/* OTCPAREINJCTL_IIO_RAS_REG supported on:                                      */
/*      SPRA0 (0x20802284)                                                      */
/*      SPRB0 (0x20802284)                                                      */
/*      SPRHBM (0x20802284)                                                     */
/*      SPRC0 (0x20802284)                                                      */
/*      SPRMCC (0x20802284)                                                     */
/*      SPRUCC (0x20802284)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* OTC Parity Error Injection Control Register
*/


#define OTCPAREINJCTL_IIO_RAS_REG 0x11220284

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 injmode : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Inject single error until ERRINJST is
                               clear[br] 1: Inject error on every valid packet
                            */
    UINT32 rsvd : 7;

                            /* Bits[7:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 einj0_start_sel : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /* Error injector0 start select */
    UINT32 einj0_stop_sel : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /* Error injector0 stop select */
    UINT32 einj1_start_sel : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /* Error injector1 start select */
    UINT32 einj1_stop_sel : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /* Error injector1 stop select */
    UINT32 bps : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Buffer-Path select[br] 000: AddrQ parity[br]
                               001: HdrQ parity[br] 010: Data uncorrected
                               ECC[br] 011: Data corrected ECC[br] 100: IOSF
                               Cmd Parity[br] 101: IOSF Data Parity[br] 110:
                               Hardware assertions
                            */
    UINT32 errinjst : 1;

                            /* Bits[27:27], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error injected status[br] 0: No error was
                               injected[br] 1: An error was injected
                            */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 eirfsel : 2;

                            /* Bits[30:29], Access Type=RW/P, default=0x00000000*/

                            /*
                               Error Injection Function Select[br] 00: Always
                               Enabled[br] 01: Select EINJ0 response
                               function.[br] 10: Select EINJ1 response
                               function.[br] 11: Reserved
                            */
    UINT32 eien : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               Error injection enable[br] 0: Disable error
                               injection[br] 1: Enable error injection
                            */

  } Bits;
  UINT32 Data;

} OTCPAREINJCTL_IIO_RAS_STRUCT;

/* ITCERRCTL_IIO_RAS_REG supported on:                                          */
/*      SPRA0 (0x2080228c)                                                      */
/*      SPRB0 (0x2080228c)                                                      */
/*      SPRHBM (0x2080228c)                                                     */
/*      SPRC0 (0x2080228c)                                                      */
/*      SPRMCC (0x2080228c)                                                     */
/*      SPRUCC (0x2080228c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Traffic Controller Error Control
*/


#define ITCERRCTL_IIO_RAS_REG 0x1122028C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 8;

                            /* Bits[7:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 itc_sb_of : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_sb_uf : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_iosf_cred_uf : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_iosf_cred_of : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_enq_data_overflow : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 sw2iosf_mps_err : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 sw2iosf_req_buf_uf : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 sw2iosf_req_buf_ov : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_irp_cred_uf : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_irp_cred_of : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_par_iosf_dat : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_par_hdr_rf : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_vtmisc_hdr_rf : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_par_addr_rf : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_ecc_cor_rf : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_ecc_uncor_rf : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_cabort : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_mabort : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 inb_unsuccessful_cmpl : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_enq_overflow : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_misc_prh_overflow : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_hw_assert : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_par_iosf_cmd : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */
    UINT32 itc_msgd_illegal_size : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to ITCERRST
                            */

  } Bits;
  UINT32 Data;

} ITCERRCTL_IIO_RAS_STRUCT;

/* ITCERRSEV_N0_IIO_RAS_REG supported on:                                       */
/*      SPRA0 (0x20802290)                                                      */
/*      SPRB0 (0x20802290)                                                      */
/*      SPRHBM (0x20802290)                                                     */
/*      SPRC0 (0x20802290)                                                      */
/*      SPRMCC (0x20802290)                                                     */
/*      SPRUCC (0x20802290)                                                     */
/* Register default value on SPRA0: 0x6A958AAA                                  */
/* Register default value on SPRB0: 0x6A958AAA                                  */
/* Register default value on SPRHBM: 0x6A958AAA                                 */
/* Register default value on SPRC0: 0x6A958AAA                                  */
/* Register default value on SPRMCC: 0x6A958AAA                                 */
/* Register default value on SPRUCC: 0x6A958AAA                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register configures the error severity level of inbound traffic controller errors. Software can program the error severity and hardware reports the error using that severity. This register is sticky and can only be reset by PWRGOOD.
*/


#define ITCERRSEV_N0_IIO_RAS_REG 0x11220290

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 itc_irp_cred_uf : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_irp_cred_of : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_par_iosf_dat : 2;

                            /* Bits[5:4], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_par_hdr_rf : 2;

                            /* Bits[7:6], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_vtmisc_hdr_rf : 2;

                            /* Bits[9:8], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_par_addr_rf : 2;

                            /* Bits[11:10], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_ecc_cor_rf : 2;

                            /* Bits[13:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_ecc_uncor_rf : 2;

                            /* Bits[15:14], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_cabort : 2;

                            /* Bits[17:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_mabort : 2;

                            /* Bits[19:18], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 inb_unsuccessful_cmpl : 2;

                            /* Bits[21:20], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_enq_overflow : 2;

                            /* Bits[23:22], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_misc_prh_overflow : 2;

                            /* Bits[25:24], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_hw_assert : 2;

                            /* Bits[27:26], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_par_iosf_cmd : 2;

                            /* Bits[29:28], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_msgd_illegal_size : 2;

                            /* Bits[31:30], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */

  } Bits;
  UINT32 Data;

} ITCERRSEV_N0_IIO_RAS_STRUCT;

/* ITCERRSEV_N1_IIO_RAS_REG supported on:                                       */
/*      SPRA0 (0x20802294)                                                      */
/*      SPRB0 (0x20802294)                                                      */
/*      SPRHBM (0x20802294)                                                     */
/*      SPRC0 (0x20802294)                                                      */
/*      SPRMCC (0x20802294)                                                     */
/*      SPRUCC (0x20802294)                                                     */
/* Register default value on SPRA0: 0x0000AAAA                                  */
/* Register default value on SPRB0: 0x0000AAAA                                  */
/* Register default value on SPRHBM: 0x0000AAAA                                 */
/* Register default value on SPRC0: 0x0000AAAA                                  */
/* Register default value on SPRMCC: 0x0000AAAA                                 */
/* Register default value on SPRUCC: 0x0000AAAA                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register configures the error severity level of inbound traffic controller errors. Software can program the error severity and hardware reports the error using that severity. This register is sticky and can only be reset by PWRGOOD.
*/


#define ITCERRSEV_N1_IIO_RAS_REG 0x11220294

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sw2iosf_req_buf_ov : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 sw2iosf_req_buf_uf : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 sw2iosf_mps_err : 2;

                            /* Bits[5:4], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_enq_data_overflow : 2;

                            /* Bits[7:6], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_iosf_cred_uf : 2;

                            /* Bits[9:8], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_iosf_cred_of : 2;

                            /* Bits[11:10], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_sb_uf : 2;

                            /* Bits[13:12], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 itc_sb_of : 2;

                            /* Bits[15:14], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to ITCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITCERRSEV_N1_IIO_RAS_STRUCT;

/* ITCERRCNTSEL_IIO_RAS_REG supported on:                                       */
/*      SPRA0 (0x20802298)                                                      */
/*      SPRB0 (0x20802298)                                                      */
/*      SPRHBM (0x20802298)                                                     */
/*      SPRC0 (0x20802298)                                                      */
/*      SPRMCC (0x20802298)                                                     */
/*      SPRUCC (0x20802298)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Traffic Controller Error Counter Selection
*/


#define ITCERRCNTSEL_IIO_RAS_REG 0x11220298

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 8;

                            /* Bits[7:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 itc_sb_of : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_sb_uf : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_iosf_cred_uf : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_iosf_cred_of : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_enq_data_overflow : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 sw2iosf_mps_err : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 sw2iosf_req_buf_uf : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 sw2iosf_req_buf_ov : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_irp_cred_uf : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_irp_cred_of : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_par_iosf_dat : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_par_hdr_rf : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_vtmisc_hdr_rf : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_par_addr_rf : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_ecc_cor_rf : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_ecc_uncor_rf : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_cabort : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_mabort : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 inb_unsuccessful_cmpl : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_enq_overflow : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_misc_prh_overflow : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_hw_assert : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_par_iosf_cmd : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */
    UINT32 itc_msgd_illegal_size : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to ITCERRST.
                            */

  } Bits;
  UINT32 Data;

} ITCERRCNTSEL_IIO_RAS_STRUCT;

/* ITCFFERRST_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x208022a0)                                                      */
/*      SPRB0 (0x208022a0)                                                      */
/*      SPRHBM (0x208022a0)                                                     */
/*      SPRC0 (0x208022a0)                                                      */
/*      SPRMCC (0x208022a0)                                                     */
/*      SPRUCC (0x208022a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* When SW clears the error status bit in TCERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define ITCFFERRST_IIO_RAS_REG 0x112202A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tc_core_error_status_log : 7;

                            /* Bits[6:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The error status log indicates which error is
                               causing the report of the first error event. The
                               encoding indicates the corresponding bit
                               position of the error in the error status
                               register.
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITCFFERRST_IIO_RAS_STRUCT;

/* ITCFFERRHD0_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022a4)                                                      */
/*      SPRB0 (0x208022a4)                                                      */
/*      SPRHBM (0x208022a4)                                                     */
/*      SPRC0 (0x208022a4)                                                      */
/*      SPRMCC (0x208022a4)                                                     */
/*      SPRUCC (0x208022a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log stores the Traffic Controller data path header information of the associated Traffic Controller error. The header indicates where the error is originating from and the address of the cycle.[br]

*/


#define ITCFFERRHD0_IIO_RAS_REG 0x112202A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 0[br] Logs the first DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} ITCFFERRHD0_IIO_RAS_STRUCT;

/* ITCFFERRHD1_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022a8)                                                      */
/*      SPRB0 (0x208022a8)                                                      */
/*      SPRHBM (0x208022a8)                                                     */
/*      SPRC0 (0x208022a8)                                                      */
/*      SPRMCC (0x208022a8)                                                     */
/*      SPRUCC (0x208022a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ITC Fatal First Error Header 1
*/


#define ITCFFERRHD1_IIO_RAS_REG 0x112202A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 1[br] Logs the second DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} ITCFFERRHD1_IIO_RAS_STRUCT;

/* ITCFFERRHD2_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022ac)                                                      */
/*      SPRB0 (0x208022ac)                                                      */
/*      SPRHBM (0x208022ac)                                                     */
/*      SPRC0 (0x208022ac)                                                      */
/*      SPRMCC (0x208022ac)                                                     */
/*      SPRUCC (0x208022ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ITC Fatal First Error Header 2
*/


#define ITCFFERRHD2_IIO_RAS_REG 0x112202AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 2[br] Logs the third DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} ITCFFERRHD2_IIO_RAS_STRUCT;

/* ITCFFERRHD3_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022b0)                                                      */
/*      SPRB0 (0x208022b0)                                                      */
/*      SPRHBM (0x208022b0)                                                     */
/*      SPRC0 (0x208022b0)                                                      */
/*      SPRMCC (0x208022b0)                                                     */
/*      SPRUCC (0x208022b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ITC Fatal First Error Header 3
*/


#define ITCFFERRHD3_IIO_RAS_REG 0x112202B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 3[br] Logs the fourth DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} ITCFFERRHD3_IIO_RAS_STRUCT;

/* ITCFNERRST_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x208022b4)                                                      */
/*      SPRB0 (0x208022b4)                                                      */
/*      SPRHBM (0x208022b4)                                                     */
/*      SPRC0 (0x208022b4)                                                      */
/*      SPRMCC (0x208022b4)                                                     */
/*      SPRUCC (0x208022b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* When SW clears the error status bit in TCERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define ITCFNERRST_IIO_RAS_REG 0x112202B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tc_core_error_status_log : 7;

                            /* Bits[6:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The error status log indicates which error is
                               causing the report of the first error event. The
                               encoding indicates the corresponding bit
                               position of the error in the error status
                               register.
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITCFNERRST_IIO_RAS_STRUCT;

/* ITCNFERRST_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x208022b8)                                                      */
/*      SPRB0 (0x208022b8)                                                      */
/*      SPRHBM (0x208022b8)                                                     */
/*      SPRC0 (0x208022b8)                                                      */
/*      SPRMCC (0x208022b8)                                                     */
/*      SPRUCC (0x208022b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* When SW clears the error status bit in TCERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define ITCNFERRST_IIO_RAS_REG 0x112202B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tc_core_error_status_log : 7;

                            /* Bits[6:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The error status log indicates which error is
                               causing the report of the first error event. The
                               encoding indicates the corresponding bit
                               position of the error in the error status
                               register.
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITCNFERRST_IIO_RAS_STRUCT;

/* ITCNFERRHD0_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022bc)                                                      */
/*      SPRB0 (0x208022bc)                                                      */
/*      SPRHBM (0x208022bc)                                                     */
/*      SPRC0 (0x208022bc)                                                      */
/*      SPRMCC (0x208022bc)                                                     */
/*      SPRUCC (0x208022bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ITC Non-Fatal First Error Header 0
*/


#define ITCNFERRHD0_IIO_RAS_REG 0x112202BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 0[br] Logs the first DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} ITCNFERRHD0_IIO_RAS_STRUCT;

/* ITCNFERRHD1_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022c0)                                                      */
/*      SPRB0 (0x208022c0)                                                      */
/*      SPRHBM (0x208022c0)                                                     */
/*      SPRC0 (0x208022c0)                                                      */
/*      SPRMCC (0x208022c0)                                                     */
/*      SPRUCC (0x208022c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ITC Non-Fatal First Error Header 1
*/


#define ITCNFERRHD1_IIO_RAS_REG 0x112202C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 1[br] Logs the second DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} ITCNFERRHD1_IIO_RAS_STRUCT;

/* ITCNFERRHD2_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022c4)                                                      */
/*      SPRB0 (0x208022c4)                                                      */
/*      SPRHBM (0x208022c4)                                                     */
/*      SPRC0 (0x208022c4)                                                      */
/*      SPRMCC (0x208022c4)                                                     */
/*      SPRUCC (0x208022c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ITC Non-Fatal First Error Header 2
*/


#define ITCNFERRHD2_IIO_RAS_REG 0x112202C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 2[br] Logs the third DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} ITCNFERRHD2_IIO_RAS_STRUCT;

/* ITCNFERRHD3_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022c8)                                                      */
/*      SPRB0 (0x208022c8)                                                      */
/*      SPRHBM (0x208022c8)                                                     */
/*      SPRC0 (0x208022c8)                                                      */
/*      SPRMCC (0x208022c8)                                                     */
/*      SPRUCC (0x208022c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ITC Non-Fatal First Error Header 3
*/


#define ITCNFERRHD3_IIO_RAS_REG 0x112202C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 3[br] Logs the fourth DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} ITCNFERRHD3_IIO_RAS_STRUCT;

/* ITCNNERRST_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x208022cc)                                                      */
/*      SPRB0 (0x208022cc)                                                      */
/*      SPRHBM (0x208022cc)                                                     */
/*      SPRC0 (0x208022cc)                                                      */
/*      SPRMCC (0x208022cc)                                                     */
/*      SPRUCC (0x208022cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* When SW clears the error status bit in TCERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define ITCNNERRST_IIO_RAS_REG 0x112202CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tc_core_error_status_log : 7;

                            /* Bits[6:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The error status log indicates which error is
                               causing the report of the first error event. The
                               encoding indicates the corresponding bit
                               position of the error in the error status
                               register.
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITCNNERRST_IIO_RAS_STRUCT;

/* OTCFFERRST_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x208022d0)                                                      */
/*      SPRB0 (0x208022d0)                                                      */
/*      SPRHBM (0x208022d0)                                                     */
/*      SPRC0 (0x208022d0)                                                      */
/*      SPRMCC (0x208022d0)                                                     */
/*      SPRUCC (0x208022d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* When SW clears the error status bit in TCERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define OTCFFERRST_IIO_RAS_REG 0x112202D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc_error_status_log : 7;

                            /* Bits[6:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The error status log indicates which error is
                               causing the report of the first error event. The
                               encoding indicates the corresponding bit
                               position of the error in the error status
                               register.
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCFFERRST_IIO_RAS_STRUCT;

/* OTCFFERRHD0_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022d4)                                                      */
/*      SPRB0 (0x208022d4)                                                      */
/*      SPRHBM (0x208022d4)                                                     */
/*      SPRC0 (0x208022d4)                                                      */
/*      SPRMCC (0x208022d4)                                                     */
/*      SPRUCC (0x208022d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log stores the Traffic Controller data path header information of the associated Traffic Controller error. The header indicates where the error is originating from and the address of the cycle.[br]

*/


#define OTCFFERRHD0_IIO_RAS_REG 0x112202D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 0[br] Logs the first DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} OTCFFERRHD0_IIO_RAS_STRUCT;

/* OTCFFERRHD1_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022d8)                                                      */
/*      SPRB0 (0x208022d8)                                                      */
/*      SPRHBM (0x208022d8)                                                     */
/*      SPRC0 (0x208022d8)                                                      */
/*      SPRMCC (0x208022d8)                                                     */
/*      SPRUCC (0x208022d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* OTC Fatal First Error Header
*/


#define OTCFFERRHD1_IIO_RAS_REG 0x112202D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 1[br] Logs the second DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} OTCFFERRHD1_IIO_RAS_STRUCT;

/* OTCFFERRHD2_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022dc)                                                      */
/*      SPRB0 (0x208022dc)                                                      */
/*      SPRHBM (0x208022dc)                                                     */
/*      SPRC0 (0x208022dc)                                                      */
/*      SPRMCC (0x208022dc)                                                     */
/*      SPRUCC (0x208022dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* OTC Fatal First Error Header
*/


#define OTCFFERRHD2_IIO_RAS_REG 0x112202DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 2[br] Logs the third DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} OTCFFERRHD2_IIO_RAS_STRUCT;

/* OTCFFERRHD3_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022e0)                                                      */
/*      SPRB0 (0x208022e0)                                                      */
/*      SPRHBM (0x208022e0)                                                     */
/*      SPRC0 (0x208022e0)                                                      */
/*      SPRMCC (0x208022e0)                                                     */
/*      SPRUCC (0x208022e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* OTC Fatal First Error Header
*/


#define OTCFFERRHD3_IIO_RAS_REG 0x112202E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 3[br] Logs the fourth DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} OTCFFERRHD3_IIO_RAS_STRUCT;

/* OTCFNERRST_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x208022e4)                                                      */
/*      SPRB0 (0x208022e4)                                                      */
/*      SPRHBM (0x208022e4)                                                     */
/*      SPRC0 (0x208022e4)                                                      */
/*      SPRMCC (0x208022e4)                                                     */
/*      SPRUCC (0x208022e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* When SW clears the error status bit in TCERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define OTCFNERRST_IIO_RAS_REG 0x112202E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc_error_status_log : 7;

                            /* Bits[6:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The error status log indicates which error is
                               causing the report of the first error event. The
                               encoding indicates the corresponding bit
                               position of the error in the error status
                               register.
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCFNERRST_IIO_RAS_STRUCT;

/* OTCNFERRST_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x208022e8)                                                      */
/*      SPRB0 (0x208022e8)                                                      */
/*      SPRHBM (0x208022e8)                                                     */
/*      SPRC0 (0x208022e8)                                                      */
/*      SPRMCC (0x208022e8)                                                     */
/*      SPRUCC (0x208022e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* When SW clears the error status bit in TCERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define OTCNFERRST_IIO_RAS_REG 0x112202E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc_error_status_log : 7;

                            /* Bits[6:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The error status log indicates which error is
                               causing the report of the first error event. The
                               encoding indicates the corresponding bit
                               position of the error in the error status
                               register.
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCNFERRST_IIO_RAS_STRUCT;

/* OTCNFERRHD0_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022ec)                                                      */
/*      SPRB0 (0x208022ec)                                                      */
/*      SPRHBM (0x208022ec)                                                     */
/*      SPRC0 (0x208022ec)                                                      */
/*      SPRMCC (0x208022ec)                                                     */
/*      SPRUCC (0x208022ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log stores the Traffic Controllerdata path header information of the associated Traffic Controller error. The header indicates where the error is originating from and the address of the cycle.[br]

*/


#define OTCNFERRHD0_IIO_RAS_REG 0x112202EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 0[br] Logs the first DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} OTCNFERRHD0_IIO_RAS_STRUCT;

/* OTCNFERRHD1_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022f0)                                                      */
/*      SPRB0 (0x208022f0)                                                      */
/*      SPRHBM (0x208022f0)                                                     */
/*      SPRC0 (0x208022f0)                                                      */
/*      SPRMCC (0x208022f0)                                                     */
/*      SPRUCC (0x208022f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* OTC Non-Fatal First Error Header 1
*/


#define OTCNFERRHD1_IIO_RAS_REG 0x112202F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 1[br] Logs the second DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} OTCNFERRHD1_IIO_RAS_STRUCT;

/* OTCNFERRHD2_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022f4)                                                      */
/*      SPRB0 (0x208022f4)                                                      */
/*      SPRHBM (0x208022f4)                                                     */
/*      SPRC0 (0x208022f4)                                                      */
/*      SPRMCC (0x208022f4)                                                     */
/*      SPRUCC (0x208022f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* OTC Non-Fatal First Error Header 2
*/


#define OTCNFERRHD2_IIO_RAS_REG 0x112202F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 2[br] Logs the third DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} OTCNFERRHD2_IIO_RAS_STRUCT;

/* OTCNFERRHD3_IIO_RAS_REG supported on:                                        */
/*      SPRA0 (0x208022f8)                                                      */
/*      SPRB0 (0x208022f8)                                                      */
/*      SPRHBM (0x208022f8)                                                     */
/*      SPRC0 (0x208022f8)                                                      */
/*      SPRMCC (0x208022f8)                                                     */
/*      SPRUCC (0x208022f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* OTC Non-Fatal First Error Header 3
*/


#define OTCNFERRHD3_IIO_RAS_REG 0x112202F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdr : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Log of Header Dword 3[br] Logs the fourth DWORD
                               of the header on an error condition
                            */

  } Bits;
  UINT32 Data;

} OTCNFERRHD3_IIO_RAS_STRUCT;

/* OTCNNERRST_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x208022fc)                                                      */
/*      SPRB0 (0x208022fc)                                                      */
/*      SPRHBM (0x208022fc)                                                     */
/*      SPRC0 (0x208022fc)                                                      */
/*      SPRMCC (0x208022fc)                                                     */
/*      SPRUCC (0x208022fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* When SW clears the error status bit in TCERRST, HW automatically clears the corresponding bit in this register.[br]

*/


#define OTCNNERRST_IIO_RAS_REG 0x112202FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc_error_status_log : 7;

                            /* Bits[6:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The error status log indicates which error is
                               causing the report of the first error event. The
                               encoding indicates the corresponding bit
                               position of the error in the error status
                               register.
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCNNERRST_IIO_RAS_STRUCT;

/* M2IOERRCTL_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x20802304)                                                      */
/*      SPRB0 (0x20802304)                                                      */
/*      SPRHBM (0x20802304)                                                     */
/*      SPRC0 (0x20802304)                                                      */
/*      SPRMCC (0x20802304)                                                     */
/*      SPRUCC (0x20802304)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register controls some miscellaneous RAS functionality in M2IO (M2IOSF)
*/


#define M2IOERRCTL_IIO_RAS_REG 0x11220304

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 m2io_ras_pcie_err_bitmap : 5;

                            /* Bits[4:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               This BitField is driven by a M2IOSF strap, and
                               drives the Bitmap field in the RAS PCIe Error
                               Messages M2ISOF sends to IEH
                            */
    UINT32 rsvd : 6;

                            /* Bits[10:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 c8_ib_header_parity_core_poison_en : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               When MS2IOSF detects an internal header parity
                               error on an inbound posted write destined for
                               the mesh, MS2IOSF sets poison on the
                               corresponding BL that it sends to the mesh.
                            */
    UINT32 c8_ib_header_parity_p2p_poison_en : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               When MS2IOSF detects an internal header parity
                               error on an inbound confined P2P posted write,
                               MS2IOSF sets EP=1 in the forwarded write on
                               IOSF.
                            */
    UINT32 c8_ib_header_parity_sticky_poison : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               When MS2IOSF detects an internal header parity
                               error on an inbound completion, MS2IOSF poisons
                               that completion and all future completions.
                            */
    UINT32 rsvd_14 : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M2IOERRCTL_IIO_RAS_STRUCT;

/* TC_ERR_MISCLOG_IIO_RAS_REG supported on:                                     */
/*      SPRA0 (0x20802310)                                                      */
/*      SPRB0 (0x20802310)                                                      */
/*      SPRHBM (0x20802310)                                                     */
/*      SPRC0 (0x20802310)                                                      */
/*      SPRMCC (0x20802310)                                                     */
/*      SPRUCC (0x20802310)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Misc logging info for TC errors
*/


#define TC_ERR_MISCLOG_IIO_RAS_REG 0x11220310

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otcfifoerridx : 11;

                            /* Bits[10:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Otcfifoerridx */
    UINT32 itcfifoerridx : 21;

                            /* Bits[31:11], Access Type=RO/V/P, default=0x00000000*/

                            /* Itcfifoerridx */

  } Bits;
  UINT32 Data;

} TC_ERR_MISCLOG_IIO_RAS_STRUCT;

/* IRPP0PLSR0_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x20802370)                                                      */
/*      SPRB0 (0x20802370)                                                      */
/*      SPRHBM (0x20802370)                                                     */
/*      SPRC0 (0x20802370)                                                      */
/*      SPRMCC (0x20802370)                                                     */
/*      SPRUCC (0x20802370)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Logs information about the poisoned request (e.g. transaction type, direction, requester ID, address).[br]
Logs only outbound poisoned completions. Does not log any inbound poisoned requests or outbound poisoned requests that are not completions.[br]
Future implementations may log all types of poisoned requests.
*/


#define IRPP0PLSR0_IIO_RAS_REG 0x11220370

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 poison_log_type : 2;

                            /* Bits[1:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Value of 00 = IIO generated inbound poison[br]
                               Value of 01 = Inbound from PCIe[br] Value of 10
                               = Poison coming from ring[br] Value of 11 = IIO
                               generated outbound poison
                            */
    UINT32 poison_log_rid : 7;

                            /* Bits[8:2], Access Type=RO/V/P, default=0x00000000*/

                            /* Switch Requestor id */
    UINT32 poison_log_len : 5;

                            /* Bits[13:9], Access Type=RO/V/P, default=0x00000000*/

                            /* Request Length */
    UINT32 poison_log_ttype : 5;

                            /* Bits[18:14], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Transaction type. It indicates for what type of
                               transaction the poison is logged here.
                            */
    UINT32 rsvd : 9;

                            /* Bits[27:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dbg_mux_en : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               Turns on the debug muxes (clocks, etc).[br]
                               Notes:[br] Locked by DBGBUSLCK
                            */
    UINT32 poison_log_overflow : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Ring poison detection event was observed when
                               valid=1. In this case, the register will retain
                               the information about the first error. This is
                               consistent with UCNA/UCNA overwrite rules in
                               Machine Check Architecture.
                            */
    UINT32 poison_log_valid : 1;

                            /* Bits[30:30], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               If Valid=0, the log does not contain any valid
                               information.[br] If Valid=1, the log contain
                               valid information. Any additional ring poison
                               detection events will set overflow bit.
                            */
    UINT32 poison_log_en : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               This bit indicates if the shadow logging is
                               enabled or disabled. A value of 0 is disabled
                               and a value of 1 is enabled. If not enabled no
                               logging takes place in this register.
                            */

  } Bits;
  UINT32 Data;

} IRPP0PLSR0_IIO_RAS_STRUCT;

/* IRPP0PLSR1_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x20802374)                                                      */
/*      SPRB0 (0x20802374)                                                      */
/*      SPRHBM (0x20802374)                                                     */
/*      SPRC0 (0x20802374)                                                      */
/*      SPRMCC (0x20802374)                                                     */
/*      SPRUCC (0x20802374)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Logs information about the poisoned request (e.g. transaction type, direction, requester ID, address).[br]
Logs only outbound poisoned completions. Does not log any inbound poisoned requests or outbound poisoned requests that are not completions.[br]
Future implementations may log all types of poisoned requests.
*/


#define IRPP0PLSR1_IIO_RAS_REG 0x11220374

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 poison_log_addr_lo : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Address 31:0 associated with the poison data
                               (e.g. Address field in poisoned BL packet).
                            */

  } Bits;
  UINT32 Data;

} IRPP0PLSR1_IIO_RAS_STRUCT;

/* IRPP0PLSR2_IIO_RAS_REG supported on:                                         */
/*      SPRA0 (0x20802378)                                                      */
/*      SPRB0 (0x20802378)                                                      */
/*      SPRHBM (0x20802378)                                                     */
/*      SPRC0 (0x20802378)                                                      */
/*      SPRMCC (0x20802378)                                                     */
/*      SPRUCC (0x20802378)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Logs information about the poisoned request (e.g. transaction type, direction, requester ID, address).[br]
Logs only outbound poisoned completions. Does not log any inbound poisoned requests or outbound poisoned requests that are not completions.[br]
Future implementations may log all types of poisoned requests.
*/


#define IRPP0PLSR2_IIO_RAS_REG 0x11220378

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 poison_log_addr_hi : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Address 63:32 associated with the poison data */

  } Bits;
  UINT32 Data;

} IRPP0PLSR2_IIO_RAS_STRUCT;

/* OTCERRST_IIO_RAS_REG supported on:                                           */
/*      SPRA0 (0x20802380)                                                      */
/*      SPRB0 (0x20802380)                                                      */
/*      SPRHBM (0x20802380)                                                     */
/*      SPRC0 (0x20802380)                                                      */
/*      SPRMCC (0x20802380)                                                     */
/*      SPRUCC (0x20802380)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* OTC Traffic Controller Error Status
*/


#define OTCERRST_IIO_RAS_REG 0x11220380

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc_crdt_uf : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* OTC Credit underflow status */
    UINT32 otc_crdt_of : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* OTC credit overflow status */
    UINT32 otc_par_hdr_rf : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error in the OTC hdr_q RF */
    UINT32 otc_par_addr_rf : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error in the OTC addr_q RF */
    UINT32 otc_ecc_uncor_rf : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* ECC uncorrectable error in the OTC dat_dword RF */
    UINT32 otc_cabort : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* MS2IOSF does not implement this error */
    UINT32 otc_mabort : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Master abort */
    UINT32 otc_mtc_tgt_err : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Multicast target error for OTC */
    UINT32 otc_ecc_cor_rf : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* ECC corrected error in the OTC dat_dword RF */
    UINT32 otc_mctp_bcast_to_dmi : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* MS2IOSF does not implement this error */
    UINT32 otc_par_rte : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* MS2IOSF does not implement this error */
    UINT32 otc_irp_dat_par : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error on incoming data from IRP */
    UINT32 otc_irp_addr_par : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Parity error on incoming addr from IRP */
    UINT32 otc_hw_assert : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               OTC triggered internal hardware assertion. Used
                               for a variety of fatal events, e.g. internal
                               pointer parity error.
                            */
    UINT32 otc_txn_dur_lock : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* MS2IOSF received a DnS Txn during Lock */
    UINT32 otc_gpsb_par : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Not implemented. Reserved for future use. */
    UINT32 otc_misc_snarf_fifo_of : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Misc cluster Snarf Fifo Overflow */
    UINT32 otc_misc_snarf_fifo_uf : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Misc cluster Snarf Fifo Underflow */
    UINT32 otc_misc_oobmsm_mult_pend : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               OOBMSM agent in Misc cluster got a request when
                               one was already pending
                            */
    UINT32 vmd_cfgbar_overflow : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               MS2IOSF received a request targeting VMD CFGBAR
                               when two requests were already in progress.
                               MS2IOSF aborted the request.
                            */
    UINT32 otc_poison_mabort : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Otc Aborted the request because of the EP bit
                               set
                            */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCERRST_IIO_RAS_STRUCT;

/* OTCERRCTL_IIO_RAS_REG supported on:                                          */
/*      SPRA0 (0x2080238c)                                                      */
/*      SPRB0 (0x2080238c)                                                      */
/*      SPRHBM (0x2080238c)                                                     */
/*      SPRC0 (0x2080238c)                                                      */
/*      SPRMCC (0x2080238c)                                                     */
/*      SPRUCC (0x2080238c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* Traffic Controller Error Control
*/


#define OTCERRCTL_IIO_RAS_REG 0x1122038C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc_crdt_uf : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_crdt_of : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_par_hdr_rf : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_par_addr_rf : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_ecc_uncor_rf : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_cabort : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_mabort : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_mtc_tgt_err : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_ecc_cor_rf : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_mctp_bcast_to_dmi : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_par_rte : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_irp_dat_par : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_irp_addr_par : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_hw_assert : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_txn_dur_lock : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_gpsb_par : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_misc_snarf_fifo_of : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_misc_snarf_fifo_uf : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_misc_oobmsm_mult_pend : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 vmd_cfgbar_overflow : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 otc_poison_mabort : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               0: Disable logging of this error[br] 1: Enable
                               logging of this error[br] For error description,
                               refer to OTCERRST
                            */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCERRCTL_IIO_RAS_STRUCT;

/* OTCERRSEV_N0_IIO_RAS_REG supported on:                                       */
/*      SPRA0 (0x20802390)                                                      */
/*      SPRB0 (0x20802390)                                                      */
/*      SPRHBM (0x20802390)                                                     */
/*      SPRC0 (0x20802390)                                                      */
/*      SPRMCC (0x20802390)                                                     */
/*      SPRUCC (0x20802390)                                                     */
/* Register default value on SPRA0: 0x9AA456AA                                  */
/* Register default value on SPRB0: 0x9AA456AA                                  */
/* Register default value on SPRHBM: 0x9AA456AA                                 */
/* Register default value on SPRC0: 0x9AA456AA                                  */
/* Register default value on SPRMCC: 0x9AA456AA                                 */
/* Register default value on SPRUCC: 0x9AA456AA                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register configures the error severity level of outbound traffic controller errors. Software can program the error severity and hardware reports the error using that severity. This register is sticky and can only be reset by PWRGOOD.
*/


#define OTCERRSEV_N0_IIO_RAS_REG 0x11220390

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc_crdt_uf : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_crdt_of : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_par_hdr_rf : 2;

                            /* Bits[5:4], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_par_addr_rf : 2;

                            /* Bits[7:6], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_ecc_uncor_rf : 2;

                            /* Bits[9:8], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_cabort : 2;

                            /* Bits[11:10], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_mabort : 2;

                            /* Bits[13:12], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_mtc_tgt_err : 2;

                            /* Bits[15:14], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_ecc_cor_rf : 2;

                            /* Bits[17:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_mctp_bcast_to_dmi : 2;

                            /* Bits[19:18], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_par_rte : 2;

                            /* Bits[21:20], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_irp_dat_par : 2;

                            /* Bits[23:22], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_irp_addr_par : 2;

                            /* Bits[25:24], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_hw_assert : 2;

                            /* Bits[27:26], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_txn_dur_lock : 2;

                            /* Bits[29:28], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_gpsb_par : 2;

                            /* Bits[31:30], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */

  } Bits;
  UINT32 Data;

} OTCERRSEV_N0_IIO_RAS_STRUCT;

/* OTCERRSEV_N1_IIO_RAS_REG supported on:                                       */
/*      SPRA0 (0x20802394)                                                      */
/*      SPRB0 (0x20802394)                                                      */
/*      SPRHBM (0x20802394)                                                     */
/*      SPRC0 (0x20802394)                                                      */
/*      SPRMCC (0x20802394)                                                     */
/*      SPRUCC (0x20802394)                                                     */
/* Register default value on SPRA0: 0x0000016A                                  */
/* Register default value on SPRB0: 0x0000016A                                  */
/* Register default value on SPRHBM: 0x0000016A                                 */
/* Register default value on SPRC0: 0x0000016A                                  */
/* Register default value on SPRMCC: 0x0000016A                                 */
/* Register default value on SPRUCC: 0x0000016A                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This register configures the error severity level of outbound traffic controller errors. Software can program the error severity and hardware reports the error using that severity. This register is sticky and can only be reset by PWRGOOD.
*/


#define OTCERRSEV_N1_IIO_RAS_REG 0x11220394

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc_misc_snarf_fifo_of : 2;

                            /* Bits[1:0], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_misc_snarf_fifo_uf : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_misc_oobmsm_mult_pend : 2;

                            /* Bits[5:4], Access Type=RW/P, default=0x00000002*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 vmd_cfgbar_overflow : 2;

                            /* Bits[7:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 otc_poison_mabort : 2;

                            /* Bits[9:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               Configures the severity of this error. For error
                               description, refer to OTCERRST.[br] 00: Error
                               Severity Level 0 (Correctable)[br] 01: Error
                               Severity Level 1 (Recoverable)[br] 10: Error
                               Severity Level 2 (Fatal)[br] 11: Reserved
                            */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCERRSEV_N1_IIO_RAS_STRUCT;

/* OTCERRCNTSEL_IIO_RAS_REG supported on:                                       */
/*      SPRA0 (0x20802398)                                                      */
/*      SPRB0 (0x20802398)                                                      */
/*      SPRHBM (0x20802398)                                                     */
/*      SPRC0 (0x20802398)                                                      */
/*      SPRMCC (0x20802398)                                                     */
/*      SPRUCC (0x20802398)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Traffic Controller Error Counter Selection
*/


#define OTCERRCNTSEL_IIO_RAS_REG 0x11220398

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc_crdt_uf : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_crdt_of : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_par_hdr_rf : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_par_addr_rf : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_ecc_uncor_rf : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_cabort : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_mabort : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_mtc_tgt_err : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_ecc_cor_rf : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_mctp_bcast_to_dmi : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_par_rte : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_irp_dat_par : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_irp_addr_par : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_hw_assert : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_txn_dur_lock : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_gpsb_par : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_misc_snarf_fifo_of : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_misc_snarf_fifo_uf : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_misc_oobmsm_mult_pend : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 vmd_cfgbar_overflow : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 otc_poison_mabort : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               Select this error to be counted in TCERRCNT. For
                               error description, refer to OTCERRST.
                            */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCERRCNTSEL_IIO_RAS_STRUCT;

/* M2IOSF_SCRATCHPAD_IIO_RAS_REG supported on:                                  */
/*      SPRA0 (0x208023fc)                                                      */
/*      SPRB0 (0x208023fc)                                                      */
/*      SPRHBM (0x208023fc)                                                     */
/*      SPRC0 (0x208023fc)                                                      */
/*      SPRMCC (0x208023fc)                                                     */
/*      SPRUCC (0x208023fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_2_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_2                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Read-write bits used for validation and debug.
*/


#define M2IOSF_SCRATCHPAD_IIO_RAS_REG 0x112203FC

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

} M2IOSF_SCRATCHPAD_IIO_RAS_STRUCT;
#endif /* _IIO_RAS_h */
