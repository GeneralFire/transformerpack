
/** @file
  IIO_DSA.h

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


#ifndef _IIO_DSA_h
#define _IIO_DSA_h
#include <Base.h>

/* VID_IIO_DSA_REG supported on:                                                */
/*      SPRA0 (0x10808000)                                                      */
/*      SPRB0 (0x10808000)                                                      */
/*      SPRHBM (0x10808000)                                                     */
/*      SPRC0 (0x10808000)                                                      */
/*      SPRMCC (0x10808000)                                                     */
/*      SPRUCC (0x10808000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Identifies the manufacturer of the device
*/


#define VID_IIO_DSA_REG 0x21010000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 vid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /* Indicates Intel (8086h) */

  } Bits;
  UINT16 Data;

} VID_IIO_DSA_STRUCT;

/* DID_IIO_DSA_REG supported on:                                                */
/*      SPRA0 (0x10808002)                                                      */
/*      SPRB0 (0x10808002)                                                      */
/*      SPRHBM (0x10808002)                                                     */
/*      SPRC0 (0x10808002)                                                      */
/*      SPRMCC (0x10808002)                                                     */
/*      SPRUCC (0x10808002)                                                     */
/* Register default value on SPRA0: 0x00000B25                                  */
/* Register default value on SPRB0: 0x00000B25                                  */
/* Register default value on SPRHBM: 0x00000B25                                 */
/* Register default value on SPRC0: 0x00000B25                                  */
/* Register default value on SPRMCC: 0x00000B25                                 */
/* Register default value on SPRUCC: 0x00000B25                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Identifies the particular device.
*/


#define DID_IIO_DSA_REG 0x21010002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 did : 16;

                            /* Bits[15:0], Access Type=RO/V/P, default=0x00000B25*/

                            /* Allocated by the vendor. */

  } Bits;
  UINT16 Data;

} DID_IIO_DSA_STRUCT;

/* PCICMD_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x10808004)                                                      */
/*      SPRB0 (0x10808004)                                                      */
/*      SPRHBM (0x10808004)                                                     */
/*      SPRC0 (0x10808004)                                                      */
/*      SPRMCC (0x10808004)                                                     */
/*      SPRUCC (0x10808004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The Command register provides coarse control over a device's ability to generate and respond to PCI cycles. When a 0 is written to this register, the device is logically disconnected from the PCI bus for all accesses except configuration accesses.
*/


#define PCICMD_IIO_DSA_REG 0x21010004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 iose : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Controls the Function's response to I/O Space
                               accesses. A value of 0 disables the response.
                               Hardwired to 0 as this Function does not support
                               I/O Space accesses.
                            */
    UINT16 mse : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Controls the Function's response to Memory Space
                               accesses. A value of 0 disables the response. A
                               value of 1 allows the Function to respond to
                               Memory Space accesses.
                            */
    UINT16 bme : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Controls the ability of the endpoint to issue
                               Memory Read/Write requests. When set, the
                               Function is allowed to issue Memory Requests.
                               When clear, the Function is not allowed to issue
                               Memory Requests. Note that as MSI-X interrupt
                               Messages are in-band memory writes, setting BME
                               to 0b disables MSI-X interrupt Messages as well.
                               Requests other than Memory Requests (eg
                               Completion) are not controlled by this bit.
                               [br]See the RAS Chapter in the HAS for
                               programming restrictions.
                            */
    UINT16 sce : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Does not apply to PCI Express and is hardwired
                               to 0b
                            */
    UINT16 mwie : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Does not apply to PCI Express and is hardwired
                               to 0b
                            */
    UINT16 vgapse : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Does not apply to PCI Express and is hardwired
                               to 0b
                            */
    UINT16 pere : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               This bit controls the logging of poisoned TLPs
                               in the Master Data Parity Error bit in the
                               Status register.
                            */
    UINT16 wcc : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Does not apply to PCI Express and is hardwired
                               to 0b
                            */
    UINT16 see : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               When Set, this bit enables reporting of Non-
                               Fatal and Fatal errors detected by the Function
                               to the Root Complex. Note that errors are
                               reported if enabled either through this bit or
                               through the PCI Express specific bits in the
                               Device Control register
                            */
    UINT16 fbe : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Does not apply to PCI Express and is hardwired
                               to 0b
                            */
    UINT16 intd : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Controls the ability of the Function to generate
                               INTx interrupts. This Function does not generate
                               INTx interrupts, so this bit it hardwired to 0b.
                            */
    UINT16 rsvd : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PCICMD_IIO_DSA_STRUCT;

/* PCISTS_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x10808006)                                                      */
/*      SPRB0 (0x10808006)                                                      */
/*      SPRHBM (0x10808006)                                                     */
/*      SPRC0 (0x10808006)                                                      */
/*      SPRMCC (0x10808006)                                                     */
/*      SPRUCC (0x10808006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The Status register is used to record status information for PCI bus related events.
*/


#define PCISTS_IIO_DSA_REG 0x21010006

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 rsvd : 3;

                            /* Bits[2:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 ints : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               When Set, indicates that an INTx emulation
                               interrupt is pending internally in the Function.
                               Hardwired to 0b as this Function does not
                               support INTx
                            */
    UINT16 cl : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Indicates the presence of an Extended Capability
                               list item. Required by all PCI Express
                               endpoints.
                            */
    UINT16 c66 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Does not apply to PCI Express and is hardwired
                               to 0b
                            */
    UINT16 rsvd_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 fbtc : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Does not apply to PCI Express and is hardwired
                               to 0b
                            */
    UINT16 mdpe : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit is Set by an Endpoint Function if the
                               Parity Error Response bit in the Command
                               register is 1b it either receives a Poisoned
                               Completion or transmits a Poisoned Request
                            */
    UINT16 dt : 2;

                            /* Bits[10:9], Access Type=RO, default=0x00000000*/

                            /*
                               Does not apply to PCI Express and is hardwired
                               to 00b
                            */
    UINT16 sta : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit is Set when a Function completes a
                               Posted or Non-Posted Request as a Completer
                               Abort error.
                            */
    UINT16 rta : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit is Set when a Requester receives a
                               Completion with Completer Abort Completion
                               Status.
                            */
    UINT16 rma : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit is Set when a Requester receives a
                               Completion with Unsupported Request Completion
                               Status.
                            */
    UINT16 sse : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit is Set when a Function sends an
                               ERR_FATAL or ERR_NONFATAL Message, and the SERR#
                               Enable bit in the Command register is 1.
                            */
    UINT16 dpe : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit is Set by a Function whenever it
                               receives a Poisoned TLP, regardless of the state
                               the Parity Error Response bit in the Command
                               register.
                            */

  } Bits;
  UINT16 Data;

} PCISTS_IIO_DSA_STRUCT;

/* RID_IIO_DSA_REG supported on:                                                */
/*      SPRA0 (0x808008)                                                        */
/*      SPRB0 (0x808008)                                                        */
/*      SPRHBM (0x808008)                                                       */
/*      SPRC0 (0x808008)                                                        */
/*      SPRMCC (0x808008)                                                       */
/*      SPRUCC (0x808008)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register specifies a device specific revision identifier.
*/


#define RID_IIO_DSA_REG 0x21000008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 rid : 8;

                            /* Bits[7:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The value is chosen by the vendor. This field
                               should be viewed as a vendor defined extension
                               to the Device ID.
                            */

  } Bits;
  UINT8 Data;

} RID_IIO_DSA_STRUCT;

/* CCRLPI_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x808009)                                                        */
/*      SPRB0 (0x808009)                                                        */
/*      SPRHBM (0x808009)                                                       */
/*      SPRC0 (0x808009)                                                        */
/*      SPRMCC (0x808009)                                                       */
/*      SPRUCC (0x808009)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* The Class Code register is read-only and is used to identify the generic function of the device and, in some cases, a specific register-level programming interface. The lower byte  identifies a specific register-level programming interface (if any) so that device independent software can interact with the device.
*/


#define CCRLPI_IIO_DSA_REG 0x21000009

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 rlpi : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               Other system peripheral. [br]This field loses
                               its value if warm reset or stronger but retains
                               its value through FLR.
                            */

  } Bits;
  UINT8 Data;

} CCRLPI_IIO_DSA_STRUCT;

/* CCSC_IIO_DSA_REG supported on:                                               */
/*      SPRA0 (0x80800a)                                                        */
/*      SPRB0 (0x80800a)                                                        */
/*      SPRHBM (0x80800a)                                                       */
/*      SPRC0 (0x80800a)                                                        */
/*      SPRMCC (0x80800a)                                                       */
/*      SPRUCC (0x80800a)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* The Class Code register is read-only and is used to identify the generic function of the device and, in some cases, a specific register-level programming interface. The middle byte  is a sub-class code which identifies more specifically the function of the device.
*/


#define CCSC_IIO_DSA_REG 0x2100000A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 sc : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               Other system peripheral. [br]This field loses
                               its value if warm reset or stronger but retains
                               its value through FLR.
                            */

  } Bits;
  UINT8 Data;

} CCSC_IIO_DSA_STRUCT;

/* CCBC_IIO_DSA_REG supported on:                                               */
/*      SPRA0 (0x80800b)                                                        */
/*      SPRB0 (0x80800b)                                                        */
/*      SPRHBM (0x80800b)                                                       */
/*      SPRC0 (0x80800b)                                                        */
/*      SPRMCC (0x80800b)                                                       */
/*      SPRUCC (0x80800b)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* The Class Code register is read-only and is used to identify the generic function of the device and, in some cases, a specific register-level programming interface. The upper byte is a base class code which broadly classifies the type of function the device performs.
*/


#define CCBC_IIO_DSA_REG 0x2100000B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 bc : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               Generic system peripheral. [br]This field loses
                               its value if warm reset or stronger but retains
                               its value through FLR.
                            */

  } Bits;
  UINT8 Data;

} CCBC_IIO_DSA_STRUCT;

/* CLS_IIO_DSA_REG supported on:                                                */
/*      SPRA0 (0x80800c)                                                        */
/*      SPRB0 (0x80800c)                                                        */
/*      SPRHBM (0x80800c)                                                       */
/*      SPRC0 (0x80800c)                                                        */
/*      SPRMCC (0x80800c)                                                       */
/*      SPRUCC (0x80800c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* The Cache Line Size register is set by the system firmware or the operating system to system cache line size.
*/


#define CLS_IIO_DSA_REG 0x2100000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cls : 8;

                            /* Bits[7:0], Access Type=RW, default=None*/

                            /*
                               This field is implemented as a read-write field
                               for legacy compatibility purposes but has no
                               effect on any device behavior. [br]This field
                               loses its value if warm reset or stronger but
                               retains its value through FLR.
                            */

  } Bits;
  UINT8 Data;

} CLS_IIO_DSA_STRUCT;

/* LATTMR_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x80800d)                                                        */
/*      SPRB0 (0x80800d)                                                        */
/*      SPRHBM (0x80800d)                                                       */
/*      SPRC0 (0x80800d)                                                        */
/*      SPRMCC (0x80800d)                                                       */
/*      SPRUCC (0x80800d)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register is also referred to as Primary Latency Timer for Type 1 Configuration Space header Functions.
*/


#define LATTMR_IIO_DSA_REG 0x2100000D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 lattmr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               The Latency Timer does not apply to PCI Express.
                               This register is hardwired to 00h.
                            */

  } Bits;
  UINT8 Data;

} LATTMR_IIO_DSA_STRUCT;

/* HDR_IIO_DSA_REG supported on:                                                */
/*      SPRA0 (0x80800e)                                                        */
/*      SPRB0 (0x80800e)                                                        */
/*      SPRHBM (0x80800e)                                                       */
/*      SPRC0 (0x80800e)                                                        */
/*      SPRMCC (0x80800e)                                                       */
/*      SPRUCC (0x80800e)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register identifies the layout of the second part of the predefined header (beginning at byte 10h in Configuration Space) and also whether or not the Device might contain multiple Functions.
*/


#define HDR_IIO_DSA_REG 0x2100000E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 ht : 7;

                            /* Bits[6:0], Access Type=RO, default=0x00000000*/

                            /* Indicates Type 0 Configuration Space Header. */
    UINT8 mfd : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               When Clear, software must not probe for
                               Functions other than Function 0. Hardwired to 0b
                               as this is a single Function Device.
                            */

  } Bits;
  UINT8 Data;

} HDR_IIO_DSA_STRUCT;

/* BIST_IIO_DSA_REG supported on:                                               */
/*      SPRA0 (0x80800f)                                                        */
/*      SPRB0 (0x80800f)                                                        */
/*      SPRHBM (0x80800f)                                                       */
/*      SPRC0 (0x80800f)                                                        */
/*      SPRMCC (0x80800f)                                                       */
/*      SPRUCC (0x80800f)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This optional register is used for control and status of BIST.
*/


#define BIST_IIO_DSA_REG 0x2100000F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 bist : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Devices that do not support BIST must always
                               return a value of 0.
                            */

  } Bits;
  UINT8 Data;

} BIST_IIO_DSA_STRUCT;

/* BAR0_N0_IIO_DSA_REG supported on:                                            */
/*      SPRA0 (0x20808010)                                                      */
/*      SPRB0 (0x20808010)                                                      */
/*      SPRHBM (0x20808010)                                                     */
/*      SPRC0 (0x20808010)                                                      */
/*      SPRMCC (0x20808010)                                                     */
/*      SPRUCC (0x20808010)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Size, type, and location of address range for control registers
*/


#define BAR0_N0_IIO_DSA_REG 0x21020010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 msi : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Base Address registers that map to Memory Space
                               must return a 0 in bit 0
                            */
    UINT32 bt : 2;

                            /* Bits[2:1], Access Type=RO, default=0x00000002*/

                            /*
                               Base register is 64 bits wide and can be mapped
                               anywhere in the 64-bit address space.
                            */
    UINT32 pf : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               This address map is pre-fetchable but assumes
                               that the IP is integrated into a platform that
                               does not do write merging beyond aligned 8-byte
                               accesses.
                            */
    UINT32 rsvd : 12;

                            /* Bits[15:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addr : 16;

                            /* Bits[31:16], Access Type=RW, default=0x00000000*/

                            /* 64 KiB */

  } Bits;
  UINT32 Data;

} BAR0_N0_IIO_DSA_STRUCT;

/* BAR0_N1_IIO_DSA_REG supported on:                                            */
/*      SPRA0 (0x20808014)                                                      */
/*      SPRB0 (0x20808014)                                                      */
/*      SPRHBM (0x20808014)                                                     */
/*      SPRC0 (0x20808014)                                                      */
/*      SPRMCC (0x20808014)                                                     */
/*      SPRUCC (0x20808014)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Size, type, and location of address range for control registers
*/


#define BAR0_N1_IIO_DSA_REG 0x21020014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* 64 KiB */

  } Bits;
  UINT32 Data;

} BAR0_N1_IIO_DSA_STRUCT;

/* BAR2_N0_IIO_DSA_REG supported on:                                            */
/*      SPRA0 (0x20808018)                                                      */
/*      SPRB0 (0x20808018)                                                      */
/*      SPRHBM (0x20808018)                                                     */
/*      SPRC0 (0x20808018)                                                      */
/*      SPRMCC (0x20808018)                                                     */
/*      SPRUCC (0x20808018)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Size, type, and location of address range for portals
*/


#define BAR2_N0_IIO_DSA_REG 0x21020018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 msi : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Base Address registers that map to Memory Space
                               must return a 0 in bit 0
                            */
    UINT32 bt : 2;

                            /* Bits[2:1], Access Type=RO, default=0x00000002*/

                            /*
                               Base register is 64 bits wide and can be mapped
                               anywhere in the 64-bit address space.
                            */
    UINT32 pf : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               This address map is pre-fetchable but assumes
                               that the IP is integrated into a platform that
                               does not do write merging beyond aligned 8-byte
                               accesses.
                            */
    UINT32 rsvd : 13;

                            /* Bits[16:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addr : 15;

                            /* Bits[31:17], Access Type=RW, default=0x00000000*/

                            /* 128 KiB */

  } Bits;
  UINT32 Data;

} BAR2_N0_IIO_DSA_STRUCT;

/* BAR2_N1_IIO_DSA_REG supported on:                                            */
/*      SPRA0 (0x2080801c)                                                      */
/*      SPRB0 (0x2080801c)                                                      */
/*      SPRHBM (0x2080801c)                                                     */
/*      SPRC0 (0x2080801c)                                                      */
/*      SPRMCC (0x2080801c)                                                     */
/*      SPRUCC (0x2080801c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Size, type, and location of address range for portals
*/


#define BAR2_N1_IIO_DSA_REG 0x2102001C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* 128 KiB */

  } Bits;
  UINT32 Data;

} BAR2_N1_IIO_DSA_STRUCT;

/* SSVID_IIO_DSA_REG supported on:                                              */
/*      SPRA0 (0x1080802c)                                                      */
/*      SPRB0 (0x1080802c)                                                      */
/*      SPRHBM (0x1080802c)                                                     */
/*      SPRC0 (0x1080802c)                                                      */
/*      SPRMCC (0x1080802c)                                                     */
/*      SPRUCC (0x1080802c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register (along with SSID) is used to uniquely identify the subsystem where the PCI device resides.
*/


#define SSVID_IIO_DSA_REG 0x2101002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ssvid : 16;

                            /* Bits[15:0], Access Type=RW, default=None*/

                            /*
                               This field should be written by boot SW.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */

  } Bits;
  UINT16 Data;

} SSVID_IIO_DSA_STRUCT;

/* SSID_IIO_DSA_REG supported on:                                               */
/*      SPRA0 (0x1080802e)                                                      */
/*      SPRB0 (0x1080802e)                                                      */
/*      SPRHBM (0x1080802e)                                                     */
/*      SPRC0 (0x1080802e)                                                      */
/*      SPRMCC (0x1080802e)                                                     */
/*      SPRUCC (0x1080802e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register (along with SSVID) is used to uniquely identify the subsystem where the PCI device resides.
*/


#define SSID_IIO_DSA_REG 0x2101002E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ssid : 16;

                            /* Bits[15:0], Access Type=RW, default=None*/

                            /*
                               This field should be written by boot SW.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */

  } Bits;
  UINT16 Data;

} SSID_IIO_DSA_STRUCT;

/* CAPPTR_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x808034)                                                        */
/*      SPRB0 (0x808034)                                                        */
/*      SPRHBM (0x808034)                                                       */
/*      SPRC0 (0x808034)                                                        */
/*      SPRMCC (0x808034)                                                       */
/*      SPRUCC (0x808034)                                                       */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This optional register is used to point to a linked list of new capabilities implemented by this device. This register is only valid if the Capabilities List bit in the Status Register is set.
*/


#define CAPPTR_IIO_DSA_REG 0x21000034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 capptr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000040*/

                            /* Points to PCI Express Capability */

  } Bits;
  UINT8 Data;

} CAPPTR_IIO_DSA_STRUCT;

/* INTL_IIO_DSA_REG supported on:                                               */
/*      SPRA0 (0x80803c)                                                        */
/*      SPRB0 (0x80803c)                                                        */
/*      SPRHBM (0x80803c)                                                       */
/*      SPRC0 (0x80803c)                                                        */
/*      SPRMCC (0x80803c)                                                       */
/*      SPRUCC (0x80803c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The Interrupt Line register communicates interrupt line routing information.
*/


#define INTL_IIO_DSA_REG 0x2100003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 intl : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Hardwired to 00h as this Function does not use
                               an Interrupt Pin
                            */

  } Bits;
  UINT8 Data;

} INTL_IIO_DSA_STRUCT;

/* INTP_IIO_DSA_REG supported on:                                               */
/*      SPRA0 (0x80803d)                                                        */
/*      SPRB0 (0x80803d)                                                        */
/*      SPRHBM (0x80803d)                                                       */
/*      SPRC0 (0x80803d)                                                        */
/*      SPRMCC (0x80803d)                                                       */
/*      SPRUCC (0x80803d)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The Interrupt Pin register is a read-only register that identifies the legacy interrupt Message(s) the Function uses
*/


#define INTP_IIO_DSA_REG 0x2100003D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 intp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               A value of 00h indicates that the Function uses
                               no legacy interrupt Message(s).
                            */

  } Bits;
  UINT8 Data;

} INTP_IIO_DSA_STRUCT;

/* MINGNT_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x80803e)                                                        */
/*      SPRB0 (0x80803e)                                                        */
/*      SPRHBM (0x80803e)                                                       */
/*      SPRC0 (0x80803e)                                                        */
/*      SPRMCC (0x80803e)                                                       */
/*      SPRUCC (0x80803e)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Does not apply to PCI Express
*/


#define MINGNT_IIO_DSA_REG 0x2100003E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 mingnt : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 00h */

  } Bits;
  UINT8 Data;

} MINGNT_IIO_DSA_STRUCT;

/* MAXLAT_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x80803f)                                                        */
/*      SPRB0 (0x80803f)                                                        */
/*      SPRHBM (0x80803f)                                                       */
/*      SPRC0 (0x80803f)                                                        */
/*      SPRMCC (0x80803f)                                                       */
/*      SPRUCC (0x80803f)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Does not apply to PCI Express
*/


#define MAXLAT_IIO_DSA_REG 0x2100003F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 maxlat : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Hardwired to 00h */

  } Bits;
  UINT8 Data;

} MAXLAT_IIO_DSA_STRUCT;

/* PCIECAPLST_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x10808040)                                                      */
/*      SPRB0 (0x10808040)                                                      */
/*      SPRHBM (0x10808040)                                                     */
/*      SPRC0 (0x10808040)                                                      */
/*      SPRMCC (0x10808040)                                                     */
/*      SPRUCC (0x10808040)                                                     */
/* Register default value on SPRA0: 0x00008010                                  */
/* Register default value on SPRB0: 0x00008010                                  */
/* Register default value on SPRHBM: 0x00008010                                 */
/* Register default value on SPRC0: 0x00008010                                  */
/* Register default value on SPRMCC: 0x00008010                                 */
/* Register default value on SPRUCC: 0x00008010                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Enumerates the PCI Express Capability Structure in the PCI Configuration list.
*/


#define PCIECAPLST_IIO_DSA_REG 0x21010040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 capid : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000010*/

                            /* Indicates the PCI Express Capability structure. */
    UINT16 nxtcap : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000080*/

                            /*
                               Offset to the nest PCI Capability structure
                               (MSI-X, in this case)
                            */

  } Bits;
  UINT16 Data;

} PCIECAPLST_IIO_DSA_STRUCT;

/* PCIECAP_IIO_DSA_REG supported on:                                            */
/*      SPRA0 (0x10808042)                                                      */
/*      SPRB0 (0x10808042)                                                      */
/*      SPRHBM (0x10808042)                                                     */
/*      SPRC0 (0x10808042)                                                      */
/*      SPRMCC (0x10808042)                                                     */
/*      SPRUCC (0x10808042)                                                     */
/* Register default value on SPRA0: 0x00000092                                  */
/* Register default value on SPRB0: 0x00000092                                  */
/* Register default value on SPRHBM: 0x00000092                                 */
/* Register default value on SPRC0: 0x00000092                                  */
/* Register default value on SPRMCC: 0x00000092                                 */
/* Register default value on SPRUCC: 0x00000092                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Identifies PCI Express device Function type and associated capabilities.
*/


#define PCIECAP_IIO_DSA_REG 0x21010042

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 capver : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000002*/

                            /*
                               Indicates the PCI-SIG defined PCI Express
                               Capability structure version number.
                            */
    UINT16 devtype : 4;

                            /* Bits[7:4], Access Type=RO, default=0x00000009*/

                            /*
                               Indicates the specific type of this PCI Express
                               Function. Root Complex Integrated Endpoint.
                            */
    UINT16 slotimp : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /* No slot associated with this Function */
    UINT16 intmsgnum : 5;

                            /* Bits[13:9], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates which MSI-X vector is used for the
                               interrupt message generated in association with
                               any of the status bits in this Capability
                               structure.
                            */
    UINT16 rsvd : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PCIECAP_IIO_DSA_STRUCT;

/* DEVCAP_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x20808044)                                                      */
/*      SPRB0 (0x20808044)                                                      */
/*      SPRHBM (0x20808044)                                                     */
/*      SPRC0 (0x20808044)                                                      */
/*      SPRMCC (0x20808044)                                                     */
/*      SPRUCC (0x20808044)                                                     */
/* Register default value on SPRA0: 0x00008022                                  */
/* Register default value on SPRB0: 0x00008022                                  */
/* Register default value on SPRHBM: 0x00008022                                 */
/* Register default value on SPRC0: 0x00008022                                  */
/* Register default value on SPRMCC: 0x00008022                                 */
/* Register default value on SPRUCC: 0x00008022                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Identifies PCI Express device Function specific capabilities.
*/


#define DEVCAP_IIO_DSA_REG 0x21020044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mpss : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000002*/

                            /*
                               Indicates the maximum payload size that the
                               Function can support for TLPs. (010b = 512 bytes
                               max payload size)
                            */
    UINT32 pfs : 2;

                            /* Bits[4:3], Access Type=RO, default=0x00000000*/

                            /* Phantom functions are not supported */
    UINT32 etfs : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /* 8-bit tag field supported */
    UINT32 l0slat : 3;

                            /* Bits[8:6], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 l1lat : 3;

                            /* Bits[11:9], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 rsvd : 3;

                            /* Bits[14:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rber : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /* Must be Set */
    UINT32 rsvd_16 : 12;

                            /* Bits[27:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 flr : 1;

                            /* Bits[28:28], Access Type=RW, default=None*/

                            /*
                               Indicates support for the Function Level reset
                               mechanism. [br]This field loses its value if
                               warm reset or stronger but retains its value
                               through FLR.
                            */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DEVCAP_IIO_DSA_STRUCT;

/* DEVCTL_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x10808048)                                                      */
/*      SPRB0 (0x10808048)                                                      */
/*      SPRHBM (0x10808048)                                                     */
/*      SPRC0 (0x10808048)                                                      */
/*      SPRMCC (0x10808048)                                                     */
/*      SPRUCC (0x10808048)                                                     */
/* Register default value on SPRA0: 0x00002910                                  */
/* Register default value on SPRB0: 0x00002910                                  */
/* Register default value on SPRHBM: 0x00002910                                 */
/* Register default value on SPRC0: 0x00002910                                  */
/* Register default value on SPRMCC: 0x00002910                                 */
/* Register default value on SPRUCC: 0x00002910                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls PCI Express device specific parameters
*/


#define DEVCTL_IIO_DSA_REG 0x21010048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 cere : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, in conjunction with other bits,
                               controls sending ERR_COR Messages.
                            */
    UINT16 nere : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, in conjunction with other bits,
                               controls sending ERR_NONFATAL Messages.
                            */
    UINT16 fere : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, in conjunction with other bits,
                               controls sending ERR_FATAL Messages.
                            */
    UINT16 urre : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, in conjunction with other bits,
                               controls the signaling of unsupported Request
                               Errors by sending error Messages.
                            */
    UINT16 ero : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               If this bit is Set, the Function is permitted to
                               set the Relaxed Ordering bit in the Attributes
                               field of transactions it initiates that do not
                               require strong write ordering.
                            */
    UINT16 mps : 3;

                            /* Bits[7:5], Access Type=RW, default=None*/

                            /*
                               This field sets the maximum TLP payload size for
                               the Function. [br]This field loses its value if
                               warm reset or stronger but retains its value
                               through FLR.
                            */
    UINT16 etfe : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               This bit, in combination with the 10-Bit Tag
                               Requester Enable bit in the Device Control 2
                               register, determines how many Tag field bits a
                               Requester is permitted to use.
                            */
    UINT16 rsvd : 2;

                            /* Bits[10:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 ens : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               If this bit is Set, the Function is permitted to
                               Set the No Snoop bit in the Requester Attributes
                               of transactions it initiates that do not require
                               hardware enforced cache coherency.
                            */
    UINT16 mrrs : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000002*/

                            /*
                               This field sets the maximum Read Request size
                               for the Function as a Requester.
                            */
    UINT16 iflr : 1;

                            /* Bits[15:15], Access Type=RW/V, default=0x00000000*/

                            /*
                               A write of 1b initiates Function Level Reset to
                               the Function. The value read by software from
                               this bit is always 0b.
                            */

  } Bits;
  UINT16 Data;

} DEVCTL_IIO_DSA_STRUCT;

/* DEVSTS_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x1080804a)                                                      */
/*      SPRB0 (0x1080804a)                                                      */
/*      SPRHBM (0x1080804a)                                                     */
/*      SPRC0 (0x1080804a)                                                      */
/*      SPRMCC (0x1080804a)                                                     */
/*      SPRUCC (0x1080804a)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Provides information about PCI Express device (Function) specific parameters.
*/


#define DEVSTS_IIO_DSA_REG 0x2101004A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ced : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit indicates status of correctable errors
                               detected. Errors are logged in this register
                               regardless of whether or not error reporting is
                               enabled in the Device Control register. Errors
                               are logged in this register regardless of the
                               settings of the AER Correctable Error Mask
                               register.
                            */
    UINT16 ned : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit indicates status of Non-fatal errors
                               detected. Errors are logged in this register
                               regardless of whether or not error reporting is
                               enabled in the Device Control register. Errors
                               are logged in this register regardless of the
                               settings of the AER Uncorrectable Error Mask
                               register.
                            */
    UINT16 fed : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit indicates status of Fatal errors
                               detected. Errors are logged in this register
                               regardless of whether or not error reporting is
                               enabled in the Device Control register. Errors
                               are logged in this register regardless of the
                               settings of the AER Uncorrectable Error Mask
                               register.
                            */
    UINT16 urd : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit indicates that the Function received an
                               Unsupported Request. Errors are logged in this
                               register regardless of whether or not error
                               reporting is enabled in the Device Control
                               register.
                            */
    UINT16 rsvd : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 tp : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               When Set, this bit indicates that the Function
                               has issued Non-Posted Requests that have not
                               been completed. This bit is cleared only when
                               all outstanding Non-Posted Requests have
                               completed or have been terminated by the
                               Completion Timeout mechanism. This bit will also
                               be cleared upon the completion of an FLR.
                            */
    UINT16 rsvd_6 : 10;

                            /* Bits[15:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} DEVSTS_IIO_DSA_STRUCT;

/* DEVCAP2_IIO_DSA_REG supported on:                                            */
/*      SPRA0 (0x20808064)                                                      */
/*      SPRB0 (0x20808064)                                                      */
/*      SPRHBM (0x20808064)                                                     */
/*      SPRC0 (0x20808064)                                                      */
/*      SPRMCC (0x20808064)                                                     */
/*      SPRUCC (0x20808064)                                                     */
/* Register default value on SPRA0: 0x00730010                                  */
/* Register default value on SPRB0: 0x00730010                                  */
/* Register default value on SPRHBM: 0x00730010                                 */
/* Register default value on SPRC0: 0x00730010                                  */
/* Register default value on SPRMCC: 0x00730010                                 */
/* Register default value on SPRUCC: 0x00730010                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Identifies additional PCI Express device Function specific capabilities.
*/


#define DEVCAP2_IIO_DSA_REG 0x21020064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctrs : 4;

                            /* Bits[3:0], Access Type=RW, default=None*/

                            /*
                               Completion Timeout programming not supported.
                               Function implements a timeout value in the range
                               50us to 50ms (closer to 20-40ms). [br]This field
                               loses its value if warm reset or stronger but
                               retains its value through FLR.
                            */
    UINT32 ctds : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Indicates support for the Completion Timeout
                               Disable mechanism.
                            */
    UINT32 rsvd : 6;

                            /* Bits[10:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ltrms : 1;

                            /* Bits[11:11], Access Type=RW, default=None*/

                            /*
                               Indicates support for the Latency Tolerance
                               Reporting (LTR) mechanism. [br]This field loses
                               its value if warm reset or stronger but retains
                               its value through FLR.
                            */
    UINT32 rsvd_12 : 4;

                            /* Bits[15:12], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tbtcs : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000001*/

                            /*
                               Indicates the Function supports 10-Bit Tag
                               Completer capability
                            */
    UINT32 tbtrs : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000001*/

                            /*
                               Indicates the Function supports 10-Bit Tag
                               Requester capability
                            */
    UINT32 rsvd_18 : 2;

                            /* Bits[19:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 effs : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000001*/

                            /*
                               If Set, the Function supports the 3-bit
                               definition of the Fmt field. If Clear, the
                               Function supports a 2-bit definition of the Fmt
                               field.
                            */
    UINT32 eetlpps : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000001*/

                            /*
                               Indicates Whether End-End TLP Prefix support is
                               offered by a Function.
                            */
    UINT32 meetlpp : 2;

                            /* Bits[23:22], Access Type=RO, default=0x00000001*/

                            /*
                               Indicates the maximum number of End-End TLP
                               Prefixes supported by this Function.
                            */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DEVCAP2_IIO_DSA_STRUCT;

/* DEVCTL2_IIO_DSA_REG supported on:                                            */
/*      SPRA0 (0x10808068)                                                      */
/*      SPRB0 (0x10808068)                                                      */
/*      SPRHBM (0x10808068)                                                     */
/*      SPRC0 (0x10808068)                                                      */
/*      SPRMCC (0x10808068)                                                     */
/*      SPRUCC (0x10808068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls additional PCI Express device specific parameters
*/


#define DEVCTL2_IIO_DSA_REG 0x21010068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ctv : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /*
                               Completion Timeout Value programmability is not
                               supported.
                            */
    UINT16 ctd : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               When Set, this bit disables the Completion
                               Timeout mechanism.
                            */
    UINT16 rsvd : 5;

                            /* Bits[9:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 ltrme : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               When Set to 1b, this bit enables the Function to
                               send LTR Messages.
                            */
    UINT16 rsvd_11 : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 tbtre : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is Set to 1b, the Requester is
                               permitted to use 10-Bit tags.
                            */
    UINT16 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} DEVCTL2_IIO_DSA_STRUCT;

/* MSIXCAPLST_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x10808080)                                                      */
/*      SPRB0 (0x10808080)                                                      */
/*      SPRHBM (0x10808080)                                                     */
/*      SPRC0 (0x10808080)                                                      */
/*      SPRMCC (0x10808080)                                                     */
/*      SPRUCC (0x10808080)                                                     */
/* Register default value on SPRA0: 0x00009011                                  */
/* Register default value on SPRB0: 0x00009011                                  */
/* Register default value on SPRHBM: 0x00009011                                 */
/* Register default value on SPRC0: 0x00009011                                  */
/* Register default value on SPRMCC: 0x00009011                                 */
/* Register default value on SPRUCC: 0x00009011                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Enumerates the MSI-X Capability structure in the PCI Configuration Space Capability list.
*/


#define MSIXCAPLST_IIO_DSA_REG 0x21010080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 capid : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000011*/

                            /* Indicates the MSI-X Capability structure. */
    UINT16 nxtcap : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000090*/

                            /*
                               Pointer to next capability (Power Management, in
                               this case)
                            */

  } Bits;
  UINT16 Data;

} MSIXCAPLST_IIO_DSA_STRUCT;

/* MSIXMSGCTL_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x10808082)                                                      */
/*      SPRB0 (0x10808082)                                                      */
/*      SPRHBM (0x10808082)                                                     */
/*      SPRC0 (0x10808082)                                                      */
/*      SPRMCC (0x10808082)                                                     */
/*      SPRUCC (0x10808082)                                                     */
/* Register default value on SPRA0: 0x00000008                                  */
/* Register default value on SPRB0: 0x00000008                                  */
/* Register default value on SPRHBM: 0x00000008                                 */
/* Register default value on SPRC0: 0x00000008                                  */
/* Register default value on SPRMCC: 0x00000008                                 */
/* Register default value on SPRUCC: 0x00000008                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSI-X controls. System SW can modify bits in this register. A device driver is not permitted to modify this register
*/


#define MSIXMSGCTL_IIO_DSA_REG 0x21010082

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 tblsz : 11;

                            /* Bits[10:0], Access Type=RO, default=0x00000008*/

                            /* MSI-X Table Size. Encoded as N-1 (N = 9 entries) */
    UINT16 rsvd : 3;

                            /* Bits[13:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 fcnmsk : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               If set, all vectors associated with the Function
                               are masked
                            */
    UINT16 msixen : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               If set, the Function is permitted to send MSI-X
                               messages.
                            */

  } Bits;
  UINT16 Data;

} MSIXMSGCTL_IIO_DSA_STRUCT;

/* MSIXTBL_IIO_DSA_REG supported on:                                            */
/*      SPRA0 (0x20808084)                                                      */
/*      SPRB0 (0x20808084)                                                      */
/*      SPRHBM (0x20808084)                                                     */
/*      SPRC0 (0x20808084)                                                      */
/*      SPRMCC (0x20808084)                                                     */
/*      SPRUCC (0x20808084)                                                     */
/* Register default value on SPRA0: 0x00002000                                  */
/* Register default value on SPRB0: 0x00002000                                  */
/* Register default value on SPRHBM: 0x00002000                                 */
/* Register default value on SPRC0: 0x00002000                                  */
/* Register default value on SPRMCC: 0x00002000                                 */
/* Register default value on SPRUCC: 0x00002000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSI-X Table Offset and Table BIR
*/


#define MSIXTBL_IIO_DSA_REG 0x21020084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bir : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the BAR used to map the MSI-X Table
                               into Memory Space. BAR 0 at 10h
                            */
    UINT32 offset : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00000400*/

                            /*
                               MSI-X Table Offset within BAR indicated by BIR.
                               Entire register is used, masking BIR to form a
                               32-bit QWORD-aligned offset.
                            */

  } Bits;
  UINT32 Data;

} MSIXTBL_IIO_DSA_STRUCT;

/* MSIXPBA_IIO_DSA_REG supported on:                                            */
/*      SPRA0 (0x20808088)                                                      */
/*      SPRB0 (0x20808088)                                                      */
/*      SPRHBM (0x20808088)                                                     */
/*      SPRC0 (0x20808088)                                                      */
/*      SPRMCC (0x20808088)                                                     */
/*      SPRUCC (0x20808088)                                                     */
/* Register default value on SPRA0: 0x00003000                                  */
/* Register default value on SPRB0: 0x00003000                                  */
/* Register default value on SPRHBM: 0x00003000                                 */
/* Register default value on SPRC0: 0x00003000                                  */
/* Register default value on SPRMCC: 0x00003000                                 */
/* Register default value on SPRUCC: 0x00003000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSI-X PBA Offset and PBA BIR
*/


#define MSIXPBA_IIO_DSA_REG 0x21020088

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bir : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the BAR used to map the MSI-X PBA into
                               Memory Space. BAR 0 at 10h
                            */
    UINT32 offset : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00000600*/

                            /*
                               MSI-X PBA Offset within BAR indicated by BIR.
                               Entire register is used, masking BIR to form a
                               32-bit QWORD-aligned offset.
                            */

  } Bits;
  UINT32 Data;

} MSIXPBA_IIO_DSA_STRUCT;

/* PMCAP_IIO_DSA_REG supported on:                                              */
/*      SPRA0 (0x20808090)                                                      */
/*      SPRB0 (0x20808090)                                                      */
/*      SPRHBM (0x20808090)                                                     */
/*      SPRC0 (0x20808090)                                                      */
/*      SPRMCC (0x20808090)                                                     */
/*      SPRUCC (0x20808090)                                                     */
/* Register default value on SPRA0: 0x00030001                                  */
/* Register default value on SPRB0: 0x00030001                                  */
/* Register default value on SPRHBM: 0x00030001                                 */
/* Register default value on SPRC0: 0x00030001                                  */
/* Register default value on SPRMCC: 0x00030001                                 */
/* Register default value on SPRUCC: 0x00030001                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Power Management Capability
*/


#define PMCAP_IIO_DSA_REG 0x21020090

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 capid : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000001*/

                            /* Indicates PCI Power Management Capability */
    UINT32 nxtcap : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               Pointer to next capability (end of list, in this
                               case)
                            */
    UINT32 ver : 3;

                            /* Bits[18:16], Access Type=RO, default=0x00000003*/

                            /* Must be hardwired to 011b per PCIe spec */
    UINT32 rsvd : 6;

                            /* Bits[24:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 d1 : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /*
                               This Function does not support the D1 Power
                               Management State
                            */
    UINT32 d2 : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /*
                               This Function does not support the D2 Power
                               Management State
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMCAP_IIO_DSA_STRUCT;

/* PMCSR_IIO_DSA_REG supported on:                                              */
/*      SPRA0 (0x20808094)                                                      */
/*      SPRB0 (0x20808094)                                                      */
/*      SPRHBM (0x20808094)                                                     */
/*      SPRC0 (0x20808094)                                                      */
/*      SPRMCC (0x20808094)                                                     */
/*      SPRUCC (0x20808094)                                                     */
/* Register default value on SPRA0: 0x00000008                                  */
/* Register default value on SPRB0: 0x00000008                                  */
/* Register default value on SPRHBM: 0x00000008                                 */
/* Register default value on SPRC0: 0x00000008                                  */
/* Register default value on SPRMCC: 0x00000008                                 */
/* Register default value on SPRUCC: 0x00000008                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register is used to manage the PCI Function's power management state
*/


#define PMCSR_IIO_DSA_REG 0x21020094

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ps : 2;

                            /* Bits[1:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               This field is used both to determine the current
                               power state of a Function and to set the
                               Function into a new power state. If an
                               unsupported, optional state value is written,
                               the data is discarded and no state change
                               occurs. 00b - D0, 01b - D1 (unsupported, 10b -
                               D2 (unsupported), 11b - D3 (hot)
                            */
    UINT32 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 nsr : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               This bit indicates the state of the Function
                               after writing the Power State field to
                               transition the Function from D3(hot) to D0
                            */
    UINT32 rsvd_4 : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMCSR_IIO_DSA_STRUCT;

/* AEREXTCAP_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808100)                                                      */
/*      SPRB0 (0x20808100)                                                      */
/*      SPRHBM (0x20808100)                                                     */
/*      SPRC0 (0x20808100)                                                      */
/*      SPRMCC (0x20808100)                                                     */
/*      SPRUCC (0x20808100)                                                     */
/* Register default value on SPRA0: 0x15020001                                  */
/* Register default value on SPRB0: 0x15020001                                  */
/* Register default value on SPRHBM: 0x15020001                                 */
/* Register default value on SPRC0: 0x15020001                                  */
/* Register default value on SPRMCC: 0x15020001                                 */
/* Register default value on SPRUCC: 0x15020001                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header
*/


#define AEREXTCAP_IIO_DSA_REG 0x21020100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extcapid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000001*/

                            /*
                               PCI-SIG defined ID number indicating the nature
                               and format of the Extended Capability
                            */
    UINT32 capver : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000002*/

                            /*
                               PCI-SIG defined version number indicating the
                               version of the Capability structure
                            */
    UINT32 nxtcap : 12;

                            /* Bits[31:20], Access Type=RO/V, default=0x00000150*/

                            /*
                               Offset to the next PCI Express Capability
                               structure
                            */

  } Bits;
  UINT32 Data;

} AEREXTCAP_IIO_DSA_STRUCT;

/* ERRUNCSTS_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808104)                                                      */
/*      SPRB0 (0x20808104)                                                      */
/*      SPRHBM (0x20808104)                                                     */
/*      SPRC0 (0x20808104)                                                      */
/*      SPRMCC (0x20808104)                                                     */
/*      SPRUCC (0x20808104)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Indicates error detection status of individual errors on a PCI Express device Function
*/


#define ERRUNCSTS_IIO_DSA_REG 0x21020104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 12;

                            /* Bits[11:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ptlp : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Set when a TLP received by this function is
                               marked as poisoned
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cto : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Set when a Non-posted requested by this function
                               is terminated via the Completion Timeout
                               mechanism
                            */
    UINT32 ca : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Set when this function sends a completion with
                               Completer Abort status
                            */
    UINT32 uc : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Set when this function receives a completion
                               that does not correspond to a Non-posted it
                               issued
                            */
    UINT32 rsvd_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mtlp : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Set when this function receives a Malformed TLP
                               (MPS violation)
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ur : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Set when this function sends a completion with
                               Unsupported Request status
                            */
    UINT32 rsvd_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ui : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Set when an Uncorrectable Internal error is
                               detected
                            */
    UINT32 rsvd_23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERRUNCSTS_IIO_DSA_STRUCT;

/* ERRUNCMSK_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808108)                                                      */
/*      SPRB0 (0x20808108)                                                      */
/*      SPRHBM (0x20808108)                                                     */
/*      SPRC0 (0x20808108)                                                      */
/*      SPRMCC (0x20808108)                                                     */
/*      SPRUCC (0x20808108)                                                     */
/* Register default value on SPRA0: 0x00400000                                  */
/* Register default value on SPRB0: 0x00400000                                  */
/* Register default value on SPRHBM: 0x00400000                                 */
/* Register default value on SPRC0: 0x00400000                                  */
/* Register default value on SPRMCC: 0x00400000                                 */
/* Register default value on SPRUCC: 0x00400000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls reporting of individual errors. A masked error is not recoded or reported in the Header Log or First Error Pointer and is not reported to the PCI Express Root Complex.
*/


#define ERRUNCMSK_IIO_DSA_REG 0x21020108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 12;

                            /* Bits[11:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ptlp : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               When Set, prevents the logging and reporting of
                               Poisoned TLP errors
                            */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cto : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               When Set, prevents the logging and reporting of
                               Completion Timeout errors
                            */
    UINT32 ca : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               When Set, prevents the logging and reporting of
                               Completer Abort errors
                            */
    UINT32 uc : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               When Set, prevents the logging and reporting of
                               Unexpected Completion errors
                            */
    UINT32 rsvd_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mtlp : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               When Set, prevents the logging and reporting of
                               Malformed TLP errors
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ur : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               When Set, prevents the logging and reporting of
                               Unsupported Request errors
                            */
    UINT32 rsvd_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ui : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /*
                               When Set, prevents the logging and reporting of
                               Uncorrectable Internal errors
                            */
    UINT32 rsvd_23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERRUNCMSK_IIO_DSA_STRUCT;

/* ERRUNCSEV_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x2080810c)                                                      */
/*      SPRB0 (0x2080810c)                                                      */
/*      SPRHBM (0x2080810c)                                                     */
/*      SPRC0 (0x2080810c)                                                      */
/*      SPRMCC (0x2080810c)                                                     */
/*      SPRUCC (0x2080810c)                                                     */
/* Register default value on SPRA0: 0x00440000                                  */
/* Register default value on SPRB0: 0x00440000                                  */
/* Register default value on SPRHBM: 0x00440000                                 */
/* Register default value on SPRC0: 0x00440000                                  */
/* Register default value on SPRMCC: 0x00440000                                 */
/* Register default value on SPRUCC: 0x00440000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls whether an individual error is reported as a Non-fatal (bit is Clear) or Fatal (bit is Set) error
*/


#define ERRUNCSEV_IIO_DSA_REG 0x2102010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 12;

                            /* Bits[11:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ptlp : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* When Set, Poisoned TLP errors are Fatal */
    UINT32 rsvd_13 : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cto : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* When Set, Completion Timeout errors are Fatal */
    UINT32 ca : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* When Set, Completer Abort errors are Fatal */
    UINT32 uc : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* When Set, Unexpected Completion errors are Fatal */
    UINT32 rsvd_17 : 1;

                            /* Bits[17:17], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mtlp : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000001*/

                            /* When Set, Malformed TLP errors are Fatal */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ur : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* When Set, Unsupported Request errors are Fatal */
    UINT32 rsvd_21 : 1;

                            /* Bits[21:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ui : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /*
                               When Set, Uncorrectable Internal errors are
                               Fatal
                            */
    UINT32 rsvd_23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERRUNCSEV_IIO_DSA_STRUCT;

/* ERRCORSTS_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808110)                                                      */
/*      SPRB0 (0x20808110)                                                      */
/*      SPRHBM (0x20808110)                                                     */
/*      SPRC0 (0x20808110)                                                      */
/*      SPRMCC (0x20808110)                                                     */
/*      SPRUCC (0x20808110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reports error status of individual correctable error sources
*/


#define ERRCORSTS_IIO_DSA_REG 0x21020110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 13;

                            /* Bits[12:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 anf : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Set when an Error classified as Advisory Non-
                               fatal is detected
                            */
    UINT32 ci : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Set when a Corrected Internal error is detected */
    UINT32 rsvd_15 : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERRCORSTS_IIO_DSA_STRUCT;

/* ERRCORMSK_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808114)                                                      */
/*      SPRB0 (0x20808114)                                                      */
/*      SPRHBM (0x20808114)                                                     */
/*      SPRC0 (0x20808114)                                                      */
/*      SPRMCC (0x20808114)                                                     */
/*      SPRUCC (0x20808114)                                                     */
/* Register default value on SPRA0: 0x00006000                                  */
/* Register default value on SPRB0: 0x00006000                                  */
/* Register default value on SPRHBM: 0x00006000                                 */
/* Register default value on SPRC0: 0x00006000                                  */
/* Register default value on SPRMCC: 0x00006000                                 */
/* Register default value on SPRUCC: 0x00006000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls the reporting of individual correctable errors
*/


#define ERRCORMSK_IIO_DSA_REG 0x21020114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 13;

                            /* Bits[12:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 anf : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /*
                               When Set, Advisory Non-Fatal errors are not
                               reported
                            */
    UINT32 ci : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /*
                               When Set, Corrected Internal errors are not
                               reported
                            */
    UINT32 rsvd_15 : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERRCORMSK_IIO_DSA_STRUCT;

/* AERCAPCTL_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808118)                                                      */
/*      SPRB0 (0x20808118)                                                      */
/*      SPRHBM (0x20808118)                                                     */
/*      SPRC0 (0x20808118)                                                      */
/*      SPRMCC (0x20808118)                                                     */
/*      SPRUCC (0x20808118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* More AER information
*/


#define AERCAPCTL_IIO_DSA_REG 0x21020118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fep : 5;

                            /* Bits[4:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Identifies the bit position of the first error
                               reported in the Uncorrectable Error Status
                               register
                            */
    UINT32 rsvd : 6;

                            /* Bits[10:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tlpplp : 1;

                            /* Bits[11:11], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               If Set and the First Error Pointer is valid,
                               indicates that the TLP Prefix Log register
                               contains valid information.
                            */
    UINT32 rsvd_12 : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} AERCAPCTL_IIO_DSA_STRUCT;

/* AERHDRLOG1_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x2080811c)                                                      */
/*      SPRB0 (0x2080811c)                                                      */
/*      SPRHBM (0x2080811c)                                                     */
/*      SPRC0 (0x2080811c)                                                      */
/*      SPRMCC (0x2080811c)                                                     */
/*      SPRUCC (0x2080811c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* First DWORD of the header for the TLP corresponding to a detected error.
*/


#define AERHDRLOG1_IIO_DSA_REG 0x2102011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdrlog : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header Log DW */

  } Bits;
  UINT32 Data;

} AERHDRLOG1_IIO_DSA_STRUCT;

/* AERHDRLOG2_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x20808120)                                                      */
/*      SPRB0 (0x20808120)                                                      */
/*      SPRHBM (0x20808120)                                                     */
/*      SPRC0 (0x20808120)                                                      */
/*      SPRMCC (0x20808120)                                                     */
/*      SPRUCC (0x20808120)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Second DWORD of the header for the TLP corresponding to a detected error.
*/


#define AERHDRLOG2_IIO_DSA_REG 0x21020120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdrlog : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header Log DW */

  } Bits;
  UINT32 Data;

} AERHDRLOG2_IIO_DSA_STRUCT;

/* AERHDRLOG3_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x20808124)                                                      */
/*      SPRB0 (0x20808124)                                                      */
/*      SPRHBM (0x20808124)                                                     */
/*      SPRC0 (0x20808124)                                                      */
/*      SPRMCC (0x20808124)                                                     */
/*      SPRUCC (0x20808124)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Third DWORD of the header for the TLP corresponding to a detected error.
*/


#define AERHDRLOG3_IIO_DSA_REG 0x21020124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdrlog : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header Log DW */

  } Bits;
  UINT32 Data;

} AERHDRLOG3_IIO_DSA_STRUCT;

/* AERHDRLOG4_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x20808128)                                                      */
/*      SPRB0 (0x20808128)                                                      */
/*      SPRHBM (0x20808128)                                                     */
/*      SPRC0 (0x20808128)                                                      */
/*      SPRMCC (0x20808128)                                                     */
/*      SPRUCC (0x20808128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Fourth DWORD of the header for the TLP corresponding to a detected error.
*/


#define AERHDRLOG4_IIO_DSA_REG 0x21020128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdrlog : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header Log DW */

  } Bits;
  UINT32 Data;

} AERHDRLOG4_IIO_DSA_STRUCT;

/* AERTLPPLOG1_IIO_DSA_REG supported on:                                        */
/*      SPRA0 (0x20808138)                                                      */
/*      SPRB0 (0x20808138)                                                      */
/*      SPRHBM (0x20808138)                                                     */
/*      SPRC0 (0x20808138)                                                      */
/*      SPRMCC (0x20808138)                                                     */
/*      SPRUCC (0x20808138)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register captures the End-End TLP Prefix (DW1) for the TLP corresponding to the detected error.
*/


#define AERTLPPLOG1_IIO_DSA_REG 0x21020138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlpplog : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* TLP Prefix Log DW */

  } Bits;
  UINT32 Data;

} AERTLPPLOG1_IIO_DSA_STRUCT;

/* AERTLPPLOG2_IIO_DSA_REG supported on:                                        */
/*      SPRA0 (0x2080813c)                                                      */
/*      SPRB0 (0x2080813c)                                                      */
/*      SPRHBM (0x2080813c)                                                     */
/*      SPRC0 (0x2080813c)                                                      */
/*      SPRMCC (0x2080813c)                                                     */
/*      SPRUCC (0x2080813c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register captures the End-End TLP Prefix (DW2) for the TLP corresponding to the detected error.
*/


#define AERTLPPLOG2_IIO_DSA_REG 0x2102013C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlpplog : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               TLP Prefix Log DW. [br]This is never used by
                               this IP.
                            */

  } Bits;
  UINT32 Data;

} AERTLPPLOG2_IIO_DSA_STRUCT;

/* AERTLPPLOG3_IIO_DSA_REG supported on:                                        */
/*      SPRA0 (0x20808140)                                                      */
/*      SPRB0 (0x20808140)                                                      */
/*      SPRHBM (0x20808140)                                                     */
/*      SPRC0 (0x20808140)                                                      */
/*      SPRMCC (0x20808140)                                                     */
/*      SPRUCC (0x20808140)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register captures the End-End TLP Prefix (DW3) for the TLP corresponding to the detected error.
*/


#define AERTLPPLOG3_IIO_DSA_REG 0x21020140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlpplog : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               TLP Prefix Log DW. [br]This is never used by
                               this IP.
                            */

  } Bits;
  UINT32 Data;

} AERTLPPLOG3_IIO_DSA_STRUCT;

/* AERTLPPLOG4_IIO_DSA_REG supported on:                                        */
/*      SPRA0 (0x20808144)                                                      */
/*      SPRB0 (0x20808144)                                                      */
/*      SPRHBM (0x20808144)                                                     */
/*      SPRC0 (0x20808144)                                                      */
/*      SPRMCC (0x20808144)                                                     */
/*      SPRUCC (0x20808144)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register captures the End-End TLP Prefix (DW4) for the TLP corresponding to the detected error.
*/


#define AERTLPPLOG4_IIO_DSA_REG 0x21020144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlpplog : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /*
                               TLP Prefix Log DW. [br]This is never used by
                               this IP.
                            */

  } Bits;
  UINT32 Data;

} AERTLPPLOG4_IIO_DSA_STRUCT;

/* LTREXTCAP_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808150)                                                      */
/*      SPRB0 (0x20808150)                                                      */
/*      SPRHBM (0x20808150)                                                     */
/*      SPRC0 (0x20808150)                                                      */
/*      SPRMCC (0x20808150)                                                     */
/*      SPRUCC (0x20808150)                                                     */
/* Register default value on SPRA0: 0x16010018                                  */
/* Register default value on SPRB0: 0x16010018                                  */
/* Register default value on SPRHBM: 0x16010018                                 */
/* Register default value on SPRC0: 0x16010018                                  */
/* Register default value on SPRMCC: 0x16010018                                 */
/* Register default value on SPRUCC: 0x16010018                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header
*/


#define LTREXTCAP_IIO_DSA_REG 0x21020150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extcapid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000018*/

                            /*
                               PCI-SIG defined ID number indicating the nature
                               and format of the Extended Capability
                            */
    UINT32 capver : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               PCI-SIG defined version number indicating the
                               version of the Capability structure
                            */
    UINT32 nxtcap : 12;

                            /* Bits[31:20], Access Type=RO/V, default=0x00000160*/

                            /*
                               Offset to the next PCI Express Capability
                               structure
                            */

  } Bits;
  UINT32 Data;

} LTREXTCAP_IIO_DSA_STRUCT;

/* MAXSNPLAT_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x10808154)                                                      */
/*      SPRB0 (0x10808154)                                                      */
/*      SPRHBM (0x10808154)                                                     */
/*      SPRC0 (0x10808154)                                                      */
/*      SPRMCC (0x10808154)                                                     */
/*      SPRUCC (0x10808154)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Maximum Snoop Latency the function is permitted to request
*/


#define MAXSNPLAT_IIO_DSA_REG 0x21010154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 value : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /* Value sent in LTR message */
    UINT16 scale : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000000*/

                            /*
                               Scale of value sent in LTR message (scale = 2 ^
                               (5 * N) ns)
                            */
    UINT16 rsvd : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} MAXSNPLAT_IIO_DSA_STRUCT;

/* MAXNSNPLAT_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x10808156)                                                      */
/*      SPRB0 (0x10808156)                                                      */
/*      SPRHBM (0x10808156)                                                     */
/*      SPRC0 (0x10808156)                                                      */
/*      SPRMCC (0x10808156)                                                     */
/*      SPRUCC (0x10808156)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Maximum No-Snoop Latency the function is permitted to request
*/


#define MAXNSNPLAT_IIO_DSA_REG 0x21010156

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 value : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /* Value sent in LTR message */
    UINT16 scale : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000000*/

                            /*
                               Scale of value sent in LTR message (scale = 2 ^
                               (5 * N) ns)
                            */
    UINT16 rsvd : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} MAXNSNPLAT_IIO_DSA_STRUCT;

/* TPHEXTCAP_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808160)                                                      */
/*      SPRB0 (0x20808160)                                                      */
/*      SPRHBM (0x20808160)                                                     */
/*      SPRC0 (0x20808160)                                                      */
/*      SPRMCC (0x20808160)                                                     */
/*      SPRUCC (0x20808160)                                                     */
/* Register default value on SPRA0: 0x17010017                                  */
/* Register default value on SPRB0: 0x17010017                                  */
/* Register default value on SPRHBM: 0x17010017                                 */
/* Register default value on SPRC0: 0x17010017                                  */
/* Register default value on SPRMCC: 0x17010017                                 */
/* Register default value on SPRUCC: 0x17010017                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header
*/


#define TPHEXTCAP_IIO_DSA_REG 0x21020160

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extcapid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000017*/

                            /*
                               PCI-SIG defined ID number indicating the nature
                               and format of the Extended Capability
                            */
    UINT32 capver : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               PCI-SIG defined version number indicating the
                               version of the Capability structure
                            */
    UINT32 nxtcap : 12;

                            /* Bits[31:20], Access Type=RO/V, default=0x00000170*/

                            /*
                               Offset to the next PCI Express Capability
                               structure
                            */

  } Bits;
  UINT32 Data;

} TPHEXTCAP_IIO_DSA_STRUCT;

/* TPHCAP_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x20808164)                                                      */
/*      SPRB0 (0x20808164)                                                      */
/*      SPRHBM (0x20808164)                                                     */
/*      SPRC0 (0x20808164)                                                      */
/*      SPRMCC (0x20808164)                                                     */
/*      SPRUCC (0x20808164)                                                     */
/* Register default value on SPRA0: 0x00010205                                  */
/* Register default value on SPRB0: 0x00010205                                  */
/* Register default value on SPRHBM: 0x00010205                                 */
/* Register default value on SPRC0: 0x00010205                                  */
/* Register default value on SPRMCC: 0x00010205                                 */
/* Register default value on SPRUCC: 0x00010205                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* TPH Capabilities (7.9.13.2)
*/


#define TPHCAP_IIO_DSA_REG 0x21020164

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 nostsupp : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /*
                               If set, indicates that the Function supports the
                               No ST Mode of oepration.
                            */
    UINT32 intvecsupp : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               If set, indicates that the Function supports the
                               Interrupt Vector Modes of operation.
                            */
    UINT32 devspecsupp : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000001*/

                            /*
                               If set, indicates that the Function supports the
                               Device Specific Mode of operation.
                            */
    UINT32 rsvd : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 exttphsupp : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               If set, indicates that the Function is capable
                               of generating Requests with a TPH TLP Prefix.
                            */
    UINT32 sttblloc : 2;

                            /* Bits[10:9], Access Type=RO, default=0x00000001*/

                            /*
                               Value indicates if and where the ST Table is
                               located.
                            */
    UINT32 rsvd_11 : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sttblsize : 11;

                            /* Bits[26:16], Access Type=RO, default=0x00000001*/

                            /*
                               Value indicates the maximum number of ST Table
                               entries the Function may use. Software reads
                               this field to determine the ST Table Size N,
                               which is encoded as N-1.
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TPHCAP_IIO_DSA_STRUCT;

/* TPHCTL_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x20808168)                                                      */
/*      SPRB0 (0x20808168)                                                      */
/*      SPRHBM (0x20808168)                                                     */
/*      SPRC0 (0x20808168)                                                      */
/*      SPRMCC (0x20808168)                                                     */
/*      SPRUCC (0x20808168)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* TPH Requester Capabilities (7.9.13.3)
*/


#define TPHCTL_IIO_DSA_REG 0x21020168

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 stmodesel : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Selects the ST Mode of operation */
    UINT32 rsvd : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tphreqen_8_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Controls the ability to issue Request TLPs using
                               TPH.
                            */
    UINT32 tphreqen_9_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Controls the ability to issue Request TLPs using
                               Extended TPH.
                            */
    UINT32 rsvd_10 : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TPHCTL_IIO_DSA_STRUCT;

/* TPHSTTBL0_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x1080816c)                                                      */
/*      SPRB0 (0x1080816c)                                                      */
/*      SPRHBM (0x1080816c)                                                     */
/*      SPRC0 (0x1080816c)                                                      */
/*      SPRMCC (0x1080816c)                                                     */
/*      SPRUCC (0x1080816c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* TPH ST Table (7.9.13.3)
*/


#define TPHSTTBL0_IIO_DSA_REG 0x2101016C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 stle : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               This field contains the lower 8 bits of a
                               Steering Tag.
                            */
    UINT16 stue : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               If the Function's Extended TPH Requester
                               Supported bit is Set, then this field contains
                               the upper 8 bits of a Steering Tag.
                            */

  } Bits;
  UINT16 Data;

} TPHSTTBL0_IIO_DSA_STRUCT;

/* TPHSTTBL1_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x1080816e)                                                      */
/*      SPRB0 (0x1080816e)                                                      */
/*      SPRHBM (0x1080816e)                                                     */
/*      SPRC0 (0x1080816e)                                                      */
/*      SPRMCC (0x1080816e)                                                     */
/*      SPRUCC (0x1080816e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* TPH ST Table (7.9.13.3)
*/


#define TPHSTTBL1_IIO_DSA_REG 0x2101016E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 stle : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               This field contains the lower 8 bits of a
                               Steering Tag.
                            */
    UINT16 stue : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               If the Function's Extended TPH Requester
                               Supported bit is Set, then this field contains
                               the upper 8 bits of a Steering Tag.
                            */

  } Bits;
  UINT16 Data;

} TPHSTTBL1_IIO_DSA_STRUCT;

/* VCEXTCAP_IIO_DSA_REG supported on:                                           */
/*      SPRA0 (0x20808170)                                                      */
/*      SPRB0 (0x20808170)                                                      */
/*      SPRHBM (0x20808170)                                                     */
/*      SPRC0 (0x20808170)                                                      */
/*      SPRMCC (0x20808170)                                                     */
/*      SPRUCC (0x20808170)                                                     */
/* Register default value on SPRA0: 0x20010002                                  */
/* Register default value on SPRB0: 0x20010002                                  */
/* Register default value on SPRHBM: 0x20010002                                 */
/* Register default value on SPRC0: 0x20010002                                  */
/* Register default value on SPRMCC: 0x20010002                                 */
/* Register default value on SPRUCC: 0x20010002                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Virtual Channel Extended Capability Header (Section 7.9.1.1)
*/


#define VCEXTCAP_IIO_DSA_REG 0x21020170

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extcapid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000002*/

                            /*
                               PCI-SIG defined ID number indicating the nature
                               and format of the Extended Capability
                            */
    UINT32 capver : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               PCI-SIG defined version number indicating the
                               version of the Capability structure
                            */
    UINT32 nxtcap : 12;

                            /* Bits[31:20], Access Type=RO/V, default=0x00000200*/

                            /*
                               Offset to the next PCI Express Capability
                               structure
                            */

  } Bits;
  UINT32 Data;

} VCEXTCAP_IIO_DSA_STRUCT;

/* PORTVCCAP1_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x20808174)                                                      */
/*      SPRB0 (0x20808174)                                                      */
/*      SPRHBM (0x20808174)                                                     */
/*      SPRC0 (0x20808174)                                                      */
/*      SPRMCC (0x20808174)                                                     */
/*      SPRUCC (0x20808174)                                                     */
/* Register default value on SPRA0: 0x00000011                                  */
/* Register default value on SPRB0: 0x00000011                                  */
/* Register default value on SPRHBM: 0x00000011                                 */
/* Register default value on SPRC0: 0x00000011                                  */
/* Register default value on SPRMCC: 0x00000011                                 */
/* Register default value on SPRUCC: 0x00000011                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Port VC Capability Register 1 (Section 7.9.1.2)
*/


#define PORTVCCAP1_IIO_DSA_REG 0x21020174

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extvccnt : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000001*/

                            /*
                               Indicates the number of (extended) Virtual
                               Channels in addition to the default VC supported
                               by the device.
                            */
    UINT32 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lpextvccnt : 3;

                            /* Bits[6:4], Access Type=RO, default=0x00000001*/

                            /*
                               Indicates the number of (extended) Virtual
                               Channels in addition to the default VC belonging
                               to the low-priority VC (LPVC) group.
                            */
    UINT32 rsvd_7 : 1;

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 refclk : 2;

                            /* Bits[9:8], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the reference clock for Virtual
                               Channels that support time-based WRR Port
                               Arbitration. Does not apply to this Endpoint IP
                            */
    UINT32 pates : 2;

                            /* Bits[11:10], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the size of Port Arbitration table
                               entry in the Function. Does not apply to this
                               Endpoint IP
                            */
    UINT32 rsvd_12 : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PORTVCCAP1_IIO_DSA_STRUCT;

/* PORTVCCAP2_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x20808178)                                                      */
/*      SPRB0 (0x20808178)                                                      */
/*      SPRHBM (0x20808178)                                                     */
/*      SPRC0 (0x20808178)                                                      */
/*      SPRMCC (0x20808178)                                                     */
/*      SPRUCC (0x20808178)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Port VC Capability Register 2 (Section 7.9.1.3)
*/


#define PORTVCCAP2_IIO_DSA_REG 0x21020178

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vcarbcap : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000001*/

                            /*
                               Indicates the types of VC Arbitration supported
                               by the Function for the LPVC
                            */
    UINT32 rsvd : 16;

                            /* Bits[23:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vcarbto : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the location of the VC Arbitration
                               Table
                            */

  } Bits;
  UINT32 Data;

} PORTVCCAP2_IIO_DSA_STRUCT;

/* PORTVCCTL_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x1080817c)                                                      */
/*      SPRB0 (0x1080817c)                                                      */
/*      SPRHBM (0x1080817c)                                                     */
/*      SPRC0 (0x1080817c)                                                      */
/*      SPRMCC (0x1080817c)                                                     */
/*      SPRUCC (0x1080817c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Port VC Control Register (Section 7.9.1.4)
*/


#define PORTVCCTL_IIO_DSA_REG 0x2101017C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ldvcarbtbl : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Used by software to update the VC Arbitration
                               Table. Does not apply to this IP.
                            */
    UINT16 vcarbsel : 3;

                            /* Bits[3:1], Access Type=RW, default=None*/

                            /*
                               Used by software to configure the VC arbitration
                               by selecting one of the supported VC Arbitration
                               schemes indicated by the VC Arbitration schemes
                               indicated by the VC Arbitration Capability field
                               in the Port VC Capability register 2. [br]This
                               field loses its value if warm reset or stronger
                               but retains its value through FLR.
                            */
    UINT16 rsvd : 12;

                            /* Bits[15:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PORTVCCTL_IIO_DSA_STRUCT;

/* PORTVCSTS_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x1080817e)                                                      */
/*      SPRB0 (0x1080817e)                                                      */
/*      SPRHBM (0x1080817e)                                                     */
/*      SPRC0 (0x1080817e)                                                      */
/*      SPRMCC (0x1080817e)                                                     */
/*      SPRUCC (0x1080817e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Port VC Status Register (Section 7.9.1.5)
*/


#define PORTVCSTS_IIO_DSA_REG 0x2101017E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 vcarbtblsts : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the coherency status of the VC
                               Arbitration Table. Does not apply to this IP.
                            */
    UINT16 rsvd : 15;

                            /* Bits[15:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PORTVCSTS_IIO_DSA_STRUCT;

/* VC0CAP_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x20808180)                                                      */
/*      SPRB0 (0x20808180)                                                      */
/*      SPRHBM (0x20808180)                                                     */
/*      SPRC0 (0x20808180)                                                      */
/*      SPRMCC (0x20808180)                                                     */
/*      SPRUCC (0x20808180)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VC Resource Capability Register (Section 7.9.1.6)
*/


#define VC0CAP_IIO_DSA_REG 0x21020180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 portarbcap : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates types of Port Arbitration supported by
                               the VC resource. Does not apply to this Endpoint
                               IP.
                            */
    UINT32 rsvd : 6;

                            /* Bits[13:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 undef : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* The value read from this bit is undefined */
    UINT32 rejsnptxn : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               When Clear, transactions with or without the No
                               Snoop bit Set within the TLP header are allowed
                               on this VC. Does not apply to this Endpoint IP.
                            */
    UINT32 maxtimslt : 7;

                            /* Bits[22:16], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the maximum number of time slots
                               (minus one) that the VC resource is capable of
                               supporting when it is configured for time-based
                               WRR Port Arbitration. Does not apply to this
                               Endpoint IP.
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pato : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the location of the Port Arbitration
                               Table associated with the VC resource. Does not
                               apply to this Endpoint IP.
                            */

  } Bits;
  UINT32 Data;

} VC0CAP_IIO_DSA_STRUCT;

/* VC0CTL_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x20808184)                                                      */
/*      SPRB0 (0x20808184)                                                      */
/*      SPRHBM (0x20808184)                                                     */
/*      SPRC0 (0x20808184)                                                      */
/*      SPRMCC (0x20808184)                                                     */
/*      SPRUCC (0x20808184)                                                     */
/* Register default value on SPRA0: 0x80000001                                  */
/* Register default value on SPRB0: 0x80000001                                  */
/* Register default value on SPRHBM: 0x80000001                                 */
/* Register default value on SPRC0: 0x80000001                                  */
/* Register default value on SPRMCC: 0x80000001                                 */
/* Register default value on SPRUCC: 0x80000001                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VC Resource Control Register (Section 7.9.1.7)
*/


#define VC0CTL_IIO_DSA_REG 0x21020184

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tcvcmap_0_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /*
                               This field indicates the TCs that are mapped to
                               the VC resource.
                            */
    UINT32 tcvcmap_7_1 : 7;

                            /* Bits[7:1], Access Type=RW, default=None*/

                            /*
                               This field indicates the TCs that are mapped to
                               the VC resource. [br]This field loses its value
                               if warm reset or stronger but retains its value
                               through FLR.
                            */
    UINT32 rsvd : 8;

                            /* Bits[15:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ldportarbtbl : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               When Set, this bit updates the Port Arbitration
                               logic from the Port Arbitration Table for the VC
                               resource. Does not apply to this Endpoint IP.
                            */
    UINT32 portarbsel : 3;

                            /* Bits[19:17], Access Type=RO, default=0x00000000*/

                            /*
                               This field configures the VC resource to provide
                               a particular Port Arbitration service. Does not
                               apply to this Endpoint IP.
                            */
    UINT32 rsvd_20 : 4;

                            /* Bits[23:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vcid : 3;

                            /* Bits[26:24], Access Type=RO, default=0x00000000*/

                            /* This field assigns a VC ID to the VC resource */
    UINT32 rsvd_27 : 4;

                            /* Bits[30:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vcen : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000001*/

                            /* This bit, when Set, enables a Virtual Channel */

  } Bits;
  UINT32 Data;

} VC0CTL_IIO_DSA_STRUCT;

/* VC0STS_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x1080818a)                                                      */
/*      SPRB0 (0x1080818a)                                                      */
/*      SPRHBM (0x1080818a)                                                     */
/*      SPRC0 (0x1080818a)                                                      */
/*      SPRMCC (0x1080818a)                                                     */
/*      SPRUCC (0x1080818a)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VC Resource Status Register (Section 7.9.1.8)
*/


#define VC0STS_IIO_DSA_REG 0x2101018A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 portarbtblsts : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               This bit indicates the coherency status of the
                               Port Arbitration Table associated with the VC
                               resource. Does not apply to this Endpoint IP.
                            */
    UINT16 vcnegpend : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               This bit indicates whether the Virtual Channel
                               negotiation is in pending state. Does not apply
                               to this non-Link IP
                            */
    UINT16 rsvd : 14;

                            /* Bits[15:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} VC0STS_IIO_DSA_STRUCT;

/* VC1CAP_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x2080818c)                                                      */
/*      SPRB0 (0x2080818c)                                                      */
/*      SPRHBM (0x2080818c)                                                     */
/*      SPRC0 (0x2080818c)                                                      */
/*      SPRMCC (0x2080818c)                                                     */
/*      SPRUCC (0x2080818c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VC Resource Capability Register (Section 7.9.1.6)
*/


#define VC1CAP_IIO_DSA_REG 0x2102018C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 portarbcap : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates types of Port Arbitration supported by
                               the VC resource. Does not apply to this Endpoint
                               IP.
                            */
    UINT32 rsvd : 6;

                            /* Bits[13:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 undef : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* The value read from this bit is undefined */
    UINT32 rejsnptxn : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               When Clear, transactions with or without the No
                               Snoop bit Set within the TLP header are allowed
                               on this VC. Does not apply to this Endpoint IP.
                            */
    UINT32 maxtimslt : 7;

                            /* Bits[22:16], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the maximum number of time slots
                               (minus one) that the VC resource is capable of
                               supporting when it is configured for time-based
                               WRR Port Arbitration. Does not apply to this
                               Endpoint IP.
                            */
    UINT32 rsvd_23 : 1;

                            /* Bits[23:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 pato : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /*
                               Indicates the location of the Port Arbitration
                               Table associated with the VC resource. Does not
                               apply to this Endpoint IP.
                            */

  } Bits;
  UINT32 Data;

} VC1CAP_IIO_DSA_STRUCT;

/* VC1CTL_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x20808190)                                                      */
/*      SPRB0 (0x20808190)                                                      */
/*      SPRHBM (0x20808190)                                                     */
/*      SPRC0 (0x20808190)                                                      */
/*      SPRMCC (0x20808190)                                                     */
/*      SPRUCC (0x20808190)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VC Resource Control Register (Section 7.9.1.7)
*/


#define VC1CTL_IIO_DSA_REG 0x21020190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tcvcmap_0_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               This field indicates the TCs that are mapped to
                               the VC resource.
                            */
    UINT32 tcvcmap_7_1 : 7;

                            /* Bits[7:1], Access Type=RW, default=None*/

                            /*
                               This field indicates the TCs that are mapped to
                               the VC resource. [br]This field loses its value
                               if warm reset or stronger but retains its value
                               through FLR.
                            */
    UINT32 rsvd : 8;

                            /* Bits[15:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ldportarbtbl : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               When Set, this bit updates the Port Arbitration
                               logic from the Port Arbitration Table for the VC
                               resource. Does not apply to this Endpoint IP.
                            */
    UINT32 portarbsel : 3;

                            /* Bits[19:17], Access Type=RO, default=0x00000000*/

                            /*
                               This field configures the VC resource to provide
                               a particular Port Arbitration service. Does not
                               apply to this Endpoint IP.
                            */
    UINT32 rsvd_20 : 4;

                            /* Bits[23:20], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vcid : 3;

                            /* Bits[26:24], Access Type=RW, default=None*/

                            /*
                               This field assigns a VC ID to the VC resource.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 rsvd_27 : 4;

                            /* Bits[30:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vcen : 1;

                            /* Bits[31:31], Access Type=RW, default=None*/

                            /*
                               This bit, when Set, enables a Virtual Channel.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */

  } Bits;
  UINT32 Data;

} VC1CTL_IIO_DSA_STRUCT;

/* VC1STS_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x10808196)                                                      */
/*      SPRB0 (0x10808196)                                                      */
/*      SPRHBM (0x10808196)                                                     */
/*      SPRC0 (0x10808196)                                                      */
/*      SPRMCC (0x10808196)                                                     */
/*      SPRUCC (0x10808196)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VC Resource Status Register (Section 7.9.1.8)
*/


#define VC1STS_IIO_DSA_REG 0x21010196

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 portarbtblsts : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               This bit indicates the coherency status of the
                               Port Arbitration Table associated with the VC
                               resource. Does not apply to this Endpoint IP.
                            */
    UINT16 vcnegpend : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               This bit indicates whether the Virtual Channel
                               negotiation is in pending state. Does not apply
                               to this non-Link IP
                            */
    UINT16 rsvd : 14;

                            /* Bits[15:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} VC1STS_IIO_DSA_STRUCT;

/* SIOVEXTCAP_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x20808200)                                                      */
/*      SPRB0 (0x20808200)                                                      */
/*      SPRHBM (0x20808200)                                                     */
/*      SPRC0 (0x20808200)                                                      */
/*      SPRMCC (0x20808200)                                                     */
/*      SPRUCC (0x20808200)                                                     */
/* Register default value on SPRA0: 0x22010023                                  */
/* Register default value on SPRB0: 0x22010023                                  */
/* Register default value on SPRHBM: 0x22010023                                 */
/* Register default value on SPRC0: 0x22010023                                  */
/* Register default value on SPRMCC: 0x22010023                                 */
/* Register default value on SPRUCC: 0x22010023                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header (7.9.6.1)
*/


#define SIOVEXTCAP_IIO_DSA_REG 0x21020200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extcapid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000023*/

                            /*
                               PCI-SIG defined ID number indicating the nature
                               and format of the Extended Capability
                            */
    UINT32 capver : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               PCI-SIG defined version number indicating the
                               version of the Capability structure
                            */
    UINT32 nxtcap : 12;

                            /* Bits[31:20], Access Type=RO/V, default=0x00000220*/

                            /*
                               Offset to the next PCI Express Capability
                               structure
                            */

  } Bits;
  UINT32 Data;

} SIOVEXTCAP_IIO_DSA_STRUCT;

/* SIOVDVSEC1_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x20808204)                                                      */
/*      SPRB0 (0x20808204)                                                      */
/*      SPRHBM (0x20808204)                                                     */
/*      SPRC0 (0x20808204)                                                      */
/*      SPRMCC (0x20808204)                                                     */
/*      SPRUCC (0x20808204)                                                     */
/* Register default value on SPRA0: 0x01808086                                  */
/* Register default value on SPRB0: 0x01808086                                  */
/* Register default value on SPRHBM: 0x01808086                                 */
/* Register default value on SPRC0: 0x01808086                                  */
/* Register default value on SPRMCC: 0x01808086                                 */
/* Register default value on SPRUCC: 0x01808086                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Designated Vendor-Specific Header 1 (7.9.6.2)
*/


#define SIOVDVSEC1_IIO_DSA_REG 0x21020204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /*
                               This field is the Vendor ID associated with the
                               vendor that defined the contents of this
                               capability
                            */
    UINT32 rev : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000000*/

                            /*
                               This field is a vendor-defined version number
                               that indicates the version of the DVSEC
                               structure
                            */
    UINT32 length : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000018*/

                            /*
                               This field indicates the number of bytes in the
                               entire DVSEC structure, including the PCI
                               Express Extended Capability header, the DVSEC
                               Header 1, DVSEC Header 2, and DVSEC vendor-
                               specific registers
                            */

  } Bits;
  UINT32 Data;

} SIOVDVSEC1_IIO_DSA_STRUCT;

/* SIOVDVSEC2_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x10808208)                                                      */
/*      SPRB0 (0x10808208)                                                      */
/*      SPRHBM (0x10808208)                                                     */
/*      SPRC0 (0x10808208)                                                      */
/*      SPRMCC (0x10808208)                                                     */
/*      SPRUCC (0x10808208)                                                     */
/* Register default value on SPRA0: 0x00000005                                  */
/* Register default value on SPRB0: 0x00000005                                  */
/* Register default value on SPRHBM: 0x00000005                                 */
/* Register default value on SPRC0: 0x00000005                                  */
/* Register default value on SPRMCC: 0x00000005                                 */
/* Register default value on SPRUCC: 0x00000005                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Designated Vendor-Specific Header 2 (7.9.6.3)
*/


#define SIOVDVSEC2_IIO_DSA_REG 0x21010208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 dvsecid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000005*/

                            /*
                               This field is a vendor-defined ID that indicates
                               the nature and format of the DVSEC structure
                            */

  } Bits;
  UINT16 Data;

} SIOVDVSEC2_IIO_DSA_STRUCT;

/* SIOVFDL_IIO_DSA_REG supported on:                                            */
/*      SPRA0 (0x80820a)                                                        */
/*      SPRB0 (0x80820a)                                                        */
/*      SPRHBM (0x80820a)                                                       */
/*      SPRC0 (0x80820a)                                                        */
/*      SPRMCC (0x80820a)                                                       */
/*      SPRUCC (0x80820a)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* See Scalable IOV Arch Spec (Section 3.13)
*/


#define SIOVFDL_IIO_DSA_REG 0x2100020A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 fdl : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* See register reference */

  } Bits;
  UINT8 Data;

} SIOVFDL_IIO_DSA_STRUCT;

/* SIOVFLAGS_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x80820b)                                                        */
/*      SPRB0 (0x80820b)                                                        */
/*      SPRHBM (0x80820b)                                                       */
/*      SPRC0 (0x80820b)                                                        */
/*      SPRMCC (0x80820b)                                                       */
/*      SPRUCC (0x80820b)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* See Scalable IOV Arch Spec (Section 3.13)
*/


#define SIOVFLAGS_IIO_DSA_REG 0x2100020B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 h : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* See register reference */
    UINT8 rsvd : 7;

                            /* Bits[7:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT8 Data;

} SIOVFLAGS_IIO_DSA_STRUCT;

/* SIOVSUPPGSZ_IIO_DSA_REG supported on:                                        */
/*      SPRA0 (0x2080820c)                                                      */
/*      SPRB0 (0x2080820c)                                                      */
/*      SPRHBM (0x2080820c)                                                     */
/*      SPRC0 (0x2080820c)                                                      */
/*      SPRMCC (0x2080820c)                                                     */
/*      SPRUCC (0x2080820c)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* See Scalable IOV Arch Spec (Section 3.13)
*/


#define SIOVSUPPGSZ_IIO_DSA_REG 0x2102020C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bits : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000001*/

                            /* See register reference */

  } Bits;
  UINT32 Data;

} SIOVSUPPGSZ_IIO_DSA_STRUCT;

/* SIOVSYSPGSZ_IIO_DSA_REG supported on:                                        */
/*      SPRA0 (0x20808210)                                                      */
/*      SPRB0 (0x20808210)                                                      */
/*      SPRHBM (0x20808210)                                                     */
/*      SPRC0 (0x20808210)                                                      */
/*      SPRMCC (0x20808210)                                                     */
/*      SPRUCC (0x20808210)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* See Scalable IOV Arch Spec (Section 3.13)
*/


#define SIOVSYSPGSZ_IIO_DSA_REG 0x21020210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bits : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000001*/

                            /* See register reference */

  } Bits;
  UINT32 Data;

} SIOVSYSPGSZ_IIO_DSA_STRUCT;

/* SIOVCAP_IIO_DSA_REG supported on:                                            */
/*      SPRA0 (0x20808214)                                                      */
/*      SPRB0 (0x20808214)                                                      */
/*      SPRHBM (0x20808214)                                                     */
/*      SPRC0 (0x20808214)                                                      */
/*      SPRMCC (0x20808214)                                                     */
/*      SPRUCC (0x20808214)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* See Scalable IOV Arch Spec (Section 3.13)
*/


#define SIOVCAP_IIO_DSA_REG 0x21020214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 imss : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000001*/

                            /* See register reference */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SIOVCAP_IIO_DSA_STRUCT;

/* ATSEXTCAP_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808220)                                                      */
/*      SPRB0 (0x20808220)                                                      */
/*      SPRHBM (0x20808220)                                                     */
/*      SPRC0 (0x20808220)                                                      */
/*      SPRMCC (0x20808220)                                                     */
/*      SPRUCC (0x20808220)                                                     */
/* Register default value on SPRA0: 0x2301000F                                  */
/* Register default value on SPRB0: 0x2301000F                                  */
/* Register default value on SPRHBM: 0x2301000F                                 */
/* Register default value on SPRC0: 0x2301000F                                  */
/* Register default value on SPRMCC: 0x2301000F                                 */
/* Register default value on SPRUCC: 0x2301000F                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header (10.5.1.1)
*/


#define ATSEXTCAP_IIO_DSA_REG 0x21020220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extcapid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000000F*/

                            /*
                               PCI-SIG defined ID number indicating the nature
                               and format of the Extended Capability
                            */
    UINT32 capver : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               PCI-SIG defined version number indicating the
                               version of the Capability structure
                            */
    UINT32 nxtcap : 12;

                            /* Bits[31:20], Access Type=RO/V, default=0x00000230*/

                            /*
                               Offset to the next PCI Express Capability
                               structure
                            */

  } Bits;
  UINT32 Data;

} ATSEXTCAP_IIO_DSA_STRUCT;

/* ATSCAP_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x10808224)                                                      */
/*      SPRB0 (0x10808224)                                                      */
/*      SPRHBM (0x10808224)                                                     */
/*      SPRC0 (0x10808224)                                                      */
/*      SPRMCC (0x10808224)                                                     */
/*      SPRUCC (0x10808224)                                                     */
/* Register default value on SPRA0: 0x00000060                                  */
/* Register default value on SPRB0: 0x00000060                                  */
/* Register default value on SPRHBM: 0x00000060                                 */
/* Register default value on SPRC0: 0x00000060                                  */
/* Register default value on SPRMCC: 0x00000060                                 */
/* Register default value on SPRUCC: 0x00000060                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ATS Capabilities (10.5.1.2)
*/


#define ATSCAP_IIO_DSA_REG 0x21010224

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 iqd : 5;

                            /* Bits[4:0], Access Type=RO, default=0x00000000*/

                            /*
                               Number of Invalidate Requests the Function can
                               accept before back pressuring (00000b = 32)
                            */
    UINT16 par : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               When Set, indicates the Untranslated Address is
                               always aligned to a 4096 byte boundary
                            */
    UINT16 gis : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000001*/

                            /*
                               If Set, the Function supports Invalidation
                               Requests that have the Global Invalidate bit Set
                            */
    UINT16 rsvd : 9;

                            /* Bits[15:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} ATSCAP_IIO_DSA_STRUCT;

/* ATSCTL_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x10808226)                                                      */
/*      SPRB0 (0x10808226)                                                      */
/*      SPRHBM (0x10808226)                                                     */
/*      SPRC0 (0x10808226)                                                      */
/*      SPRMCC (0x10808226)                                                     */
/*      SPRUCC (0x10808226)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ATS Controls (10.5.1.3)
*/


#define ATSCTL_IIO_DSA_REG 0x21010226

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 stu : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Minimum number of 4096-byte blocks that are
                               indicated in a Translation Completion or
                               Invalidate Request. Number of blocks = 2 ^ STU
                            */
    UINT16 rsvd : 10;

                            /* Bits[14:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When Set, function is enable to cache
                               translations. [br]See the RAS Chapter in the HAS
                               for programming restrictions.
                            */

  } Bits;
  UINT16 Data;

} ATSCTL_IIO_DSA_STRUCT;

/* PASIDEXTCAP_IIO_DSA_REG supported on:                                        */
/*      SPRA0 (0x20808230)                                                      */
/*      SPRB0 (0x20808230)                                                      */
/*      SPRHBM (0x20808230)                                                     */
/*      SPRC0 (0x20808230)                                                      */
/*      SPRMCC (0x20808230)                                                     */
/*      SPRUCC (0x20808230)                                                     */
/* Register default value on SPRA0: 0x2401001B                                  */
/* Register default value on SPRB0: 0x2401001B                                  */
/* Register default value on SPRHBM: 0x2401001B                                 */
/* Register default value on SPRC0: 0x2401001B                                  */
/* Register default value on SPRMCC: 0x2401001B                                 */
/* Register default value on SPRUCC: 0x2401001B                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header
*/


#define PASIDEXTCAP_IIO_DSA_REG 0x21020230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extcapid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000001B*/

                            /*
                               PCI-SIG defined ID number indicating the nature
                               and format of the Extended Capability
                            */
    UINT32 capver : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               PCI-SIG defined version number indicating the
                               version of the Capability structure
                            */
    UINT32 nxtcap : 12;

                            /* Bits[31:20], Access Type=RO/V, default=0x00000240*/

                            /*
                               Offset to the next PCI Express Capability
                               structure
                            */

  } Bits;
  UINT32 Data;

} PASIDEXTCAP_IIO_DSA_STRUCT;

/* PASIDCAP_IIO_DSA_REG supported on:                                           */
/*      SPRA0 (0x10808234)                                                      */
/*      SPRB0 (0x10808234)                                                      */
/*      SPRHBM (0x10808234)                                                     */
/*      SPRC0 (0x10808234)                                                      */
/*      SPRMCC (0x10808234)                                                     */
/*      SPRUCC (0x10808234)                                                     */
/* Register default value on SPRA0: 0x00001404                                  */
/* Register default value on SPRB0: 0x00001404                                  */
/* Register default value on SPRHBM: 0x00001404                                 */
/* Register default value on SPRC0: 0x00001404                                  */
/* Register default value on SPRMCC: 0x00001404                                 */
/* Register default value on SPRUCC: 0x00001404                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PASID-related capabilities
*/


#define PASIDCAP_IIO_DSA_REG 0x21010234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 eps : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               If Set, function supports sending TLPs that have
                               the Execute Requested bit Set
                            */
    UINT16 pms : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000001*/

                            /*
                               If Set, function supports sending requests with
                               the Privileged Mode Requested bit Set
                            */
    UINT16 rsvd_3 : 5;

                            /* Bits[7:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 maxwid : 5;

                            /* Bits[12:8], Access Type=RO, default=0x00000014*/

                            /* Highest PASID bit position supported */
    UINT16 rsvd_13 : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PASIDCAP_IIO_DSA_STRUCT;

/* PASIDCTL_IIO_DSA_REG supported on:                                           */
/*      SPRA0 (0x10808236)                                                      */
/*      SPRB0 (0x10808236)                                                      */
/*      SPRHBM (0x10808236)                                                     */
/*      SPRC0 (0x10808236)                                                      */
/*      SPRMCC (0x10808236)                                                     */
/*      SPRUCC (0x10808236)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls for PASID-related functionality
*/


#define PASIDCTL_IIO_DSA_REG 0x21010236

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 pe : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Function is permitted to send and receive TLPs
                               that contain the PASID TLP prefix. [br]See the
                               RAS Chapter in the HAS for programming
                               restrictions.
                            */
    UINT16 epe : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               If Set, function is permitted to send Requests
                               with the Execute Requested bit Set
                            */
    UINT16 pme : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               If Set, function is permitted to send Requests
                               with the Privileged Mode Requested bit Set
                            */
    UINT16 rsvd : 13;

                            /* Bits[15:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PASIDCTL_IIO_DSA_STRUCT;

/* PRSEXTCAP_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808240)                                                      */
/*      SPRB0 (0x20808240)                                                      */
/*      SPRHBM (0x20808240)                                                     */
/*      SPRC0 (0x20808240)                                                      */
/*      SPRMCC (0x20808240)                                                     */
/*      SPRUCC (0x20808240)                                                     */
/* Register default value on SPRA0: 0x00010013                                  */
/* Register default value on SPRB0: 0x00010013                                  */
/* Register default value on SPRHBM: 0x00010013                                 */
/* Register default value on SPRC0: 0x00010013                                  */
/* Register default value on SPRMCC: 0x00010013                                 */
/* Register default value on SPRUCC: 0x00010013                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header (10.5.2.1)
*/


#define PRSEXTCAP_IIO_DSA_REG 0x21020240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 extcapid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000013*/

                            /*
                               PCI-SIG defined ID number indicating the nature
                               and format of the Extended Capability
                            */
    UINT32 capver : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               PCI-SIG defined version number indicating the
                               version of the Capability structure
                            */
    UINT32 nxtcap : 12;

                            /* Bits[31:20], Access Type=RO/V, default=0x00000000*/

                            /*
                               Offset to the next PCI Express Capability
                               structure
                            */

  } Bits;
  UINT32 Data;

} PRSEXTCAP_IIO_DSA_STRUCT;

/* PRSCTL_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x10808244)                                                      */
/*      SPRB0 (0x10808244)                                                      */
/*      SPRHBM (0x10808244)                                                     */
/*      SPRC0 (0x10808244)                                                      */
/*      SPRMCC (0x10808244)                                                     */
/*      SPRUCC (0x10808244)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls for Page Request activities (10.5.2.2)
*/


#define PRSCTL_IIO_DSA_REG 0x21010244

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               When Set, function is allowed to make Page
                               Requests. [br]See the RAS Chapter in the HAS for
                               programming restrictions.
                            */
    UINT16 rst : 1;

                            /* Bits[1:1], Access Type=RW/V, default=0x00000000*/

                            /*
                               When written to 1b, clears Page Request credit
                               counter and pending request state when Enable
                               bit is cleared or being cleared. [br]Because of
                               device programming restrictions on clearing
                               PRSCTL.EN, this bit has no effect on hardware.
                            */
    UINT16 rsvd : 14;

                            /* Bits[15:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PRSCTL_IIO_DSA_STRUCT;

/* PRSSTS_IIO_DSA_REG supported on:                                             */
/*      SPRA0 (0x10808246)                                                      */
/*      SPRB0 (0x10808246)                                                      */
/*      SPRHBM (0x10808246)                                                     */
/*      SPRC0 (0x10808246)                                                      */
/*      SPRMCC (0x10808246)                                                     */
/*      SPRUCC (0x10808246)                                                     */
/* Register default value on SPRA0: 0x00008100                                  */
/* Register default value on SPRB0: 0x00008100                                  */
/* Register default value on SPRHBM: 0x00008100                                 */
/* Register default value on SPRC0: 0x00008100                                  */
/* Register default value on SPRMCC: 0x00008100                                 */
/* Register default value on SPRUCC: 0x00008100                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Status of Page Requests (10.5.2.3)
*/


#define PRSSTS_IIO_DSA_REG 0x21010246

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 rf : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               When Set, indicates the function has received a
                               PRG Response Message indicating a Response
                               Failure
                            */
    UINT16 uprgi : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               When Set, indicates the function has received a
                               PRG Response Message containing a PRG index with
                               no matching request
                            */
    UINT16 rsvd : 6;

                            /* Bits[7:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 stop : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000001*/

                            /*
                               When Enable is Clear, indicates whether
                               previously issued Page Requests have completed
                            */
    UINT16 rsvd_9 : 6;

                            /* Bits[14:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 prpr : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /*
                               If Set, function expects a PASID on PRG Response
                               Messages when corresponding Page Requests had a
                               PASID
                            */

  } Bits;
  UINT16 Data;

} PRSSTS_IIO_DSA_STRUCT;

/* PRSREQCAP_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808248)                                                      */
/*      SPRB0 (0x20808248)                                                      */
/*      SPRHBM (0x20808248)                                                     */
/*      SPRC0 (0x20808248)                                                      */
/*      SPRMCC (0x20808248)                                                     */
/*      SPRUCC (0x20808248)                                                     */
/* Register default value on SPRA0: 0x00000200                                  */
/* Register default value on SPRB0: 0x00000200                                  */
/* Register default value on SPRHBM: 0x00000200                                 */
/* Register default value on SPRC0: 0x00000200                                  */
/* Register default value on SPRMCC: 0x00000200                                 */
/* Register default value on SPRUCC: 0x00000200                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Maximum Number of Outstanding Page Requests
*/


#define PRSREQCAP_IIO_DSA_REG 0x21020248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000200*/

                            /* How many Page Requests can the function issue */

  } Bits;
  UINT32 Data;

} PRSREQCAP_IIO_DSA_STRUCT;

/* PRSREQALLOC_IIO_DSA_REG supported on:                                        */
/*      SPRA0 (0x2080824c)                                                      */
/*      SPRB0 (0x2080824c)                                                      */
/*      SPRHBM (0x2080824c)                                                     */
/*      SPRC0 (0x2080824c)                                                      */
/*      SPRMCC (0x2080824c)                                                     */
/*      SPRUCC (0x2080824c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Maximum Number of Outstanding Page Requests Allowed
*/


#define PRSREQALLOC_IIO_DSA_REG 0x2102024C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 alloc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               How many Page Requests will system SW allow.
                               [br]See the RAS Chapter in the HAS for
                               programming restrictions.
                            */

  } Bits;
  UINT32 Data;

} PRSREQALLOC_IIO_DSA_STRUCT;

/* SETIDVAL_IIO_DSA_REG supported on:                                           */
/*      SPRA0 (0x20808400)                                                      */
/*      SPRB0 (0x20808400)                                                      */
/*      SPRHBM (0x20808400)                                                     */
/*      SPRC0 (0x20808400)                                                      */
/*      SPRMCC (0x20808400)                                                     */
/*      SPRUCC (0x20808400)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Values Captured with SetID Value message
*/


#define SETIDVAL_IIO_DSA_REG 0x21020400

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ppop : 8;

                            /* Bits[7:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Process portion of process ID */
    UINT32 mid : 8;

                            /* Bits[15:8], Access Type=RO/V/P, default=0x00000000*/

                            /* Manufacturing ID */
    UINT32 msid : 8;

                            /* Bits[23:16], Access Type=RO/V/P, default=0x00000000*/

                            /* Manufacturing Stepping ID */
    UINT32 dpop : 4;

                            /* Bits[27:24], Access Type=RO/V/P, default=0x00000000*/

                            /* Dot Portion of Process ID */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SETIDVAL_IIO_DSA_STRUCT;









/* DEFTR3_N1_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x2080843c)                                                      */
/*      SPRB0 (0x2080843c)                                                      */
/*      SPRHBM (0x2080843c)                                                     */
/*      SPRC0 (0x2080843c)                                                      */
/*      SPRMCC (0x2080843c)                                                     */
/*      SPRUCC (0x2080843c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register disables capabilities advertised in GRPCAP.
*/


#define DEFTR3_N1_IIO_DSA_REG 0x2102043C

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

} DEFTR3_N1_IIO_DSA_STRUCT;


/* DEFTR4_N1_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808444)                                                      */
/*      SPRB0 (0x20808444)                                                      */
/*      SPRHBM (0x20808444)                                                     */
/*      SPRC0 (0x20808444)                                                      */
/*      SPRMCC (0x20808444)                                                     */
/*      SPRUCC (0x20808444)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register disables capabilities advertised in ENGCAP.
*/


#define DEFTR4_N1_IIO_DSA_REG 0x21020444

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

} DEFTR4_N1_IIO_DSA_STRUCT;














/* DEFTR11_N1_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x2080847c)                                                      */
/*      SPRB0 (0x2080847c)                                                      */
/*      SPRHBM (0x2080847c)                                                     */
/*      SPRC0 (0x2080847c)                                                      */
/*      SPRMCC (0x2080847c)                                                     */
/*      SPRUCC (0x2080847c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register overrides capabilities advertised in EVNTCAP_0.
*/


#define DEFTR11_N1_IIO_DSA_REG 0x2102047C

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

} DEFTR11_N1_IIO_DSA_STRUCT;


/* DEFTR12_N1_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x20808484)                                                      */
/*      SPRB0 (0x20808484)                                                      */
/*      SPRHBM (0x20808484)                                                     */
/*      SPRC0 (0x20808484)                                                      */
/*      SPRMCC (0x20808484)                                                     */
/*      SPRUCC (0x20808484)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register overrides capabilities advertised in EVNTCAP_1.
*/


#define DEFTR12_N1_IIO_DSA_REG 0x21020484

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

} DEFTR12_N1_IIO_DSA_STRUCT;


/* DEFTR13_N1_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x2080848c)                                                      */
/*      SPRB0 (0x2080848c)                                                      */
/*      SPRHBM (0x2080848c)                                                     */
/*      SPRC0 (0x2080848c)                                                      */
/*      SPRMCC (0x2080848c)                                                     */
/*      SPRUCC (0x2080848c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register overrides capabilities advertised in EVNTCAP_2.
*/


#define DEFTR13_N1_IIO_DSA_REG 0x2102048C

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

} DEFTR13_N1_IIO_DSA_STRUCT;


/* DEFTR14_N1_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x20808494)                                                      */
/*      SPRB0 (0x20808494)                                                      */
/*      SPRHBM (0x20808494)                                                     */
/*      SPRC0 (0x20808494)                                                      */
/*      SPRMCC (0x20808494)                                                     */
/*      SPRUCC (0x20808494)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register overrides capabilities advertised in EVNTCAP_3.
*/


#define DEFTR14_N1_IIO_DSA_REG 0x21020494

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

} DEFTR14_N1_IIO_DSA_STRUCT;


/* DEFTR15_N1_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x2080849c)                                                      */
/*      SPRB0 (0x2080849c)                                                      */
/*      SPRHBM (0x2080849c)                                                     */
/*      SPRC0 (0x2080849c)                                                      */
/*      SPRMCC (0x2080849c)                                                     */
/*      SPRUCC (0x2080849c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register overrides capabilities advertised in EVNTCAP_4.
*/


#define DEFTR15_N1_IIO_DSA_REG 0x2102049C

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

} DEFTR15_N1_IIO_DSA_STRUCT;


/* DEFTR16_N1_IIO_DSA_REG supported on:                                         */
/*      SPRA0 (0x208084a4)                                                      */
/*      SPRB0 (0x208084a4)                                                      */
/*      SPRHBM (0x208084a4)                                                     */
/*      SPRC0 (0x208084a4)                                                      */
/*      SPRMCC (0x208084a4)                                                     */
/*      SPRUCC (0x208084a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register overrides IOSF channel (CHID) used for different types of traffic.  This register currently does not have any effect on the hardware.  It has been provided to allow functionality to be added in the future if time permits.
*/


#define DEFTR16_N1_IIO_DSA_REG 0x210204A4

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

} DEFTR16_N1_IIO_DSA_STRUCT;







/* CRC32DBG0_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808540)                                                      */
/*      SPRB0 (0x20808540)                                                      */
/*      SPRHBM (0x20808540)                                                     */
/*      SPRC0 (0x20808540)                                                      */
/*      SPRMCC (0x20808540)                                                     */
/*      SPRUCC (0x20808540)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register acts as a debug aid for CRC32 operation by shadowing internal values.
*/


#define CRC32DBG0_IIO_DSA_REG 0x21020540

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 crc32accval : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field shadows the internal CRC32
                               accumulator value in non-inverted bit reflected
                               format for debug.
                            */

  } Bits;
  UINT32 Data;

} CRC32DBG0_IIO_DSA_STRUCT;

/* CRC32DBG1_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808544)                                                      */
/*      SPRB0 (0x20808544)                                                      */
/*      SPRHBM (0x20808544)                                                     */
/*      SPRC0 (0x20808544)                                                      */
/*      SPRMCC (0x20808544)                                                     */
/*      SPRUCC (0x20808544)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register acts as a debug aid for CRC32 operation by shadowing internal values.
*/


#define CRC32DBG1_IIO_DSA_REG 0x21020544

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 crc32accval : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field shadows the internal CRC32
                               accumulator value in non-inverted bit reflected
                               format for debug.
                            */

  } Bits;
  UINT32 Data;

} CRC32DBG1_IIO_DSA_STRUCT;

/* CRC32DBG2_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808548)                                                      */
/*      SPRB0 (0x20808548)                                                      */
/*      SPRHBM (0x20808548)                                                     */
/*      SPRC0 (0x20808548)                                                      */
/*      SPRMCC (0x20808548)                                                     */
/*      SPRUCC (0x20808548)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register acts as a debug aid for CRC32 operation by shadowing internal values.
*/


#define CRC32DBG2_IIO_DSA_REG 0x21020548

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 crc32accval : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field shadows the internal CRC32
                               accumulator value in non-inverted bit reflected
                               format for debug.
                            */

  } Bits;
  UINT32 Data;

} CRC32DBG2_IIO_DSA_STRUCT;

/* CRC32DBG3_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x2080854c)                                                      */
/*      SPRB0 (0x2080854c)                                                      */
/*      SPRHBM (0x2080854c)                                                     */
/*      SPRC0 (0x2080854c)                                                      */
/*      SPRMCC (0x2080854c)                                                     */
/*      SPRUCC (0x2080854c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register acts as a debug aid for CRC32 operation by shadowing internal values.
*/


#define CRC32DBG3_IIO_DSA_REG 0x2102054C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 crc32accval : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field shadows the internal CRC32
                               accumulator value in non-inverted bit reflected
                               format for debug.
                            */

  } Bits;
  UINT32 Data;

} CRC32DBG3_IIO_DSA_STRUCT;









/* OSWRAC_N0_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808618)                                                      */
/*      SPRB0 (0x20808618)                                                      */
/*      SPRHBM (0x20808618)                                                     */
/*      SPRC0 (0x20808618)                                                      */
/*      SPRMCC (0x20808618)                                                     */
/*      SPRUCC (0x20808618)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls read accesses to registers in the OS_W policy group
*/


#define OSWRAC_N0_IIO_DSA_REG 0x21020618

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 oswrac_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */

  } Bits;
  UINT32 Data;

} OSWRAC_N0_IIO_DSA_STRUCT;

/* OSWRAC_N1_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x2080861c)                                                      */
/*      SPRB0 (0x2080861c)                                                      */
/*      SPRHBM (0x2080861c)                                                     */
/*      SPRC0 (0x2080861c)                                                      */
/*      SPRMCC (0x2080861c)                                                     */
/*      SPRUCC (0x2080861c)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls read accesses to registers in the OS_W policy group
*/


#define OSWRAC_N1_IIO_DSA_REG 0x2102061C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 oswrac_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswrac_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */

  } Bits;
  UINT32 Data;

} OSWRAC_N1_IIO_DSA_STRUCT;

/* OSWWAC_N0_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808620)                                                      */
/*      SPRB0 (0x20808620)                                                      */
/*      SPRHBM (0x20808620)                                                     */
/*      SPRC0 (0x20808620)                                                      */
/*      SPRMCC (0x20808620)                                                     */
/*      SPRUCC (0x20808620)                                                     */
/* Register default value on SPRA0: 0x0300021F                                  */
/* Register default value on SPRB0: 0x0300021F                                  */
/* Register default value on SPRHBM: 0x0300021F                                 */
/* Register default value on SPRC0: 0x0300021F                                  */
/* Register default value on SPRMCC: 0x0300021F                                 */
/* Register default value on SPRUCC: 0x0300021F                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls write accesses to registers in the OS_W policy group
*/


#define OSWWAC_N0_IIO_DSA_REG 0x21020620

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 oswwac_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */

  } Bits;
  UINT32 Data;

} OSWWAC_N0_IIO_DSA_STRUCT;

/* OSWWAC_N1_IIO_DSA_REG supported on:                                          */
/*      SPRA0 (0x20808624)                                                      */
/*      SPRB0 (0x20808624)                                                      */
/*      SPRHBM (0x20808624)                                                     */
/*      SPRC0 (0x20808624)                                                      */
/*      SPRMCC (0x20808624)                                                     */
/*      SPRUCC (0x20808624)                                                     */
/* Register default value on SPRA0: 0x20000C00                                  */
/* Register default value on SPRB0: 0x20000C00                                  */
/* Register default value on SPRHBM: 0x20000C00                                 */
/* Register default value on SPRC0: 0x20000C00                                  */
/* Register default value on SPRMCC: 0x20000C00                                 */
/* Register default value on SPRUCC: 0x20000C00                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls write accesses to registers in the OS_W policy group
*/


#define OSWWAC_N1_IIO_DSA_REG 0x21020624

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 oswwac_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oswwac_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */

  } Bits;
  UINT32 Data;

} OSWWAC_N1_IIO_DSA_STRUCT;

/* OSWCP_N0_IIO_DSA_REG supported on:                                           */
/*      SPRA0 (0x20808628)                                                      */
/*      SPRB0 (0x20808628)                                                      */
/*      SPRHBM (0x20808628)                                                     */
/*      SPRC0 (0x20808628)                                                      */
/*      SPRMCC (0x20808628)                                                     */
/*      SPRUCC (0x20808628)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls read/write accesses to OS_W RAC, WAC, and CP registers
*/


#define OSWCP_N0_IIO_DSA_REG 0x21020628

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 oscp_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */

  } Bits;
  UINT32 Data;

} OSWCP_N0_IIO_DSA_STRUCT;

/* OSWCP_N1_IIO_DSA_REG supported on:                                           */
/*      SPRA0 (0x2080862c)                                                      */
/*      SPRB0 (0x2080862c)                                                      */
/*      SPRHBM (0x2080862c)                                                     */
/*      SPRC0 (0x2080862c)                                                      */
/*      SPRMCC (0x2080862c)                                                     */
/*      SPRUCC (0x2080862c)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/dsa_top/dsa_cfg_inst                      */
/* SPRUCC Register File:    sprsp_top/dsa_top[0]/dsa_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_1_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls read/write accesses to OS_W RAC, WAC, and CP registers
*/


#define OSWCP_N1_IIO_DSA_REG 0x2102062C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 oscp_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */
    UINT32 oscp_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               SAI value indexes this field. If the
                               corresponding bit is set, the access is allowed.
                               [br]This field loses its value if warm reset or
                               stronger but retains its value through FLR.
                            */

  } Bits;
  UINT32 Data;

} OSWCP_N1_IIO_DSA_STRUCT;






#endif /* _IIO_DSA_h */
