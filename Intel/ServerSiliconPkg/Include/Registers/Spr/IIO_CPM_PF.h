
/** @file
  IIO_CPM_PF.h

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
/* CPM_CONFIG_SPACE                                                             */
/*  Security_ReadAccess_Str:                                                    */
/*    DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | DFX_INTEL_MANUFACTURING_SAI | */
/*    PM_PCS_SAI | HOSTIA_BOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI |      */
/*    HOSTIA_UCODE_SAI | HOSTIA_POSTBOOT_SAI | OOB_MSM_SAI                      */
/*  Security_WriteAccess_Str:                                                   */
/*    DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | DFX_INTEL_MANUFACTURING_SAI | */
/*    PM_PCS_SAI | HOSTIA_BOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI |      */
/*    HOSTIA_UCODE_SAI | HOSTIA_POSTBOOT_SAI | OOB_MSM_SAI                      */
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
/* CPM_CONFIG_SPACE                                                             */
/*  Security_ReadAccess_Str:                                                    */
/*    DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | DFX_INTEL_MANUFACTURING_SAI | */
/*    PM_PCS_SAI | HOSTIA_BOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI |      */
/*    HOSTIA_UCODE_SAI | HOSTIA_POSTBOOT_SAI | OOB_MSM_SAI                      */
/*  Security_WriteAccess_Str:                                                   */
/*    DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | DFX_INTEL_MANUFACTURING_SAI | */
/*    PM_PCS_SAI | HOSTIA_BOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI |      */
/*    HOSTIA_UCODE_SAI | HOSTIA_POSTBOOT_SAI | OOB_MSM_SAI                      */
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
/* CPM_CONFIG_SPACE                                                             */
/*  Security_ReadAccess_Str:                                                    */
/*    DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | DFX_INTEL_MANUFACTURING_SAI | */
/*    PM_PCS_SAI | HOSTIA_BOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI |      */
/*    HOSTIA_UCODE_SAI | HOSTIA_POSTBOOT_SAI | OOB_MSM_SAI                      */
/*  Security_WriteAccess_Str:                                                   */
/*    DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | DFX_INTEL_MANUFACTURING_SAI | */
/*    PM_PCS_SAI | HOSTIA_BOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI |      */
/*    HOSTIA_UCODE_SAI | HOSTIA_POSTBOOT_SAI | OOB_MSM_SAI                      */
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
/* CPM_CONFIG_SPACE                                                             */
/*  Security_ReadAccess_Str:                                                    */
/*    DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | DFX_INTEL_MANUFACTURING_SAI | */
/*    PM_PCS_SAI | HOSTIA_BOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI |      */
/*    HOSTIA_UCODE_SAI | HOSTIA_POSTBOOT_SAI | OOB_MSM_SAI                      */
/*  Security_WriteAccess_Str:                                                   */
/*    DFX_INTEL_PRODUCTION_SAI | UNCORE_PMA_SAI | DFX_INTEL_MANUFACTURING_SAI | */
/*    PM_PCS_SAI | HOSTIA_BOOT_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_SMM_SAI |      */
/*    HOSTIA_UCODE_SAI | HOSTIA_POSTBOOT_SAI | OOB_MSM_SAI                      */
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


#ifndef _IIO_CPM_PF_h
#define _IIO_CPM_PF_h
#include <Base.h>

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PVID_IIO_CPM_PF_REG supported on:                                            */
/*      SPRA0 (0x10d00000)                                                      */
/*      SPRB0 (0x10d00000)                                                      */
/*      SPRC0 (0x10d00000)                                                      */
/*      SPRUCC (0x10d00000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Vendor Identificaion Register
*/


#define PVID_IIO_CPM_PF_REG 0x21210000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 vid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /*
                               Vendor Identification: This register field
                               contains the PCI standard identification for
                               Intel, 8086h.
                            */

  } Bits;
  UINT16 Data;

} PVID_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */



#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PDID_IIO_CPM_PF_REG supported on:                                            */
/*      SPRA0 (0x10d00002)                                                      */
/*      SPRB0 (0x10d00002)                                                      */
/*      SPRC0 (0x10d00002)                                                      */
/*      SPRUCC (0x10d00002)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Device Identification Register
*/


#define PDID_IIO_CPM_PF_REG 0x21210002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 pdid : 16;

                            /* Bits[15:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Device Identification Number. PDID are HCA =
                               0x4940, HCT = 0x4942, HCB = 0x4944. This field
                               is loaded by hardware straps. Default will be
                               based on the hardware straps.
                            */

  } Bits;
  UINT16 Data;

} PDID_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPCICMD_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x10d00004)                                                      */
/*      SPRB0 (0x10d00004)                                                      */
/*      SPRC0 (0x10d00004)                                                      */
/*      SPRUCC (0x10d00004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Device Command Register
*/


#define PPCICMD_IIO_CPM_PF_REG 0x21210004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 io : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               I/O Space Enable: The device does not implement
                               this functionality since it claims no I/O
                               regions. The bit is hardwired to 0.
                            */
    UINT16 mem : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Memory Space Enable: Setting this bit enables
                               access to the memory regions the device claims
                               through its BARs. EP will return unsupported
                               request completion status & error message in
                               response to memory transactions it receives when
                               this bit is clear.
                            */
    UINT16 bm : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bus Master Enable: Controls the ability of
                               EP(HI) to issue Memory Read/Write
                               Requests.Clearing (0) this bit prevents EP(HI)
                               from issuing any Memory Requests. Note that as
                               MSIs are in-band memory writes, disabling the
                               bus master enable bit disables MSI as well. PCIe
                               messages are not affected by this bit. Please
                               refer to for more details on how the EP(HI)
                               handles CPP commands when this bit is clear.
                            */
    UINT16 ss : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Special Cycle Enable: The device does not
                               implement this functionality/Does not apply to
                               PCIe. The bit is hardwired to 0.
                            */
    UINT16 mwe : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Write and Invalidate Enable: The device
                               does not implement this functionality/Does not
                               apply to PCIe. The bit is hardwired to 0.
                            */
    UINT16 vps : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               VGA Palette Snoop Enable: The device does not
                               implement this functionality/Does not apply to
                               PCIe. The bit is hardwired to 0.
                            */
    UINT16 per : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Parity Error Enable: Controls the setting of the
                               Master Data Parity Error bit in the Device
                               Status Register ()The Master Data Parity Error
                               bit is set by the EP if its Parity Error Enable
                               bit is set and either of the following two
                               conditions occurs:If the EP poisons a write
                               request.If the Parity Error Enable bit is
                               cleared, the Master Data Parity Error status bit
                               is never setThe default value of this bit is 0.
                            */
    UINT16 reserved1 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* Reserved/Does not apply to PCIe. */
    UINT16 ser : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               SERR# Enable: When set, this bit enables the
                               non-fatal and fatal errors detected by the EP to
                               be reported to the RC.Note that the error
                               reporting can also be enabled via the PCIe
                               specific bits in the PCIe device control
                               register ()The default value of this bit is 0.
                            */
    UINT16 fbtb : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Fast Back-to-Back Enable: EP(HI) does not
                               implement this functionality and it is not
                               applicable to PCIe devices. The bit is hardwired
                               to 0.
                            */
    UINT16 intd : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Disable: Setting this bit disables
                               generation of INTX messages by the EP(HI).
                               Default value is 1 which disables INTX message
                               generation as INTX is deprecated.
                            */
    UINT16 reserved : 5;

                            /* Bits[15:11], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PPCICMD_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPCISTS_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x10d00006)                                                      */
/*      SPRB0 (0x10d00006)                                                      */
/*      SPRC0 (0x10d00006)                                                      */
/*      SPRUCC (0x10d00006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF device status register
*/


#define PPCISTS_IIO_CPM_PF_REG 0x21210006

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 reserved2 : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT16 intsts : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Status: Indicates that the EP(HI) has
                               transmitted a INTX message and is awaiting
                               servicing. This bit does not includes MSIs
                               generated by the EP(HI).
                            */
    UINT16 cl : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Capabilities List: This bit is hardwired to 1 to
                               indicate that EP(HI) has a capabilities list.
                            */
    UINT16 mc66 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               66 MHz Capable: Does not apply to PCI Express.
                               The bit is hardwired to 0.
                            */
    UINT16 reserved1 : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT16 fb2b : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Fast Back-to-Back Capable: Does not apply to PCI
                               Express. The bit is hardwired to 0.
                            */
    UINT16 mdpe : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Master Data Parity Error Detected: This bit is
                               set by EP(HI), as a Requestor if the Parity
                               Error Enable bit in the Command register is 1b
                               and either of the following two conditions
                               occurs:If the Parity Error Enable bit is 0b,
                               this bit is never set.Default value of this
                               field is 0.
                            */
    UINT16 dst : 2;

                            /* Bits[10:9], Access Type=RO, default=0x00000000*/

                            /*
                               DEVSEL Timing: Does not apply to PCI Express.
                               These bits are hardwired to 0.
                            */
    UINT16 sta : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Signaled Target Abort Status: This bit is set
                               when the device completes a Request using
                               Completer Abort Completion Status. Default value
                               of this field is 0. HI does not support this
                               feature. Bit will always remain 0.
                            */
    UINT16 rta : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Received Target Abort Status: This bit is set
                               when EP(HI), as a Requestor receives a
                               Completion with Completer Abort Completion
                               Status.Default value of this field is 0.
                            */
    UINT16 rma : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Received Master Abort Status: This bit is set
                               when EP(HI), as a Requestor receives a
                               Completion with Unsupported Request Completion
                               Status.Default value of this field is 0.
                            */
    UINT16 sse : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Signaled System Error: This bit is set by the
                               EP(HI) when it sends a ERR_FATAL or ERR_NONFATAL
                               message and the SERR bit in the Device Command
                               register bit is set.Default value of this field
                               is 0.
                            */
    UINT16 dpe : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Detected Parity Error: This bit is set by EP(HI)
                               whenever it receives a Poisoned TLP, regardless
                               of the state the Parity Error Enable bit in the
                               Command register.Default value of this field is
                               0.
                            */

  } Bits;
  UINT16 Data;

} PPCISTS_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PRID_IIO_CPM_PF_REG supported on:                                            */
/*      SPRA0 (0x20d00008)                                                      */
/*      SPRB0 (0x20d00008)                                                      */
/*      SPRC0 (0x20d00008)                                                      */
/*      SPRUCC (0x20d00008)                                                     */
/* Register default value on SPRA0: 0x0B400000                                  */
/* Register default value on SPRB0: 0x0B400000                                  */
/* Register default value on SPRC0: 0x0B400000                                  */
/* Register default value on SPRUCC: 0x0B400000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF revision ID register and PF class code Register
*/


#define PRID_IIO_CPM_PF_REG 0x21220008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ridl : 4;

                            /* Bits[3:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Minor Revision. Incremented for each stepping
                               which does not modify all masks. Reset for each
                               major revision. Value is set from fuses. 00b x0
                               stepping 01b x1 stepping 10b x2 stepping 11b x3
                               stepping
                            */
    UINT32 ridu : 4;

                            /* Bits[7:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               Major Revision. Steppings which require all
                               masks to be regenerated. Value is set from
                               fuses. 00b A stepping 01b: B stepping 10b C
                               stepping 11b D stepping
                            */
    UINT32 cc : 24;

                            /* Bits[31:8], Access Type=RW/O/P, default=0x000B4000*/

                            /*
                               Class Code: This value indicates the base class,
                               subclass, and interface.0B4000h = Base class:
                               Processor, Sub-class Co-processor, no specific
                               register level programming interfaces are
                               defined.
                            */

  } Bits;
  UINT32 Data;

} PRID_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */






#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PCLS_IIO_CPM_PF_REG supported on:                                            */
/*      SPRA0 (0xd0000c)                                                        */
/*      SPRB0 (0xd0000c)                                                        */
/*      SPRC0 (0xd0000c)                                                        */
/*      SPRUCC (0xd0000c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Cache Line size Register
*/


#define PCLS_IIO_CPM_PF_REG 0x2120000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cls : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               Cache Line Size: Not used by PCI Express
                               Endpoint. Left for compatility reason only.
                            */

  } Bits;
  UINT8 Data;

} PCLS_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */



#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PHDR_IIO_CPM_PF_REG supported on:                                            */
/*      SPRA0 (0xd0000e)                                                        */
/*      SPRB0 (0xd0000e)                                                        */
/*      SPRC0 (0xd0000e)                                                        */
/*      SPRUCC (0xd0000e)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Header type Register
*/


#define PHDR_IIO_CPM_PF_REG 0x2120000E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 hdr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               PCI Header Type: The header type of the EP(HI)
                               device.00h (Bit 7 = 0b) indicates a single-
                               function device with standard header layout.
                            */

  } Bits;
  UINT8 Data;

} PHDR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */



#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PARAMLBAR_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x20d00010)                                                      */
/*      SPRB0 (0x20d00010)                                                      */
/*      SPRC0 (0x20d00010)                                                      */
/*      SPRUCC (0x20d00010)                                                     */
/* Register default value on SPRA0: 0x00000004                                  */
/* Register default value on SPRB0: 0x00000004                                  */
/* Register default value on SPRC0: 0x00000004                                  */
/* Register default value on SPRUCC: 0x00000004                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF memory Lower Base Address Register
*/


#define PARAMLBAR_IIO_CPM_PF_REG 0x21220010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mem : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Space Indicator: Hardwired to 0 to
                               identify the region as in memory space.
                            */
    UINT32 typ : 2;

                            /* Bits[2:1], Access Type=RO, default=0x00000002*/

                            /*
                               Addressing Type: Hardwired to indicate a 64-bit
                               region.
                            */
    UINT32 pref : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Prefetchable: Hardwired to 1 to indicate that
                               the region is prefetchable.0: non-prefetchable1:
                               prefetchable
                            */
    UINT32 zero : 15;

                            /* Bits[18:4], Access Type=RO, default=0x00000000*/

                            /*
                               Lower Bits: Hardwired to 0 (8MB region) - the
                               size of this window is equal to aRAM capacity
                            */
    UINT32 addr : 13;

                            /* Bits[31:19], Access Type=RW, default=0x00000000*/

                            /*
                               Lower Programmable Base Address: These bits are
                               set by BIOS to locate the base address of the
                               region.
                            */

  } Bits;
  UINT32 Data;

} PARAMLBAR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PARAMUBAR_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x20d00014)                                                      */
/*      SPRB0 (0x20d00014)                                                      */
/*      SPRC0 (0x20d00014)                                                      */
/*      SPRUCC (0x20d00014)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Memory Upper Base Address Register
*/


#define PARAMUBAR_IIO_CPM_PF_REG 0x21220014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Upper Programmable Base Address: These bits are
                               set by BIOS to locate the base address of the
                               region.
                            */

  } Bits;
  UINT32 Data;

} PARAMUBAR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMISCLBAR_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x20d00018)                                                      */
/*      SPRB0 (0x20d00018)                                                      */
/*      SPRC0 (0x20d00018)                                                      */
/*      SPRUCC (0x20d00018)                                                     */
/* Register default value on SPRA0: 0x00000004                                  */
/* Register default value on SPRB0: 0x00000004                                  */
/* Register default value on SPRC0: 0x00000004                                  */
/* Register default value on SPRUCC: 0x00000004                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Miscellaneous Lower Base Address Register
*/


#define PMISCLBAR_IIO_CPM_PF_REG 0x21220018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mem : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Space Indicator: Hardwired to 0 to
                               identify the region as in memory space.
                            */
    UINT32 typ : 2;

                            /* Bits[2:1], Access Type=RO, default=0x00000002*/

                            /*
                               Addressing Type: Hardwired to indicate a 64-bit
                               region.
                            */
    UINT32 pref : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Prefetchable: Hardwired to 0 to indicate that
                               the region is non-prefetchable.
                            */
    UINT32 zero : 19;

                            /* Bits[22:4], Access Type=RO, default=0x00000000*/

                            /* Lower Bits: Hardwired to 0 (8MB region). */
    UINT32 addr : 9;

                            /* Bits[31:23], Access Type=RW, default=0x00000000*/

                            /*
                               Lower Programmable Base Address: These bits are
                               set by BIOS to locate the base address of the
                               region.
                            */

  } Bits;
  UINT32 Data;

} PMISCLBAR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMISCUBAR_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x20d0001c)                                                      */
/*      SPRB0 (0x20d0001c)                                                      */
/*      SPRC0 (0x20d0001c)                                                      */
/*      SPRUCC (0x20d0001c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Miscellaneous Upper Base Address Register
*/


#define PMISCUBAR_IIO_CPM_PF_REG 0x2122001C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Upper Programmable Base Address: These bits are
                               set by BIOS to locate the base address of the
                               region.
                            */

  } Bits;
  UINT32 Data;

} PMISCUBAR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PWQMRINGCSRLBAR_IIO_CPM_PF_REG supported on:                                 */
/*      SPRA0 (0x20d00020)                                                      */
/*      SPRB0 (0x20d00020)                                                      */
/*      SPRC0 (0x20d00020)                                                      */
/*      SPRUCC (0x20d00020)                                                     */
/* Register default value on SPRA0: 0x00000004                                  */
/* Register default value on SPRB0: 0x00000004                                  */
/* Register default value on SPRC0: 0x00000004                                  */
/* Register default value on SPRUCC: 0x00000004                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF WQM Ring CSR Lower Base Address Register
*/


#define PWQMRINGCSRLBAR_IIO_CPM_PF_REG 0x21220020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mem : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Space Indicator: Hardwired to 0 to
                               identify the region as in memory space.
                            */
    UINT32 typ : 2;

                            /* Bits[2:1], Access Type=RO, default=0x00000002*/

                            /*
                               Addressing Type: Hardwired to indicate a 64-bit
                               region.
                            */
    UINT32 pref : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Prefetchable: Hardwired to 0 to indicate that
                               the region is non-prefetchable.To Be determined
                            */
    UINT32 zero : 17;

                            /* Bits[20:4], Access Type=RO, default=0x00000000*/

                            /* Lower Bits: Hardwired to 0 (1MB region). */
    UINT32 addr : 11;

                            /* Bits[31:21], Access Type=RW, default=0x00000000*/

                            /*
                               Lower Programmable Base Address: These bits are
                               set by BIOS to locate the base address of the
                               region.
                            */

  } Bits;
  UINT32 Data;

} PWQMRINGCSRLBAR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PWQMRINGCSRUBAR_IIO_CPM_PF_REG supported on:                                 */
/*      SPRA0 (0x20d00024)                                                      */
/*      SPRB0 (0x20d00024)                                                      */
/*      SPRC0 (0x20d00024)                                                      */
/*      SPRUCC (0x20d00024)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF WQM Ring CSR Upper Base Address Register
*/


#define PWQMRINGCSRUBAR_IIO_CPM_PF_REG 0x21220024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Upper Programmable Base Address: These bits are
                               set by BIOS to locate the base address of the
                               region.
                            */

  } Bits;
  UINT32 Data;

} PWQMRINGCSRUBAR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSVID_IIO_CPM_PF_REG supported on:                                           */
/*      SPRA0 (0x10d0002c)                                                      */
/*      SPRB0 (0x10d0002c)                                                      */
/*      SPRC0 (0x10d0002c)                                                      */
/*      SPRUCC (0x10d0002c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Subsystem Vendor ID Register
*/


#define PSVID_IIO_CPM_PF_REG 0x2121002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 svid : 16;

                            /* Bits[15:0], Access Type=RW/O/P, default=0x00008086*/

                            /*
                               Subsystem Vendor ID: This field is reset to ID
                               assigned to Intel. This value is loaded from
                               hardware straps.
                            */

  } Bits;
  UINT16 Data;

} PSVID_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSID_IIO_CPM_PF_REG supported on:                                            */
/*      SPRA0 (0x10d0002e)                                                      */
/*      SPRB0 (0x10d0002e)                                                      */
/*      SPRC0 (0x10d0002e)                                                      */
/*      SPRUCC (0x10d0002e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Subsystem ID Register
*/


#define PSID_IIO_CPM_PF_REG 0x2121002E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 sid : 16;

                            /* Bits[15:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               Subsystem ID: Vendor supplied device ID. Reset
                               to 0h. Value is loaded from hardware straps.
                            */

  } Bits;
  UINT16 Data;

} PSID_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */




#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PCP_IIO_CPM_PF_REG supported on:                                             */
/*      SPRA0 (0xd00034)                                                        */
/*      SPRB0 (0xd00034)                                                        */
/*      SPRC0 (0xd00034)                                                        */
/*      SPRUCC (0xd00034)                                                       */
/* Register default value on SPRA0: 0x000000B0                                  */
/* Register default value on SPRB0: 0x000000B0                                  */
/* Register default value on SPRC0: 0x000000B0                                  */
/* Register default value on SPRUCC: 0x000000B0                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Capabilities Pointer Register
*/


#define PCP_IIO_CPM_PF_REG 0x21200034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x000000B0*/

                            /*
                               Pointer to First Capability Structure: Value
                               points to the configuration space offset of the
                               first capability structure (MSI).
                            */

  } Bits;
  UINT8 Data;

} PCP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PIRQL_IIO_CPM_PF_REG supported on:                                           */
/*      SPRA0 (0xd0003c)                                                        */
/*      SPRB0 (0xd0003c)                                                        */
/*      SPRC0 (0xd0003c)                                                        */
/*      SPRUCC (0xd0003c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Interrupt Line Register
*/


#define PIRQL_IIO_CPM_PF_REG 0x2120003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 irql : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Line: BIOS writes the interrupt
                               routing information to this register to indicate
                               which input of the interrupt controller this
                               device is connected to. The device itself does
                               not use this information.
                            */

  } Bits;
  UINT8 Data;

} PIRQL_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PIRQP_IIO_CPM_PF_REG supported on:                                           */
/*      SPRA0 (0xd0003d)                                                        */
/*      SPRB0 (0xd0003d)                                                        */
/*      SPRC0 (0xd0003d)                                                        */
/*      SPRUCC (0xd0003d)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Interrupt Pin Register
*/


#define PIRQP_IIO_CPM_PF_REG 0x2120003D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 irqp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Pin: Set to 01h to indicate that
                               EP(HI) uses INTA# as its interrupt pin.
                            */

  } Bits;
  UINT8 Data;

} PIRQP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */









#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMSIX_IIO_CPM_PF_REG supported on:                                           */
/*      SPRA0 (0xd00060)                                                        */
/*      SPRB0 (0xd00060)                                                        */
/*      SPRC0 (0xd00060)                                                        */
/*      SPRUCC (0xd00060)                                                       */
/* Register default value on SPRA0: 0x00000011                                  */
/* Register default value on SPRB0: 0x00000011                                  */
/* Register default value on SPRC0: 0x00000011                                  */
/* Register default value on SPRUCC: 0x00000011                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Message Signalled Interrupt X Capability ID Register
*/


#define PMSIX_IIO_CPM_PF_REG 0x21200060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 msix : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000011*/

                            /*
                               Capability ID: PCI SIG assigned capability
                               record ID (11h, MSI-X capability). 11h
                               identifies the EP(HI) as a device that is MSI-X
                               capable.
                            */

  } Bits;
  UINT8 Data;

} PMSIX_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMSIXNCP_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0xd00061)                                                        */
/*      SPRB0 (0xd00061)                                                        */
/*      SPRC0 (0xd00061)                                                        */
/*      SPRUCC (0xd00061)                                                       */
/* Register default value on SPRA0: 0x0000006C                                  */
/* Register default value on SPRB0: 0x0000006C                                  */
/* Register default value on SPRC0: 0x0000006C                                  */
/* Register default value on SPRUCC: 0x0000006C                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF MSIX Next Capability Pointer Register
*/


#define PMSIXNCP_IIO_CPM_PF_REG 0x21200061

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 mcp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x0000006C*/

                            /*
                               Next Capability Pointer: Power Management
                               Capability
                            */

  } Bits;
  UINT8 Data;

} PMSIXNCP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMSIXCNTL_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x10d00062)                                                      */
/*      SPRB0 (0x10d00062)                                                      */
/*      SPRC0 (0x10d00062)                                                      */
/*      SPRUCC (0x10d00062)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Message Signalled Interrupt X Control Register
*/


#define PMSIXCNTL_IIO_CPM_PF_REG 0x21210062

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ts : 7;

                            /* Bits[6:0], Access Type=RO, default=0x00000040*/

                            /*
                               MSI-X Table Size: Number of vectors (encoded as
                               N-1) supported by the HI.
                            */
    UINT16 reserved : 7;

                            /* Bits[13:7], Access Type=RO, default=0x00000000*/

                            /* Always returns 0s when read. */
    UINT16 fm : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Function Mask: This bit controls the masking of
                               all vectors implemented in the EP(HI).When this
                               bit is 0, each vectors mask bit determines
                               whether the vector is masked or not.Whent this
                               bit is 1, all vecotrs are masked regardless of
                               the per vector masking bit.
                            */
    UINT16 msixen : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               MSI-X Enable: This bit enables the EP(HI) to
                               generates interrupts using the MSI-X tables
                               instead of the legacy INTx messages.When this
                               bit is set to 1, the EP(HI) will not generate
                               INTx messages and must use the MSI-X to signal
                               interrupts.The device driver should not clear
                               this bit to mask interrupts. This bit will be
                               set by the system PCI device manager.
                            */

  } Bits;
  UINT16 Data;

} PMSIXCNTL_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMSIXTBIR_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x20d00064)                                                      */
/*      SPRB0 (0x20d00064)                                                      */
/*      SPRC0 (0x20d00064)                                                      */
/*      SPRUCC (0x20d00064)                                                     */
/* Register default value on SPRA0: 0x00400002                                  */
/* Register default value on SPRB0: 0x00400002                                  */
/* Register default value on SPRC0: 0x00400002                                  */
/* Register default value on SPRUCC: 0x00400002                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF MSI-X Table Offset & Table BIR Register
*/


#define PMSIXTBIR_IIO_CPM_PF_REG 0x21220064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tbir : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000002*/

                            /*
                               Table BAR Indicator Register: The BIR points to
                               the HI PMISCBAR register (18h).
                            */
    UINT32 to : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00080000*/

                            /*
                               Table Offset: MSI-X Table is at an offset into
                               PMISCBAR (indicated by TBIR). The offset os
                               0x400000.
                            */

  } Bits;
  UINT32 Data;

} PMSIXTBIR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMSIXPBABIR_IIO_CPM_PF_REG supported on:                                     */
/*      SPRA0 (0x20d00068)                                                      */
/*      SPRB0 (0x20d00068)                                                      */
/*      SPRC0 (0x20d00068)                                                      */
/*      SPRUCC (0x20d00068)                                                     */
/* Register default value on SPRA0: 0x00408002                                  */
/* Register default value on SPRB0: 0x00408002                                  */
/* Register default value on SPRC0: 0x00408002                                  */
/* Register default value on SPRUCC: 0x00408002                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF MSI-X Pending Bit Array & BIR Offset Register
*/


#define PMSIXPBABIR_IIO_CPM_PF_REG 0x21220068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pbabir : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000002*/

                            /*
                               Pending Bit Array BAR Indicator Register: The
                               PBABIR points to the HI PMISCBAR register (18h).
                            */
    UINT32 pbao : 29;

                            /* Bits[31:3], Access Type=RO, default=0x00081000*/

                            /*
                               Pending Bit Array Offset: MSI-X PBA offset in
                               PMICBAR (indicated by PBABIR). The MSI-X PBA is
                               at offset of 0x408000 in PMISCBAR.
                            */

  } Bits;
  UINT32 Data;

} PMSIXPBABIR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPMCAP_IIO_CPM_PF_REG supported on:                                          */
/*      SPRA0 (0xd0006c)                                                        */
/*      SPRB0 (0xd0006c)                                                        */
/*      SPRC0 (0xd0006c)                                                        */
/*      SPRUCC (0xd0006c)                                                       */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Power Management Capabilities ID Register
*/


#define PPMCAP_IIO_CPM_PF_REG 0x2120006C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 pmid : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000001*/

                            /*
                               Capability ID: PCI SIG assigned capability
                               record ID (01h, Power Management capability)
                            */

  } Bits;
  UINT8 Data;

} PPMCAP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPMCP_IIO_CPM_PF_REG supported on:                                           */
/*      SPRA0 (0xd0006d)                                                        */
/*      SPRB0 (0xd0006d)                                                        */
/*      SPRC0 (0xd0006d)                                                        */
/*      SPRUCC (0xd0006d)                                                       */
/* Register default value on SPRA0: 0x00000074                                  */
/* Register default value on SPRB0: 0x00000074                                  */
/* Register default value on SPRC0: 0x00000074                                  */
/* Register default value on SPRUCC: 0x00000074                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Power Management Next Capability Pointer Register
*/


#define PPMCP_IIO_CPM_PF_REG 0x2120006D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 pmcp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000074*/

                            /* Next Capability Pointer: PCI Express Capability. */

  } Bits;
  UINT8 Data;

} PPMCP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPMC_IIO_CPM_PF_REG supported on:                                            */
/*      SPRA0 (0x10d0006e)                                                      */
/*      SPRB0 (0x10d0006e)                                                      */
/*      SPRC0 (0x10d0006e)                                                      */
/*      SPRUCC (0x10d0006e)                                                     */
/* Register default value on SPRA0: 0x00000023                                  */
/* Register default value on SPRB0: 0x00000023                                  */
/* Register default value on SPRC0: 0x00000023                                  */
/* Register default value on SPRUCC: 0x00000023                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Power Management Capabilities Register
*/


#define PPMC_IIO_CPM_PF_REG 0x2121006E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 pmv : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000003*/

                            /*
                               Version. This field is set to 3 to indicate that
                               the EP(HI) is compliant with the PCI bus Power
                               Management Interface Specification Rev 1.2.
                            */
    UINT16 pmc : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* PME clock. Does not apply to PCIe. */
    UINT16 reserved : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT16 dsi : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Device specific initialization: A one in this
                               bit indicates that immediately after entry into
                               the D0 Uninitialized state, the function
                               requires additional configuration above and
                               beyond setup of its PCI configuration Header
                               registers before the Class driver can use the
                               function.For the EP(HI) this bit is set to 1.
                            */
    UINT16 ac : 3;

                            /* Bits[8:6], Access Type=RO, default=0x00000000*/

                            /* Aux current: Not relevant for EP(HI). */
    UINT16 d1 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* D1 Support. EP(HI) does not support D1 state. */
    UINT16 d2 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /* D2 Support. EP(HI) does not support D2 state. */
    UINT16 pme : 5;

                            /* Bits[15:11], Access Type=RO, default=0x00000000*/

                            /*
                               PME Support: Set to indicate that the EP(HI)
                               will not assert PME# in any state.
                            */

  } Bits;
  UINT16 Data;

} PPMC_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPMCSR_IIO_CPM_PF_REG supported on:                                          */
/*      SPRA0 (0x20d00070)                                                      */
/*      SPRB0 (0x20d00070)                                                      */
/*      SPRC0 (0x20d00070)                                                      */
/*      SPRUCC (0x20d00070)                                                     */
/* Register default value on SPRA0: 0x00000008                                  */
/* Register default value on SPRB0: 0x00000008                                  */
/* Register default value on SPRC0: 0x00000008                                  */
/* Register default value on SPRUCC: 0x00000008                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF Power Management Control and Status Register
*/


#define PPMCSR_IIO_CPM_PF_REG 0x21220070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ps : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               Power State. This field is used to determine the
                               current power state of HI and to set a new power
                               state. If software selects a Power state that is
                               not supported by the HI (D2/D1), the writes must
                               complete normally on PCIe, but the write data is
                               discarded and no state change occurs. D0 = 00b,
                               D1 = 01b, D2 = 10b, and D3 = 11b.
                            */
    UINT32 reserved2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT32 nsr : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               No_Soft_Reset: Set to 1. Setting to 1 indicates
                               that when the device transitions from D3hot to
                               D0 because of Power State commands do not
                               perform an internal reset. Configuration Context
                               is preserved.
                            */
    UINT32 reserved1 : 4;

                            /* Bits[7:4], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 pme : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               PME Enable: Hardwired to 0 since EP(HI) does not
                               generate a PME.
                            */
    UINT32 dsel : 4;

                            /* Bits[12:9], Access Type=RO, default=0x00000000*/

                            /*
                               Data Select: Set to 0 for EP(HI) because the
                               EP(HI) does not implement the Data register (The
                               Data register is an optional, 8-bit read-only
                               register that provides a mechanism for the
                               function to report state dependent operating
                               data such as power consumed or heat
                               dissipation)If the Data register is not
                               implemented, this field should be read only and
                               return 0000b when the PMCSR is read.
                            */
    UINT32 dsc : 2;

                            /* Bits[14:13], Access Type=RO, default=0x00000000*/

                            /*
                               Data Scale: Set to 0 because the EP(HI) does not
                               implement the Data register.If the Data register
                               has not been implemented, this field must return
                               00b when the PMCSR is read.
                            */
    UINT32 pms : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               PME Status: Set to 0 since EP(HI) will not
                               generate PME.
                            */
    UINT32 reserved : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* Reserved/NA/Not Supported for EP(HI). */

  } Bits;
  UINT32 Data;

} PPMCSR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPCID_IIO_CPM_PF_REG supported on:                                           */
/*      SPRA0 (0xd00074)                                                        */
/*      SPRB0 (0xd00074)                                                        */
/*      SPRC0 (0xd00074)                                                        */
/*      SPRUCC (0xd00074)                                                       */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF PCI Express Capability Register
*/


#define PPCID_IIO_CPM_PF_REG 0x21200074

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 pciecid : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000010*/

                            /*
                               Capability ID: PCI SIG assigned capability
                               record ID (10h, PCI Express capability)
                            */

  } Bits;
  UINT8 Data;

} PPCID_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPCP_IIO_CPM_PF_REG supported on:                                            */
/*      SPRA0 (0xd00075)                                                        */
/*      SPRB0 (0xd00075)                                                        */
/*      SPRC0 (0xd00075)                                                        */
/*      SPRUCC (0xd00075)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF PCI Express Next Capability Pointer Register
*/


#define PPCP_IIO_CPM_PF_REG 0x21200075

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 pcienp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Next Capability Pointer: Last Capability. */

  } Bits;
  UINT8 Data;

} PPCP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPCR_IIO_CPM_PF_REG supported on:                                            */
/*      SPRA0 (0x10d00076)                                                      */
/*      SPRB0 (0x10d00076)                                                      */
/*      SPRC0 (0x10d00076)                                                      */
/*      SPRUCC (0x10d00076)                                                     */
/* Register default value on SPRA0: 0x00000092                                  */
/* Register default value on SPRB0: 0x00000092                                  */
/* Register default value on SPRC0: 0x00000092                                  */
/* Register default value on SPRUCC: 0x00000092                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF PCI Express Capabilities Register
*/


#define PPCR_IIO_CPM_PF_REG 0x21210076

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 cv : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000002*/

                            /*
                               Capability Version: Indicates PCI-SIG defined
                               PCI Express capability structure version
                               number.EP(HI) is PCIe 3.0 Specification
                               Compliant.
                            */
    UINT16 dpt : 4;

                            /* Bits[7:4], Access Type=RO, default=0x00000009*/

                            /*
                               Device/Port Type: Indicates the type of PCI
                               Express logical device. Hardwired to 1001b
                               (RCiEP)
                            */
    UINT16 si : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Slot Implemented: This bit when set indicates
                               that the PCI Express Link associated with this
                               port is connected to a slot. Hardwired to 0 for
                               EP(HI).
                            */
    UINT16 imn : 5;

                            /* Bits[13:9], Access Type=RO, default=0x00000000*/

                            /*
                               Interrupt Message Number: This field indicates
                               which MSI vector is used for the interrupt
                               message generated in association with the status
                               bits in either the Slot Status field of this
                               capability structure (applies to only RC or
                               Switch)Not applicable to EP(HI).
                            */
    UINT16 reserved : 2;

                            /* Bits[15:14], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PPCR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPDCAP_IIO_CPM_PF_REG supported on:                                          */
/*      SPRA0 (0x20d00078)                                                      */
/*      SPRB0 (0x20d00078)                                                      */
/*      SPRC0 (0x20d00078)                                                      */
/*      SPRUCC (0x20d00078)                                                     */
/* Register default value on SPRA0: 0x10008062                                  */
/* Register default value on SPRB0: 0x10008062                                  */
/* Register default value on SPRC0: 0x10008062                                  */
/* Register default value on SPRUCC: 0x10008062                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF PCI Express Device Capabilities Register
*/


#define PPDCAP_IIO_CPM_PF_REG 0x21220078

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mps : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000002*/

                            /*
                               Max_Payload_Size Supported: This field indicates
                               the maximum payload size that HI can support for
                               TLPs. This value is set to indicate 512B. The
                               defined encodings are: 000b = 128B 001b = 256B
                               010b = 512B (Maximum supported) 011b = 1KB 100b
                               = 2KB 101b = 4KB
                            */
    UINT32 pfs : 2;

                            /* Bits[4:3], Access Type=RO, default=0x00000000*/

                            /*
                               Phantom Functions Supported: This field
                               indicates the support for use of unclaimed
                               function numbers to extend the number of
                               outstanding transactions allowed. EP(HI) does
                               not use this capability.
                            */
    UINT32 etfs : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Extended Tag Field Supported: This field
                               indicates the maximum supported size of the Tag
                               field as a Requester. When Clear indicates 5-bit
                               Tag field is supported. When set 8-bit Tag field
                               is supported. Supports 96 outstanding requests.
                            */
    UINT32 el0l : 3;

                            /* Bits[8:6], Access Type=RO, default=0x00000001*/

                            /*
                               Endpoint L0s Acceptable Latency: This field
                               indicates the acceptable latency that an
                               Endpoint can withstand due to the transition
                               from L1 state to the L0 state. This value is
                               hardcoded to the latency of 64-128ns.Since CPM
                               does not have a physical layer, this
                               functionality is not implemented.
                            */
    UINT32 el1l : 3;

                            /* Bits[11:9], Access Type=RO, default=0x00000000*/

                            /*
                               Endpoint L1 Acceptable Latency: This field
                               indicates the acceptable latency that an
                               Endpoint can withstand due to the transition
                               from L1 state to the L0 state. Hardcoded to the
                               lowest value of 1us.Since CPM does not have a
                               physical layer, this functionality is not
                               implemented.
                            */
    UINT32 attn : 3;

                            /* Bits[14:12], Access Type=RO, default=0x00000000*/

                            /*
                               Attention Button/Indicator Present and Power
                               Indicator Present. None of these are implemented
                               in the EP(HI).
                            */
    UINT32 rbep : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /*
                               Role-Based Error Reporting: Indicates that
                               EP(HI) conforms to Role based error reporting
                               ECN for PCIe 1.0a and which was subsequently
                               rolled in PCIe 1.1 and future revisions.
                            */
    UINT32 reserved1 : 2;

                            /* Bits[17:16], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 cspv : 8;

                            /* Bits[25:18], Access Type=RO, default=0x00000000*/

                            /*
                               Captured Slot Power Limit Value: Does not apply
                               to EP(HI).
                            */
    UINT32 csps : 2;

                            /* Bits[27:26], Access Type=RO, default=0x00000000*/

                            /*
                               Captured Slot Power Limit Scale: Does not apply
                               to EP(HI).
                            */
    UINT32 flr : 1;

                            /* Bits[28:28], Access Type=RO, default=0x00000001*/

                            /*
                               Function level reset: When set indicates that
                               the device supports the FLR feature. When SR-IOV
                               is enabled both PF & VF have to support FLR
                            */
    UINT32 reserved0 : 3;

                            /* Bits[31:29], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT32 Data;

} PPDCAP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPDCNTL_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x10d0007c)                                                      */
/*      SPRB0 (0x10d0007c)                                                      */
/*      SPRC0 (0x10d0007c)                                                      */
/*      SPRUCC (0x10d0007c)                                                     */
/* Register default value on SPRA0: 0x00005950                                  */
/* Register default value on SPRB0: 0x00005950                                  */
/* Register default value on SPRC0: 0x00005950                                  */
/* Register default value on SPRUCC: 0x00005950                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF PCI Express Device Control Register
*/


#define PPDCNTL_IIO_CPM_PF_REG 0x2121007C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 cere : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Correctable Error Reporting Enable: This bit, in
                               conjunction with other bits, controls sending
                               ERR_COR Messages to the root port. When clear
                               disables sending of error messages.
                            */
    UINT16 nere : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Non-Fatal Error Reporting Enable: This bit, in
                               conjunction with other bits, controls sending
                               ERR_NONFATAL Messages to the root port. When
                               clear disables sending of error messages.
                            */
    UINT16 fere : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Fatal Error Reporting Enable: This bit, in
                               conjunction with other bits, controls sending
                               ERR_FATAL Messages to the root port. When clear
                               disables sending of error messages.
                            */
    UINT16 urro : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Unsupported Request Reporting Enable: This bit,
                               in conjunction with other bits, controls the
                               signaling of Unsupported Requests by sending
                               Error Messages to the root port. When clear it
                               disables sending of error messages.
                            */
    UINT16 ero : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Enable Relaxed Ordering: If this bit is set,
                               EP(HI) is permitted to set the Relaxed Ordering
                               bit in the Attributes field of transactions it
                               initiates.Note that seting this bit does not
                               cause the EP(HI) to set the RO on every
                               transaction it issues.
                            */
    UINT16 mps : 3;

                            /* Bits[7:5], Access Type=RW/P, default=0x00000002*/

                            /*
                               Max_Payload_Size (MPS): This field sets the
                               maximum TLP payload for the HI. As a Receiver,
                               the HI must handle TLPs as large as the set
                               value; as a Transmitter, the HI must not
                               generate TLPs exceeding the set value. The HI is
                               capable of generating up to 512B MPS. However
                               requests generated by the HI will be limited by
                               the programmed value in this field. 000b = 128B,
                               001b = 256B, 010b = 512B, (Default) 111b = 64B.
                               Others values not supported by the HI.
                            */
    UINT16 etfe : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Extended Tag Field Enable: When set, this bit
                               enables a device to use an 8-bit Tag field as a
                               requester. HI supports 256 outstanding requests.
                               This bit is always enabled by default to support
                               8-bit tag. Writing this bit won't have any
                               effect.
                            */
    UINT16 pfe : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Phantom Functions Enable: When set, this bit
                               enables a device to use unclaimed functions as
                               Phantom Functions. Does not apply to EP(HI).
                            */
    UINT16 apme : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Auxiliary (AUX) Power PM Enable: This bit when
                               set enables a device to draw AUX power
                               independent of PME AUX power. Does not apply to
                               EP(HI)
                            */
    UINT16 ens : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Enable No Snoop (NS): If this bit is set to 1,
                               EP(HI) is permitted to set the No Snoop bit in
                               the Requester Attributes of transactions it
                               initiates. When clear all transactions will have
                               the No Snoop bit clear.Note that setting this
                               bit will not cause the EP(HI) to set the No
                               Snoop attribute on every memory requests that it
                               initiates.
                            */
    UINT16 mrs : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000005*/

                            /*
                               Max_Read_Request_Size: This field sets the
                               maximum Read Request size for the HI as a
                               Requester. The HI is capable of generating up to
                               4KB read requests. However requests generated by
                               the HI will be limited by the programmed value
                               in this field. Defined encodings for this field
                               are: 000b = 128B, 001b = 256B 010b = 512B 011b =
                               1024B 100b = 2048B 101b = 4096B (Default) 111b =
                               64B
                            */
    UINT16 startflr : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Initiate FLR - Used to initiate FLR transition.
                               A write of 1 initiates FLR transition. Since
                               hardware must not respond to any cycles till FLR
                               completion, the value read by software from this
                               bit is 0.
                            */

  } Bits;
  UINT16 Data;

} PPDCNTL_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPDSTAT_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x10d0007e)                                                      */
/*      SPRB0 (0x10d0007e)                                                      */
/*      SPRC0 (0x10d0007e)                                                      */
/*      SPRUCC (0x10d0007e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PF PCI Express Device Status Register
*/


#define PPDSTAT_IIO_CPM_PF_REG 0x2121007E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ced : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Correctable Error Detected: This bit indicates
                               status of correctable errors detected.A one
                               indicates that an error was detected since the
                               last time this bit was cleared.Errors are
                               updated in this field regardless of whether the
                               error reporting bit is set in the Device Control
                               Register.
                            */
    UINT16 ned : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Non-Fatal Error Detected: This bit indicates
                               status of Nonfatal errors detected.A one
                               indicates that an error was detected since the
                               last time this bit was cleared.Errors are
                               updated in this field regardless of whether the
                               error reporting bit is set in the Device Control
                               Register.
                            */
    UINT16 fed : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Fatal Error Detected: This bit indicates status
                               of Fatal errors detected.A one indicates that an
                               error was detected since the last time this bit
                               was cleared.Errors are updated in this field
                               regardless of whether the error reporting bit is
                               set in the Device Control Register.
                            */
    UINT16 urd : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Unsupported Request Detected: This bit indicates
                               that EP(HI) received an Unsupported Request.A
                               one indicates that an error was detected since
                               the last time this bit was cleared.Errors are
                               updated in this field regardless of whether the
                               error reporting bit is set in the Device Control
                               Register.
                            */
    UINT16 apd : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               AUX Power Detected: Devices that require AUX
                               power report this bit as set if AUX power is
                               detected by the device.
                            */
    UINT16 tp : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               Transactions Pending: This bit when set
                               indicates that EP(HI) has issued Non-Posted
                               Requests which have not been completed either
                               with a completion packet or completion timeout
                               mechanism.
                            */
    UINT16 reserved : 10;

                            /* Bits[15:6], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PPDSTAT_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PLCAPR_IIO_CPM_PF_REG supported on:                                          */
/*      SPRA0 (0x20d00080)                                                      */
/*      SPRB0 (0x20d00080)                                                      */
/*      SPRC0 (0x20d00080)                                                      */
/*      SPRUCC (0x20d00080)                                                     */
/* Register default value on SPRA0: 0x00400C11                                  */
/* Register default value on SPRB0: 0x00400C11                                  */
/* Register default value on SPRC0: 0x00400C11                                  */
/* Register default value on SPRUCC: 0x00400C11                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Link Capabilities Register
*/


#define PLCAPR_IIO_CPM_PF_REG 0x21220080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 maxspeed : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000001*/

                            /*
                               Max Link Speed - Not applicable to IOSF. Set to
                               0x1.
                            */
    UINT32 linkw : 6;

                            /* Bits[9:4], Access Type=RO, default=0x00000001*/

                            /*
                               Max Link Width - Not applicable to IOSF. Set to
                               0x1.
                            */
    UINT32 aslpm : 2;

                            /* Bits[11:10], Access Type=RO, default=0x00000003*/

                            /*
                               Active State Link PM Support - Indicates the
                               level of active state power management supported
                               in EP(HI). Defined encodings are:00b -
                               Reserved01b - L0s Entry Supported10b -
                               Reserved11b - L0s and L1 Supported
                            */
    UINT32 l0el : 3;

                            /* Bits[14:12], Access Type=RO, default=0x00000000*/

                            /*
                               L0s Exit Latency- Indicates the exit latency
                               from L0s to L0 state.000b - Less than 64ns001b -
                               64ns - 128ns010b - 128ns - 256ns011b - 256ns -
                               512ns100b - 512ns - 1 s101b - 1 s - 2 s110b - 2
                               s - 4 s111b - Reserved
                            */
    UINT32 l1el : 3;

                            /* Bits[17:15], Access Type=RO, default=0x00000000*/

                            /*
                               L1 Exit Latency- Indicates the exit latency from
                               L1 to L0 state.000b - Less than 1 s001b - 1 s -
                               2 s010b - 2 s - 4 s011b - 4 s - 8 s100b - 8 s -
                               16 s101b - 16 s - 32 s110b - 32 s - 64 s111b -
                               L1 transition not supported
                            */
    UINT32 na_iosf : 3;

                            /* Bits[20:18], Access Type=RO, default=0x00000000*/

                            /* Does not apply to EP(HI) */
    UINT32 lbn : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /* Link Bandwidth Notification Capability: */
    UINT32 aspm : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000001*/

                            /* ASPM Optionality Compliance */
    UINT32 reserved : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 portnum : 8;

                            /* Bits[31:24], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               Port Number: Assigned by EP(HI) after link
                               training phase.
                            */

  } Bits;
  UINT32 Data;

} PLCAPR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PLCNTLR_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x10d00084)                                                      */
/*      SPRB0 (0x10d00084)                                                      */
/*      SPRC0 (0x10d00084)                                                      */
/*      SPRUCC (0x10d00084)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Link Control Register
*/


#define PLCNTLR_IIO_CPM_PF_REG 0x21210084

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 aspmc : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /*
                               Active State Link PM Control - this field
                               controls the active state PM supported on the
                               link. Link PM functionality is determined by the
                               lowest common denominator of all functions.
                               Defined encodings are:00b - PM Disabled 01b -
                               L0s Entry Supported 10b - Reserved 11b - L0s and
                               L1 Supported. Defined for PCIe Compliance. Not
                               valid for IOSF.
                            */
    UINT16 reserved2 : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT16 rcb : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               Read Completion Boundary (RCB): For PCIe
                               Endpoints this field is set optionally by
                               configuration software to indicate the RCB value
                               of the Root Port upstream from the Endpoint. 0b
                               = 64B 1b = 128 byte
                            */
    UINT16 linkdis : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Link Disable: This bit disables the Link by
                               directing theLTSSM to the Disabled state when
                               set to 1b. Not valid for IOSF.
                            */
    UINT16 retrain : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Retrain Link: A write of 1b to this bit
                               initiates Link retraining by directing the
                               Physical Layer LTSSM to the Recovery state. Not
                               valid for IOSF.
                            */
    UINT16 cclkcfg : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Common Clock Configuration - when set indicates
                               that HI and the root port at the other end of
                               the link are operating with a common reference
                               clock. A value of 0 indicates that they
                               operating with an asynchronous clock. This
                               parameter affects the L0s Exit Latencies. After
                               changing the value in this bit in both
                               components on a Link, software must trigger the
                               Link to retrain by writing a 1b to the Retrain
                               Link bit. Defined for PCIe Compliance. Not valid
                               for IOSF.
                            */
    UINT16 extsync : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Extended Sync. When set to one, this bit forces
                               the transmission of:- 4096 FTS ordered sets
                               during the L0s state - followed by a single SKP
                               ordered set prior to entering the L0 state, - as
                               well as the transmission of 1024 TS1 ordered
                               sets in the L1 state prior to entering the
                               Recovery state. This mode gives external devices
                               (e.g., logic analyzers) that may be monitoring
                               Link activity time to achieve bit and symbol
                               lock before the Link enters the L0 or Recovery
                               state and resumes communication. Defined for
                               PCIe Compliance. Not valid for IOSF.
                            */
    UINT16 eclkpm : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Enable Clock Power Management - Defined for PCIe
                               Compliance. Not valid for IOSF.
                            */
    UINT16 wd : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               Hardware Autonomous Width Disable - When set to
                               1b this bit disables hardware from changing the
                               link width for reasons other than attempting to
                               correct unreliable link operation by reducing
                               link width.
                            */
    UINT16 lbwminte : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Link Bandwidth Management Interrupt Enable -
                               When set to 1b this bit enables the generation
                               of an interrupt to indicate that the Link
                               Bandwidth Management Status bit has been set.
                            */
    UINT16 lbwinte : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Link Autonomous Bandwidth Interrupt Enable -
                               When set to 1b this bit enables the generation
                               of an interrupt to indicate that the Link
                               Autonomous Bandwidth Status bit has been set.
                            */
    UINT16 reserved : 4;

                            /* Bits[15:12], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PLCNTLR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PLSR_IIO_CPM_PF_REG supported on:                                            */
/*      SPRA0 (0x10d00086)                                                      */
/*      SPRB0 (0x10d00086)                                                      */
/*      SPRC0 (0x10d00086)                                                      */
/*      SPRUCC (0x10d00086)                                                     */
/* Register default value on SPRA0: 0x00001011                                  */
/* Register default value on SPRB0: 0x00001011                                  */
/* Register default value on SPRC0: 0x00001011                                  */
/* Register default value on SPRUCC: 0x00001011                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Link Status Register
*/


#define PLSR_IIO_CPM_PF_REG 0x21210086

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 nls : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000001*/

                            /*
                               Negotiated Link Speed: Not applicable to IOSF.
                               Set to 0x1.
                            */
    UINT16 nlw : 6;

                            /* Bits[9:4], Access Type=RO, default=0x00000001*/

                            /*
                               Negotiated Link Width: Not applicable to IOSF.
                               Set to 0x1.
                            */
    UINT16 lte : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Link Training Error - Indicates that a link
                               training error has occurred.
                            */
    UINT16 ltinprog : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Link Training - Indicates that link training is
                               in progress.
                            */
    UINT16 sclkcfg : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000001*/

                            /*
                               Slot Clock Configuration - When set indicates
                               that EP(HI) uses the physical reference clock
                               that the platform provides on the connector.
                               This bit must be cleared if EP(HI) uses an
                               independent clock.Defined for PCIe Compliance
                            */
    UINT16 dlact : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /* Data Link Layer Link Active */
    UINT16 lbwms : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /* Link Bandwidth Management Status */
    UINT16 reserved : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PLSR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PDCAPR2_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x20d00098)                                                      */
/*      SPRB0 (0x20d00098)                                                      */
/*      SPRC0 (0x20d00098)                                                      */
/*      SPRUCC (0x20d00098)                                                     */
/* Register default value on SPRA0: 0x00710012                                  */
/* Register default value on SPRB0: 0x00710012                                  */
/* Register default value on SPRC0: 0x00710012                                  */
/* Register default value on SPRUCC: 0x00710012                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Device Capabilities 2 Register
*/


#define PDCAPR2_IIO_CPM_PF_REG 0x21220098

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctors : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000002*/

                            /*
                               Completion Timeout Ranges Supported:This field
                               indicates support for the optional completion
                               timeout programmability mechanism. This
                               mechanism enables system software to modify the
                               completion timeout value.Four time value ranges
                               are defined:Bits are set according to the
                               following table to show the timeout value ranges
                               that are supported.It is strongly recommended
                               that the completion timeout mechanism not expire
                               in less than 10 ms
                            */
    UINT32 ctods : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               Completion Timeout Disable Supported.A value of
                               1b indicates support for the completion timeout
                               disable mechanism.
                            */
    UINT32 reserved1 : 11;

                            /* Bits[15:5], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 tenbittagcmptr : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000001*/

                            /*
                               10-Bit Tag Completer Supported. If this bit is
                               Set, the Function supports 10-Bit Tag Completer
                               capability; otherwise, the Function does not.
                            */
    UINT32 tenbittagreqtr : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               10-Bit Tag Requester Supported. If this bit is
                               Set, the Function supports 10-Bit Tag Requester
                               capability; otherwise, the Function does not.
                               This bit must not be Set if the 10-Bit Tag
                               Completer Supported bit is Clear. If the
                               Function is an RCiEP, this bit must be Clear if
                               the RC does not support 10-Bit Tag Completer
                               capability for Requests coming from this RCiEP.
                               Note that 10-Bit Tag field generation must be
                               enabled by the 10- Bit Tag Requester Enable bit
                               in the Device Control 2 register of the
                               Requester Function before 10-Bit Tags can be
                               generated by the Requester. Note : CPM doesn't
                               support 10 bit tag a Requestor.
                            */
    UINT32 reserved0 : 2;

                            /* Bits[19:18], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 effs : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000001*/

                            /*
                               Extended Fmt Field Supported: If Set, the
                               Function supports the 3-bit definition of the
                               Fmt field. If Clear, the Function supports a
                               2-bit definition of the Fmt field.
                            */
    UINT32 eetps : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000001*/

                            /*
                               End-End TLP Prefix Supported : Indicates whether
                               End-End TLP Prefix support is offered by a
                               Function.
                            */
    UINT32 meetp : 2;

                            /* Bits[23:22], Access Type=RO, default=0x00000001*/

                            /*
                               Max End-End TLP Prefixes : Indicates the maximum
                               number of End-End TLP Prefixes supported by this
                               Function.
                            */
    UINT32 reserved2 : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PDCAPR2_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PDCNTR2_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x10d0009c)                                                      */
/*      SPRB0 (0x10d0009c)                                                      */
/*      SPRC0 (0x10d0009c)                                                      */
/*      SPRUCC (0x10d0009c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Device Control 2 Register
*/


#define PDCNTR2_IIO_CPM_PF_REG 0x2121009C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ctov : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               Completion Timeout Value:In devices that support
                               completion timeout programmability, this field
                               enables system software to modify the completion
                               timeout value.Encoding:Values not defined are
                               reserved.Software is permitted to change the
                               value in this field at any time. For requests
                               already pending when the completion timeout
                               value is changed, hardware is permitted to use
                               either the new or the old value for the
                               outstanding requests and is permitted to base
                               the start time for each request either when this
                               value was changed or when each request was
                               issued.The default value for this field is
                               0000b.
                            */
    UINT16 ctodis : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Completion Timeout Disable:When set to 1b, this
                               bit disables the completion timeout mechanism.
                               Software is permitted to set or clear this bit
                               at any time. When set, the completion timeout
                               detection mechanism is disabled. If there are
                               outstanding requests when the bit is cleared, it
                               is permitted but not required for hardware to
                               apply the completion timeout mechanism to the
                               outstanding requests. If this is done, it is
                               permitted to base the start time for each
                               request on either the time this bit was cleared
                               or the time each request was issued.The default
                               value for this bit is 0b.
                            */
    UINT16 reserved0 : 7;

                            /* Bits[11:5], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */
    UINT16 tenbittagreqtren : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               10-Bit Tag Requester Enable. This bit, in
                               combination with the Extended Tag Field Enable
                               bit in the Device Control register, determines
                               how many Tag field bits a Requester is permitted
                               to use. When the 10-Bit Tag Requester Enable bit
                               is Set, the Requester is permitted to use 10-Bit
                               Tags. If software changes the value of this bit
                               while the Function has outstanding Non-Posted
                               Requests, the result is undefined. Functions
                               that do not implement 10-Bit Tag Requester
                               capability are permitted to hardwire this bit to
                               0b. Default value of this bit is 0b. Note: CPM
                               does not support 10 Bit tag when CPM is the
                               requestor.
                            */
    UINT16 reserved1 : 3;

                            /* Bits[15:13], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT16 Data;

} PDCNTR2_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PLCAPR2_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x20d000a0)                                                      */
/*      SPRB0 (0x20d000a0)                                                      */
/*      SPRC0 (0x20d000a0)                                                      */
/*      SPRUCC (0x20d000a0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Link Capabilities 2 Register
*/


#define PLCAPR2_IIO_CPM_PF_REG 0x212200A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved2 : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 reserved1 : 24;

                            /* Bits[31:8], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PLCAPR2_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */



#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PLCNTLR2_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x10d000a4)                                                      */
/*      SPRB0 (0x10d000a4)                                                      */
/*      SPRC0 (0x10d000a4)                                                      */
/*      SPRUCC (0x10d000a4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Link Control2 Register
*/


#define PLCNTLR2_IIO_CPM_PF_REG 0x212100A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 tlnks : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /* Target Link Speed. Not applicable to IOSF. */
    UINT16 encomp : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /* Enter Compliance. Not applicable to IOSF. */
    UINT16 hwautosd : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Hardware Autonomous Speed Disable. Not
                               applicable to IOSF.
                            */
    UINT16 sdem : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Selectable De-emphasisThis bit is not applicable
                               and reserved for Endpoints.
                            */
    UINT16 tmarg : 3;

                            /* Bits[9:7], Access Type=RO, default=0x00000000*/

                            /* Transmit Margin. Not applicable to IOSF. */
    UINT16 emc : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               Enter Modified Compliance. Not applicable to
                               IOSF.
                            */
    UINT16 csos : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /* Compliance SOS. Not applicable to IOSF. */
    UINT16 cde : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /* Compliance De-emphasis. Not applicable to IOSF */
    UINT16 reserved : 3;

                            /* Bits[15:13], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PLCNTLR2_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PLSR2_IIO_CPM_PF_REG supported on:                                           */
/*      SPRA0 (0x10d000a6)                                                      */
/*      SPRB0 (0x10d000a6)                                                      */
/*      SPRC0 (0x10d000a6)                                                      */
/*      SPRUCC (0x10d000a6)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Link Status Register
*/


#define PLSR2_IIO_CPM_PF_REG 0x212100A6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 cdel : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Current De-emphasis Level. Not applicable to
                               IOSF.
                            */
    UINT16 reserved : 15;

                            /* Bits[15:1], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PLSR2_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMSICID_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0xd000b0)                                                        */
/*      SPRB0 (0xd000b0)                                                        */
/*      SPRC0 (0xd000b0)                                                        */
/*      SPRUCC (0xd000b0)                                                       */
/* Register default value on SPRA0: 0x00000005                                  */
/* Register default value on SPRB0: 0x00000005                                  */
/* Register default value on SPRC0: 0x00000005                                  */
/* Register default value on SPRUCC: 0x00000005                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Capability ID Register
*/


#define PMSICID_IIO_CPM_PF_REG 0x212000B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 mcid : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000005*/

                            /*
                               Capability ID: PCI SIG assigned capability
                               record ID (05h, MSI capability)
                            */

  } Bits;
  UINT8 Data;

} PMSICID_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMSINCP_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0xd000b1)                                                        */
/*      SPRB0 (0xd000b1)                                                        */
/*      SPRC0 (0xd000b1)                                                        */
/*      SPRUCC (0xd000b1)                                                       */
/* Register default value on SPRA0: 0x00000060                                  */
/* Register default value on SPRB0: 0x00000060                                  */
/* Register default value on SPRC0: 0x00000060                                  */
/* Register default value on SPRUCC: 0x00000060                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Next Capability Pointer Register
*/


#define PMSINCP_IIO_CPM_PF_REG 0x212000B1

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 mcp : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000060*/

                            /* Next Capability Pointer: MSI-X capability. */

  } Bits;
  UINT8 Data;

} PMSINCP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMSICTL_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x10d000b2)                                                      */
/*      SPRB0 (0x10d000b2)                                                      */
/*      SPRC0 (0x10d000b2)                                                      */
/*      SPRUCC (0x10d000b2)                                                     */
/* Register default value on SPRA0: 0x00000180                                  */
/* Register default value on SPRB0: 0x00000180                                  */
/* Register default value on SPRC0: 0x00000180                                  */
/* Register default value on SPRUCC: 0x00000180                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Control Register
*/


#define PMSICTL_IIO_CPM_PF_REG 0x212100B2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 msie : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               MSI Enable: System software sets this bit to
                               enable MSI signaling. A device driver is
                               prohibited from writing this bit to mask a
                               devices service request.If 1, the device can use
                               an MSI to request service.If 0, the device
                               cannot use an MSI to request service.
                            */
    UINT16 mmc : 3;

                            /* Bits[3:1], Access Type=RO, default=0x00000000*/

                            /*
                               Multiple Message Capable: System software reads
                               this field to determine the number of requested
                               messages.Hardwired to 0 to request one message.
                            */
    UINT16 mme : 3;

                            /* Bits[6:4], Access Type=RW, default=0x00000000*/

                            /*
                               Multiple Message Enable: System software writes
                               to this field to indicate the number of
                               allocated messages (less than or equal to the
                               number of requested messages in MMC). A value of
                               0 corresponds to one message.
                            */
    UINT16 c64 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000001*/

                            /*
                               64 bit Address Capable: Hardwired to 1 to
                               indicate the device generate 64b message
                               addresses.
                            */
    UINT16 mc : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000001*/

                            /*
                               Per-Vector Masking Capable: Per-vector masking
                               capable.
                            */
    UINT16 reserved : 7;

                            /* Bits[15:9], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PMSICTL_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMSILADDR_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x20d000b4)                                                      */
/*      SPRB0 (0x20d000b4)                                                      */
/*      SPRC0 (0x20d000b4)                                                      */
/*      SPRUCC (0x20d000b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Lower Address Register
*/


#define PMSILADDR_IIO_CPM_PF_REG 0x212200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 2;

                            /* Bits[1:0], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 addr : 30;

                            /* Bits[31:2], Access Type=RW, default=0x00000000*/

                            /*
                               Lower Message Address: Written by the system to
                               indicate the lower 30-bits of the address to use
                               for the MSI memory write transaction.
                            */

  } Bits;
  UINT32 Data;

} PMSILADDR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMSIUADDR_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x20d000b8)                                                      */
/*      SPRB0 (0x20d000b8)                                                      */
/*      SPRC0 (0x20d000b8)                                                      */
/*      SPRUCC (0x20d000b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Upper Address Register
*/


#define PMSIUADDR_IIO_CPM_PF_REG 0x212200B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Upper Message Address: Written by the system to
                               indicate the lower 62-bits of the address to use
                               for the MSI memory write transaction. The lower
                               two bits will always be written as 0.
                            */

  } Bits;
  UINT32 Data;

} PMSIUADDR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMSIDATA_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x10d000bc)                                                      */
/*      SPRB0 (0x10d000bc)                                                      */
/*      SPRC0 (0x10d000bc)                                                      */
/*      SPRUCC (0x10d000bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Data Register
*/


#define PMSIDATA_IIO_CPM_PF_REG 0x212100BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 data : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               Message Data: Written by the system to indicate
                               the lower 16 bits of the data written in the MSI
                               memory write DWORD transaction. The upper 16
                               bits of the transaction are written as 0.
                            */

  } Bits;
  UINT16 Data;

} PMSIDATA_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMSIMSK_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x20d000c0)                                                      */
/*      SPRB0 (0x20d000c0)                                                      */
/*      SPRC0 (0x20d000c0)                                                      */
/*      SPRUCC (0x20d000c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Mask Register
*/


#define PMSIMSK_IIO_CPM_PF_REG 0x212200C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Mask Bits: Only on bit defined. Refer to the
                               interrupt section.
                            */
    UINT32 reserved : 31;

                            /* Bits[31:1], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} PMSIMSK_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PMSIPND_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x20d000c4)                                                      */
/*      SPRB0 (0x20d000c4)                                                      */
/*      SPRC0 (0x20d000c4)                                                      */
/*      SPRUCC (0x20d000c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Message Signalled Interrupt Pending Register
*/


#define PMSIPND_IIO_CPM_PF_REG 0x212200C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pndb : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* Pending Bits. Only bit 0 used. */
    UINT32 reserved : 31;

                            /* Bits[31:1], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} PMSIPND_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */








#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPCIEAERCAPID_IIO_CPM_PF_REG supported on:                                   */
/*      SPRA0 (0x20d00100)                                                      */
/*      SPRB0 (0x20d00100)                                                      */
/*      SPRC0 (0x20d00100)                                                      */
/*      SPRUCC (0x20d00100)                                                     */
/* Register default value on SPRA0: 0x14820001                                  */
/* Register default value on SPRB0: 0x14820001                                  */
/* Register default value on SPRC0: 0x14820001                                  */
/* Register default value on SPRUCC: 0x14820001                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PCI Express AER Capability ID Register
*/


#define PPCIEAERCAPID_IIO_CPM_PF_REG 0x21220100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pcieaercid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000001*/

                            /*
                               Advanced Error Capability ID: PCI Express
                               Extended Capability ID indicating Advanced Error
                               Reporting Capability.
                            */
    UINT32 pcieaercvn : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000002*/

                            /*
                               Advanced Error Capability Version Number: PCI
                               Express Advanced Error Reporting Extended
                               Capability Version Number.
                            */
    UINT32 pcieaerncp : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000148*/

                            /*
                               Next PCI Express Extended Capability Pointer:The
                               Next Capability Pointer default value will be
                               dependent on the ACC_VF_ENABLE strap.1 - 138h
                               (Default) (ARI Capability)0 - 000h (This (AER)
                               is the last extended capability)
                            */

  } Bits;
  UINT32 Data;

} PPCIEAERCAPID_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPAERUCS_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x20d00104)                                                      */
/*      SPRB0 (0x20d00104)                                                      */
/*      SPRC0 (0x20d00104)                                                      */
/*      SPRUCC (0x20d00104)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PCI Express AER Uncorrectable Error Status Register
*/


#define PPAERUCS_IIO_CPM_PF_REG 0x21220104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved2 : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 dlpe : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Data Link Protocol Error: Set whenever a data
                               link protocol error is detected.Not applicable
                               for IOSF.
                            */
    UINT32 sdes : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /* Surprise Down Error:Not supported. */
    UINT32 reserved1 : 6;

                            /* Bits[11:6], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 ptlpr : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Poisoned TLP Received: As a receiver, set
                               whenever a poisoned TLP is received from PCI
                               Express. The header is logged. Note that
                               internal queue errors are not covered by this
                               bit, they are logged by the Configuration target
                               of the transaction.
                            */
    UINT32 fcpes : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Flow Control Protocol Error Status: Set whenever
                               a flow control protocol error is detected.Not
                               supported.
                            */
    UINT32 ct : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Completion Timeout : As a requester, set
                               whenever an outbound Non Posted Request does not
                               receive a completion within 16-32ms.
                            */
    UINT32 ca : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* Completer Abort: HI deos not support CA. */
    UINT32 ec : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Unexpected Completion: As a receiver, set
                               whenever a completion is received that does not
                               match the requestor ID or outstanding Tag. The
                               Header is logged.
                            */
    UINT32 ro : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Receiver Overflow: Set if PCI Express receive
                               buffers overflow. Not applicable for IOSF.
                            */
    UINT32 mtlp : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Malformed TLP: As a receiver, set whenever a
                               malformed TLP is detected. The Header is logged.
                            */
    UINT32 ecrcc : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               ECRC Check: As a receiver, set when ECRC check
                               fails. The Header is logged.
                            */
    UINT32 ur : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Unsupported Request Error Status: As a receiver,
                               Set whenever an unsupported request is detected.
                               The Header is logged.
                            */
    UINT32 acs : 1;

                            /* Bits[21:21], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* ACS Violation Status */
    UINT32 ieunc : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Uncorrectable Internal Error Status: */
    UINT32 reserved0 : 9;

                            /* Bits[31:23], Access Type=RO, default=0x00000000*/

                            /* Reserved0: */

  } Bits;
  UINT32 Data;

} PPAERUCS_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPAERUCM_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x20d00108)                                                      */
/*      SPRB0 (0x20d00108)                                                      */
/*      SPRC0 (0x20d00108)                                                      */
/*      SPRUCC (0x20d00108)                                                     */
/* Register default value on SPRA0: 0x00400000                                  */
/* Register default value on SPRB0: 0x00400000                                  */
/* Register default value on SPRC0: 0x00400000                                  */
/* Register default value on SPRUCC: 0x00400000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PCI Express AER Uncorrectable Error Mask Register
*/


#define PPAERUCM_IIO_CPM_PF_REG 0x21220108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved2 : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT32 dlpe : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               Data Link Protocol Error Mask : When 1 error
                               reporting is masked.Not applicable for IOSF.
                            */
    UINT32 sdes : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /* Surprise Down Error:Not supported. */
    UINT32 reserved1 : 6;

                            /* Bits[11:6], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 ptlpr : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               Poisoned TLP Received Error Mask : When 1 error
                               reporting is masked.
                            */
    UINT32 fcpes : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Flow Control Protocol Error Mask : When 1 error
                               reporting is masked.Not supported.Not applicable
                               for IOSF.
                            */
    UINT32 ct : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               Completion Time Out Error Mask : When 1 error
                               reporting is masked.
                            */
    UINT32 ca : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Completer Abort Error Mask : When 1 error
                               reporting is masked. Not supported.
                            */
    UINT32 ec : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Unexpected Completion Error Mask : When 1 error
                               reporting is masked.
                            */
    UINT32 ro : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               Receiver Overflow Error Mask : When 1 error
                               reporting is masked.Not applicable for IOSF.
                            */
    UINT32 mtlp : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Malformed TLP Error Mask : When 1 error
                               reporting is masked.
                            */
    UINT32 ecrcc : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               ECRC Check Error Mask : When 1 error reporting
                               is masked.
                            */
    UINT32 ur : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               Unsupported Request Error Mask : When 1 error
                               reporting is masked.
                            */
    UINT32 acs : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* ACS Violation Mask */
    UINT32 ieunc : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /*
                               Uncorrectable Internal Error Mask : When 1 error
                               reporting is masked.
                            */
    UINT32 reserved0 : 9;

                            /* Bits[31:23], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT32 Data;

} PPAERUCM_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPAERUCSEV_IIO_CPM_PF_REG supported on:                                      */
/*      SPRA0 (0x20d0010c)                                                      */
/*      SPRB0 (0x20d0010c)                                                      */
/*      SPRC0 (0x20d0010c)                                                      */
/*      SPRUCC (0x20d0010c)                                                     */
/* Register default value on SPRA0: 0x00440000                                  */
/* Register default value on SPRB0: 0x00440000                                  */
/* Register default value on SPRC0: 0x00440000                                  */
/* Register default value on SPRUCC: 0x00440000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PCI Express AER Uncorrectable Error Severity Register
*/


#define PPAERUCSEV_IIO_CPM_PF_REG 0x2122010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved2 : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000000*/

                            /* reserved2 */
    UINT32 dlpe : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               data link protocol error severity:Not applicable
                               for IOSF.
                            */
    UINT32 sdes : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               surprise down error severity:not supported.Not
                               applicable for IOSF.
                            */
    UINT32 reserved1 : 6;

                            /* Bits[11:6], Access Type=RO, default=0x00000000*/

                            /* reserved1 */
    UINT32 ptlpr : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* poisoned tlp received severity: */
    UINT32 fcpes : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               flow control protocol error severity:not
                               supported.Not applicable for IOSF.
                            */
    UINT32 ct : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* completion time out severity: */
    UINT32 ca : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /* completer abort severity: Not supported. */
    UINT32 ec : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* unexpected completion severity: */
    UINT32 ro : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               receiver overflow severity:Not applicable for
                               IOSF.
                            */
    UINT32 mtlp : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000001*/

                            /* malformed tlp severity: */
    UINT32 ecrcc : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* ecrc check severity: */
    UINT32 ur : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               unsupported request error status
                               severity(uress):
                            */
    UINT32 acs : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /* ACS Violation Severity */
    UINT32 ieunc : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /* Uncorrectable Internal Error status sevirity */
    UINT32 reserved0 : 9;

                            /* Bits[31:23], Access Type=RO, default=0x00000000*/

                            /* reserved0 */

  } Bits;
  UINT32 Data;

} PPAERUCSEV_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPAERCS_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x20d00110)                                                      */
/*      SPRB0 (0x20d00110)                                                      */
/*      SPRC0 (0x20d00110)                                                      */
/*      SPRUCC (0x20d00110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PCI Express AER Correctable Error Register
*/


#define PPAERCS_IIO_CPM_PF_REG 0x21220110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 res : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Receiver Error Status : Set whenever the
                               physical layer detects a receiver error.Not
                               supported since CPM is an IOSF device.
                            */
    UINT32 reserved2 : 5;

                            /* Bits[5:1], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT32 dlpe : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Bad TLP Status : Sets this bit on CRC errors or
                               sequence number out of range on TLP.Not
                               supported since CPM is an IOSF device.
                            */
    UINT32 bdllps : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               Bad DLLP Status : Sets this bit on CRC errors on
                               DLLP.Not supported since CPM is an IOSF device.
                            */
    UINT32 rnrs : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               REPLAY NUM Rollover Status : Set whenever the
                               replay number rolls over from 11 to 00.Not
                               supported since CPM is an IOSF device.
                            */
    UINT32 reserved1 : 3;

                            /* Bits[11:9], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 rtts : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Replay Timer Timeout Status : Set whenever a
                               replay timer timeout occurs.Not supported since
                               CPM is an IOSF device.
                            */
    UINT32 anfes : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Advisory Non Fatal Error status */
    UINT32 iecor : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Corrected Internal Error Status */
    UINT32 reserved0 : 17;

                            /* Bits[31:15], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT32 Data;

} PPAERCS_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPAERCM_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x20d00114)                                                      */
/*      SPRB0 (0x20d00114)                                                      */
/*      SPRC0 (0x20d00114)                                                      */
/*      SPRUCC (0x20d00114)                                                     */
/* Register default value on SPRA0: 0x00006000                                  */
/* Register default value on SPRB0: 0x00006000                                  */
/* Register default value on SPRC0: 0x00006000                                  */
/* Register default value on SPRUCC: 0x00006000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PCI Express AER Correctable Error Mask Register
*/


#define PPAERCM_IIO_CPM_PF_REG 0x21220114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 res : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Receiver Error Mask:Not supported since CPM is
                               an IOSF device
                            */
    UINT32 reserved2 : 5;

                            /* Bits[5:1], Access Type=RO, default=0x00000000*/

                            /* Reserved2 */
    UINT32 dlpe : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               Bad TLP Mask:Not supported since CPM is an IOSF
                               device
                            */
    UINT32 bdllps : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               Bad DLLP Mask:Not supported since CPM is an IOSF
                               device
                            */
    UINT32 rnrs : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               REPLAY NUM Rollover Mask:Not supported since CPM
                               is an IOSF device
                            */
    UINT32 reserved1 : 3;

                            /* Bits[11:9], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 rtts : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Replay Timer Timeout MaskNot supported since CPM
                               is an IOSF device
                            */
    UINT32 anfes : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /*
                               Advisory Non-Fatal Error Mask : this bit is set
                               by default to enable compatibility with software
                               that does not comprehend Role-Based Error
                               Reporting.
                            */
    UINT32 iecor : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /* Corrected Internal Error Status */
    UINT32 reserved0 : 17;

                            /* Bits[31:15], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT32 Data;

} PPAERCM_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPAERCTLCAP_IIO_CPM_PF_REG supported on:                                     */
/*      SPRA0 (0x20d00118)                                                      */
/*      SPRB0 (0x20d00118)                                                      */
/*      SPRC0 (0x20d00118)                                                      */
/*      SPRUCC (0x20d00118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PCI Express AER Control and Capability Register
*/


#define PPAERCTLCAP_IIO_CPM_PF_REG 0x21220118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tfep : 5;

                            /* Bits[4:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The First Error Pointer: Identifies the bit
                               position of the first error reported in the
                               pprucs. This register will not update until all
                               bits in the ERRUNC STS register are cleared.
                            */
    UINT32 ecrcgc : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Generation Capable : Indicates the EP(HI)
                               is not capable of generating ECRC.
                            */
    UINT32 ecrcge : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Generation Enable : When set enables ECRC
                               generation.
                            */
    UINT32 ecrccc : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Check Capable : Indicates EP(HI) is capable
                               of checking ECRC.
                            */
    UINT32 ecrcce : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               ECRC Check Enable : When set enables ECRC
                               checking.
                            */
    UINT32 reserved1 : 2;

                            /* Bits[10:9], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 tplp : 1;

                            /* Bits[11:11], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               TLP Prefix log present If Set and the First
                               Error Pointer is valid, indicates that the TLP
                               Prefix Log register contains valid information.
                               If Clear or if First Error Pointer is invalid,
                               the TLP Prefix Log register is undefined.
                               Default value of this bit is 0.
                            */
    UINT32 reserved : 20;

                            /* Bits[31:12], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT32 Data;

} PPAERCTLCAP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPAERHDRLOG0_IIO_CPM_PF_REG supported on:                                    */
/*      SPRA0 (0x20d0011c)                                                      */
/*      SPRB0 (0x20d0011c)                                                      */
/*      SPRC0 (0x20d0011c)                                                      */
/*      SPRUCC (0x20d0011c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PCI Express AER Header Log 0 Register
*/


#define PPAERHDRLOG0_IIO_CPM_PF_REG 0x2122011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdrlogdw0 : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               1st DWord of the Header for the PCI Express
                               packet in error (HDRLOGDW0): Once an error is
                               logged in this register, it remains locked for
                               further error logging until the time the
                               software clears the status bit that cause the
                               header log i.e. the error pointer is rearmed to
                               log again. The Header is stored in Big Endian
                               order as defined in PCIe spec.
                            */

  } Bits;
  UINT32 Data;

} PPAERHDRLOG0_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPAERHDRLOG1_IIO_CPM_PF_REG supported on:                                    */
/*      SPRA0 (0x20d00120)                                                      */
/*      SPRB0 (0x20d00120)                                                      */
/*      SPRC0 (0x20d00120)                                                      */
/*      SPRUCC (0x20d00120)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PCI Express AER Header Log 1 Register
*/


#define PPAERHDRLOG1_IIO_CPM_PF_REG 0x21220120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdrlogdw1 : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               2nd DWord of the Header for the PCI Express
                               packet in error (HDRLOGDW1): Once an error is
                               logged in this register, it remains locked for
                               further error logging until the time the
                               software clears the status bit that cause the
                               header log i.e. the error pointer is rearmed to
                               log again. The Header is stored in Big Endian
                               order as defined in PCIe spec.
                            */

  } Bits;
  UINT32 Data;

} PPAERHDRLOG1_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPAERHDRLOG2_IIO_CPM_PF_REG supported on:                                    */
/*      SPRA0 (0x20d00124)                                                      */
/*      SPRB0 (0x20d00124)                                                      */
/*      SPRC0 (0x20d00124)                                                      */
/*      SPRUCC (0x20d00124)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PCI Express AER Header Log 2 Register
*/


#define PPAERHDRLOG2_IIO_CPM_PF_REG 0x21220124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdrlogdw2 : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               3rd DWord of the Header for the PCI Express
                               packet in error (HDRLOGDW2): Once an error is
                               logged in this register, it remains locked for
                               further error logging until the time the
                               software clears the status bit that cause the
                               header log i.e. the error pointer is rearmed to
                               log again. The Header is stored in Big Endian
                               order as defined in PCIe spec.
                            */

  } Bits;
  UINT32 Data;

} PPAERHDRLOG2_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPAERHDRLOG3_IIO_CPM_PF_REG supported on:                                    */
/*      SPRA0 (0x20d00128)                                                      */
/*      SPRB0 (0x20d00128)                                                      */
/*      SPRC0 (0x20d00128)                                                      */
/*      SPRUCC (0x20d00128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PCI Express AER Header Log 3 Register
*/


#define PPAERHDRLOG3_IIO_CPM_PF_REG 0x21220128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hdrdwlog3 : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               4th DWord of the Header for the PCI Express
                               packet in error (HDRDWLOG3): Once an error is
                               logged in this register, it remains locked for
                               further error logging until the time the
                               software clears the status bit that cause the
                               header log i.e. the error pointer is rearmed to
                               log again. The Header is stored in Big Endian
                               order as defined in PCIe spec.
                            */

  } Bits;
  UINT32 Data;

} PPAERHDRLOG3_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPAERTPLOG_IIO_CPM_PF_REG supported on:                                      */
/*      SPRA0 (0x20d00138)                                                      */
/*      SPRB0 (0x20d00138)                                                      */
/*      SPRC0 (0x20d00138)                                                      */
/*      SPRUCC (0x20d00138)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PCI Express AER TLP Prefix Log Register
*/


#define PPAERTPLOG_IIO_CPM_PF_REG 0x21220138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlpprefixlog : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               TLP Prefix log for TLP corresponding to detected
                               error.
                            */

  } Bits;
  UINT32 Data;

} PPAERTPLOG_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PARIDHDR_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x20d00148)                                                      */
/*      SPRB0 (0x20d00148)                                                      */
/*      SPRC0 (0x20d00148)                                                      */
/*      SPRUCC (0x20d00148)                                                     */
/* Register default value on SPRA0: 0x1501000E                                  */
/* Register default value on SPRB0: 0x1501000E                                  */
/* Register default value on SPRC0: 0x1501000E                                  */
/* Register default value on SPRUCC: 0x1501000E                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Alternative Routing ID Capability Header
*/


#define PARIDHDR_IIO_CPM_PF_REG 0x21220148

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 arieci : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000000E*/

                            /*
                               PCI Express Extended Capability ID - The PCI SIG
                               has assigned 000Eh to the ARI extended
                               capability.
                            */
    UINT32 aricv : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               Capability Version - This is set to 1h for the
                               most current version of the specification.
                            */
    UINT32 arinco : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000150*/

                            /*
                               Next Capability Offset - This field contains
                               140h which points to the next item in the
                               extended capabilities list, the SR-IOV extended
                               capability.
                            */

  } Bits;
  UINT32 Data;

} PARIDHDR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PARIDCAP_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x10d0014c)                                                      */
/*      SPRB0 (0x10d0014c)                                                      */
/*      SPRC0 (0x10d0014c)                                                      */
/*      SPRUCC (0x10d0014c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* ARI Capabilities Register
*/


#define PARIDCAP_IIO_CPM_PF_REG 0x2121014C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 mfvc : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               MFVC Functional Groups Capability : Hardwired to
                               Zero as not support
                            */
    UINT16 acs : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Functional Groups Capability : Hardwired to
                               Zero as not support.
                            */
    UINT16 reserved : 6;

                            /* Bits[7:2], Access Type=RO, default=0x00000000*/

                            /* Reserved. */
    UINT16 vnfn : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               Next Function Number: The function number of the
                               next highest numbered PF in a multi-function
                               device.Only one physical function exists.
                               Therefore, value = 0h.
                            */

  } Bits;
  UINT16 Data;

} PARIDCAP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PARIDCTL_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x10d0014e)                                                      */
/*      SPRB0 (0x10d0014e)                                                      */
/*      SPRC0 (0x10d0014e)                                                      */
/*      SPRUCC (0x10d0014e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Alternative Routing ID Control Register
*/


#define PARIDCTL_IIO_CPM_PF_REG 0x2121014E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 mfvc : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               MFVC Functional Groups Enable : Hardwired to
                               Zero as CPM does not support.
                            */
    UINT16 acs : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Functional Groups Enable : Hardwired to Zero
                               as CPM does not support.
                            */
    UINT16 reserved1 : 2;

                            /* Bits[3:2], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT16 fg : 3;

                            /* Bits[6:4], Access Type=RO, default=0x00000000*/

                            /*
                               Function Group: Hardwired to Zero as CPM does
                               not support Function Groups.
                            */
    UINT16 reserved0 : 9;

                            /* Bits[15:7], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT16 Data;

} PARIDCTL_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVCAPID_IIO_CPM_PF_REG supported on:                                     */
/*      SPRA0 (0x10d00150)                                                      */
/*      SPRB0 (0x10d00150)                                                      */
/*      SPRC0 (0x10d00150)                                                      */
/*      SPRUCC (0x10d00150)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SR IOV Capability ID Register
*/


#define PSRIOVCAPID_IIO_CPM_PF_REG 0x21210150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 sriovod : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000010*/

                            /*
                               Capability ID: PCI SIG assigned a capability
                               record ID for SR-IOV per the 1.1 revision
                            */

  } Bits;
  UINT16 Data;

} PSRIOVCAPID_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVCVNC_IIO_CPM_PF_REG supported on:                                      */
/*      SPRA0 (0x10d00152)                                                      */
/*      SPRB0 (0x10d00152)                                                      */
/*      SPRC0 (0x10d00152)                                                      */
/*      SPRUCC (0x10d00152)                                                     */
/* Register default value on SPRA0: 0x00001B01                                  */
/* Register default value on SPRB0: 0x00001B01                                  */
/* Register default value on SPRC0: 0x00001B01                                  */
/* Register default value on SPRUCC: 0x00001B01                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV Capability Version and Next Capability Pointer Register
*/


#define PSRIOVCVNC_IIO_CPM_PF_REG 0x21210152

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 sriovcv : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000001*/

                            /*
                               Capability Version - This is set to 1h for the
                               SR-IOV.
                            */
    UINT16 sriovnco : 12;

                            /* Bits[15:4], Access Type=RO, default=0x000001B0*/

                            /*
                               Next Capability Offset. Points to ACS Capability
                               Structure.
                            */

  } Bits;
  UINT16 Data;

} PSRIOVCVNC_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVCAP_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x20d00154)                                                      */
/*      SPRB0 (0x20d00154)                                                      */
/*      SPRC0 (0x20d00154)                                                      */
/*      SPRUCC (0x20d00154)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV Capabilities Register 
*/


#define PSRIOVCAP_IIO_CPM_PF_REG 0x21220154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vfmcap : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               VF Migration Capable - If Set, the PF is
                               Migration Capable and operating under a
                               Migration Capable MR-PCIM.
                            */
    UINT32 reserved0 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /* Reserved. */
    UINT32 vftenbittagreqtrsuptd : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               VF 10-Bit Tag Requester Supported. If Set, all
                               VFs must support 10-Bit Tag Requester
                               capability. If Clear, VFs must not support
                               10-Bit Tag Requester capability. If the 10-Bit
                               Tag Requester Supported bit in the PFs Device
                               Capabilities 2 Register is Clear, this bit must
                               be Clear. Note: CPM doesn't support 10 Bit tag
                               when CPM is the requestor.
                            */
    UINT32 reserved1 : 18;

                            /* Bits[20:3], Access Type=RO, default=0x00000000*/

                            /* Reserved. */
    UINT32 vfmint : 11;

                            /* Bits[31:21], Access Type=RO, default=0x00000000*/

                            /*
                               VF Migration Interrupt Message Number -
                               Indicates the MSI vector used for migration
                               interrupts. The value in this field has no
                               significance if bit 0 of this capability is
                               Clear.
                            */

  } Bits;
  UINT32 Data;

} PSRIOVCAP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVCS_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x20d00158)                                                      */
/*      SPRB0 (0x20d00158)                                                      */
/*      SPRC0 (0x20d00158)                                                      */
/*      SPRUCC (0x20d00158)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV Control and Status Register
*/


#define PSRIOVCS_IIO_CPM_PF_REG 0x21220158

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vfe : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               VF Enable - Enables / Disables VFs. Default
                               value is 0b
                            */
    UINT32 vfme : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               VF Migration Enable - Enables / Disables VF
                               Migration Support.
                            */
    UINT32 vfmie : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               VF Migration Interrupt Enable - Enables /
                               Disables VF Migration State Change Interrupt.
                            */
    UINT32 vfmse : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               VF MSE - Memory Space Enable for Virtual
                               Functions.
                            */
    UINT32 vfari : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               ARI Capable Hierarchy - Device may locate VFs in
                               Function numbers 1 to 255 of the captured Bus
                               number. HI supports ARI and will locate the
                               VF1-VF128 in Function 1 through Function 128.
                               This field is reserved when device is configured
                               as RCiEP.
                            */
    UINT32 vftenbittagreqtren : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               VF 10-Bit Tag Requester Enable. If Set, all VFs
                               must use 10-Bit Tags for all Non-Posted Requests
                               they generate. If Clear, VFs must not use 10-Bit
                               Tags for Non-Posted Requests they generate. If
                               software changes the value of this bit while any
                               VFs have outstanding Non-Posted Requests, the
                               result is undefined. If the VF 10-Bit Tag
                               Requester Supported bit in the SR-IOV
                               Capabilities register is Clear, this bit is
                               permitted to be hardwired to 0b. Default value
                               is 0b. Note: CPM doesn't support 10 Bit tag as
                               requestor.
                            */
    UINT32 reserved : 26;

                            /* Bits[31:6], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSRIOVCS_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVMTOTINI_IIO_CPM_PF_REG supported on:                                   */
/*      SPRA0 (0x20d0015c)                                                      */
/*      SPRB0 (0x20d0015c)                                                      */
/*      SPRC0 (0x20d0015c)                                                      */
/*      SPRUCC (0x20d0015c)                                                     */
/* Register default value on SPRA0: 0x00100010                                  */
/* Register default value on SPRB0: 0x00100010                                  */
/* Register default value on SPRC0: 0x00100010                                  */
/* Register default value on SPRUCC: 0x00100010                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV Initial and Total VFs Register
*/


#define PSRIOVMTOTINI_IIO_CPM_PF_REG 0x2122015C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 initvf : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000010*/

                            /*
                               InitialVFs indicates the number of VFs that are
                               initially associated with the PF.Since EP(HI)
                               does not support VF migration this field must be
                               equal to the TOTVF field
                            */
    UINT32 totvf : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000010*/

                            /*
                               TotalVFs indicates the maximum number of VFs
                               that could be associated with the PF.Since
                               EP(HI) does not support VF migration this field
                               must be equal to the INITVF field
                            */

  } Bits;
  UINT32 Data;

} PSRIOVMTOTINI_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVNUMVF_IIO_CPM_PF_REG supported on:                                     */
/*      SPRA0 (0x20d00160)                                                      */
/*      SPRB0 (0x20d00160)                                                      */
/*      SPRC0 (0x20d00160)                                                      */
/*      SPRUCC (0x20d00160)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV Number of VFs Register
*/


#define PSRIOVNUMVF_IIO_CPM_PF_REG 0x21220160

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 numvf : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               Num VFs defines the number of VFs software has
                               assigned to the PF. Software sets NumVFs to any
                               value between 1 and TotalVF (see TotalVF above)
                               as part of the process of creating VFs.NumVFs
                               VFs shall be visible in the PCI-Express fabric
                               after both NumVFs is set to a valid value and VF
                               Enable is set to one. Visible in the PCI-Express
                               fabric means that the VF shall respond to PCI
                               Express transactions targeting the VF, following
                               all other rules defined by this specification
                               and the base specification.
                            */
    UINT32 fundep : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000000*/

                            /*
                               This field describes dependencies between PFs.
                               VF dependencies are the same as the
                               dependenciesof their associated PFs. If a PF is
                               independent from other PFs of a Device, this
                               field shall contain the Function Number of the
                               PF.
                            */
    UINT32 reserved : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PSRIOVNUMVF_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVFVFO_IIO_CPM_PF_REG supported on:                                      */
/*      SPRA0 (0x10d00164)                                                      */
/*      SPRB0 (0x10d00164)                                                      */
/*      SPRC0 (0x10d00164)                                                      */
/*      SPRUCC (0x10d00164)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV First VF Offset Register
*/


#define PSRIOVFVFO_IIO_CPM_PF_REG 0x21210164

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 vfoffset : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000001*/

                            /*
                               First VF Offset is a constant and defines the
                               Routing ID (RID) offset of the first VF that is
                               associated with the PF that contains this
                               Capability structure. The first VFs 16-bit RID
                               is calculated by adding the contents of this
                               field to the RID of the PF.
                            */

  } Bits;
  UINT16 Data;

} PSRIOVFVFO_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVVFS_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x10d00166)                                                      */
/*      SPRB0 (0x10d00166)                                                      */
/*      SPRC0 (0x10d00166)                                                      */
/*      SPRUCC (0x10d00166)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV VF Stride Register
*/


#define PSRIOVVFS_IIO_CPM_PF_REG 0x21210166

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 vfstride : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000001*/

                            /*
                               VF Stride is a constant and defines the Routing
                               ID (RID) offset from one VF to the next one for
                               all VFs associated with the PF that contains
                               this Capability structure. The nextVFs 16-bit
                               RID is calculated by adding the contents of this
                               field to the RID of the current VF.
                            */

  } Bits;
  UINT16 Data;

} PSRIOVVFS_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVFDID_IIO_CPM_PF_REG supported on:                                      */
/*      SPRA0 (0x20d00168)                                                      */
/*      SPRB0 (0x20d00168)                                                      */
/*      SPRC0 (0x20d00168)                                                      */
/*      SPRUCC (0x20d00168)                                                     */
/* Register default value on SPRA0: 0x006F0000                                  */
/* Register default value on SPRB0: 0x006F0000                                  */
/* Register default value on SPRC0: 0x006F0000                                  */
/* Register default value on SPRUCC: 0x006F0000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV VF Device ID Register
*/


#define PSRIOVFDID_IIO_CPM_PF_REG 0x21220168

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT32 vsriovdid : 16;

                            /* Bits[31:16], Access Type=RO/V, default=0x0000006F*/

                            /*
                               VF Device ID. The Device ID that is presented to
                               the OS for every VF. vsriovdid is equal to (pdid
                               + 1). Refer to PDID.
                            */

  } Bits;
  UINT32 Data;

} PSRIOVFDID_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVPAGESIZE_IIO_CPM_PF_REG supported on:                                  */
/*      SPRA0 (0x20d0016c)                                                      */
/*      SPRB0 (0x20d0016c)                                                      */
/*      SPRC0 (0x20d0016c)                                                      */
/*      SPRUCC (0x20d0016c)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV Supported Page Size Register
*/


#define PSRIOVPAGESIZE_IIO_CPM_PF_REG 0x2122016C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 supsyspagsz : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000001*/

                            /*
                               This field defines the supported system page
                               sizes. This PF supports a page size of 2^(n+12)
                               if bit n is Set. For example, if bit 0 is Set,
                               the PF supports 4k byte page sizes. Only 4K
                               pages size is supported. A page size describes
                               the minimum alignment requirements for VF BAR
                               resources.
                            */

  } Bits;
  UINT32 Data;

} PSRIOVPAGESIZE_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVSYSPS_IIO_CPM_PF_REG supported on:                                     */
/*      SPRA0 (0x20d00170)                                                      */
/*      SPRB0 (0x20d00170)                                                      */
/*      SPRC0 (0x20d00170)                                                      */
/*      SPRUCC (0x20d00170)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV System Page Size Register
*/


#define PSRIOVSYSPS_IIO_CPM_PF_REG 0x21220170

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 syspagsiz : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000001*/

                            /*
                               This field must define the page size the system
                               will use to map the VFs memory addresses.
                               Software must set the value of the System Page
                               Size to one of the page sizes set in the
                               Supported Page Sizes field. As with Supported
                               Page Sizes, if bit n is Set in System Page Size,
                               the VFs associated with this PF are required to
                               support a page size of 2^(n+12). For example, if
                               bit 1 is Set, the system is using an 8k byte
                               page size. The results are undefined if more
                               than one bit is set in System Page Size. The
                               results are undefined if a bit is Set in System
                               Page Size that is not Set in Supported Page
                               Sizes. When System Page Size is set, the VF
                               associated with this PF is required to align all
                               BAR resources on a System Page Size boundary.
                               Each VF BARn or VF BARn pair (described below)
                               shall be aligned on a System Page Size boundary.
                               Each VF BARn or VF BARn pair defining a non-zero
                               address space shall be sized to consume an
                               integer multiple of System Page Size bytes. All
                               data structures requiring page size alignment
                               within a VF shall be aligned on a System Page
                               Size boundary.
                            */

  } Bits;
  UINT32 Data;

} PSRIOVSYSPS_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVLBAR0_IIO_CPM_PF_REG supported on:                                     */
/*      SPRA0 (0x20d00174)                                                      */
/*      SPRB0 (0x20d00174)                                                      */
/*      SPRC0 (0x20d00174)                                                      */
/*      SPRUCC (0x20d00174)                                                     */
/* Register default value on SPRA0: 0x00000004                                  */
/* Register default value on SPRB0: 0x00000004                                  */
/* Register default value on SPRC0: 0x00000004                                  */
/* Register default value on SPRUCC: 0x00000004                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV Lower BAR0 Register. 64KB Windows per VF.
*/


#define PSRIOVLBAR0_IIO_CPM_PF_REG 0x21220174

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vfmem : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Space Indicator: Hardwired to 0 to
                               identify the region as in memory space.
                            */
    UINT32 vftyp : 2;

                            /* Bits[2:1], Access Type=RO, default=0x00000002*/

                            /*
                               Addressing Type: Hardwired to indicate a 64-bit
                               region.
                            */
    UINT32 vfpref : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Prefetchable: Hardwired to 0 to indicate that
                               the region is not prefetchable. - To Be
                               Determined
                            */
    UINT32 vfzero : 11;

                            /* Bits[14:4], Access Type=RO, default=0x00000000*/

                            /* Lower Bits: Hardwired to 0 (32KB region). */
    UINT32 vfbar0 : 17;

                            /* Bits[31:15], Access Type=RW, default=0x00000000*/

                            /*
                               Lower SRIOV Base Address 0: These bits are used
                               to define the memory window for Virtual
                               Functions. This window is used by Virtual
                               Functions to access the WQM Ring CSRs.
                            */

  } Bits;
  UINT32 Data;

} PSRIOVLBAR0_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVUBAR0_IIO_CPM_PF_REG supported on:                                     */
/*      SPRA0 (0x20d00178)                                                      */
/*      SPRB0 (0x20d00178)                                                      */
/*      SPRC0 (0x20d00178)                                                      */
/*      SPRUCC (0x20d00178)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV Upper BAR0 Register
*/


#define PSRIOVUBAR0_IIO_CPM_PF_REG 0x21220178

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vfbar0 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Upper SRIOV Base Address 0. */

  } Bits;
  UINT32 Data;

} PSRIOVUBAR0_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVLBAR1_IIO_CPM_PF_REG supported on:                                     */
/*      SPRA0 (0x20d0017c)                                                      */
/*      SPRB0 (0x20d0017c)                                                      */
/*      SPRC0 (0x20d0017c)                                                      */
/*      SPRUCC (0x20d0017c)                                                     */
/* Register default value on SPRA0: 0x00000004                                  */
/* Register default value on SPRB0: 0x00000004                                  */
/* Register default value on SPRC0: 0x00000004                                  */
/* Register default value on SPRUCC: 0x00000004                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV Lower BAR1 Register. This VF window is 4KB.
*/


#define PSRIOVLBAR1_IIO_CPM_PF_REG 0x2122017C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vfmem : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Space Indicator: Hardwired to 0 to
                               identify the region as in memory space.
                            */
    UINT32 vftyp : 2;

                            /* Bits[2:1], Access Type=RO, default=0x00000002*/

                            /*
                               Addressing Type: Hardwired to indicate a 64-bit
                               region.
                            */
    UINT32 vfpref : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Prefetchable: Hardwired to 0 to indicate that
                               the region is not prefetchable.
                            */
    UINT32 vfzero : 9;

                            /* Bits[12:4], Access Type=RO, default=0x00000000*/

                            /* Lower Bits: Hardwired to 0 (8KB region) */
    UINT32 vfbar1 : 19;

                            /* Bits[31:13], Access Type=RW, default=0x00000000*/

                            /*
                               Lower SRIOV Base Address 1: These bits are used
                               to define the memory window for Virtual
                               Functions. This window is used by Virtual
                               Functions to access: VINTSOU[x]. VINTMSK[x],
                               PF2VM[x] , VM2PF[x], VINTSOUPF2VM[x] registers.
                            */

  } Bits;
  UINT32 Data;

} PSRIOVLBAR1_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVUBAR1_IIO_CPM_PF_REG supported on:                                     */
/*      SPRA0 (0x20d00180)                                                      */
/*      SPRB0 (0x20d00180)                                                      */
/*      SPRC0 (0x20d00180)                                                      */
/*      SPRUCC (0x20d00180)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV Upper BAR1 Register
*/


#define PSRIOVUBAR1_IIO_CPM_PF_REG 0x21220180

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vfbar1 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Upper SRIOV Base Address 1. */

  } Bits;
  UINT32 Data;

} PSRIOVUBAR1_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVLBAR2_IIO_CPM_PF_REG supported on:                                     */
/*      SPRA0 (0x20d00184)                                                      */
/*      SPRB0 (0x20d00184)                                                      */
/*      SPRC0 (0x20d00184)                                                      */
/*      SPRUCC (0x20d00184)                                                     */
/* Register default value on SPRA0: 0x00000004                                  */
/* Register default value on SPRB0: 0x00000004                                  */
/* Register default value on SPRC0: 0x00000004                                  */
/* Register default value on SPRUCC: 0x00000004                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV Lower BAR2 Register. This VF window is 32KB.
*/


#define PSRIOVLBAR2_IIO_CPM_PF_REG 0x21220184

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vfmem : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Space Indicator: Hardwired to 0 to
                               identify the region as in memory space.
                            */
    UINT32 vftyp : 2;

                            /* Bits[2:1], Access Type=RO, default=0x00000002*/

                            /*
                               Addressing Type: Hardwired to indicate a 64-bit
                               region.
                            */
    UINT32 vfpref : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Prefetchable: Hardwired to 0 to indicate that
                               the region is not prefetchable.
                            */
    UINT32 vfzero : 11;

                            /* Bits[14:4], Access Type=RO, default=0x00000000*/

                            /* Lower Bits: Hardwired to 0 (32KB region) */
    UINT32 vfbar2 : 17;

                            /* Bits[31:15], Access Type=RW, default=0x00000000*/

                            /*
                               Lower SRIOV Base Address 2: These bits are used
                               to define the memory window for Virtual
                               Functions. This window is used by Virtual
                               Functions to access UQs.
                            */

  } Bits;
  UINT32 Data;

} PSRIOVLBAR2_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVUBAR2_IIO_CPM_PF_REG supported on:                                     */
/*      SPRA0 (0x20d00188)                                                      */
/*      SPRB0 (0x20d00188)                                                      */
/*      SPRC0 (0x20d00188)                                                      */
/*      SPRUCC (0x20d00188)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV Upper BAR1 Register
*/


#define PSRIOVUBAR2_IIO_CPM_PF_REG 0x21220188

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vfbar2 : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* Upper SRIOV Base Address 2. */

  } Bits;
  UINT32 Data;

} PSRIOVUBAR2_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSRIOVVFMA_IIO_CPM_PF_REG supported on:                                      */
/*      SPRA0 (0x20d0018c)                                                      */
/*      SPRB0 (0x20d0018c)                                                      */
/*      SPRC0 (0x20d0018c)                                                      */
/*      SPRUCC (0x20d0018c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV VF Migration Array Register
*/


#define PSRIOVVFMA_IIO_CPM_PF_REG 0x2122018C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reserved : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} PSRIOVVFMA_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */



#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PACSCAPID_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x10d001b0)                                                      */
/*      SPRB0 (0x10d001b0)                                                      */
/*      SPRC0 (0x10d001b0)                                                      */
/*      SPRUCC (0x10d001b0)                                                     */
/* Register default value on SPRA0: 0x0000000D                                  */
/* Register default value on SPRB0: 0x0000000D                                  */
/* Register default value on SPRC0: 0x0000000D                                  */
/* Register default value on SPRUCC: 0x0000000D                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* This is ACS capability .
*/


#define PACSCAPID_IIO_CPM_PF_REG 0x212101B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 acscid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000000D*/

                            /*
                               Capability ID: PCI SIG assigned a capability
                               record ID for CAS per the 1.0 revision
                            */

  } Bits;
  UINT16 Data;

} PACSCAPID_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PACSCVNC_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x10d001b2)                                                      */
/*      SPRB0 (0x10d001b2)                                                      */
/*      SPRC0 (0x10d001b2)                                                      */
/*      SPRUCC (0x10d001b2)                                                     */
/* Register default value on SPRA0: 0x00001B81                                  */
/* Register default value on SPRB0: 0x00001B81                                  */
/* Register default value on SPRC0: 0x00001B81                                  */
/* Register default value on SPRUCC: 0x00001B81                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* ACS Capability Version
*/


#define PACSCVNC_IIO_CPM_PF_REG 0x212101B2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 acscv : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000001*/

                            /*
                               Capability Version - This is set to 1h for SR-
                               IOV.
                            */
    UINT16 acsco : 12;

                            /* Bits[15:4], Access Type=RO, default=0x000001B8*/

                            /*
                               Next Capability Offset. Points to ATS capability
                               structure.
                            */

  } Bits;
  UINT16 Data;

} PACSCVNC_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PACSCAP_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x20d001b4)                                                      */
/*      SPRB0 (0x20d001b4)                                                      */
/*      SPRC0 (0x20d001b4)                                                      */
/*      SPRUCC (0x20d001b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* ACS Capabilities Register
*/


#define PACSCAP_IIO_CPM_PF_REG 0x212201B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 acssv : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Source Validation (V). Hard-wired to zero,
                               not supported.
                            */
    UINT32 acstb : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Translation Blocking (B). Hard-wired to
                               zero, not supported.
                            */
    UINT32 acsp2prr : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Request Redirect (R). Hard-wired to
                               zero, not supported.
                            */
    UINT32 acsp2pcr : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Completion Redirect (C). Hard-wired to
                               zero, not supported.
                            */
    UINT32 acsuf : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Upstream Forwarding (U). Hard-wired to zero,
                               not supported.
                            */
    UINT32 acsp2pec : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Egress Control (E). Hard-wired to zero,
                               not supported.
                            */
    UINT32 acsdtp2p : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Direct Translated P2P (T). Hard-wired to
                               zero, not supported.
                            */
    UINT32 reserved1 : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 acsecvs : 8;

                            /* Bits[15:8], Access Type=RO, default=0x00000000*/

                            /*
                               Egress Control Vector Size..Hard-wired to zero,
                               not supported.
                            */
    UINT32 acssve : 1;

                            /* Bits[16:16], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Source Validation Enable (V). Hard-wired to
                               zero, not supported.
                            */
    UINT32 acstbe : 1;

                            /* Bits[17:17], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Translation Blocking Enable (B). Hard-wired
                               to zero, not supported.
                            */
    UINT32 acsp2prre : 1;

                            /* Bits[18:18], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Request Redirect Enable (R). Hard-wired
                               to zero, not supported.
                            */
    UINT32 acsp2pcre : 1;

                            /* Bits[19:19], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Completion Redirect Enable (C). Hard-
                               wired to zero, not supported.
                            */
    UINT32 acsufe : 1;

                            /* Bits[20:20], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Upstream Forwarding Enable (U). Hard-wired
                               to zero, not supported.
                            */
    UINT32 acsp2pece : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               ACS P2P Egress Control Enable (E). Hard-wired to
                               zero, not supported.
                            */
    UINT32 acsdtp2pe : 1;

                            /* Bits[22:22], Access Type=RO, default=0x00000000*/

                            /*
                               ACS Direct Translated P2P Enable (T). Hard-wired
                               to zero, not supported.
                            */
    UINT32 reserved : 9;

                            /* Bits[31:23], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT32 Data;

} PACSCAP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PATSHDR_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x20d001b8)                                                      */
/*      SPRB0 (0x20d001b8)                                                      */
/*      SPRC0 (0x20d001b8)                                                      */
/*      SPRUCC (0x20d001b8)                                                     */
/* Register default value on SPRA0: 0x1C01000F                                  */
/* Register default value on SPRB0: 0x1C01000F                                  */
/* Register default value on SPRC0: 0x1C01000F                                  */
/* Register default value on SPRUCC: 0x1C01000F                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* ATS Capability Header
*/


#define PATSHDR_IIO_CPM_PF_REG 0x212201B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 atseci : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000000F*/

                            /*
                               PCI Express Extended Capability ID - The PCI SIG
                               has assigned 000Fh to the ATS extended
                               capability.
                            */
    UINT32 atscv : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               Capability Version - This is set to 1h for the
                               most current version of the specification.
                            */
    UINT32 atsnco : 12;

                            /* Bits[31:20], Access Type=RO, default=0x000001C0*/

                            /*
                               Next Capability Offset - Points to PRS
                               Capability Structure.
                            */

  } Bits;
  UINT32 Data;

} PATSHDR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PATSCAP_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x10d001bc)                                                      */
/*      SPRB0 (0x10d001bc)                                                      */
/*      SPRC0 (0x10d001bc)                                                      */
/*      SPRUCC (0x10d001bc)                                                     */
/* Register default value on SPRA0: 0x00000060                                  */
/* Register default value on SPRB0: 0x00000060                                  */
/* Register default value on SPRC0: 0x00000060                                  */
/* Register default value on SPRUCC: 0x00000060                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* ATS Capability Register
*/


#define PATSCAP_IIO_CPM_PF_REG 0x212101BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 iqd : 5;

                            /* Bits[4:0], Access Type=RO, default=0x00000000*/

                            /*
                               Invalidate Queue Depth - The number of
                               Invalidate Requests that the Function can accept
                               before putting backpressure on the Upstream
                               connection. If 0 0000b, the Function can accept
                               32 Invalidate Requests.
                            */
    UINT16 par : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000001*/

                            /*
                               Page Aligned Request - If Set, indicates the
                               Untranslated Address is always aligned to a 4096
                               byte boundary. Setting this field is
                               recommended. This field permits software to
                               distinguish between implementations compatible
                               with this specification and those compatible
                               with an earlier version of this specification
                               that in which a Requester was permitted to
                               supply anything in bits [11:2].
                            */
    UINT16 gis : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000001*/

                            /*
                               Global Invalidate Support - If Set, the Function
                               supports Invalidation Requests that have the
                               Global Invalidate bit Set. If Clear, the
                               Function ignores the Global Invalidate bit in
                               all Invalidate Requests.
                            */
    UINT16 rsvd : 9;

                            /* Bits[15:7], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PATSCAP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PATSCTL_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x10d001be)                                                      */
/*      SPRB0 (0x10d001be)                                                      */
/*      SPRC0 (0x10d001be)                                                      */
/*      SPRUCC (0x10d001be)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* ATS Control Register
*/


#define PATSCTL_IIO_CPM_PF_REG 0x212101BE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 stu : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /*
                               Smallest Translation Unit (STU) This value
                               indicates to the Function the minimum number of
                               4096-byte blocks that is indicated in a
                               Translation Completions or Invalidate Requests.
                               This is a power of 2 multiplier and the number
                               of blocks is 2STU. A value of 0 0000b indicates
                               one block and a value of 1 1111b indicates 231
                               blocks (or 8 TB total). Default value is 0
                               0000b.
                            */
    UINT16 reserved1 : 10;

                            /* Bits[14:5], Access Type=RO, default=0x00000000*/

                            /* Reserved */
    UINT16 en : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When Set, the Function is enabled to cache
                               translations. Default value is 0b.
                            */

  } Bits;
  UINT16 Data;

} PATSCTL_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPRSHDR_IIO_CPM_PF_REG supported on:                                         */
/*      SPRA0 (0x20d001c0)                                                      */
/*      SPRB0 (0x20d001c0)                                                      */
/*      SPRC0 (0x20d001c0)                                                      */
/*      SPRUCC (0x20d001c0)                                                     */
/* Register default value on SPRA0: 0x1D010013                                  */
/* Register default value on SPRB0: 0x1D010013                                  */
/* Register default value on SPRC0: 0x1D010013                                  */
/* Register default value on SPRUCC: 0x1D010013                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PRS Capability Header
*/


#define PPRSHDR_IIO_CPM_PF_REG 0x212201C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 prseci : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000013*/

                            /*
                               PCI Express Extended Capability ID - The PCI SIG
                               has assigned 0013h to the PRS extended
                               capability.
                            */
    UINT32 prscv : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               Capability Version - This is set to 0h for the
                               most current version of the specification.
                            */
    UINT32 prsnco : 12;

                            /* Bits[31:20], Access Type=RO, default=0x000001D0*/

                            /*
                               Next Capability Offset - Points to PASID TLP
                               Prefix Capability Structure.
                            */

  } Bits;
  UINT32 Data;

} PPRSHDR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPRSPRCTL_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x10d001c4)                                                      */
/*      SPRB0 (0x10d001c4)                                                      */
/*      SPRC0 (0x10d001c4)                                                      */
/*      SPRUCC (0x10d001c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* This is PRS Control.
*/


#define PPRSPRCTL_IIO_CPM_PF_REG 0x212101C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 en : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               This field, when set, indicates that the
                               associated device is allowed to make page
                               requests. If this field is clear, the associated
                               device is not allowed to issue page requests. If
                               both this flag and the Stopped flag are clear,
                               then the associated device will not issue any
                               additional page requests, but there are
                               previously issued page requests that have not
                               yet completed and the device is waiting to
                               complete all outstanding transactions before
                               transitioning to the Stopped state. When a
                               devices Page Request Interface is transitioned
                               from not-Enabled to Enabled, its status flags
                               (Stopped, Response Failure, and Unexpected
                               Response flags) are cleared. Enabling a device
                               that has not successfully stopped has
                               indeterminate results. Default value is 0b.
                            */
    UINT16 reset1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When the Enable flag is clear, or is being
                               cleared in the same register update that sets
                               this flag, writing a 1 to this flag, clears the
                               associated implementation dependent page request
                               credit counter and pending request state for the
                               associated Page Request Interface. When the
                               Enable flag is set and/or when a zero is written
                               to this flag, this flag does not initiate any
                               actions. Reads of this field return 0b
                            */
    UINT16 reserved1 : 14;

                            /* Bits[15:2], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT16 Data;

} PPRSPRCTL_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPRSPRSTS_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x10d001c6)                                                      */
/*      SPRB0 (0x10d001c6)                                                      */
/*      SPRC0 (0x10d001c6)                                                      */
/*      SPRUCC (0x10d001c6)                                                     */
/* Register default value on SPRA0: 0x00008100                                  */
/* Register default value on SPRB0: 0x00008100                                  */
/* Register default value on SPRC0: 0x00008100                                  */
/* Register default value on SPRUCC: 0x00008100                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PRS Response Status
*/


#define PPRSPRSTS_IIO_CPM_PF_REG 0x212101C6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 rf : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Response Failure (RF) This flag, when set,
                               indicates that the device has received a PRG
                               Response Message indicating a Response Failure.
                               The device expects no further responses from the
                               host (any received are ignored). This flag is
                               set by the device and cleared when a one is
                               written to the flag. Default value is 0b.
                            */
    UINT16 uprgi : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               Unexpected Page Request Group Index (UPRGI) This
                               flag, when set, indicates that the device has
                               received a PRG Response Message containing a PRG
                               index that has no matching request. This flag is
                               set by the device and cleared when a one is
                               written to the flag. Default value is 0b.
                            */
    UINT16 reserved2 : 6;

                            /* Bits[7:2], Access Type=RO, default=0x00000000*/

                            /* Reserved. */
    UINT16 s : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000001*/

                            /*
                               Stopped (S) This flag, when set, indicates that
                               the associated page request interface will not
                               issue any page requests, but that at least one
                               previously issued request has yet to complete.
                               When the Enable flag is cleared, after having
                               been previously set, the interface transitions
                               to the stopped state and sets this flag. After
                               all page requests currently outstanding at the
                               device have completed, this flag is cleared and
                               the interface enters the disabled state (the
                               flag will not transition from set to clear if no
                               requests are outstanding when the interface is
                               disabled). Resetting the interface will cause an
                               immediate transition to the disabled state.
                               While in the stopping state, receipt of a
                               Response Failure message will result in the
                               immediate transition to the disabled state (this
                               flag clears). Default value is 1b.
                            */
    UINT16 reserved3 : 6;

                            /* Bits[14:9], Access Type=RO, default=0x00000000*/

                            /* Reserved. */
    UINT16 prgspr : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000001*/

                            /*
                               PRG Response PASID Required - If Set, the
                               Function expects a PASID TLP Prefix on PRG
                               Response Messages when the corresponding Page
                               Requests had a PASID TLP Prefix. If Clear, the
                               Function does not expect PASID TLP Prefixes on
                               any PRG Response Message. Function behavior is
                               undefined if this bit is Clear and the Function
                               receives a PRG Response Message with a PASID TLP
                               Prefix. Function behavior is undefined if this
                               bit is Set and the Function receives a PRG
                               Response Message with no PASID TLP Prefix when
                               the corresponding Page Requests had a PASID TLP
                               Prefix.
                            */

  } Bits;
  UINT16 Data;

} PPRSPRSTS_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPRSOPRCL_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x20d001c8)                                                      */
/*      SPRB0 (0x20d001c8)                                                      */
/*      SPRC0 (0x20d001c8)                                                      */
/*      SPRUCC (0x20d001c8)                                                     */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Outstanding Page Request Capacity Limit.
*/


#define PPRSOPRCL_IIO_CPM_PF_REG 0x212201C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 oprcl : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000040*/

                            /*
                               Outstanding Page Request Capacity Limit This
                               field contains the number of outstanding page
                               request messages the associated device
                               physically supports. This is the upper limit on
                               the number of pages that can be usefully
                               allocated to the device (an allocation larger
                               than this limit is treated as equal to this
                               limit).
                            */

  } Bits;
  UINT32 Data;

} PPRSOPRCL_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPRSOPRAL_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x20d001cc)                                                      */
/*      SPRB0 (0x20d001cc)                                                      */
/*      SPRC0 (0x20d001cc)                                                      */
/*      SPRUCC (0x20d001cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Outstanding Page Request Allocation Limit. 
*/


#define PPRSOPRAL_IIO_CPM_PF_REG 0x212201CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 opral : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Outstanding Page Request Allocation Limit This
                               field contains the number of outstanding page
                               request messages the associated device is
                               allowed to issue (have outstanding at any given
                               instance). Behavior is undefined if this flag is
                               changed while the Enable flag is set. The number
                               of PRGs a device has outstanding is less than or
                               equal to the number of request messages it has
                               issued. For example, if system software
                               allocates 1000 messages to a device then a
                               single PRG could use all 1000 of the possible
                               requests. Conversely, at one request per PRG the
                               device would run out of PRG indices (of which
                               there are only 512) before it consumes all its
                               page request credits. A device must pre-allocate
                               its request availability for any given PRG, that
                               is, all the requests required by a given PRG
                               must be available before any of the requests may
                               be issued. Default value is 0b.
                            */

  } Bits;
  UINT32 Data;

} PPRSOPRAL_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPASIDHDR_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x20d001d0)                                                      */
/*      SPRB0 (0x20d001d0)                                                      */
/*      SPRC0 (0x20d001d0)                                                      */
/*      SPRUCC (0x20d001d0)                                                     */
/* Register default value on SPRA0: 0x1D81001B                                  */
/* Register default value on SPRB0: 0x1D81001B                                  */
/* Register default value on SPRC0: 0x1D81001B                                  */
/* Register default value on SPRUCC: 0x1D81001B                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PASID Capability Header
*/


#define PPASIDHDR_IIO_CPM_PF_REG 0x212201D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pasideci : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000001B*/

                            /*
                               PCI Express Extended Capability ID - The PCI SIG
                               has assigned 001Bh to the PASID extended
                               capability.
                            */
    UINT32 pasidcv : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               Capability Version - This is set to 1h for the
                               most current version of the specification.
                            */
    UINT32 pasidnco : 12;

                            /* Bits[31:20], Access Type=RO, default=0x000001D8*/

                            /*
                               Next Capability Offset: Points to sIOV DVSEC
                               Capability Structure.
                            */

  } Bits;
  UINT32 Data;

} PPASIDHDR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPASIDCAP_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x10d001d4)                                                      */
/*      SPRB0 (0x10d001d4)                                                      */
/*      SPRC0 (0x10d001d4)                                                      */
/*      SPRUCC (0x10d001d4)                                                     */
/* Register default value on SPRA0: 0x00001404                                  */
/* Register default value on SPRB0: 0x00001404                                  */
/* Register default value on SPRC0: 0x00001404                                  */
/* Register default value on SPRUCC: 0x00001404                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PASID Capability Register
*/


#define PPASIDCAP_IIO_CPM_PF_REG 0x212101D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 reserved1 : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /* Reserved. */
    UINT16 eps : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Execute Permission Supported If Set, the
                               Endpoint supports sending TLPs that have the
                               Execute Requested bit Set. If Clear, the
                               Endpoint will never Set the Execute Requested
                               bit.
                            */
    UINT16 pms : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000001*/

                            /*
                               Privileged Mode Supported If Set, the Endpoint
                               supports operating in Privileged and Non-
                               Privileged modes, and supports sending requests
                               that have the Privileged Mode Requested bit Set.
                               If Clear, the Endpoint will never Set the
                               Privileged Mode Requested bit.
                            */
    UINT16 reserved2 : 5;

                            /* Bits[7:3], Access Type=RO, default=0x00000000*/

                            /* Reserved. */
    UINT16 mpw : 5;

                            /* Bits[12:8], Access Type=RO, default=0x00000014*/

                            /*
                               Max PASID Width Indicates the width of the PASID
                               field supported by the Endpoint. The value n
                               indicates support for PASID values 0 through
                               2n-1 (inclusive). The value 0 indicates support
                               for a single PASID (0). The value 20 indicates
                               support for all PASID values (20 bits). This
                               field must be between 0 and 20 (inclusive).
                            */
    UINT16 reserved3 : 3;

                            /* Bits[15:13], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT16 Data;

} PPASIDCAP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PPASIDCTL_IIO_CPM_PF_REG supported on:                                       */
/*      SPRA0 (0x10d001d6)                                                      */
/*      SPRB0 (0x10d001d6)                                                      */
/*      SPRC0 (0x10d001d6)                                                      */
/*      SPRUCC (0x10d001d6)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* PASID Control Register
*/


#define PPASIDCTL_IIO_CPM_PF_REG 0x212101D6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 pasidenable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               PASID Enable If Set, the Endpoint is permitted
                               to send and receive TLPs that contain a PASID
                               TLP Prefix. If Clear, the Endpoint is not
                               permitted to do so. Behavior is undefined if the
                               Endpoint supports ATS and this bit changes value
                               when ATS Enable (E) is Set (see the Address
                               Translation Specification). Default is 0b.
                            */
    UINT16 epe : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Execute Permission Enable If Set, the Endpoint
                               is permitted to send Requests that have Execute
                               Requested Set. If Clear, the Endpoint is not
                               permitted to do so. Behavior is undefined if the
                               Endpoint supports ATS and this bit changes value
                               when ATS Enable is Set (see the Address
                               Translation Specification). If Execute
                               Permission Supported is Clear, this bit is
                               RsvdP. Default is 0b
                            */
    UINT16 pme : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Privileged Mode Enable If Set, the Endpoint is
                               permitted to send Requests that have the
                               Privileged Mode Requested Set. If Clear, the
                               Endpoint is not permitted to do so. Behavior is
                               undefined if the Endpoint supports ATS and this
                               bit changes value when ATS Enable is Set (see
                               the Address Translation Specification). If
                               Privileged Mode Supported is Clear, this bit is
                               RsvdP. Default is 0b.
                            */
    UINT16 rsvd : 13;

                            /* Bits[15:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PPASIDCTL_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSIOVCAPID_IIO_CPM_PF_REG supported on:                                      */
/*      SPRA0 (0x20d001d8)                                                      */
/*      SPRB0 (0x20d001d8)                                                      */
/*      SPRC0 (0x20d001d8)                                                      */
/*      SPRUCC (0x20d001d8)                                                     */
/* Register default value on SPRA0: 0x00010023                                  */
/* Register default value on SPRB0: 0x00010023                                  */
/* Register default value on SPRC0: 0x00010023                                  */
/* Register default value on SPRUCC: 0x00010023                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* sIOV Capability ID Register
*/


#define PSIOVCAPID_IIO_CPM_PF_REG 0x212201D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 siovod : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000023*/

                            /* Capability ID: PCI SIG assigned capability. */
    UINT32 cv : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               Capability Version - This is set to 0h for the
                               most current version of the specification.
                            */
    UINT32 nco : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000000*/

                            /* Next Capability Offset - Last Capability. */

  } Bits;
  UINT32 Data;

} PSIOVCAPID_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSIOVDVSEC0_IIO_CPM_PF_REG supported on:                                     */
/*      SPRA0 (0x20d001dc)                                                      */
/*      SPRB0 (0x20d001dc)                                                      */
/*      SPRC0 (0x20d001dc)                                                      */
/*      SPRUCC (0x20d001dc)                                                     */
/* Register default value on SPRA0: 0x01808086                                  */
/* Register default value on SPRB0: 0x01808086                                  */
/* Register default value on SPRC0: 0x01808086                                  */
/* Register default value on SPRUCC: 0x01808086                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* sIOV DVSEC Register 0.
*/


#define PSIOVDVSEC0_IIO_CPM_PF_REG 0x212201DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 vid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /* DVSEC Vendor ID. */
    UINT32 rev : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000000*/

                            /* This is DVSEC Revision. */
    UINT32 length : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000018*/

                            /* This is DVSEC Length. */

  } Bits;
  UINT32 Data;

} PSIOVDVSEC0_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSIOVDVSEC1_IIO_CPM_PF_REG supported on:                                     */
/*      SPRA0 (0x20d001e0)                                                      */
/*      SPRB0 (0x20d001e0)                                                      */
/*      SPRC0 (0x20d001e0)                                                      */
/*      SPRUCC (0x20d001e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* sIOV DVSEC Register 0.
*/


#define PSIOVDVSEC1_IIO_CPM_PF_REG 0x212201E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 siovid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000000*/

                            /* DVSEC ID for sIOV. */
    UINT32 fdl : 8;

                            /* Bits[23:16], Access Type=RO, default=0x00000000*/

                            /* Function Dependency Link. */
    UINT32 flags : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000000*/

                            /* This is Flags bitfield for psiovdcsec. */

  } Bits;
  UINT32 Data;

} PSIOVDVSEC1_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSIOVPAGESIZE_IIO_CPM_PF_REG supported on:                                   */
/*      SPRA0 (0x20d001e4)                                                      */
/*      SPRB0 (0x20d001e4)                                                      */
/*      SPRC0 (0x20d001e4)                                                      */
/*      SPRUCC (0x20d001e4)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* sIOV Supported Page Size Register
*/


#define PSIOVPAGESIZE_IIO_CPM_PF_REG 0x212201E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 supsyspagsz : 32;

                            /* Bits[31:0], Access Type=RO, default=0x00000001*/

                            /*
                               This field defines the supported system page
                               sizes. This PF supports a page size of 2^(n+12)
                               if bit n is Set. For example, if bit 0 is Set,
                               the PF supports 4k byte page sizes. Only 4K
                               pages size is supported. A page size describes
                               the minimum alignment requirements for VF BAR
                               resources.
                            */

  } Bits;
  UINT32 Data;

} PSIOVPAGESIZE_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSIOVSYSPS_IIO_CPM_PF_REG supported on:                                      */
/*      SPRA0 (0x20d001e8)                                                      */
/*      SPRB0 (0x20d001e8)                                                      */
/*      SPRC0 (0x20d001e8)                                                      */
/*      SPRUCC (0x20d001e8)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* sIOV System Page Size Register
*/


#define PSIOVSYSPS_IIO_CPM_PF_REG 0x212201E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 syspagsiz : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000001*/

                            /*
                               This field must define the page size the system
                               will use to map the VFs memory addresses.
                               Software must set the value of the System Page
                               Size to one of the page sizes set in the
                               Supported Page Sizes field. As with Supported
                               Page Sizes, if bit n is Set in System Page Size,
                               the VFs associated with this PF are required to
                               support a page size of 2^(n+12). For example, if
                               bit 1 is Set, the system is using an 8k byte
                               page size. The results are undefined if more
                               than one bit is set in System Page Size. The
                               results are undefined if a bit is Set in System
                               Page Size that is not Set in Supported Page
                               Sizes. When System Page Size is set, the VF
                               associated with this PF is required to align all
                               BAR resources on a System Page Size boundary.
                               Each VF BARn or VF BARn pair (described below)
                               shall be aligned on a System Page Size boundary.
                               Each VF BARn or VF BARn pair defining a non-zero
                               address space shall be sized to consume an
                               integer multiple of System Page Size bytes. All
                               data structures requiring page size alignment
                               within a VF shall be aligned on a System Page
                               Size boundary.
                            */

  } Bits;
  UINT32 Data;

} PSIOVSYSPS_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PSIOVCAP_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x20d001ec)                                                      */
/*      SPRB0 (0x20d001ec)                                                      */
/*      SPRC0 (0x20d001ec)                                                      */
/*      SPRUCC (0x20d001ec)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* SRIOV Capabilities Register 
*/


#define PSIOVCAP_IIO_CPM_PF_REG 0x212201EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 imssupport : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000001*/

                            /* This is IMS Support. */
    UINT32 reserved : 31;

                            /* Bits[31:1], Access Type=RO, default=0x00000000*/

                            /* Reserved. */

  } Bits;
  UINT32 Data;

} PSIOVCAP_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */








/* MCSTRCV_N1_IIO_CPM_PF_REG supported on:                                      */
/*      SPRA0 (0x20d0023c)                                                      */
/*      SPRB0 (0x20d0023c)                                                      */
/*      SPRHBM (0x20d0023c)                                                     */
/*      SPRC0 (0x20d0023c)                                                      */
/*      SPRMCC (0x20d0023c)                                                     */
/*      SPRUCC (0x20d0023c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The MC_Receive register provides a bit vector denoting which Multicast groups the Function should accept
*/


#define MCSTRCV_N1_IIO_CPM_PF_REG 0x2122023C

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

} MCSTRCV_N1_IIO_CPM_PF_STRUCT;


/* MCSTBLKALL_N1_IIO_CPM_PF_REG supported on:                                   */
/*      SPRA0 (0x20d00244)                                                      */
/*      SPRB0 (0x20d00244)                                                      */
/*      SPRHBM (0x20d00244)                                                     */
/*      SPRC0 (0x20d00244)                                                      */
/*      SPRMCC (0x20d00244)                                                     */
/*      SPRUCC (0x20d00244)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Multicast Block All
*/


#define MCSTBLKALL_N1_IIO_CPM_PF_REG 0x21220244

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

} MCSTBLKALL_N1_IIO_CPM_PF_STRUCT;



#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PFIEERRUNCSTSR_IIO_CPM_PF_REG supported on:                                  */
/*      SPRA0 (0x20d00280)                                                      */
/*      SPRB0 (0x20d00280)                                                      */
/*      SPRC0 (0x20d00280)                                                      */
/*      SPRUCC (0x20d00280)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* IE Uncorrectable Error Status Register. This register will only log the first 2 errors. After the first 2 errors, no errors will be logged until this register is cleared entirely. The first error bit set in this register will be reflected in pfieerruncfer. The second error bit set in this register will be reflected in pfieerruncner. Clearing this register entirely will also clear pfieerruncfer and pfieerruncner. Note that some of the errors defined in this register are ORed of multiple errors. Therefore, even if multiple errors of the same error type are detected, the error will only reflect in this register once. The second error will be of a different error type.
*/


#define PFIEERRUNCSTSR_IIO_CPM_PF_REG 0x21220280

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meunc : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               ORed MEx Uncorrectable Error: The ECC logic in
                               the ME ustore detected an uncorrectable error.
                               ORed of himeuncerrlog bits.
                            */
    UINT32 cppcmdparerr : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               ORed CPP Command Parity Error. OR'ed of bits in
                               register hicppagentcmdparerrlog.
                            */
    UINT32 cfc0 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* ORed of CFC Push Error and Pull Error. */
    UINT32 ri_mem_par_err : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               ORed RI Memory Parity Errors. ORed of
                               ri_mem_par_err_sts0 register outputs.
                            */
    UINT32 ti_mem_par_err : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               ORed TI Memory Parity Errors. ORed of
                               ti_mem_par_err_sts0 and ti_mem_par_err_sts1
                               registers outputs.
                            */
    UINT32 tipullparerr : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               TI Pull parity error. Error logged in
                               ticppintsts.
                            */
    UINT32 ripushparerr : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               RI Push parity error. Error logged in
                               ricppintsts.
                            */
    UINT32 ripushpullerr : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               RI ORed Push and Pull error. Errors logged in
                               ricppintsts.
                            */
    UINT32 tipushpullerr : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               TI ORed Push Error and Pull Error. Errors logged
                               in ticppintsts.
                            */
    UINT32 timiscerr : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* TI Misc Errors. Errors logged in timiscsts. */
    UINT32 iosfp_cmd_parerr : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Command Parity error detected on IOSFP Command
                               to CPM. This will result in a CPM Stop and
                               Scream and can cause CPM Hang.
                            */
    UINT32 reserved1 : 21;

                            /* Bits[31:11], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PFIEERRUNCSTSR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PFIEERRUNCMSKR_IIO_CPM_PF_REG supported on:                                  */
/*      SPRA0 (0x20d00284)                                                      */
/*      SPRB0 (0x20d00284)                                                      */
/*      SPRC0 (0x20d00284)                                                      */
/*      SPRUCC (0x20d00284)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* IE Uncorrectable Error Mask Register. Any bit that is masked will not reflect in pfieerruncfer and pfieerruncner.
*/


#define PFIEERRUNCMSKR_IIO_CPM_PF_REG 0x21220284

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meunc : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* ORed MEx Uncorrectable Error Mask. */
    UINT32 cppcmdparerr : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* ORed CPP Command Parity Error Mask. */
    UINT32 cfc0 : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* ORed of CFC Push Error and Pull Error Mask. */
    UINT32 ri_mem_par_err : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* ORed RI Memory Parity Errors Mask. */
    UINT32 ti_mem_par_err : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* ORed TI Memory Parity Errors Mask. */
    UINT32 tipullparerr : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* TI Pull parity error Mask. */
    UINT32 ripushparerr : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* RI Push parity error Mask. */
    UINT32 ripushpullerr : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* RI ORed Push Error and Pull Error Mask. */
    UINT32 tipushpullerr : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* TI ORed Push Error and Pull Error Mask. */
    UINT32 timiscerr : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* TI Misc Errors. Errors logged in timiscsts. */
    UINT32 iosfp_cmd_parerr : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* IOSF Primary Command Parity error Mask. */
    UINT32 reserved1 : 21;

                            /* Bits[31:11], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PFIEERRUNCMSKR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PFIEERRUNCFER_IIO_CPM_PF_REG supported on:                                   */
/*      SPRA0 (0x20d00288)                                                      */
/*      SPRB0 (0x20d00288)                                                      */
/*      SPRC0 (0x20d00288)                                                      */
/*      SPRUCC (0x20d00288)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* IE Uncorrectable Error First Error Register. The first bit set in pfieerruncstsr will reflect in the corresponding bit in this register. This register will only be cleared when pfieerruncstsr is cleared entirely.
*/


#define PFIEERRUNCFER_IIO_CPM_PF_REG 0x21220288

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meunc : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* ORed MEx Uncorrectable Error. */
    UINT32 cppcmdparerr : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* ORed CPP Command Parity Error. */
    UINT32 cfc0 : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /* ORed of CFC Push Error and Pull Error. */
    UINT32 ri_mem_par_err : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /* ORed RI Memory Parity Errors. */
    UINT32 ti_mem_par_err : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /* ORed TI Memory Parity Errors. */
    UINT32 tipullparerr : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /* TI Pull parity error. */
    UINT32 ripushparerr : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /* RI Push parity error. */
    UINT32 ripushpullerr : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /* RI ORed Push Error and Pull Error. */
    UINT32 tipushpullerr : 1;

                            /* Bits[8:8], Access Type=RO/V/P, default=0x00000000*/

                            /* TI ORed Push Error and Pull Error. */
    UINT32 timiscerr : 1;

                            /* Bits[9:9], Access Type=RO/V/P, default=0x00000000*/

                            /* TI Misc Errors. Errors logged in timiscsts. */
    UINT32 iosfp_cmd_parerr : 1;

                            /* Bits[10:10], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Command Parity error detected on IOSFP Command
                               to CPM. This will result in a CPM Stop and
                               Scream and can cause CPM Hang.
                            */
    UINT32 reserved1 : 21;

                            /* Bits[31:11], Access Type=RO/V/P, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PFIEERRUNCFER_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PFIEERRUNCNER_IIO_CPM_PF_REG supported on:                                   */
/*      SPRA0 (0x20d0028c)                                                      */
/*      SPRB0 (0x20d0028c)                                                      */
/*      SPRC0 (0x20d0028c)                                                      */
/*      SPRUCC (0x20d0028c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* IE Uncorrectable Error Next Error Register. The second bit set in pfieerruncstsr will reflect in the corresponding bit in this register. This register will only be cleared when pfieerruncstsr is cleared entirely.
*/


#define PFIEERRUNCNER_IIO_CPM_PF_REG 0x2122028C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meunc : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* ORed MEx Uncorrectable Error. */
    UINT32 cppcmdparerr : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* ORed CPP Command Parity Error. */
    UINT32 cfc0 : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /* ORed of CFC Push Error and Pull Error. */
    UINT32 ri_mem_par_err : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /* ORed RI Memory Parity Errors. */
    UINT32 ti_mem_par_err : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /* ORed TI Memory Parity Errors. */
    UINT32 tipullparerr : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /* TI Pull parity error. */
    UINT32 ripushparerr : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /* RI Push parity error. */
    UINT32 ripushpullerr : 1;

                            /* Bits[7:7], Access Type=RO/V/P, default=0x00000000*/

                            /* RI ORed Push Error and Pull Error. */
    UINT32 tipushpullerr : 1;

                            /* Bits[8:8], Access Type=RO/V/P, default=0x00000000*/

                            /* TI ORed Push Error and Pull Error. */
    UINT32 timiscerr : 1;

                            /* Bits[9:9], Access Type=RO/V/P, default=0x00000000*/

                            /* TI Misc Errors. Errors logged in timiscsts. */
    UINT32 iosfp_cmd_parerr : 1;

                            /* Bits[10:10], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Command Parity error detected on IOSFP Command
                               to CPM. This will result in a CPM Stop and
                               Scream and can cause CPM Hang.
                            */
    UINT32 reserved1 : 21;

                            /* Bits[31:11], Access Type=RO/V, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PFIEERRUNCNER_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PFIEERRCORSTSR_IIO_CPM_PF_REG supported on:                                  */
/*      SPRA0 (0x20d002b0)                                                      */
/*      SPRB0 (0x20d002b0)                                                      */
/*      SPRC0 (0x20d002b0)                                                      */
/*      SPRUCC (0x20d002b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* IE Correctable Error Status Register. This register will only log the first error. After the first error, no error will be logged until this register is cleared entirely. The first error bit set in this register will be reflected in pfieerrcorfer. Clearing this register entirely will also clear pfieerrcorfer.
*/


#define PFIEERRCORSTSR_IIO_CPM_PF_REG 0x212202B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mecor : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               ORed MEx Correctable Error. OR'ed of enabled
                               bits in register hi_me_cor_errlog
                            */
    UINT32 reserved1 : 31;

                            /* Bits[31:1], Access Type=RO/V/P, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PFIEERRCORSTSR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PFIEERRCORMSKR_IIO_CPM_PF_REG supported on:                                  */
/*      SPRA0 (0x20d002b4)                                                      */
/*      SPRB0 (0x20d002b4)                                                      */
/*      SPRC0 (0x20d002b4)                                                      */
/*      SPRUCC (0x20d002b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* IE Correctable Error Mask Register. Any bit that is masked will not reflect in pfieerrcorfer and pfieerrcorner. 
*/


#define PFIEERRCORMSKR_IIO_CPM_PF_REG 0x212202B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mecor : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* ORed MEx Correctable Error Mask. */
    UINT32 reserved1 : 31;

                            /* Bits[31:1], Access Type=RO, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PFIEERRCORMSKR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PFIEERRCORFER_IIO_CPM_PF_REG supported on:                                   */
/*      SPRA0 (0x20d002b8)                                                      */
/*      SPRB0 (0x20d002b8)                                                      */
/*      SPRC0 (0x20d002b8)                                                      */
/*      SPRUCC (0x20d002b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* IE Correctable Error First Error Register. The first bit set in pfieerrcorstsr will reflect in the corresponding bit in this register. This register will only be cleared when pfieerrcorstsr is cleared entirely.
*/


#define PFIEERRCORFER_IIO_CPM_PF_REG 0x212202B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mecor : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               ORed MEx Correctable Error from both SSM
                               Clusters.
                            */
    UINT32 reserved1 : 31;

                            /* Bits[31:1], Access Type=RO/V/P, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PFIEERRCORFER_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PFIEERRCORNER_IIO_CPM_PF_REG supported on:                                   */
/*      SPRA0 (0x20d002bc)                                                      */
/*      SPRB0 (0x20d002bc)                                                      */
/*      SPRC0 (0x20d002bc)                                                      */
/*      SPRUCC (0x20d002bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* This register is not implemented since there is only 1 correctable error type. Just a place holder for future error in pfieerrcorstsr. IE Correctable Error Next Error Register. The second bit set in pfieerrcorstsr will reflect in the corresponding bit in this register. This register will only be cleared when pfieerrcorstsr is cleared entirely.
*/


#define PFIEERRCORNER_IIO_CPM_PF_REG 0x212202BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mecor : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* ORed MEx Correctable Error. */
    UINT32 reserved1 : 31;

                            /* Bits[31:1], Access Type=RO/V/P, default=0x00000000*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PFIEERRCORNER_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PFCGCIOSFPRIR_IIO_CPM_PF_REG supported on:                                   */
/*      SPRA0 (0x20d002c0)                                                      */
/*      SPRB0 (0x20d002c0)                                                      */
/*      SPRC0 (0x20d002c0)                                                      */
/*      SPRUCC (0x20d002c0)                                                     */
/* Register default value on SPRA0: 0x00008010                                  */
/* Register default value on SPRB0: 0x00008010                                  */
/* Register default value on SPRC0: 0x00008010                                  */
/* Register default value on SPRUCC: 0x00008010                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Clock Gating Control IOSF Primary Register
*/


#define PFCGCIOSFPRIR_IIO_CPM_PF_REG 0x212202C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000010*/

                            /*
                               Idle Clock Timer (ICT):This field indicates the
                               number of clocks that the IP block must be idle
                               before the clock disable process begins.Note: 16
                               (default value) is the minimum number of clocks
                               recommended.
                            */
    UINT32 reserved1 : 7;

                            /* Bits[14:8], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Idle Clock Gating Enable (ICGE):This bit when
                               set enables clock gating to occur when the IP
                               block is idle longer than the Idle Clock Timer.
                            */
    UINT32 parchk : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /* Parity Check Enable */
    UINT32 reserved0 : 15;

                            /* Bits[31:17], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT32 Data;

} PFCGCIOSFPRIR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* PFCGCIOSFSBR_IIO_CPM_PF_REG supported on:                                    */
/*      SPRA0 (0x20d002c4)                                                      */
/*      SPRB0 (0x20d002c4)                                                      */
/*      SPRC0 (0x20d002c4)                                                      */
/*      SPRUCC (0x20d002c4)                                                     */
/* Register default value on SPRA0: 0x00008010                                  */
/* Register default value on SPRB0: 0x00008010                                  */
/* Register default value on SPRC0: 0x00008010                                  */
/* Register default value on SPRUCC: 0x00008010                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Clock Gating Control IOSF Sideband Register
*/


#define PFCGCIOSFSBR_IIO_CPM_PF_REG 0x212202C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ict : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000010*/

                            /*
                               Idle Clock Timer (ICT):This field indicates the
                               number of clocks that the IP block must be idle
                               before the clock disable process begins.Note: 16
                               (default value) is the minimum number of clocks
                               recommended.
                            */
    UINT32 reserved1 : 7;

                            /* Bits[14:8], Access Type=RO, default=0x00000000*/

                            /* Reserved1 */
    UINT32 icge : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Idle Clock Gating Enable (ICGE):This bit when
                               set enables clock gating to occur when the IP
                               block is idle longer than the Idle Clock Timer.
                            */
    UINT32 reserved0 : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00000000*/

                            /* Reserved0 */

  } Bits;
  UINT32 Data;

} PFCGCIOSFSBR_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* FUSECTL0_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x20d002c8)                                                      */
/*      SPRB0 (0x20d002c8)                                                      */
/*      SPRC0 (0x20d002c8)                                                      */
/*      SPRUCC (0x20d002c8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Fuse Control and Status Register 0. Access to disabled units within CPM will result in undefined behavior,  causing System Hang.
*/


#define FUSECTL0_IIO_CPM_PF_REG 0x212202C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable_production_mode : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Production_CPM Fuse: This fuse bit provides a
                               mechanism to prevent access to some of the
                               internal resources within CPM. This bit can be
                               overriden to allow access to those resources for
                               debugging purpose. When 1 debug features are
                               disabled and not available. When 0, debug
                               features are enabled.
                            */
    UINT32 disable_fw_signingkey_dbg : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /*
                               Firmware Signing Debug Key Disable: There are 4
                               different keys that can be used to Sign & Verify
                               the FW that is loaded into the ME. When this bit
                               is set (fuse is blown) then Debug Public Key is
                               disable. The default value of this field (0),
                               enables the public key. Which means that the
                               authentication FW should not compare the hash of
                               that debug public key, as stored in ROM, to the
                               hash of the public key in the file being
                               authenticated; therefore, images signed with
                               that key will not be considered
                               verified/authenticated. This key should be used
                               for internal development and debug. This key
                               should be disable for production parts. It is
                               feasible that multiple Firmware signing keys
                               (bits[30:27]) are be enable. In such scenarios,
                               the FW will be considered verified/authenticated
                               if any one of the enable keys match the hash of
                               hte public key in the file being authenticated.
                            */
    UINT32 disable_fw_signingkey_2 : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /*
                               Firmware Signing Key 2 Disable: There are 4
                               different keys that can be used to Sign & Verify
                               the FW that is loaded into the ME.When this bit
                               is set (fuse is blown) then Public Key 2 is
                               disabled. The default value of this field (0),
                               enables the public key.Which means that the
                               authentication FW should not compare the hash of
                               that public key 2, as stored in ROM, to the hash
                               of the public key in the file being
                               authenticated; therefore, images signed with
                               that key will not be considered
                               verified/authenticated.It is feasible that
                               multiple Firmware signing keys (bits[30:27]) are
                               be enabled. In such scenarios, the FW will be
                               considered verified/authenticated if any one of
                               the enabled keys match the hash of hte public
                               key in the file being authenticated.
                            */
    UINT32 disable_fw_signingkey_1 : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               Firmware Signing Key 1 Disable: There are 4
                               different keys that can be used to Sign & Verify
                               the FW that is loaded into the ME.When this bit
                               is set (fuse is blown) then Public Key 1 is
                               disabled. The default value of this field (0),
                               enables the public key.Which means that the
                               authentication FW should not compare the hash of
                               that public key 1, as stored in ROM, to the hash
                               of the public key in the file being
                               authenticated; therefore, images signed with
                               that key will not be considered
                               verified/authenticated.It is feasible that
                               multiple Firmware signing keys (bits[30:27]) are
                               be enabled. In such scenarios, the FW will be
                               considered verified/authenticated if any one of
                               the enabled keys match the hash of hte public
                               key in the file being authenticated.
                            */
    UINT32 disable_fw_signingkey_0 : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               Firmware Signing Key 0 Disable: There are 4
                               different keys that can be used to Sign & Verify
                               the FW that is loaded into the ME.When this bit
                               is set (fuse is blown) then Public Key 0 is
                               disabled. The default value of this field (0),
                               enables the public key.Which means that the
                               authentication FW should not compare the hash of
                               that public key 0 as stored in ROM, to the hash
                               of the public key in the file being
                               authenticated; therefore, images signed with
                               that key will not be considered
                               verified/authenticated.It is feasible that
                               multiple Firmware signing keys (bits[30:27] and
                               bits[24:16]) are be enabled. In such scenarios,
                               the FW will be considered verified/authenticated
                               if any one of the enabled keys match the hash of
                               the public key in the file being authenticated.
                            */
    UINT32 low_sku : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               Low_SKU fuse bit when set exposes the amount of
                               CPM services as defined by specifications
                               (Crypto/PKE/(De)-Compression, 50/50/50 Gbps). By
                               default the CPM services are fully enable.
                            */
    UINT32 mid_sku : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               Mid_SKU fuse bit when set exposes the amount of
                               CPM services as defined by specifications
                               (Crypto/PKE/(De)-Compression, 100/100/100 Gbps).
                               By default the CPM services are fully enable.
                            */
    UINT32 high_sku : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               High_SKU fuse bit when set exposes the amount of
                               CPM services as defined by specifications
                               (Crypto/PKE/(De)-Compression, 200/200/200 Gbps).
                               By default the CPM services are fully enable.
                            */
    UINT32 pcie_revision_id : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               CPM device PCIe revision ID for PF and VF (7:4 -
                               Major Revision,3:0 Minor Revision).
                            */
    UINT32 disable_kpt : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               Disables KPT (Key Protection Technology). No
                               hardware functionallity is controled by this
                               fuse bit. CPM firmware uses this bit to enable
                               FW reponsible for KPT.
                            */
    UINT32 max_system_address : 4;

                            /* Bits[20:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               Maximum supported system address width to
                               generate canonical virtual addresss. 0000 - 57
                               bits virtual address 0001, 0010, 0100, 1000 -
                               Future expansion All other values are reserved.
                            */
    UINT32 enable_pm : 1;

                            /* Bits[21:21], Access Type=RO/V, default=0x00000000*/

                            /* Enables dynamic power management. */
    UINT32 enable_pm_idle : 1;

                            /* Bits[22:22], Access Type=RO/V, default=0x00000000*/

                            /* Enables dynamic CPP cluster trunk clock gating. */
    UINT32 enable_deep_pm_idle : 1;

                            /* Bits[23:23], Access Type=RO/V, default=0x00000000*/

                            /*
                               Enables power gating of CPP cluster post SOC
                               reset.
                            */
    UINT32 spare_fuse1 : 8;

                            /* Bits[31:24], Access Type=RO/V, default=0x00000000*/

                            /* Spare fuses for future use. */

  } Bits;
  UINT32 Data;

} FUSECTL0_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* FUSECTL1_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x20d002cc)                                                      */
/*      SPRB0 (0x20d002cc)                                                      */
/*      SPRC0 (0x20d002cc)                                                      */
/*      SPRUCC (0x20d002cc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Fuse Control and Status Register 1. Access to disabled units within CPM will result in undefined behavior,  causing System Hang.
*/


#define FUSECTL1_IIO_CPM_PF_REG 0x212202CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 disable_cph : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Disable Cipher Slice: When set indicates the
                               Cipher, Wireless cipher and UCS Slices are
                               disable.
                            */
    UINT32 disable_auth : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /*
                               Disable Authentication Slice: When set indicates
                               the Authentication and wireless authentication
                               and UCS Slice are disable after FCU_DONE is set.
                               All Auth Slices will be enable until firmware
                               authentication process is complete.
                            */
    UINT32 disable_pke : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /*
                               Disable PKE Slice: When set indicates the PKE is
                               disable after FCU_DONE is set. All PKE slices
                               will be enable until firmware authentication
                               process is complete.
                            */
    UINT32 disable_cpr : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               Disable Compression, decompression, UCS and
                               Translator Slices: When set indicates the
                               Compression, decompression and Translator Slices
                               are disable.
                            */
    UINT32 disable_ucs : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* Disable UCS and decompression slices. */
    UINT32 disable_eia3 : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               Disable EIA3 and EEA3 in Authentication slice
                               and Cipher Slice respectively.
                            */
    UINT32 spare_fuse7 : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /* Spare Fuse */
    UINT32 disable_smx : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /* Disable SM3 and SM4 in Cipher slices. */
    UINT32 spare_fuse2 : 24;

                            /* Bits[31:8], Access Type=RO/V, default=0x00000000*/

                            /* Spare fuses for future use. */

  } Bits;
  UINT32 Data;

} FUSECTL1_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* FUSECTL2_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x20d002d0)                                                      */
/*      SPRB0 (0x20d002d0)                                                      */
/*      SPRC0 (0x20d002d0)                                                      */
/*      SPRUCC (0x20d002d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Fuse Control Register 2
*/


#define FUSECTL2_IIO_CPM_PF_REG 0x212202D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 min_power_request : 9;

                            /* Bits[8:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Dynamic manufacturing fuse that controls the
                               Power Request Telemetry CPM exposes to
                               SAPMA/P-code when it has no active jobs to
                               process and can give up most of its power
                               budget.
                            */
    UINT32 spare_fuse3 : 7;

                            /* Bits[15:9], Access Type=RO/V, default=0x00000000*/

                            /* Spare_Fuse3 for future uses */
    UINT32 max_power_request : 9;

                            /* Bits[24:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               Dynamic manufacturing fuse that controls the
                               Power Request Telemetry CPM exposes to
                               SAPMA/P-code when it has active jobs to process
                               and needs its full power budget.
                            */
    UINT32 spare_fuse4 : 7;

                            /* Bits[31:25], Access Type=RO/V, default=0x00000000*/

                            /* Spare_Fuse4 for future uses */

  } Bits;
  UINT32 Data;

} FUSECTL2_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* FUSECTL3_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x20d002d4)                                                      */
/*      SPRB0 (0x20d002d4)                                                      */
/*      SPRC0 (0x20d002d4)                                                      */
/*      SPRUCC (0x20d002d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Fuse Control Register 3
*/


#define FUSECTL3_IIO_CPM_PF_REG 0x212202D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sram_tuning : 20;

                            /* Bits[19:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Dynamic manufacturing SRAM tuning fuses to
                               assist memory read/write.
                            */
    UINT32 rf_tuning : 6;

                            /* Bits[25:20], Access Type=RO/V, default=0x00000000*/

                            /*
                               Dynamic manufacturing RF tuning fuses to assist
                               memory read/write.
                            */
    UINT32 rom_tuning : 5;

                            /* Bits[30:26], Access Type=RO/V, default=0x00000000*/

                            /*
                               Dynamic manufacturing ROM tuning fuses to assist
                               memory read/write.
                            */
    UINT32 spare_fuse5 : 1;

                            /* Bits[31:31], Access Type=RO/V, default=0x00000000*/

                            /* Spare_Fuse4 for future uses */

  } Bits;
  UINT32 Data;

} FUSECTL3_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* FUSECTL4_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x20d002d8)                                                      */
/*      SPRB0 (0x20d002d8)                                                      */
/*      SPRC0 (0x20d002d8)                                                      */
/*      SPRUCC (0x20d002d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Fuse Control Register 3
*/


#define FUSECTL4_IIO_CPM_PF_REG 0x212202D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 disable_cpp0_me : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Disable_CPP0_ME Fuses. When programmed the
                               corresponding MEs are disabled.
                            */

  } Bits;
  UINT32 Data;

} FUSECTL4_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */


#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* FUSECTL5_IIO_CPM_PF_REG supported on:                                        */
/*      SPRA0 (0x20d002dc)                                                      */
/*      SPRB0 (0x20d002dc)                                                      */
/*      SPRC0 (0x20d002dc)                                                      */
/*      SPRUCC (0x20d002dc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRB0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRC0 Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* SPRUCC Register File:    sprsp_top/cpm[0]/i_cpm_hi_sla_env/cpm_pf_i/cpm_csr_pf0_i*/
/* Struct generated from SPRA0 BDF: 13_0_0                                      */
/* SPRA0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRB0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRC0 Security PolicyGroup: CPM_CONFIG_SPACE                                 */
/* SPRUCC Security PolicyGroup: CPM_CONFIG_SPACE                                */
/* Fuse Control Register 3
*/


#define FUSECTL5_IIO_CPM_PF_REG 0x212202DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 powergate_cpp0_cluster : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               PowerGate_CPP0_Cluster Fuse. When programmed
                               this fuse shall disable the CPP0 Cluster.
                            */
    UINT32 powergate_cpp0_cpr : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /*
                               PowerGate_CPP0_CPR Fuse. When programmed this
                               fuse shall disable the CPP0 CPR Slice.
                            */
    UINT32 powergate_cpp0_pke : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /*
                               PowerGate_CPP0_PKE Fuse. When programmed this
                               fuse shall disable the CPP0 PKE Slice.
                            */
    UINT32 powergate_cpp0_ucs : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               PowerGate_CPP0_UCS Fuse. When programmed this
                               fuse shall disable the CPP0 UCS Slice.
                            */
    UINT32 spare_fuse6 : 4;

                            /* Bits[7:4], Access Type=RO/V, default=0x00000000*/

                            /* Spare Fuses for Future Uses */
    UINT32 reserved5 : 24;

                            /* Bits[31:8], Access Type=RO/V, default=0x00000000*/

                            /* Reserved fields */

  } Bits;
  UINT32 Data;

} FUSECTL5_IIO_CPM_PF_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRC0_HOST) || defined(SPRUCC_HOST) */



























































































































#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* LUTERRINJCTL_IIO_CPM_PF_REG supported on:                                    */
/*      SPRB0 (0x20d009d0)                                                      */
/*      SPRHBM (0x20d009d0)                                                     */
/*      SPRC0 (0x20d009d0)                                                      */
/*      SPRMCC (0x20d009d0)                                                     */
/*      SPRUCC (0x20d009d0)                                                     */
/* Register default value on SPRB0: 0x00FFF000                                  */
/* Register default value on SPRHBM: 0x00FFF000                                 */
/* Register default value on SPRC0: 0x00FFF000                                  */
/* Register default value on SPRMCC: 0x00FFF000                                 */
/* Register default value on SPRUCC: 0x00FFF000                                 */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* Struct generated from SPRB0 BDF: 13_0_0                                      */
/* SPRB0 Security PolicyGroup: DFX2                                             */
/* SPRHBM Security PolicyGroup: DFX2                                            */
/* SPRC0 Security PolicyGroup: DFX2                                             */
/* SPRMCC Security PolicyGroup: DFX2                                            */
/* SPRUCC Security PolicyGroup: DFX2                                            */
/* LUT Error Injection Control
*/

#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* LUTERRINJMSK_IIO_CPM_PF_REG supported on:                                    */
/*      SPRB0 (0x20d009d4)                                                      */
/*      SPRHBM (0x20d009d4)                                                     */
/*      SPRC0 (0x20d009d4)                                                      */
/*      SPRMCC (0x20d009d4)                                                     */
/*      SPRUCC (0x20d009d4)                                                     */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* Struct generated from SPRB0 BDF: 13_0_0                                      */
/* SPRB0 Security PolicyGroup: DFX2                                             */
/* SPRHBM Security PolicyGroup: DFX2                                            */
/* SPRC0 Security PolicyGroup: DFX2                                             */
/* SPRMCC Security PolicyGroup: DFX2                                            */
/* SPRUCC Security PolicyGroup: DFX2                                            */
/* LUT Error Injection Mask
*/

#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* LUTERRINJMSKWID_IIO_CPM_PF_REG supported on:                                 */
/*      SPRB0 (0x20d009d8)                                                      */
/*      SPRHBM (0x20d009d8)                                                     */
/*      SPRC0 (0x20d009d8)                                                      */
/*      SPRMCC (0x20d009d8)                                                     */
/*      SPRUCC (0x20d009d8)                                                     */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_eep/cfg_inst    */
/* Struct generated from SPRB0 BDF: 13_0_0                                      */
/* SPRB0 Security PolicyGroup: DFX2                                             */
/* SPRHBM Security PolicyGroup: DFX2                                            */
/* SPRC0 Security PolicyGroup: DFX2                                             */
/* SPRMCC Security PolicyGroup: DFX2                                            */
/* SPRUCC Security PolicyGroup: DFX2                                            */
/* LUT Error Injection Mask Width ID
*/

#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */




































































































































#endif /* _IIO_CPM_PF_h */
