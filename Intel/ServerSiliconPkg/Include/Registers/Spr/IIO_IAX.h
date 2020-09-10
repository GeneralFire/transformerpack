
/** @file
  IIO_IAX.h

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


#ifndef _IIO_IAX_h
#define _IIO_IAX_h
#include <Base.h>

/* VID_IIO_IAX_REG supported on:                                                */
/*      SPRA0 (0x10810000)                                                      */
/*      SPRB0 (0x10810000)                                                      */
/*      SPRHBM (0x10810000)                                                     */
/*      SPRC0 (0x10810000)                                                      */
/*      SPRMCC (0x10810000)                                                     */
/*      SPRUCC (0x10810000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Identifies the manufacturer of the device
*/


#define VID_IIO_IAX_REG 0x21110000

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

} VID_IIO_IAX_STRUCT;

/* DID_IIO_IAX_REG supported on:                                                */
/*      SPRA0 (0x10810002)                                                      */
/*      SPRB0 (0x10810002)                                                      */
/*      SPRHBM (0x10810002)                                                     */
/*      SPRC0 (0x10810002)                                                      */
/*      SPRMCC (0x10810002)                                                     */
/*      SPRUCC (0x10810002)                                                     */
/* Register default value on SPRA0: 0x00000CFE                                  */
/* Register default value on SPRB0: 0x00000CFE                                  */
/* Register default value on SPRHBM: 0x00000CFE                                 */
/* Register default value on SPRC0: 0x00000CFE                                  */
/* Register default value on SPRMCC: 0x00000CFE                                 */
/* Register default value on SPRUCC: 0x00000CFE                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Identifies the particular device.
*/


#define DID_IIO_IAX_REG 0x21110002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 did : 16;

                            /* Bits[15:0], Access Type=RO/V/P, default=0x00000CFE*/

                            /* Allocated by the vendor. */

  } Bits;
  UINT16 Data;

} DID_IIO_IAX_STRUCT;

/* PCICMD_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x10810004)                                                      */
/*      SPRB0 (0x10810004)                                                      */
/*      SPRHBM (0x10810004)                                                     */
/*      SPRC0 (0x10810004)                                                      */
/*      SPRMCC (0x10810004)                                                     */
/*      SPRUCC (0x10810004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The Command register provides coarse control over a device's ability to generate and respond to PCI cycles. When a 0 is written to this register, the device is logically disconnected from the PCI bus for all accesses except configuration accesses.
*/


#define PCICMD_IIO_IAX_REG 0x21110004

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

} PCICMD_IIO_IAX_STRUCT;

/* PCISTS_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x10810006)                                                      */
/*      SPRB0 (0x10810006)                                                      */
/*      SPRHBM (0x10810006)                                                     */
/*      SPRC0 (0x10810006)                                                      */
/*      SPRMCC (0x10810006)                                                     */
/*      SPRUCC (0x10810006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The Status register is used to record status information for PCI bus related events.
*/


#define PCISTS_IIO_IAX_REG 0x21110006

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

} PCISTS_IIO_IAX_STRUCT;

/* RID_IIO_IAX_REG supported on:                                                */
/*      SPRA0 (0x810008)                                                        */
/*      SPRB0 (0x810008)                                                        */
/*      SPRHBM (0x810008)                                                       */
/*      SPRC0 (0x810008)                                                        */
/*      SPRMCC (0x810008)                                                       */
/*      SPRUCC (0x810008)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register specifies a device specific revision identifier.
*/


#define RID_IIO_IAX_REG 0x21100008

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

} RID_IIO_IAX_STRUCT;

/* CCRLPI_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x810009)                                                        */
/*      SPRB0 (0x810009)                                                        */
/*      SPRHBM (0x810009)                                                       */
/*      SPRC0 (0x810009)                                                        */
/*      SPRMCC (0x810009)                                                       */
/*      SPRUCC (0x810009)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* The Class Code register is read-only and is used to identify the generic function of the device and, in some cases, a specific register-level programming interface. The lower byte  identifies a specific register-level programming interface (if any) so that device independent software can interact with the device.
*/


#define CCRLPI_IIO_IAX_REG 0x21100009

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

} CCRLPI_IIO_IAX_STRUCT;

/* CCSC_IIO_IAX_REG supported on:                                               */
/*      SPRA0 (0x81000a)                                                        */
/*      SPRB0 (0x81000a)                                                        */
/*      SPRHBM (0x81000a)                                                       */
/*      SPRC0 (0x81000a)                                                        */
/*      SPRMCC (0x81000a)                                                       */
/*      SPRUCC (0x81000a)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* The Class Code register is read-only and is used to identify the generic function of the device and, in some cases, a specific register-level programming interface. The middle byte  is a sub-class code which identifies more specifically the function of the device.
*/


#define CCSC_IIO_IAX_REG 0x2110000A

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

} CCSC_IIO_IAX_STRUCT;

/* CCBC_IIO_IAX_REG supported on:                                               */
/*      SPRA0 (0x81000b)                                                        */
/*      SPRB0 (0x81000b)                                                        */
/*      SPRHBM (0x81000b)                                                       */
/*      SPRC0 (0x81000b)                                                        */
/*      SPRMCC (0x81000b)                                                       */
/*      SPRUCC (0x81000b)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* The Class Code register is read-only and is used to identify the generic function of the device and, in some cases, a specific register-level programming interface. The upper byte is a base class code which broadly classifies the type of function the device performs.
*/


#define CCBC_IIO_IAX_REG 0x2110000B

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

} CCBC_IIO_IAX_STRUCT;

/* CLS_IIO_IAX_REG supported on:                                                */
/*      SPRA0 (0x81000c)                                                        */
/*      SPRB0 (0x81000c)                                                        */
/*      SPRHBM (0x81000c)                                                       */
/*      SPRC0 (0x81000c)                                                        */
/*      SPRMCC (0x81000c)                                                       */
/*      SPRUCC (0x81000c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* The Cache Line Size register is set by the system firmware or the operating system to system cache line size.
*/


#define CLS_IIO_IAX_REG 0x2110000C

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

} CLS_IIO_IAX_STRUCT;

/* LATTMR_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x81000d)                                                        */
/*      SPRB0 (0x81000d)                                                        */
/*      SPRHBM (0x81000d)                                                       */
/*      SPRC0 (0x81000d)                                                        */
/*      SPRMCC (0x81000d)                                                       */
/*      SPRUCC (0x81000d)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register is also referred to as Primary Latency Timer for Type 1 Configuration Space header Functions.
*/


#define LATTMR_IIO_IAX_REG 0x2110000D

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

} LATTMR_IIO_IAX_STRUCT;

/* HDR_IIO_IAX_REG supported on:                                                */
/*      SPRA0 (0x81000e)                                                        */
/*      SPRB0 (0x81000e)                                                        */
/*      SPRHBM (0x81000e)                                                       */
/*      SPRC0 (0x81000e)                                                        */
/*      SPRMCC (0x81000e)                                                       */
/*      SPRUCC (0x81000e)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register identifies the layout of the second part of the predefined header (beginning at byte 10h in Configuration Space) and also whether or not the Device might contain multiple Functions.
*/


#define HDR_IIO_IAX_REG 0x2110000E

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

} HDR_IIO_IAX_STRUCT;

/* BIST_IIO_IAX_REG supported on:                                               */
/*      SPRA0 (0x81000f)                                                        */
/*      SPRB0 (0x81000f)                                                        */
/*      SPRHBM (0x81000f)                                                       */
/*      SPRC0 (0x81000f)                                                        */
/*      SPRMCC (0x81000f)                                                       */
/*      SPRUCC (0x81000f)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This optional register is used for control and status of BIST.
*/


#define BIST_IIO_IAX_REG 0x2110000F

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

} BIST_IIO_IAX_STRUCT;

/* BAR0_N0_IIO_IAX_REG supported on:                                            */
/*      SPRA0 (0x20810010)                                                      */
/*      SPRB0 (0x20810010)                                                      */
/*      SPRHBM (0x20810010)                                                     */
/*      SPRC0 (0x20810010)                                                      */
/*      SPRMCC (0x20810010)                                                     */
/*      SPRUCC (0x20810010)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Size, type, and location of address range for control registers
*/


#define BAR0_N0_IIO_IAX_REG 0x21120010

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

} BAR0_N0_IIO_IAX_STRUCT;

/* BAR0_N1_IIO_IAX_REG supported on:                                            */
/*      SPRA0 (0x20810014)                                                      */
/*      SPRB0 (0x20810014)                                                      */
/*      SPRHBM (0x20810014)                                                     */
/*      SPRC0 (0x20810014)                                                      */
/*      SPRMCC (0x20810014)                                                     */
/*      SPRUCC (0x20810014)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Size, type, and location of address range for control registers
*/


#define BAR0_N1_IIO_IAX_REG 0x21120014

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

} BAR0_N1_IIO_IAX_STRUCT;

/* BAR2_N0_IIO_IAX_REG supported on:                                            */
/*      SPRA0 (0x20810018)                                                      */
/*      SPRB0 (0x20810018)                                                      */
/*      SPRHBM (0x20810018)                                                     */
/*      SPRC0 (0x20810018)                                                      */
/*      SPRMCC (0x20810018)                                                     */
/*      SPRUCC (0x20810018)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Size, type, and location of address range for portals
*/


#define BAR2_N0_IIO_IAX_REG 0x21120018

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

} BAR2_N0_IIO_IAX_STRUCT;

/* BAR2_N1_IIO_IAX_REG supported on:                                            */
/*      SPRA0 (0x2081001c)                                                      */
/*      SPRB0 (0x2081001c)                                                      */
/*      SPRHBM (0x2081001c)                                                     */
/*      SPRC0 (0x2081001c)                                                      */
/*      SPRMCC (0x2081001c)                                                     */
/*      SPRUCC (0x2081001c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Size, type, and location of address range for portals
*/


#define BAR2_N1_IIO_IAX_REG 0x2112001C

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

} BAR2_N1_IIO_IAX_STRUCT;

/* SSVID_IIO_IAX_REG supported on:                                              */
/*      SPRA0 (0x1081002c)                                                      */
/*      SPRB0 (0x1081002c)                                                      */
/*      SPRHBM (0x1081002c)                                                     */
/*      SPRC0 (0x1081002c)                                                      */
/*      SPRMCC (0x1081002c)                                                     */
/*      SPRUCC (0x1081002c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register (along with SSID) is used to uniquely identify the subsystem where the PCI device resides.
*/


#define SSVID_IIO_IAX_REG 0x2111002C

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

} SSVID_IIO_IAX_STRUCT;

/* SSID_IIO_IAX_REG supported on:                                               */
/*      SPRA0 (0x1081002e)                                                      */
/*      SPRB0 (0x1081002e)                                                      */
/*      SPRHBM (0x1081002e)                                                     */
/*      SPRC0 (0x1081002e)                                                      */
/*      SPRMCC (0x1081002e)                                                     */
/*      SPRUCC (0x1081002e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register (along with SSVID) is used to uniquely identify the subsystem where the PCI device resides.
*/


#define SSID_IIO_IAX_REG 0x2111002E

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

} SSID_IIO_IAX_STRUCT;

/* CAPPTR_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x810034)                                                        */
/*      SPRB0 (0x810034)                                                        */
/*      SPRHBM (0x810034)                                                       */
/*      SPRC0 (0x810034)                                                        */
/*      SPRMCC (0x810034)                                                       */
/*      SPRUCC (0x810034)                                                       */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This optional register is used to point to a linked list of new capabilities implemented by this device. This register is only valid if the Capabilities List bit in the Status Register is set.
*/


#define CAPPTR_IIO_IAX_REG 0x21100034

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

} CAPPTR_IIO_IAX_STRUCT;

/* INTL_IIO_IAX_REG supported on:                                               */
/*      SPRA0 (0x81003c)                                                        */
/*      SPRB0 (0x81003c)                                                        */
/*      SPRHBM (0x81003c)                                                       */
/*      SPRC0 (0x81003c)                                                        */
/*      SPRMCC (0x81003c)                                                       */
/*      SPRUCC (0x81003c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The Interrupt Line register communicates interrupt line routing information.
*/


#define INTL_IIO_IAX_REG 0x2110003C

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

} INTL_IIO_IAX_STRUCT;

/* INTP_IIO_IAX_REG supported on:                                               */
/*      SPRA0 (0x81003d)                                                        */
/*      SPRB0 (0x81003d)                                                        */
/*      SPRHBM (0x81003d)                                                       */
/*      SPRC0 (0x81003d)                                                        */
/*      SPRMCC (0x81003d)                                                       */
/*      SPRUCC (0x81003d)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The Interrupt Pin register is a read-only register that identifies the legacy interrupt Message(s) the Function uses
*/


#define INTP_IIO_IAX_REG 0x2110003D

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

} INTP_IIO_IAX_STRUCT;

/* MINGNT_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x81003e)                                                        */
/*      SPRB0 (0x81003e)                                                        */
/*      SPRHBM (0x81003e)                                                       */
/*      SPRC0 (0x81003e)                                                        */
/*      SPRMCC (0x81003e)                                                       */
/*      SPRUCC (0x81003e)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Does not apply to PCI Express
*/


#define MINGNT_IIO_IAX_REG 0x2110003E

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

} MINGNT_IIO_IAX_STRUCT;

/* MAXLAT_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x81003f)                                                        */
/*      SPRB0 (0x81003f)                                                        */
/*      SPRHBM (0x81003f)                                                       */
/*      SPRC0 (0x81003f)                                                        */
/*      SPRMCC (0x81003f)                                                       */
/*      SPRUCC (0x81003f)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Does not apply to PCI Express
*/


#define MAXLAT_IIO_IAX_REG 0x2110003F

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

} MAXLAT_IIO_IAX_STRUCT;

/* PCIECAPLST_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x10810040)                                                      */
/*      SPRB0 (0x10810040)                                                      */
/*      SPRHBM (0x10810040)                                                     */
/*      SPRC0 (0x10810040)                                                      */
/*      SPRMCC (0x10810040)                                                     */
/*      SPRUCC (0x10810040)                                                     */
/* Register default value on SPRA0: 0x00008010                                  */
/* Register default value on SPRB0: 0x00008010                                  */
/* Register default value on SPRHBM: 0x00008010                                 */
/* Register default value on SPRC0: 0x00008010                                  */
/* Register default value on SPRMCC: 0x00008010                                 */
/* Register default value on SPRUCC: 0x00008010                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Enumerates the PCI Express Capability Structure in the PCI Configuration list.
*/


#define PCIECAPLST_IIO_IAX_REG 0x21110040

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

} PCIECAPLST_IIO_IAX_STRUCT;

/* PCIECAP_IIO_IAX_REG supported on:                                            */
/*      SPRA0 (0x10810042)                                                      */
/*      SPRB0 (0x10810042)                                                      */
/*      SPRHBM (0x10810042)                                                     */
/*      SPRC0 (0x10810042)                                                      */
/*      SPRMCC (0x10810042)                                                     */
/*      SPRUCC (0x10810042)                                                     */
/* Register default value on SPRA0: 0x00000092                                  */
/* Register default value on SPRB0: 0x00000092                                  */
/* Register default value on SPRHBM: 0x00000092                                 */
/* Register default value on SPRC0: 0x00000092                                  */
/* Register default value on SPRMCC: 0x00000092                                 */
/* Register default value on SPRUCC: 0x00000092                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Identifies PCI Express device Function type and associated capabilities.
*/


#define PCIECAP_IIO_IAX_REG 0x21110042

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

} PCIECAP_IIO_IAX_STRUCT;

/* DEVCAP_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x20810044)                                                      */
/*      SPRB0 (0x20810044)                                                      */
/*      SPRHBM (0x20810044)                                                     */
/*      SPRC0 (0x20810044)                                                      */
/*      SPRMCC (0x20810044)                                                     */
/*      SPRUCC (0x20810044)                                                     */
/* Register default value on SPRA0: 0x00008022                                  */
/* Register default value on SPRB0: 0x00008022                                  */
/* Register default value on SPRHBM: 0x00008022                                 */
/* Register default value on SPRC0: 0x00008022                                  */
/* Register default value on SPRMCC: 0x00008022                                 */
/* Register default value on SPRUCC: 0x00008022                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Identifies PCI Express device Function specific capabilities.
*/


#define DEVCAP_IIO_IAX_REG 0x21120044

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

} DEVCAP_IIO_IAX_STRUCT;

/* DEVCTL_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x10810048)                                                      */
/*      SPRB0 (0x10810048)                                                      */
/*      SPRHBM (0x10810048)                                                     */
/*      SPRC0 (0x10810048)                                                      */
/*      SPRMCC (0x10810048)                                                     */
/*      SPRUCC (0x10810048)                                                     */
/* Register default value on SPRA0: 0x00002910                                  */
/* Register default value on SPRB0: 0x00002910                                  */
/* Register default value on SPRHBM: 0x00002910                                 */
/* Register default value on SPRC0: 0x00002910                                  */
/* Register default value on SPRMCC: 0x00002910                                 */
/* Register default value on SPRUCC: 0x00002910                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls PCI Express device specific parameters
*/


#define DEVCTL_IIO_IAX_REG 0x21110048

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

} DEVCTL_IIO_IAX_STRUCT;

/* DEVSTS_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x1081004a)                                                      */
/*      SPRB0 (0x1081004a)                                                      */
/*      SPRHBM (0x1081004a)                                                     */
/*      SPRC0 (0x1081004a)                                                      */
/*      SPRMCC (0x1081004a)                                                     */
/*      SPRUCC (0x1081004a)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Provides information about PCI Express device (Function) specific parameters.
*/


#define DEVSTS_IIO_IAX_REG 0x2111004A

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
                               been completed. This bit is cleared only when al
                               outstanding Non-Posted Requests have completed
                               or have been terminated by the Completion
                               Timeout mechanism. This bit will also be cleared
                               upon the completion of an FLR.
                            */
    UINT16 rsvd_6 : 10;

                            /* Bits[15:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} DEVSTS_IIO_IAX_STRUCT;

/* DEVCAP2_IIO_IAX_REG supported on:                                            */
/*      SPRA0 (0x20810064)                                                      */
/*      SPRB0 (0x20810064)                                                      */
/*      SPRHBM (0x20810064)                                                     */
/*      SPRC0 (0x20810064)                                                      */
/*      SPRMCC (0x20810064)                                                     */
/*      SPRUCC (0x20810064)                                                     */
/* Register default value on SPRA0: 0x00730010                                  */
/* Register default value on SPRB0: 0x00730010                                  */
/* Register default value on SPRHBM: 0x00730010                                 */
/* Register default value on SPRC0: 0x00730010                                  */
/* Register default value on SPRMCC: 0x00730010                                 */
/* Register default value on SPRUCC: 0x00730010                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Identifies additional PCI Express device Function specific capabilities.
*/


#define DEVCAP2_IIO_IAX_REG 0x21120064

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

} DEVCAP2_IIO_IAX_STRUCT;

/* DEVCTL2_IIO_IAX_REG supported on:                                            */
/*      SPRA0 (0x10810068)                                                      */
/*      SPRB0 (0x10810068)                                                      */
/*      SPRHBM (0x10810068)                                                     */
/*      SPRC0 (0x10810068)                                                      */
/*      SPRMCC (0x10810068)                                                     */
/*      SPRUCC (0x10810068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls additional PCI Express device specific parameters
*/


#define DEVCTL2_IIO_IAX_REG 0x21110068

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

} DEVCTL2_IIO_IAX_STRUCT;

/* MSIXCAPLST_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x10810080)                                                      */
/*      SPRB0 (0x10810080)                                                      */
/*      SPRHBM (0x10810080)                                                     */
/*      SPRC0 (0x10810080)                                                      */
/*      SPRMCC (0x10810080)                                                     */
/*      SPRUCC (0x10810080)                                                     */
/* Register default value on SPRA0: 0x00009011                                  */
/* Register default value on SPRB0: 0x00009011                                  */
/* Register default value on SPRHBM: 0x00009011                                 */
/* Register default value on SPRC0: 0x00009011                                  */
/* Register default value on SPRMCC: 0x00009011                                 */
/* Register default value on SPRUCC: 0x00009011                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Enumerates the MSI-X Capability structure in the PCI Configuration Space Capability list.
*/


#define MSIXCAPLST_IIO_IAX_REG 0x21110080

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

} MSIXCAPLST_IIO_IAX_STRUCT;

/* MSIXMSGCTL_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x10810082)                                                      */
/*      SPRB0 (0x10810082)                                                      */
/*      SPRHBM (0x10810082)                                                     */
/*      SPRC0 (0x10810082)                                                      */
/*      SPRMCC (0x10810082)                                                     */
/*      SPRUCC (0x10810082)                                                     */
/* Register default value on SPRA0: 0x00000008                                  */
/* Register default value on SPRB0: 0x00000008                                  */
/* Register default value on SPRHBM: 0x00000008                                 */
/* Register default value on SPRC0: 0x00000008                                  */
/* Register default value on SPRMCC: 0x00000008                                 */
/* Register default value on SPRUCC: 0x00000008                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSI-X controls. System SW can modify bits in this register. A device driver is not permitted to modify this register
*/


#define MSIXMSGCTL_IIO_IAX_REG 0x21110082

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

} MSIXMSGCTL_IIO_IAX_STRUCT;

/* MSIXTBL_IIO_IAX_REG supported on:                                            */
/*      SPRA0 (0x20810084)                                                      */
/*      SPRB0 (0x20810084)                                                      */
/*      SPRHBM (0x20810084)                                                     */
/*      SPRC0 (0x20810084)                                                      */
/*      SPRMCC (0x20810084)                                                     */
/*      SPRUCC (0x20810084)                                                     */
/* Register default value on SPRA0: 0x00002000                                  */
/* Register default value on SPRB0: 0x00002000                                  */
/* Register default value on SPRHBM: 0x00002000                                 */
/* Register default value on SPRC0: 0x00002000                                  */
/* Register default value on SPRMCC: 0x00002000                                 */
/* Register default value on SPRUCC: 0x00002000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSI-X Table Offset and Table BIR
*/


#define MSIXTBL_IIO_IAX_REG 0x21120084

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

} MSIXTBL_IIO_IAX_STRUCT;

/* MSIXPBA_IIO_IAX_REG supported on:                                            */
/*      SPRA0 (0x20810088)                                                      */
/*      SPRB0 (0x20810088)                                                      */
/*      SPRHBM (0x20810088)                                                     */
/*      SPRC0 (0x20810088)                                                      */
/*      SPRMCC (0x20810088)                                                     */
/*      SPRUCC (0x20810088)                                                     */
/* Register default value on SPRA0: 0x00003000                                  */
/* Register default value on SPRB0: 0x00003000                                  */
/* Register default value on SPRHBM: 0x00003000                                 */
/* Register default value on SPRC0: 0x00003000                                  */
/* Register default value on SPRMCC: 0x00003000                                 */
/* Register default value on SPRUCC: 0x00003000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSI-X PBA Offset and PBA BIR
*/


#define MSIXPBA_IIO_IAX_REG 0x21120088

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

} MSIXPBA_IIO_IAX_STRUCT;

/* PMCAP_IIO_IAX_REG supported on:                                              */
/*      SPRA0 (0x20810090)                                                      */
/*      SPRB0 (0x20810090)                                                      */
/*      SPRHBM (0x20810090)                                                     */
/*      SPRC0 (0x20810090)                                                      */
/*      SPRMCC (0x20810090)                                                     */
/*      SPRUCC (0x20810090)                                                     */
/* Register default value on SPRA0: 0x00030001                                  */
/* Register default value on SPRB0: 0x00030001                                  */
/* Register default value on SPRHBM: 0x00030001                                 */
/* Register default value on SPRC0: 0x00030001                                  */
/* Register default value on SPRMCC: 0x00030001                                 */
/* Register default value on SPRUCC: 0x00030001                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Power Management Capability
*/


#define PMCAP_IIO_IAX_REG 0x21120090

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

} PMCAP_IIO_IAX_STRUCT;

/* PMCSR_IIO_IAX_REG supported on:                                              */
/*      SPRA0 (0x20810094)                                                      */
/*      SPRB0 (0x20810094)                                                      */
/*      SPRHBM (0x20810094)                                                     */
/*      SPRC0 (0x20810094)                                                      */
/*      SPRMCC (0x20810094)                                                     */
/*      SPRUCC (0x20810094)                                                     */
/* Register default value on SPRA0: 0x00000008                                  */
/* Register default value on SPRB0: 0x00000008                                  */
/* Register default value on SPRHBM: 0x00000008                                 */
/* Register default value on SPRC0: 0x00000008                                  */
/* Register default value on SPRMCC: 0x00000008                                 */
/* Register default value on SPRUCC: 0x00000008                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register is used to manage the PCI Function's power management state
*/


#define PMCSR_IIO_IAX_REG 0x21120094

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

} PMCSR_IIO_IAX_STRUCT;

/* AEREXTCAP_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810100)                                                      */
/*      SPRB0 (0x20810100)                                                      */
/*      SPRHBM (0x20810100)                                                     */
/*      SPRC0 (0x20810100)                                                      */
/*      SPRMCC (0x20810100)                                                     */
/*      SPRUCC (0x20810100)                                                     */
/* Register default value on SPRA0: 0x15020001                                  */
/* Register default value on SPRB0: 0x15020001                                  */
/* Register default value on SPRHBM: 0x15020001                                 */
/* Register default value on SPRC0: 0x15020001                                  */
/* Register default value on SPRMCC: 0x15020001                                 */
/* Register default value on SPRUCC: 0x15020001                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header
*/


#define AEREXTCAP_IIO_IAX_REG 0x21120100

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

} AEREXTCAP_IIO_IAX_STRUCT;

/* ERRUNCSTS_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810104)                                                      */
/*      SPRB0 (0x20810104)                                                      */
/*      SPRHBM (0x20810104)                                                     */
/*      SPRC0 (0x20810104)                                                      */
/*      SPRMCC (0x20810104)                                                     */
/*      SPRUCC (0x20810104)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Indicates error detection status of individual errors on a PCI Express device Function
*/


#define ERRUNCSTS_IIO_IAX_REG 0x21120104

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

} ERRUNCSTS_IIO_IAX_STRUCT;

/* ERRUNCMSK_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810108)                                                      */
/*      SPRB0 (0x20810108)                                                      */
/*      SPRHBM (0x20810108)                                                     */
/*      SPRC0 (0x20810108)                                                      */
/*      SPRMCC (0x20810108)                                                     */
/*      SPRUCC (0x20810108)                                                     */
/* Register default value on SPRA0: 0x00400000                                  */
/* Register default value on SPRB0: 0x00400000                                  */
/* Register default value on SPRHBM: 0x00400000                                 */
/* Register default value on SPRC0: 0x00400000                                  */
/* Register default value on SPRMCC: 0x00400000                                 */
/* Register default value on SPRUCC: 0x00400000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls reporting of individual errors. A masked error is not recoded or reported in the Header Log or First Error Pointer and is not reported to the PCI Express Root Complex.
*/


#define ERRUNCMSK_IIO_IAX_REG 0x21120108

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

} ERRUNCMSK_IIO_IAX_STRUCT;

/* ERRUNCSEV_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x2081010c)                                                      */
/*      SPRB0 (0x2081010c)                                                      */
/*      SPRHBM (0x2081010c)                                                     */
/*      SPRC0 (0x2081010c)                                                      */
/*      SPRMCC (0x2081010c)                                                     */
/*      SPRUCC (0x2081010c)                                                     */
/* Register default value on SPRA0: 0x00440000                                  */
/* Register default value on SPRB0: 0x00440000                                  */
/* Register default value on SPRHBM: 0x00440000                                 */
/* Register default value on SPRC0: 0x00440000                                  */
/* Register default value on SPRMCC: 0x00440000                                 */
/* Register default value on SPRUCC: 0x00440000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls whether an individual error is reported as a Non-fatal (bit is Clear) or Fatal (bit is Set) error
*/


#define ERRUNCSEV_IIO_IAX_REG 0x2112010C

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

} ERRUNCSEV_IIO_IAX_STRUCT;

/* ERRCORSTS_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810110)                                                      */
/*      SPRB0 (0x20810110)                                                      */
/*      SPRHBM (0x20810110)                                                     */
/*      SPRC0 (0x20810110)                                                      */
/*      SPRMCC (0x20810110)                                                     */
/*      SPRUCC (0x20810110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Reports error status of individual correctable error sources
*/


#define ERRCORSTS_IIO_IAX_REG 0x21120110

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

} ERRCORSTS_IIO_IAX_STRUCT;

/* ERRCORMSK_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810114)                                                      */
/*      SPRB0 (0x20810114)                                                      */
/*      SPRHBM (0x20810114)                                                     */
/*      SPRC0 (0x20810114)                                                      */
/*      SPRMCC (0x20810114)                                                     */
/*      SPRUCC (0x20810114)                                                     */
/* Register default value on SPRA0: 0x00006000                                  */
/* Register default value on SPRB0: 0x00006000                                  */
/* Register default value on SPRHBM: 0x00006000                                 */
/* Register default value on SPRC0: 0x00006000                                  */
/* Register default value on SPRMCC: 0x00006000                                 */
/* Register default value on SPRUCC: 0x00006000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls the reporting of individual correctable errors
*/


#define ERRCORMSK_IIO_IAX_REG 0x21120114

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

} ERRCORMSK_IIO_IAX_STRUCT;

/* AERCAPCTL_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810118)                                                      */
/*      SPRB0 (0x20810118)                                                      */
/*      SPRHBM (0x20810118)                                                     */
/*      SPRC0 (0x20810118)                                                      */
/*      SPRMCC (0x20810118)                                                     */
/*      SPRUCC (0x20810118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* More AER information
*/


#define AERCAPCTL_IIO_IAX_REG 0x21120118

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

} AERCAPCTL_IIO_IAX_STRUCT;

/* AERHDRLOG1_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x2081011c)                                                      */
/*      SPRB0 (0x2081011c)                                                      */
/*      SPRHBM (0x2081011c)                                                     */
/*      SPRC0 (0x2081011c)                                                      */
/*      SPRMCC (0x2081011c)                                                     */
/*      SPRUCC (0x2081011c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* First DWORD of the header for the TLP corresponding to a detected error.
*/


#define AERHDRLOG1_IIO_IAX_REG 0x2112011C

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

} AERHDRLOG1_IIO_IAX_STRUCT;

/* AERHDRLOG2_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x20810120)                                                      */
/*      SPRB0 (0x20810120)                                                      */
/*      SPRHBM (0x20810120)                                                     */
/*      SPRC0 (0x20810120)                                                      */
/*      SPRMCC (0x20810120)                                                     */
/*      SPRUCC (0x20810120)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Second DWORD of the header for the TLP corresponding to a detected error.
*/


#define AERHDRLOG2_IIO_IAX_REG 0x21120120

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

} AERHDRLOG2_IIO_IAX_STRUCT;

/* AERHDRLOG3_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x20810124)                                                      */
/*      SPRB0 (0x20810124)                                                      */
/*      SPRHBM (0x20810124)                                                     */
/*      SPRC0 (0x20810124)                                                      */
/*      SPRMCC (0x20810124)                                                     */
/*      SPRUCC (0x20810124)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Third DWORD of the header for the TLP corresponding to a detected error.
*/


#define AERHDRLOG3_IIO_IAX_REG 0x21120124

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

} AERHDRLOG3_IIO_IAX_STRUCT;

/* AERHDRLOG4_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x20810128)                                                      */
/*      SPRB0 (0x20810128)                                                      */
/*      SPRHBM (0x20810128)                                                     */
/*      SPRC0 (0x20810128)                                                      */
/*      SPRMCC (0x20810128)                                                     */
/*      SPRUCC (0x20810128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Fourth DWORD of the header for the TLP corresponding to a detected error.
*/


#define AERHDRLOG4_IIO_IAX_REG 0x21120128

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

} AERHDRLOG4_IIO_IAX_STRUCT;

/* AERTLPPLOG1_IIO_IAX_REG supported on:                                        */
/*      SPRA0 (0x20810138)                                                      */
/*      SPRB0 (0x20810138)                                                      */
/*      SPRHBM (0x20810138)                                                     */
/*      SPRC0 (0x20810138)                                                      */
/*      SPRMCC (0x20810138)                                                     */
/*      SPRUCC (0x20810138)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register captures the End-End TLP Prefix (DW1) for the TLP corresponding to the detected error.
*/


#define AERTLPPLOG1_IIO_IAX_REG 0x21120138

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

} AERTLPPLOG1_IIO_IAX_STRUCT;

/* AERTLPPLOG2_IIO_IAX_REG supported on:                                        */
/*      SPRA0 (0x2081013c)                                                      */
/*      SPRB0 (0x2081013c)                                                      */
/*      SPRHBM (0x2081013c)                                                     */
/*      SPRC0 (0x2081013c)                                                      */
/*      SPRMCC (0x2081013c)                                                     */
/*      SPRUCC (0x2081013c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register captures the End-End TLP Prefix (DW2) for the TLP corresponding to the detected error.
*/


#define AERTLPPLOG2_IIO_IAX_REG 0x2112013C

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

} AERTLPPLOG2_IIO_IAX_STRUCT;

/* AERTLPPLOG3_IIO_IAX_REG supported on:                                        */
/*      SPRA0 (0x20810140)                                                      */
/*      SPRB0 (0x20810140)                                                      */
/*      SPRHBM (0x20810140)                                                     */
/*      SPRC0 (0x20810140)                                                      */
/*      SPRMCC (0x20810140)                                                     */
/*      SPRUCC (0x20810140)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register captures the End-End TLP Prefix (DW3) for the TLP corresponding to the detected error.
*/


#define AERTLPPLOG3_IIO_IAX_REG 0x21120140

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

} AERTLPPLOG3_IIO_IAX_STRUCT;

/* AERTLPPLOG4_IIO_IAX_REG supported on:                                        */
/*      SPRA0 (0x20810144)                                                      */
/*      SPRB0 (0x20810144)                                                      */
/*      SPRHBM (0x20810144)                                                     */
/*      SPRC0 (0x20810144)                                                      */
/*      SPRMCC (0x20810144)                                                     */
/*      SPRUCC (0x20810144)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register captures the End-End TLP Prefix (DW4) for the TLP corresponding to the detected error.
*/


#define AERTLPPLOG4_IIO_IAX_REG 0x21120144

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

} AERTLPPLOG4_IIO_IAX_STRUCT;

/* LTREXTCAP_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810150)                                                      */
/*      SPRB0 (0x20810150)                                                      */
/*      SPRHBM (0x20810150)                                                     */
/*      SPRC0 (0x20810150)                                                      */
/*      SPRMCC (0x20810150)                                                     */
/*      SPRUCC (0x20810150)                                                     */
/* Register default value on SPRA0: 0x16010018                                  */
/* Register default value on SPRB0: 0x16010018                                  */
/* Register default value on SPRHBM: 0x16010018                                 */
/* Register default value on SPRC0: 0x16010018                                  */
/* Register default value on SPRMCC: 0x16010018                                 */
/* Register default value on SPRUCC: 0x16010018                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header
*/


#define LTREXTCAP_IIO_IAX_REG 0x21120150

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

} LTREXTCAP_IIO_IAX_STRUCT;

/* MAXSNPLAT_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x10810154)                                                      */
/*      SPRB0 (0x10810154)                                                      */
/*      SPRHBM (0x10810154)                                                     */
/*      SPRC0 (0x10810154)                                                      */
/*      SPRMCC (0x10810154)                                                     */
/*      SPRUCC (0x10810154)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Maximum Snoop Latency the function is permitted to request
*/


#define MAXSNPLAT_IIO_IAX_REG 0x21110154

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

} MAXSNPLAT_IIO_IAX_STRUCT;

/* MAXNSNPLAT_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x10810156)                                                      */
/*      SPRB0 (0x10810156)                                                      */
/*      SPRHBM (0x10810156)                                                     */
/*      SPRC0 (0x10810156)                                                      */
/*      SPRMCC (0x10810156)                                                     */
/*      SPRUCC (0x10810156)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Maximum No-Snoop Latency the function is permitted to request
*/


#define MAXNSNPLAT_IIO_IAX_REG 0x21110156

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

} MAXNSNPLAT_IIO_IAX_STRUCT;

/* TPHEXTCAP_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810160)                                                      */
/*      SPRB0 (0x20810160)                                                      */
/*      SPRHBM (0x20810160)                                                     */
/*      SPRC0 (0x20810160)                                                      */
/*      SPRMCC (0x20810160)                                                     */
/*      SPRUCC (0x20810160)                                                     */
/* Register default value on SPRA0: 0x17010017                                  */
/* Register default value on SPRB0: 0x17010017                                  */
/* Register default value on SPRHBM: 0x17010017                                 */
/* Register default value on SPRC0: 0x17010017                                  */
/* Register default value on SPRMCC: 0x17010017                                 */
/* Register default value on SPRUCC: 0x17010017                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header
*/


#define TPHEXTCAP_IIO_IAX_REG 0x21120160

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

} TPHEXTCAP_IIO_IAX_STRUCT;

/* TPHCAP_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x20810164)                                                      */
/*      SPRB0 (0x20810164)                                                      */
/*      SPRHBM (0x20810164)                                                     */
/*      SPRC0 (0x20810164)                                                      */
/*      SPRMCC (0x20810164)                                                     */
/*      SPRUCC (0x20810164)                                                     */
/* Register default value on SPRA0: 0x00010205                                  */
/* Register default value on SPRB0: 0x00010205                                  */
/* Register default value on SPRHBM: 0x00010205                                 */
/* Register default value on SPRC0: 0x00010205                                  */
/* Register default value on SPRMCC: 0x00010205                                 */
/* Register default value on SPRUCC: 0x00010205                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* TPH Capabilities (7.9.13.2)
*/


#define TPHCAP_IIO_IAX_REG 0x21120164

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

} TPHCAP_IIO_IAX_STRUCT;

/* TPHCTL_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x20810168)                                                      */
/*      SPRB0 (0x20810168)                                                      */
/*      SPRHBM (0x20810168)                                                     */
/*      SPRC0 (0x20810168)                                                      */
/*      SPRMCC (0x20810168)                                                     */
/*      SPRUCC (0x20810168)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* TPH Requester Capabilities (7.9.13.3)
*/


#define TPHCTL_IIO_IAX_REG 0x21120168

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

} TPHCTL_IIO_IAX_STRUCT;

/* TPHSTTBL0_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x1081016c)                                                      */
/*      SPRB0 (0x1081016c)                                                      */
/*      SPRHBM (0x1081016c)                                                     */
/*      SPRC0 (0x1081016c)                                                      */
/*      SPRMCC (0x1081016c)                                                     */
/*      SPRUCC (0x1081016c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* TPH ST Table (7.9.13.3)
*/


#define TPHSTTBL0_IIO_IAX_REG 0x2111016C

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

} TPHSTTBL0_IIO_IAX_STRUCT;

/* TPHSTTBL1_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x1081016e)                                                      */
/*      SPRB0 (0x1081016e)                                                      */
/*      SPRHBM (0x1081016e)                                                     */
/*      SPRC0 (0x1081016e)                                                      */
/*      SPRMCC (0x1081016e)                                                     */
/*      SPRUCC (0x1081016e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* TPH ST Table (7.9.13.3)
*/


#define TPHSTTBL1_IIO_IAX_REG 0x2111016E

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

} TPHSTTBL1_IIO_IAX_STRUCT;

/* VCEXTCAP_IIO_IAX_REG supported on:                                           */
/*      SPRA0 (0x20810170)                                                      */
/*      SPRB0 (0x20810170)                                                      */
/*      SPRHBM (0x20810170)                                                     */
/*      SPRC0 (0x20810170)                                                      */
/*      SPRMCC (0x20810170)                                                     */
/*      SPRUCC (0x20810170)                                                     */
/* Register default value on SPRA0: 0x20010002                                  */
/* Register default value on SPRB0: 0x20010002                                  */
/* Register default value on SPRHBM: 0x20010002                                 */
/* Register default value on SPRC0: 0x20010002                                  */
/* Register default value on SPRMCC: 0x20010002                                 */
/* Register default value on SPRUCC: 0x20010002                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Virtual Channel Extended Capability Header (Section 7.9.1.1)
*/


#define VCEXTCAP_IIO_IAX_REG 0x21120170

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

} VCEXTCAP_IIO_IAX_STRUCT;

/* PORTVCCAP1_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x20810174)                                                      */
/*      SPRB0 (0x20810174)                                                      */
/*      SPRHBM (0x20810174)                                                     */
/*      SPRC0 (0x20810174)                                                      */
/*      SPRMCC (0x20810174)                                                     */
/*      SPRUCC (0x20810174)                                                     */
/* Register default value on SPRA0: 0x00000011                                  */
/* Register default value on SPRB0: 0x00000011                                  */
/* Register default value on SPRHBM: 0x00000011                                 */
/* Register default value on SPRC0: 0x00000011                                  */
/* Register default value on SPRMCC: 0x00000011                                 */
/* Register default value on SPRUCC: 0x00000011                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Port VC Capability Register 1 (Section 7.9.1.2)
*/


#define PORTVCCAP1_IIO_IAX_REG 0x21120174

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

} PORTVCCAP1_IIO_IAX_STRUCT;

/* PORTVCCAP2_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x20810178)                                                      */
/*      SPRB0 (0x20810178)                                                      */
/*      SPRHBM (0x20810178)                                                     */
/*      SPRC0 (0x20810178)                                                      */
/*      SPRMCC (0x20810178)                                                     */
/*      SPRUCC (0x20810178)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Port VC Capability Register 2 (Section 7.9.1.3)
*/


#define PORTVCCAP2_IIO_IAX_REG 0x21120178

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

} PORTVCCAP2_IIO_IAX_STRUCT;

/* PORTVCCTL_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x1081017c)                                                      */
/*      SPRB0 (0x1081017c)                                                      */
/*      SPRHBM (0x1081017c)                                                     */
/*      SPRC0 (0x1081017c)                                                      */
/*      SPRMCC (0x1081017c)                                                     */
/*      SPRUCC (0x1081017c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Port VC Control Register (Section 7.9.1.4)
*/


#define PORTVCCTL_IIO_IAX_REG 0x2111017C

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

} PORTVCCTL_IIO_IAX_STRUCT;

/* PORTVCSTS_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x1081017e)                                                      */
/*      SPRB0 (0x1081017e)                                                      */
/*      SPRHBM (0x1081017e)                                                     */
/*      SPRC0 (0x1081017e)                                                      */
/*      SPRMCC (0x1081017e)                                                     */
/*      SPRUCC (0x1081017e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Port VC Status Register (Section 7.9.1.5)
*/


#define PORTVCSTS_IIO_IAX_REG 0x2111017E

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

} PORTVCSTS_IIO_IAX_STRUCT;

/* VC0CAP_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x20810180)                                                      */
/*      SPRB0 (0x20810180)                                                      */
/*      SPRHBM (0x20810180)                                                     */
/*      SPRC0 (0x20810180)                                                      */
/*      SPRMCC (0x20810180)                                                     */
/*      SPRUCC (0x20810180)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VC Resource Capability Register (Section 7.9.1.6)
*/


#define VC0CAP_IIO_IAX_REG 0x21120180

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

} VC0CAP_IIO_IAX_STRUCT;

/* VC0CTL_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x20810184)                                                      */
/*      SPRB0 (0x20810184)                                                      */
/*      SPRHBM (0x20810184)                                                     */
/*      SPRC0 (0x20810184)                                                      */
/*      SPRMCC (0x20810184)                                                     */
/*      SPRUCC (0x20810184)                                                     */
/* Register default value on SPRA0: 0x80000001                                  */
/* Register default value on SPRB0: 0x80000001                                  */
/* Register default value on SPRHBM: 0x80000001                                 */
/* Register default value on SPRC0: 0x80000001                                  */
/* Register default value on SPRMCC: 0x80000001                                 */
/* Register default value on SPRUCC: 0x80000001                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VC Resource Control Register (Section 7.9.1.7)
*/


#define VC0CTL_IIO_IAX_REG 0x21120184

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

                            /* This field assigns a VC ID to the VC resource. */
    UINT32 rsvd_27 : 4;

                            /* Bits[30:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vcen : 1;

                            /* Bits[31:31], Access Type=RO, default=0x00000001*/

                            /* This bit, when Set, enables a Virtual Channel. */

  } Bits;
  UINT32 Data;

} VC0CTL_IIO_IAX_STRUCT;

/* VC0STS_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x1081018a)                                                      */
/*      SPRB0 (0x1081018a)                                                      */
/*      SPRHBM (0x1081018a)                                                     */
/*      SPRC0 (0x1081018a)                                                      */
/*      SPRMCC (0x1081018a)                                                     */
/*      SPRUCC (0x1081018a)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VC Resource Status Register (Section 7.9.1.8)
*/


#define VC0STS_IIO_IAX_REG 0x2111018A

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

} VC0STS_IIO_IAX_STRUCT;

/* VC1CAP_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x2081018c)                                                      */
/*      SPRB0 (0x2081018c)                                                      */
/*      SPRHBM (0x2081018c)                                                     */
/*      SPRC0 (0x2081018c)                                                      */
/*      SPRMCC (0x2081018c)                                                     */
/*      SPRUCC (0x2081018c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VC Resource Capability Register (Section 7.9.1.6)
*/


#define VC1CAP_IIO_IAX_REG 0x2112018C

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

} VC1CAP_IIO_IAX_STRUCT;

/* VC1CTL_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x20810190)                                                      */
/*      SPRB0 (0x20810190)                                                      */
/*      SPRHBM (0x20810190)                                                     */
/*      SPRC0 (0x20810190)                                                      */
/*      SPRMCC (0x20810190)                                                     */
/*      SPRUCC (0x20810190)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VC Resource Control Register (Section 7.9.1.7)
*/


#define VC1CTL_IIO_IAX_REG 0x21120190

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
                               the VC resource.
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

} VC1CTL_IIO_IAX_STRUCT;

/* VC1STS_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x10810196)                                                      */
/*      SPRB0 (0x10810196)                                                      */
/*      SPRHBM (0x10810196)                                                     */
/*      SPRC0 (0x10810196)                                                      */
/*      SPRMCC (0x10810196)                                                     */
/*      SPRUCC (0x10810196)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* VC Resource Status Register (Section 7.9.1.8)
*/


#define VC1STS_IIO_IAX_REG 0x21110196

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

} VC1STS_IIO_IAX_STRUCT;

/* SIOVEXTCAP_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x20810200)                                                      */
/*      SPRB0 (0x20810200)                                                      */
/*      SPRHBM (0x20810200)                                                     */
/*      SPRC0 (0x20810200)                                                      */
/*      SPRMCC (0x20810200)                                                     */
/*      SPRUCC (0x20810200)                                                     */
/* Register default value on SPRA0: 0x22010023                                  */
/* Register default value on SPRB0: 0x22010023                                  */
/* Register default value on SPRHBM: 0x22010023                                 */
/* Register default value on SPRC0: 0x22010023                                  */
/* Register default value on SPRMCC: 0x22010023                                 */
/* Register default value on SPRUCC: 0x22010023                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header (7.9.6.1)
*/


#define SIOVEXTCAP_IIO_IAX_REG 0x21120200

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

} SIOVEXTCAP_IIO_IAX_STRUCT;

/* SIOVDVSEC1_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x20810204)                                                      */
/*      SPRB0 (0x20810204)                                                      */
/*      SPRHBM (0x20810204)                                                     */
/*      SPRC0 (0x20810204)                                                      */
/*      SPRMCC (0x20810204)                                                     */
/*      SPRUCC (0x20810204)                                                     */
/* Register default value on SPRA0: 0x01808086                                  */
/* Register default value on SPRB0: 0x01808086                                  */
/* Register default value on SPRHBM: 0x01808086                                 */
/* Register default value on SPRC0: 0x01808086                                  */
/* Register default value on SPRMCC: 0x01808086                                 */
/* Register default value on SPRUCC: 0x01808086                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Designated Vendor-Specific Header 1 (7.9.6.2)
*/


#define SIOVDVSEC1_IIO_IAX_REG 0x21120204

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

} SIOVDVSEC1_IIO_IAX_STRUCT;

/* SIOVDVSEC2_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x10810208)                                                      */
/*      SPRB0 (0x10810208)                                                      */
/*      SPRHBM (0x10810208)                                                     */
/*      SPRC0 (0x10810208)                                                      */
/*      SPRMCC (0x10810208)                                                     */
/*      SPRUCC (0x10810208)                                                     */
/* Register default value on SPRA0: 0x00000005                                  */
/* Register default value on SPRB0: 0x00000005                                  */
/* Register default value on SPRHBM: 0x00000005                                 */
/* Register default value on SPRC0: 0x00000005                                  */
/* Register default value on SPRMCC: 0x00000005                                 */
/* Register default value on SPRUCC: 0x00000005                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Designated Vendor-Specific Header 2 (7.9.6.3)
*/


#define SIOVDVSEC2_IIO_IAX_REG 0x21110208

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

} SIOVDVSEC2_IIO_IAX_STRUCT;

/* SIOVFDL_IIO_IAX_REG supported on:                                            */
/*      SPRA0 (0x81020a)                                                        */
/*      SPRB0 (0x81020a)                                                        */
/*      SPRHBM (0x81020a)                                                       */
/*      SPRC0 (0x81020a)                                                        */
/*      SPRMCC (0x81020a)                                                       */
/*      SPRUCC (0x81020a)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* See Scalable IOV Arch Spec (Section 3.13)
*/


#define SIOVFDL_IIO_IAX_REG 0x2110020A

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

} SIOVFDL_IIO_IAX_STRUCT;

/* SIOVFLAGS_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x81020b)                                                        */
/*      SPRB0 (0x81020b)                                                        */
/*      SPRHBM (0x81020b)                                                       */
/*      SPRC0 (0x81020b)                                                        */
/*      SPRMCC (0x81020b)                                                       */
/*      SPRUCC (0x81020b)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* See Scalable IOV Arch Spec (Section 3.13)
*/


#define SIOVFLAGS_IIO_IAX_REG 0x2110020B

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

} SIOVFLAGS_IIO_IAX_STRUCT;

/* SIOVSUPPGSZ_IIO_IAX_REG supported on:                                        */
/*      SPRA0 (0x2081020c)                                                      */
/*      SPRB0 (0x2081020c)                                                      */
/*      SPRHBM (0x2081020c)                                                     */
/*      SPRC0 (0x2081020c)                                                      */
/*      SPRMCC (0x2081020c)                                                     */
/*      SPRUCC (0x2081020c)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* See Scalable IOV Arch Spec (Section 3.13)
*/


#define SIOVSUPPGSZ_IIO_IAX_REG 0x2112020C

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

} SIOVSUPPGSZ_IIO_IAX_STRUCT;

/* SIOVSYSPGSZ_IIO_IAX_REG supported on:                                        */
/*      SPRA0 (0x20810210)                                                      */
/*      SPRB0 (0x20810210)                                                      */
/*      SPRHBM (0x20810210)                                                     */
/*      SPRC0 (0x20810210)                                                      */
/*      SPRMCC (0x20810210)                                                     */
/*      SPRUCC (0x20810210)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* See Scalable IOV Arch Spec (Section 3.13)
*/


#define SIOVSYSPGSZ_IIO_IAX_REG 0x21120210

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

} SIOVSYSPGSZ_IIO_IAX_STRUCT;

/* SIOVCAP_IIO_IAX_REG supported on:                                            */
/*      SPRA0 (0x20810214)                                                      */
/*      SPRB0 (0x20810214)                                                      */
/*      SPRHBM (0x20810214)                                                     */
/*      SPRC0 (0x20810214)                                                      */
/*      SPRMCC (0x20810214)                                                     */
/*      SPRUCC (0x20810214)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* See Scalable IOV Arch Spec (Section 3.13)
*/


#define SIOVCAP_IIO_IAX_REG 0x21120214

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

} SIOVCAP_IIO_IAX_STRUCT;

/* ATSEXTCAP_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810220)                                                      */
/*      SPRB0 (0x20810220)                                                      */
/*      SPRHBM (0x20810220)                                                     */
/*      SPRC0 (0x20810220)                                                      */
/*      SPRMCC (0x20810220)                                                     */
/*      SPRUCC (0x20810220)                                                     */
/* Register default value on SPRA0: 0x2301000F                                  */
/* Register default value on SPRB0: 0x2301000F                                  */
/* Register default value on SPRHBM: 0x2301000F                                 */
/* Register default value on SPRC0: 0x2301000F                                  */
/* Register default value on SPRMCC: 0x2301000F                                 */
/* Register default value on SPRUCC: 0x2301000F                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header (10.5.1.1)
*/


#define ATSEXTCAP_IIO_IAX_REG 0x21120220

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

} ATSEXTCAP_IIO_IAX_STRUCT;

/* ATSCAP_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x10810224)                                                      */
/*      SPRB0 (0x10810224)                                                      */
/*      SPRHBM (0x10810224)                                                     */
/*      SPRC0 (0x10810224)                                                      */
/*      SPRMCC (0x10810224)                                                     */
/*      SPRUCC (0x10810224)                                                     */
/* Register default value on SPRA0: 0x00000060                                  */
/* Register default value on SPRB0: 0x00000060                                  */
/* Register default value on SPRHBM: 0x00000060                                 */
/* Register default value on SPRC0: 0x00000060                                  */
/* Register default value on SPRMCC: 0x00000060                                 */
/* Register default value on SPRUCC: 0x00000060                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ATS Capabilities (10.5.1.2)
*/


#define ATSCAP_IIO_IAX_REG 0x21110224

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

} ATSCAP_IIO_IAX_STRUCT;

/* ATSCTL_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x10810226)                                                      */
/*      SPRB0 (0x10810226)                                                      */
/*      SPRHBM (0x10810226)                                                     */
/*      SPRC0 (0x10810226)                                                      */
/*      SPRMCC (0x10810226)                                                     */
/*      SPRUCC (0x10810226)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* ATS Controls (10.5.1.3)
*/


#define ATSCTL_IIO_IAX_REG 0x21110226

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

} ATSCTL_IIO_IAX_STRUCT;

/* PASIDEXTCAP_IIO_IAX_REG supported on:                                        */
/*      SPRA0 (0x20810230)                                                      */
/*      SPRB0 (0x20810230)                                                      */
/*      SPRHBM (0x20810230)                                                     */
/*      SPRC0 (0x20810230)                                                      */
/*      SPRMCC (0x20810230)                                                     */
/*      SPRUCC (0x20810230)                                                     */
/* Register default value on SPRA0: 0x2401001B                                  */
/* Register default value on SPRB0: 0x2401001B                                  */
/* Register default value on SPRHBM: 0x2401001B                                 */
/* Register default value on SPRC0: 0x2401001B                                  */
/* Register default value on SPRMCC: 0x2401001B                                 */
/* Register default value on SPRUCC: 0x2401001B                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header
*/


#define PASIDEXTCAP_IIO_IAX_REG 0x21120230

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

} PASIDEXTCAP_IIO_IAX_STRUCT;

/* PASIDCAP_IIO_IAX_REG supported on:                                           */
/*      SPRA0 (0x10810234)                                                      */
/*      SPRB0 (0x10810234)                                                      */
/*      SPRHBM (0x10810234)                                                     */
/*      SPRC0 (0x10810234)                                                      */
/*      SPRMCC (0x10810234)                                                     */
/*      SPRUCC (0x10810234)                                                     */
/* Register default value on SPRA0: 0x00001404                                  */
/* Register default value on SPRB0: 0x00001404                                  */
/* Register default value on SPRHBM: 0x00001404                                 */
/* Register default value on SPRC0: 0x00001404                                  */
/* Register default value on SPRMCC: 0x00001404                                 */
/* Register default value on SPRUCC: 0x00001404                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PASID-related capabilities
*/


#define PASIDCAP_IIO_IAX_REG 0x21110234

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

} PASIDCAP_IIO_IAX_STRUCT;

/* PASIDCTL_IIO_IAX_REG supported on:                                           */
/*      SPRA0 (0x10810236)                                                      */
/*      SPRB0 (0x10810236)                                                      */
/*      SPRHBM (0x10810236)                                                     */
/*      SPRC0 (0x10810236)                                                      */
/*      SPRMCC (0x10810236)                                                     */
/*      SPRUCC (0x10810236)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls for PASID-related functionality
*/


#define PASIDCTL_IIO_IAX_REG 0x21110236

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

} PASIDCTL_IIO_IAX_STRUCT;

/* PRSEXTCAP_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810240)                                                      */
/*      SPRB0 (0x20810240)                                                      */
/*      SPRHBM (0x20810240)                                                     */
/*      SPRC0 (0x20810240)                                                      */
/*      SPRMCC (0x20810240)                                                     */
/*      SPRUCC (0x20810240)                                                     */
/* Register default value on SPRA0: 0x00010013                                  */
/* Register default value on SPRB0: 0x00010013                                  */
/* Register default value on SPRHBM: 0x00010013                                 */
/* Register default value on SPRC0: 0x00010013                                  */
/* Register default value on SPRMCC: 0x00010013                                 */
/* Register default value on SPRUCC: 0x00010013                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Extended Capability Header (10.5.2.1)
*/


#define PRSEXTCAP_IIO_IAX_REG 0x21120240

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

} PRSEXTCAP_IIO_IAX_STRUCT;

/* PRSCTL_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x10810244)                                                      */
/*      SPRB0 (0x10810244)                                                      */
/*      SPRHBM (0x10810244)                                                     */
/*      SPRC0 (0x10810244)                                                      */
/*      SPRMCC (0x10810244)                                                     */
/*      SPRUCC (0x10810244)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls for Page Request activities (10.5.2.2)
*/


#define PRSCTL_IIO_IAX_REG 0x21110244

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

} PRSCTL_IIO_IAX_STRUCT;

/* PRSSTS_IIO_IAX_REG supported on:                                             */
/*      SPRA0 (0x10810246)                                                      */
/*      SPRB0 (0x10810246)                                                      */
/*      SPRHBM (0x10810246)                                                     */
/*      SPRC0 (0x10810246)                                                      */
/*      SPRMCC (0x10810246)                                                     */
/*      SPRUCC (0x10810246)                                                     */
/* Register default value on SPRA0: 0x00008100                                  */
/* Register default value on SPRB0: 0x00008100                                  */
/* Register default value on SPRHBM: 0x00008100                                 */
/* Register default value on SPRC0: 0x00008100                                  */
/* Register default value on SPRMCC: 0x00008100                                 */
/* Register default value on SPRUCC: 0x00008100                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Status of Page Requests (10.5.2.3)
*/


#define PRSSTS_IIO_IAX_REG 0x21110246

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

} PRSSTS_IIO_IAX_STRUCT;

/* PRSREQCAP_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810248)                                                      */
/*      SPRB0 (0x20810248)                                                      */
/*      SPRHBM (0x20810248)                                                     */
/*      SPRC0 (0x20810248)                                                      */
/*      SPRMCC (0x20810248)                                                     */
/*      SPRUCC (0x20810248)                                                     */
/* Register default value on SPRA0: 0x00000100                                  */
/* Register default value on SPRB0: 0x00000100                                  */
/* Register default value on SPRHBM: 0x00000100                                 */
/* Register default value on SPRC0: 0x00000100                                  */
/* Register default value on SPRMCC: 0x00000100                                 */
/* Register default value on SPRUCC: 0x00000100                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Maximum Number of Outstanding Page Requests
*/


#define PRSREQCAP_IIO_IAX_REG 0x21120248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cap : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000100*/

                            /* How many Page Requests can the function issue */

  } Bits;
  UINT32 Data;

} PRSREQCAP_IIO_IAX_STRUCT;

/* PRSREQALLOC_IIO_IAX_REG supported on:                                        */
/*      SPRA0 (0x2081024c)                                                      */
/*      SPRB0 (0x2081024c)                                                      */
/*      SPRHBM (0x2081024c)                                                     */
/*      SPRC0 (0x2081024c)                                                      */
/*      SPRMCC (0x2081024c)                                                     */
/*      SPRUCC (0x2081024c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Maximum Number of Outstanding Page Requests Allowed
*/


#define PRSREQALLOC_IIO_IAX_REG 0x2112024C

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

} PRSREQALLOC_IIO_IAX_STRUCT;

/* SETIDVAL_IIO_IAX_REG supported on:                                           */
/*      SPRA0 (0x20810400)                                                      */
/*      SPRB0 (0x20810400)                                                      */
/*      SPRHBM (0x20810400)                                                     */
/*      SPRC0 (0x20810400)                                                      */
/*      SPRMCC (0x20810400)                                                     */
/*      SPRUCC (0x20810400)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Values Captured with SetID Value message
*/


#define SETIDVAL_IIO_IAX_REG 0x21120400

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

} SETIDVAL_IIO_IAX_STRUCT;









/* DEFTR3_N1_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x2081043c)                                                      */
/*      SPRB0 (0x2081043c)                                                      */
/*      SPRHBM (0x2081043c)                                                     */
/*      SPRC0 (0x2081043c)                                                      */
/*      SPRMCC (0x2081043c)                                                     */
/*      SPRUCC (0x2081043c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register disables capabilities advertised in GRPCAP.
*/


#define DEFTR3_N1_IIO_IAX_REG 0x2112043C

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

} DEFTR3_N1_IIO_IAX_STRUCT;


/* DEFTR4_N1_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810444)                                                      */
/*      SPRB0 (0x20810444)                                                      */
/*      SPRHBM (0x20810444)                                                     */
/*      SPRC0 (0x20810444)                                                      */
/*      SPRMCC (0x20810444)                                                     */
/*      SPRUCC (0x20810444)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register disables capabilities advertised in ENGCAP.
*/


#define DEFTR4_N1_IIO_IAX_REG 0x21120444

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

} DEFTR4_N1_IIO_IAX_STRUCT;














/* DEFTR11_N1_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x2081047c)                                                      */
/*      SPRB0 (0x2081047c)                                                      */
/*      SPRHBM (0x2081047c)                                                     */
/*      SPRC0 (0x2081047c)                                                      */
/*      SPRMCC (0x2081047c)                                                     */
/*      SPRUCC (0x2081047c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register overrides capabilities advertised in EVNTCAP_0.
*/


#define DEFTR11_N1_IIO_IAX_REG 0x2112047C

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

} DEFTR11_N1_IIO_IAX_STRUCT;


/* DEFTR12_N1_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x20810484)                                                      */
/*      SPRB0 (0x20810484)                                                      */
/*      SPRHBM (0x20810484)                                                     */
/*      SPRC0 (0x20810484)                                                      */
/*      SPRMCC (0x20810484)                                                     */
/*      SPRUCC (0x20810484)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register overrides capabilities advertised in EVNTCAP_1.
*/


#define DEFTR12_N1_IIO_IAX_REG 0x21120484

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

} DEFTR12_N1_IIO_IAX_STRUCT;


/* DEFTR13_N1_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x2081048c)                                                      */
/*      SPRB0 (0x2081048c)                                                      */
/*      SPRHBM (0x2081048c)                                                     */
/*      SPRC0 (0x2081048c)                                                      */
/*      SPRMCC (0x2081048c)                                                     */
/*      SPRUCC (0x2081048c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register overrides capabilities advertised in EVNTCAP_2.
*/


#define DEFTR13_N1_IIO_IAX_REG 0x2112048C

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

} DEFTR13_N1_IIO_IAX_STRUCT;


/* DEFTR14_N1_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x20810494)                                                      */
/*      SPRB0 (0x20810494)                                                      */
/*      SPRHBM (0x20810494)                                                     */
/*      SPRC0 (0x20810494)                                                      */
/*      SPRMCC (0x20810494)                                                     */
/*      SPRUCC (0x20810494)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register overrides capabilities advertised in EVNTCAP_3.
*/


#define DEFTR14_N1_IIO_IAX_REG 0x21120494

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

} DEFTR14_N1_IIO_IAX_STRUCT;


/* DEFTR15_N1_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x2081049c)                                                      */
/*      SPRB0 (0x2081049c)                                                      */
/*      SPRHBM (0x2081049c)                                                     */
/*      SPRC0 (0x2081049c)                                                      */
/*      SPRMCC (0x2081049c)                                                     */
/*      SPRUCC (0x2081049c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register overrides capabilities advertised in EVNTCAP_4.
*/


#define DEFTR15_N1_IIO_IAX_REG 0x2112049C

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

} DEFTR15_N1_IIO_IAX_STRUCT;


/* DEFTR16_N1_IIO_IAX_REG supported on:                                         */
/*      SPRA0 (0x208104a4)                                                      */
/*      SPRB0 (0x208104a4)                                                      */
/*      SPRHBM (0x208104a4)                                                     */
/*      SPRC0 (0x208104a4)                                                      */
/*      SPRMCC (0x208104a4)                                                     */
/*      SPRUCC (0x208104a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: P_U_CODE                                         */
/* SPRB0 Security PolicyGroup: P_U_CODE                                         */
/* SPRHBM Security PolicyGroup: P_U_CODE                                        */
/* SPRC0 Security PolicyGroup: P_U_CODE                                         */
/* SPRMCC Security PolicyGroup: P_U_CODE                                        */
/* SPRUCC Security PolicyGroup: P_U_CODE                                        */
/* This defeature register overrides IOSF channel (CHID) used for different types of traffic.  This register currently does not have any effect on the hardware.  It has been provided to allow functionality to be added in the future if time permits.
*/


#define DEFTR16_N1_IIO_IAX_REG 0x211204A4

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

} DEFTR16_N1_IIO_IAX_STRUCT;















/* OSWRAC_N0_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810618)                                                      */
/*      SPRB0 (0x20810618)                                                      */
/*      SPRHBM (0x20810618)                                                     */
/*      SPRC0 (0x20810618)                                                      */
/*      SPRMCC (0x20810618)                                                     */
/*      SPRUCC (0x20810618)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls read accesses to registers in the OS_W policy group
*/


#define OSWRAC_N0_IIO_IAX_REG 0x21120618

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

} OSWRAC_N0_IIO_IAX_STRUCT;

/* OSWRAC_N1_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x2081061c)                                                      */
/*      SPRB0 (0x2081061c)                                                      */
/*      SPRHBM (0x2081061c)                                                     */
/*      SPRC0 (0x2081061c)                                                      */
/*      SPRMCC (0x2081061c)                                                     */
/*      SPRUCC (0x2081061c)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls read accesses to registers in the OS_W policy group
*/


#define OSWRAC_N1_IIO_IAX_REG 0x2112061C

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

} OSWRAC_N1_IIO_IAX_STRUCT;

/* OSWWAC_N0_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810620)                                                      */
/*      SPRB0 (0x20810620)                                                      */
/*      SPRHBM (0x20810620)                                                     */
/*      SPRC0 (0x20810620)                                                      */
/*      SPRMCC (0x20810620)                                                     */
/*      SPRUCC (0x20810620)                                                     */
/* Register default value on SPRA0: 0x0300021F                                  */
/* Register default value on SPRB0: 0x0300021F                                  */
/* Register default value on SPRHBM: 0x0300021F                                 */
/* Register default value on SPRC0: 0x0300021F                                  */
/* Register default value on SPRMCC: 0x0300021F                                 */
/* Register default value on SPRUCC: 0x0300021F                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls write accesses to registers in the OS_W policy group
*/


#define OSWWAC_N0_IIO_IAX_REG 0x21120620

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

} OSWWAC_N0_IIO_IAX_STRUCT;

/* OSWWAC_N1_IIO_IAX_REG supported on:                                          */
/*      SPRA0 (0x20810624)                                                      */
/*      SPRB0 (0x20810624)                                                      */
/*      SPRHBM (0x20810624)                                                     */
/*      SPRC0 (0x20810624)                                                      */
/*      SPRMCC (0x20810624)                                                     */
/*      SPRUCC (0x20810624)                                                     */
/* Register default value on SPRA0: 0x20000C00                                  */
/* Register default value on SPRB0: 0x20000C00                                  */
/* Register default value on SPRHBM: 0x20000C00                                 */
/* Register default value on SPRC0: 0x20000C00                                  */
/* Register default value on SPRMCC: 0x20000C00                                 */
/* Register default value on SPRUCC: 0x20000C00                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls write accesses to registers in the OS_W policy group
*/


#define OSWWAC_N1_IIO_IAX_REG 0x21120624

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

} OSWWAC_N1_IIO_IAX_STRUCT;

/* OSWCP_N0_IIO_IAX_REG supported on:                                           */
/*      SPRA0 (0x20810628)                                                      */
/*      SPRB0 (0x20810628)                                                      */
/*      SPRHBM (0x20810628)                                                     */
/*      SPRC0 (0x20810628)                                                      */
/*      SPRMCC (0x20810628)                                                     */
/*      SPRUCC (0x20810628)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls read/write accesses to OS_W RAC, WAC, and CP registers
*/


#define OSWCP_N0_IIO_IAX_REG 0x21120628

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

} OSWCP_N0_IIO_IAX_STRUCT;

/* OSWCP_N1_IIO_IAX_REG supported on:                                           */
/*      SPRA0 (0x2081062c)                                                      */
/*      SPRB0 (0x2081062c)                                                      */
/*      SPRHBM (0x2081062c)                                                     */
/*      SPRC0 (0x2081062c)                                                      */
/*      SPRMCC (0x2081062c)                                                     */
/*      SPRUCC (0x2081062c)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRB0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRHBM Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRC0 Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                    */
/* SPRMCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* SPRUCC Register File:    sprsp_top/iax_top[0]/iax_cfg_inst                   */
/* Struct generated from SPRA0 BDF: 8_2_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Controls read/write accesses to OS_W RAC, WAC, and CP registers
*/


#define OSWCP_N1_IIO_IAX_REG 0x2112062C

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

} OSWCP_N1_IIO_IAX_STRUCT;






#endif /* _IIO_IAX_h */
