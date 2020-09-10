
/** @file
  IIO_PCIENTB.h

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


#ifndef _IIO_PCIENTB_h
#define _IIO_PCIENTB_h
#include <Base.h>

/* VID_IIO_PCIENTB_REG supported on:                                            */
/*      SPRA0 (0x10048000)                                                      */
/*      SPRB0 (0x10048000)                                                      */
/*      SPRHBM (0x10048000)                                                     */
/*      SPRC0 (0x10048000)                                                      */
/*      SPRMCC (0x10048000)                                                     */
/*      SPRUCC (0x10048000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Vendor ID
*/


#define VID_IIO_PCIENTB_REG 0x0F010000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 vid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00008086*/

                            /*
                               This field identifies Intel as the manufacturer
                               of the device.
                            */

  } Bits;
  UINT16 Data;

} VID_IIO_PCIENTB_STRUCT;

/* DID_IIO_PCIENTB_REG supported on:                                            */
/*      SPRA0 (0x10048002)                                                      */
/*      SPRB0 (0x10048002)                                                      */
/*      SPRHBM (0x10048002)                                                     */
/*      SPRC0 (0x10048002)                                                      */
/*      SPRMCC (0x10048002)                                                     */
/*      SPRUCC (0x10048002)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Device ID
*/


#define DID_IIO_PCIENTB_REG 0x0F010002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 did : 16;

                            /* Bits[15:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field identifies the particular function as
                               allocated by Intel. The Device ID register is
                               HwInit and the value in this register identifies
                               the particular Function. The Device ID must be
                               allocated by Intel. The Device ID, in
                               conjunction with the Vendor ID and Revision ID,
                               are used as one mechanism for software to
                               determine which driver should be loaded. The
                               chosen values should not result in the use of an
                               incompatible device driver. Refer to PCI Device
                               ID Map section from SoC/FC for more details. The
                               upper 9-bit of DID value is driven by SetIDValue
                               message, and the remaining 7-bit is set from
                               strap value if supported.
                            */

  } Bits;
  UINT16 Data;

} DID_IIO_PCIENTB_STRUCT;

/* PCICMD_IIO_PCIENTB_REG supported on:                                         */
/*      SPRA0 (0x10048004)                                                      */
/*      SPRB0 (0x10048004)                                                      */
/*      SPRHBM (0x10048004)                                                     */
/*      SPRC0 (0x10048004)                                                      */
/*      SPRMCC (0x10048004)                                                     */
/*      SPRUCC (0x10048004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register controls how the device behaves on the primary interface (PCI Express).
*/


#define PCICMD_IIO_PCIENTB_REG 0x0F010004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 iose : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               0 - Respond to all I/O Requests on the primary
                               interface with an Unsupported Request
                               Completion. Forward all I/O transactions from
                               the secondary interface to the primary interface
                               (if supported). 1 - Enable forwarding of I/O
                               Requests to the secondary interface. The
                               External EP NTB port does not support I/O space
                               accesses from the remote CPU.
                            */
    UINT16 mse : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Respond to all Memory Requests on the
                               primary interface as Unsupported Request
                               Received. Forward all memory requests from the
                               secondary interface to the primary interface. 1
                               - Enable forwarding of memory transactions to
                               the secondary interface and any internal
                               function.
                            */
    UINT16 bme : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               This bit controls the ability of the Function to
                               issue Memory and I/O read or write requests, and
                               the ability of Root or Switch port to forward
                               memory and I/O read or write requests in the
                               upstream direction. When this bit is 0b, memory
                               and I/O requests received at the root port or
                               downstream side of a switch port (secondary
                               side) must be handled as an Unsupported Request
                               (UR). For Non-posted requests, a completion with
                               UR completion status must be returned. For an
                               endpoint, when this bit is Set, the PCI Express
                               Function is allowed to issue Memory or I/O
                               Requests. When this bit is Clear, the PCI
                               Express Function is not allowed to issue any
                               Memory or I/O Requests. The forwarding of
                               requests other than memory or I/O requests is
                               not controlled by this bit. AtomicOp Requester
                               Enable bit in Device Control 2 register must
                               also be set in order for an AtomicOp Requests to
                               be forwarded. MSI/MSI-X interrupt Messages are
                               in-band memory writes, setting the Bus Master
                               Enable bit = 0b disables MSI/MSI-X interrupt
                               Messages as well. MSI interrupts are inband
                               memory writes and are blocked when this bit is
                               0b.
                            */
    UINT16 sce : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI-Express. Hardwired to 0. */
    UINT16 mwie : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI-Express. Hardwired to 0. */
    UINT16 vga_pse : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI-Express. Hardwired to 0. */
    UINT16 pere : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               This bit controls the setting of the master data
                               parity error bit in the Status Register ('PCI
                               Status Register (PCISTS)' on page 186) in
                               response to a parity error received on the PCI
                               Express interface (either internal queue errors
                               or a poisoned TLP).
                            */
    UINT16 wcc : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI-Express. Hardwired to 0. */
    UINT16 see : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               When set, this bit enables reporting of Non-
                               Fatal and Fatal errors detected by the Function
                               of the Root Complex. For Type 1 Configuration
                               Space headers, this bit controls transmission by
                               the primary interface of ERR_NONFATAL and
                               ERR_FATAL error messages forwarded from the
                               secondary interface. ERR_COR messages are not
                               affected by this bit. Errors are reported when
                               enabled either through this bit or through the
                               PCI Express-specific bits in the Device Control
                               Register ('Device Control Register (DEVCTL)' on
                               page 228).
                            */
    UINT16 fbe : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* Not applicable to PCI-Express. Hardwired to 0. */
    UINT16 intxd : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               This bit controls the ability of the PCI-Express
                               Function to generate legacy INTx interrupt
                               message. When set, functions are prevented from
                               asserting INTx interrupt messages. Any INTx
                               emulation interrupts already asserted by the
                               function must be deasserted when this bit is set
                               by generating a Deassert_INTx message(s). This
                               bit does not affect the ability to route
                               interrupt messages received at the PCI-Express
                               port. However, this bit controls the generation
                               of legacy interrupts for PCI-Express errors
                               detected internally in this port (e.g. Malformed
                               TLP, CRC error, completion time out etc.) or
                               when receiving root port error messages or
                               interrupts due to HP/PM events generated in
                               legacy mode. This bit has no effect on
                               interrupts that pass through the port from the
                               secondary side of root ports, switch ports, and
                               bridges.
                            */
    UINT16 rsvd : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PCICMD_IIO_PCIENTB_STRUCT;

/* PCISTS_IIO_PCIENTB_REG supported on:                                         */
/*      SPRA0 (0x10048006)                                                      */
/*      SPRB0 (0x10048006)                                                      */
/*      SPRHBM (0x10048006)                                                     */
/*      SPRC0 (0x10048006)                                                      */
/*      SPRMCC (0x10048006)                                                     */
/*      SPRUCC (0x10048006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status
*/


#define PCISTS_IIO_PCIENTB_REG 0x0F010006

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 rsvd : 3;

                            /* Bits[2:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 ints : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               When set, this bit indicates that an INTx
                               emulation interrupt is pending internally in
                               this function. For Type 1 configuration header
                               functions, forwarded INTx messages are not
                               reflected in this bit. unless the INTx messages
                               is being generated from the Type 1 configuration
                               header function.
                            */
    UINT16 cape : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               This bit indicates the presence of an Extended
                               capabilities list items. Offset 34H indicates
                               the offset for the first entry in the linked
                               list of capabilities. All PCI Express Functions
                               are required to have a PCI Express Capability
                               Structure. So this bit must be hardwired to 1b.
                            */
    UINT16 c66 : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /* This bit has no meaning on PCI Express. */
    UINT16 rsvd_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 fbc : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* This bit has no meaning on PCI Express. */
    UINT16 mdpd : 1;

                            /* Bits[8:8], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit is set by a requester (primary side for
                               type1 configuration header functions) if the
                               parity error response enable bit (PERE) in the
                               Command Register ('PCI Command Register
                               (PCICMD)' on page 181) is set and either of the
                               following two conditions occur: Requester
                               receives a completion marked poisoned. Requester
                               sends a poisoned request (includes writes and
                               messages) If the parity error bit is 0b, this
                               bit is never set.
                            */
    UINT16 dvt : 2;

                            /* Bits[10:9], Access Type=RO, default=0x00000000*/

                            /*
                               These bits have no meaning on PCI Express. Fast
                               decode timing is reported.
                            */
    UINT16 sta : 1;

                            /* Bits[11:11], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit is Set when the port completes a Posted
                               or Non-Posted Request as a Completer Abort
                               error. This applies to a Function with a Type 1
                               Configuration header when the Completer Abort
                               was generated by its Primary Side. If the given
                               Configuration or MMIO access to the PCIe
                               internal resource does not follow formation
                               rule, then it is completed with CA status and is
                               logged as PCISTS.STA. Refer to Section 1.11.5,
                               'Master/Target abort Logging in
                               Primary/Secondary Status registers' on page 40
                               of the PCIe Gen3 Chapter.
                            */
    UINT16 rta : 1;

                            /* Bits[12:12], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit is set when a requester receives a CA
                               completions status. On Type 1 configuration
                               header functions, the bit is set when a
                               'Completer Abort' is received on the primary
                               side.
                            */
    UINT16 rma : 1;

                            /* Bits[13:13], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit is set when the requester receives a
                               completion with an UR completion status. On Type
                               1 configuration header functions, the bit is set
                               when a UR completions status is received on the
                               primary side.
                            */
    UINT16 sse : 1;

                            /* Bits[14:14], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit is set when ERR_FATAL or ERR_NONFATAL
                               messages are sent to the root complex and the
                               SERR enable bit in the PCICMD Register ('PCI
                               Command Register (PCICMD)' on page 181) is set.
                            */
    UINT16 dpe : 1;

                            /* Bits[15:15], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit is set when a poisoned TLP is received
                               from PCIe or IOSF. This bit is set even when the
                               parity error response enable bit (bit[6] of the
                               PCICMD Register'PCI Command Register (PCICMD)'
                               on page 181) is not set. On Type 1 configuration
                               header functions, the bit is set when the
                               poisoned TLP is received on the primary side.
                            */

  } Bits;
  UINT16 Data;

} PCISTS_IIO_PCIENTB_STRUCT;

/* RID_IIO_PCIENTB_REG supported on:                                            */
/*      SPRA0 (0x48008)                                                         */
/*      SPRB0 (0x48008)                                                         */
/*      SPRHBM (0x48008)                                                        */
/*      SPRC0 (0x48008)                                                         */
/*      SPRMCC (0x48008)                                                        */
/*      SPRUCC (0x48008)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Revision ID
*/


#define RID_IIO_PCIENTB_REG 0x0F000008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 rid : 8;

                            /* Bits[7:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field specifies the specific revision of
                               this function and should be viewed as a
                               extension to the Device ID. Default values will
                               be determined by soft strap or SetIDValue or
                               fuses if supported.
                            */

  } Bits;
  UINT8 Data;

} RID_IIO_PCIENTB_STRUCT;

/* CCRPI_IIO_PCIENTB_REG supported on:                                          */
/*      SPRA0 (0x48009)                                                         */
/*      SPRB0 (0x48009)                                                         */
/*      SPRHBM (0x48009)                                                        */
/*      SPRC0 (0x48009)                                                         */
/*      SPRMCC (0x48009)                                                        */
/*      SPRUCC (0x48009)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Class Code Programming Interface
*/


#define CCRPI_IIO_PCIENTB_REG 0x0F000009

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 rlpi : 8;

                            /* Bits[7:0], Access Type=RW/L, default=None*/

                            /*
                               This field identifies a specific register level
                               programming interface (if any) so that device
                               independent software can interact with the
                               Function. Encodings for interface are provided
                               in the PCI Code and ID Assignment Specification.
                               All unspecified encodings are Reserved. This
                               variable needs to be customized for the end
                               project requirements as the functionality and
                               type of end point is not known at this point.
                            */

  } Bits;
  UINT8 Data;

} CCRPI_IIO_PCIENTB_STRUCT;

/* CCRSCC_IIO_PCIENTB_REG supported on:                                         */
/*      SPRA0 (0x4800a)                                                         */
/*      SPRB0 (0x4800a)                                                         */
/*      SPRHBM (0x4800a)                                                        */
/*      SPRC0 (0x4800a)                                                         */
/*      SPRMCC (0x4800a)                                                        */
/*      SPRUCC (0x4800a)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Class Code Sub-Class Code
*/


#define CCRSCC_IIO_PCIENTB_REG 0x0F00000A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 sc : 8;

                            /* Bits[7:0], Access Type=RW/L, default=None*/

                            /*
                               Specifies a base class sub-class, which
                               identifies more specifically the operation of
                               the Function. Encodings for sub-class are
                               provided in the PCI Code and ID Assignment
                               Specification. All unspecified encodings are
                               Reserved. For PCIe External/Internal EP, this
                               field is hardwired to 80h, indicating a 'Other
                               device'.
                            */

  } Bits;
  UINT8 Data;

} CCRSCC_IIO_PCIENTB_STRUCT;

/* CCRBCC_IIO_PCIENTB_REG supported on:                                         */
/*      SPRA0 (0x4800b)                                                         */
/*      SPRB0 (0x4800b)                                                         */
/*      SPRHBM (0x4800b)                                                        */
/*      SPRC0 (0x4800b)                                                         */
/*      SPRMCC (0x4800b)                                                        */
/*      SPRUCC (0x4800b)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* The CCR register has an attribute of RW-L PRST to allow configuration software to modify and lock the register from further update. Lock Key bit is located in the Personality Lock Key Control Register ('PLKCTL').
*/


#define CCRBCC_IIO_PCIENTB_REG 0x0F00000B

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 bc : 8;

                            /* Bits[7:0], Access Type=RW/L, default=None*/

                            /*
                               A code that broadly classifies the type of
                               operation the Function performs. Encodings for
                               base class, are provided in the PCI Code and ID
                               Assignment Specification. All unspecified
                               encodings are Reserved. For PCIe External EP,
                               this field is hardwired to 01h, indicating a
                               'Mass Storage Controller'. The value of 03h
                               indicates that this is a display controller for
                               an endpoint as an example. The value of 06h
                               indicates that this is a bridge device.
                            */

  } Bits;
  UINT8 Data;

} CCRBCC_IIO_PCIENTB_STRUCT;

/* CLS_IIO_PCIENTB_REG supported on:                                            */
/*      SPRA0 (0x4800c)                                                         */
/*      SPRB0 (0x4800c)                                                         */
/*      SPRHBM (0x4800c)                                                        */
/*      SPRC0 (0x4800c)                                                         */
/*      SPRMCC (0x4800c)                                                        */
/*      SPRUCC (0x4800c)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Cacheline Size
*/


#define CLS_IIO_PCIENTB_REG 0x0F00000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cls : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               These bits specify the system cache-line size in
                               units of DWords. This field is implemented by
                               PCI Express devices but has no effect on device
                               behavior.
                            */

  } Bits;
  UINT8 Data;

} CLS_IIO_PCIENTB_STRUCT;

/* PLAT_IIO_PCIENTB_REG supported on:                                           */
/*      SPRA0 (0x4800d)                                                         */
/*      SPRB0 (0x4800d)                                                         */
/*      SPRHBM (0x4800d)                                                        */
/*      SPRC0 (0x4800d)                                                         */
/*      SPRMCC (0x4800d)                                                        */
/*      SPRUCC (0x4800d)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register denotes the maximum time slice for a burst transaction in legacy PCI 2.3 on the primary interface. It does not affect/influence PCI-Express functionality.
*/


#define PLAT_IIO_PCIENTB_REG 0x0F00000D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 prim_lat_timer : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /*
                               Not applicable to PCI-Express. Hardwired to 00h.
                               This register is typically called as 'latency
                               Timer' for Endpoints in PCI 3.0 specification
                            */

  } Bits;
  UINT8 Data;

} PLAT_IIO_PCIENTB_STRUCT;

/* HDR_IIO_PCIENTB_REG supported on:                                            */
/*      SPRA0 (0x4800e)                                                         */
/*      SPRB0 (0x4800e)                                                         */
/*      SPRHBM (0x4800e)                                                        */
/*      SPRC0 (0x4800e)                                                         */
/*      SPRMCC (0x4800e)                                                        */
/*      SPRUCC (0x4800e)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Header Type
*/


#define HDR_IIO_PCIENTB_REG 0x0F00000E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 htype : 7;

                            /* Bits[6:0], Access Type=RW/L, default=None*/

                            /*
                               These bits define the layout of addresses 10h
                               through 3Fh in the configuration space. For an
                               end point device, default is 00h indicating a
                               conventional type 00h PCI header.
                            */
    UINT8 mfd : 1;

                            /* Bits[7:7], Access Type=RW/L, default=None*/

                            /*
                               Set the value '1' to indicate that the end point
                               is a multifunction device although only function
                               0 is defined in this IAS document This is a
                               single function device only.
                            */

  } Bits;
  UINT8 Data;

} HDR_IIO_PCIENTB_STRUCT;

/* BIST_IIO_PCIENTB_REG supported on:                                           */
/*      SPRA0 (0x4800f)                                                         */
/*      SPRB0 (0x4800f)                                                         */
/*      SPRHBM (0x4800f)                                                        */
/*      SPRC0 (0x4800f)                                                         */
/*      SPRMCC (0x4800f)                                                        */
/*      SPRUCC (0x4800f)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register is used for reporting control and status information of BIST checks within a PCIExpress port. It is not supported in this implementation.
*/


#define BIST_IIO_PCIENTB_REG 0x0F00000F

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 bist_tst : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000000*/

                            /* Not supported. Hardwired to 0h */

  } Bits;
  UINT8 Data;

} BIST_IIO_PCIENTB_STRUCT;

/* MBAR01_IP_N0_IIO_PCIENTB_REG supported on:                                   */
/*      SPRA0 (0x20048010)                                                      */
/*      SPRB0 (0x20048010)                                                      */
/*      SPRHBM (0x20048010)                                                     */
/*      SPRC0 (0x20048010)                                                      */
/*      SPRMCC (0x20048010)                                                     */
/*      SPRUCC (0x20048010)                                                     */
/* Register default value on SPRA0: 0x00000004                                  */
/* Register default value on SPRB0: 0x00000004                                  */
/* Register default value on SPRHBM: 0x00000004                                 */
/* Register default value on SPRC0: 0x00000004                                  */
/* Register default value on SPRMCC: 0x00000004                                 */
/* Register default value on SPRUCC: 0x00000004                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This 64-bit register is used for accessing CFG/MMIO resources consumed for the given device (end point). MBAR01_IP_Alias* will be utilized as alias of MBAR01_IP to accommodate validation environment and resolve SpecTr scope with unique range.
*/


#define MBAR01_IP_N0_IIO_PCIENTB_REG 0x0F020010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 msi : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               This Base Address Register indicates memory
                               space.
                            */
    UINT32 type : 2;

                            /* Bits[2:1], Access Type=RO, default=0x00000002*/

                            /*
                               The memory mapped space can be placed anywhere
                               within the 64-bit addressable region of the
                               device.
                            */
    UINT32 pref : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* The memory mapped space is not prefetchable. */
    UINT32 rsvd : 13;

                            /* Bits[16:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baddr : 15;

                            /* Bits[31:17], Access Type=RW, default=0x00000000*/

                            /*
                               This marks the base address of the 128KB region
                               for the memory mapped space MBAR01 requested by
                               the internal end point device in the local host
                               memory space.
                            */

  } Bits;
  UINT32 Data;

} MBAR01_IP_N0_IIO_PCIENTB_STRUCT;

/* MBAR01_IP_N1_IIO_PCIENTB_REG supported on:                                   */
/*      SPRA0 (0x20048014)                                                      */
/*      SPRB0 (0x20048014)                                                      */
/*      SPRHBM (0x20048014)                                                     */
/*      SPRC0 (0x20048014)                                                      */
/*      SPRMCC (0x20048014)                                                     */
/*      SPRUCC (0x20048014)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This 64-bit register is used for accessing CFG/MMIO resources consumed for the given device (end point). MBAR01_IP_Alias* will be utilized as alias of MBAR01_IP to accommodate validation environment and resolve SpecTr scope with unique range.
*/


#define MBAR01_IP_N1_IIO_PCIENTB_REG 0x0F020014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 baddr : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               This marks the base address of the 128KB region
                               for the memory mapped space MBAR01 requested by
                               the internal end point device in the local host
                               memory space.
                            */

  } Bits;
  UINT32 Data;

} MBAR01_IP_N1_IIO_PCIENTB_STRUCT;

/* MBAR23_IP_N0_IIO_PCIENTB_REG supported on:                                   */
/*      SPRA0 (0x20048018)                                                      */
/*      SPRB0 (0x20048018)                                                      */
/*      SPRHBM (0x20048018)                                                     */
/*      SPRC0 (0x20048018)                                                      */
/*      SPRMCC (0x20048018)                                                     */
/*      SPRUCC (0x20048018)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register is used by the processor on the internal side of the NTB to setup a 64-bit prefetchable memory window. 
This window region can be resized using Resizable BAR utility instead of the SZ register as used in Jasper.
*/


#define MBAR23_IP_N0_IIO_PCIENTB_REG 0x0F020018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 msi : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               This Base Address Register indicates memory
                               space.
                            */
    UINT32 type : 2;

                            /* Bits[2:1], Access Type=RO, default=0x00000002*/

                            /*
                               The memory mapped space can be placed anywhere
                               within the 64-bit addressable region of the
                               device.
                            */
    UINT32 pref : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /* The memory mapped space is prefetchable. */
    UINT32 rsvd : 8;

                            /* Bits[11:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baddr : 20;

                            /* Bits[31:12], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the location of the base address of the
                               MBAR23_IP on the internal NTB.
                            */

  } Bits;
  UINT32 Data;

} MBAR23_IP_N0_IIO_PCIENTB_STRUCT;

/* MBAR23_IP_N1_IIO_PCIENTB_REG supported on:                                   */
/*      SPRA0 (0x2004801c)                                                      */
/*      SPRB0 (0x2004801c)                                                      */
/*      SPRHBM (0x2004801c)                                                     */
/*      SPRC0 (0x2004801c)                                                      */
/*      SPRMCC (0x2004801c)                                                     */
/*      SPRUCC (0x2004801c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register is used by the processor on the internal side of the NTB to setup a 64-bit prefetchable memory window. 
This window region can be resized using Resizable BAR utility instead of the SZ register as used in Jasper.
*/


#define MBAR23_IP_N1_IIO_PCIENTB_REG 0x0F02001C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 baddr : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the location of the base address of the
                               MBAR23_IP on the internal NTB.
                            */

  } Bits;
  UINT32 Data;

} MBAR23_IP_N1_IIO_PCIENTB_STRUCT;

/* MBAR45_IP_N0_IIO_PCIENTB_REG supported on:                                   */
/*      SPRA0 (0x20048020)                                                      */
/*      SPRB0 (0x20048020)                                                      */
/*      SPRHBM (0x20048020)                                                     */
/*      SPRC0 (0x20048020)                                                      */
/*      SPRMCC (0x20048020)                                                     */
/*      SPRUCC (0x20048020)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register is used by the processor on the internal side of the NTB to setup a second 64-bit prefetchable memory window. 
This window region can be resized using Resizable BAR utility instead of the SZ register as used in Jasper.
*/


#define MBAR45_IP_N0_IIO_PCIENTB_REG 0x0F020020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 msi : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               This BAR register resource is memory (as opposed
                               to I/O).
                            */
    UINT32 type : 2;

                            /* Bits[2:1], Access Type=RO, default=0x00000002*/

                            /*
                               The memory type claimed by this BAR is 64-bit
                               addressable.
                            */
    UINT32 pref : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /* The memory mapped space is prefetchable. */
    UINT32 rsvd : 8;

                            /* Bits[11:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 baddr : 20;

                            /* Bits[31:12], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the location of this BAR region and written
                               by SW.
                            */

  } Bits;
  UINT32 Data;

} MBAR45_IP_N0_IIO_PCIENTB_STRUCT;

/* MBAR45_IP_N1_IIO_PCIENTB_REG supported on:                                   */
/*      SPRA0 (0x20048024)                                                      */
/*      SPRB0 (0x20048024)                                                      */
/*      SPRHBM (0x20048024)                                                     */
/*      SPRC0 (0x20048024)                                                      */
/*      SPRMCC (0x20048024)                                                     */
/*      SPRUCC (0x20048024)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register is used by the processor on the internal side of the NTB to setup a second 64-bit prefetchable memory window. 
This window region can be resized using Resizable BAR utility instead of the SZ register as used in Jasper.
*/


#define MBAR45_IP_N1_IIO_PCIENTB_REG 0x0F020024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 baddr : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Sets the location of this BAR region and written
                               by SW.
                            */

  } Bits;
  UINT32 Data;

} MBAR45_IP_N1_IIO_PCIENTB_STRUCT;

/* SVID_IIO_PCIENTB_REG supported on:                                           */
/*      SPRA0 (0x1004802c)                                                      */
/*      SPRB0 (0x1004802c)                                                      */
/*      SPRHBM (0x1004802c)                                                     */
/*      SPRC0 (0x1004802c)                                                      */
/*      SPRMCC (0x1004802c)                                                     */
/*      SPRUCC (0x1004802c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register identifies the manufacturer of the system. This 16-bit register combined with the Device Identification Register uniquely identify any PCI device. It is present only in an endpoint in the legacy PCI space. For Rootports, upstream and downstream ports in a switch, the SVID and SID is assigned to the Capability structure in the compatible PCIe space defined in Section 1.5.4.1, 'Subsystem Capability List Register (SSCAPLST)' on page 317
*/


#define SVID_IIO_PCIENTB_REG 0x0F01002C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 svid : 16;

                            /* Bits[15:0], Access Type=RW/L, default=None*/

                            /* The default value specifies Intel. */

  } Bits;
  UINT16 Data;

} SVID_IIO_PCIENTB_STRUCT;

/* SID_IIO_PCIENTB_REG supported on:                                            */
/*      SPRA0 (0x1004802e)                                                      */
/*      SPRB0 (0x1004802e)                                                      */
/*      SPRHBM (0x1004802e)                                                     */
/*      SPRC0 (0x1004802e)                                                      */
/*      SPRMCC (0x1004802e)                                                     */
/*      SPRUCC (0x1004802e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register identifies the subsystem.
*/


#define SID_IIO_PCIENTB_REG 0x0F01002E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 sid : 16;

                            /* Bits[15:0], Access Type=RW/L, default=None*/

                            /*
                               Assigned by the subsystem vendor to uniquely
                               identify the subsystem.
                            */

  } Bits;
  UINT16 Data;

} SID_IIO_PCIENTB_STRUCT;

/* CAPPTR_IIO_PCIENTB_REG supported on:                                         */
/*      SPRA0 (0x48034)                                                         */
/*      SPRB0 (0x48034)                                                         */
/*      SPRHBM (0x48034)                                                        */
/*      SPRC0 (0x48034)                                                         */
/*      SPRMCC (0x48034)                                                        */
/*      SPRUCC (0x48034)                                                        */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Capabilities Pointer
*/


#define CAPPTR_IIO_PCIENTB_REG 0x0F000034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cptr : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000040*/

                            /*
                               Contains the offset of the first item in the
                               list of capabilities. (EXPCAPLST)
                            */

  } Bits;
  UINT8 Data;

} CAPPTR_IIO_PCIENTB_STRUCT;

/* INTL_IIO_PCIENTB_REG supported on:                                           */
/*      SPRA0 (0x4803c)                                                         */
/*      SPRB0 (0x4803c)                                                         */
/*      SPRHBM (0x4803c)                                                        */
/*      SPRC0 (0x4803c)                                                         */
/*      SPRMCC (0x4803c)                                                        */
/*      SPRUCC (0x4803c)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Interrupt Line
*/


#define INTL_IIO_PCIENTB_REG 0x0F00003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 intl : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               This register is used to communicate interrupt
                               line routing information. The device itself does
                               not use this value, rather it is used by device
                               drivers and operating systems. (compatibility)
                            */

  } Bits;
  UINT8 Data;

} INTL_IIO_PCIENTB_STRUCT;

/* INTP_IIO_PCIENTB_REG supported on:                                           */
/*      SPRA0 (0x4803d)                                                         */
/*      SPRB0 (0x4803d)                                                         */
/*      SPRHBM (0x4803d)                                                        */
/*      SPRC0 (0x4803d)                                                         */
/*      SPRMCC (0x4803d)                                                        */
/*      SPRUCC (0x4803d)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Interrupt Pin
*/


#define INTP_IIO_PCIENTB_REG 0x0F00003D

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 intp : 8;

                            /* Bits[7:0], Access Type=RW/L, default=None*/

                            /*
                               This register tells which interrupt pin the
                               function uses. 01h: Generate INTA 02h: Generate
                               INTB 03h: Generate INTC 04h: Generate INTD
                               Others: Reserved BIOS has the ability to write
                               this register once during boot to setup the
                               correct interrupt for the Function.
                            */

  } Bits;
  UINT8 Data;

} INTP_IIO_PCIENTB_STRUCT;

/* EXPCAPLST_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x10048040)                                                      */
/*      SPRB0 (0x10048040)                                                      */
/*      SPRHBM (0x10048040)                                                     */
/*      SPRC0 (0x10048040)                                                      */
/*      SPRMCC (0x10048040)                                                     */
/*      SPRUCC (0x10048040)                                                     */
/* Register default value on SPRA0: 0x0000A010                                  */
/* Register default value on SPRB0: 0x0000A010                                  */
/* Register default value on SPRHBM: 0x0000A010                                 */
/* Register default value on SPRC0: 0x0000A010                                  */
/* Register default value on SPRMCC: 0x0000A010                                 */
/* Register default value on SPRUCC: 0x0000A010                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* PCI Express Capability List
*/


#define EXPCAPLST_IIO_PCIENTB_REG 0x0F010040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 capid : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000010*/

                            /* Identifies the function as PCI Express capable. */
    UINT16 np : 8;

                            /* Bits[15:8], Access Type=RO, default=0x000000A0*/

                            /*
                               Contains the offset of the next item in the
                               capabilities list. (PMCAPLST)
                            */

  } Bits;
  UINT16 Data;

} EXPCAPLST_IIO_PCIENTB_STRUCT;

/* EXPCAP_IIO_PCIENTB_REG supported on:                                         */
/*      SPRA0 (0x10048042)                                                      */
/*      SPRB0 (0x10048042)                                                      */
/*      SPRHBM (0x10048042)                                                     */
/*      SPRC0 (0x10048042)                                                      */
/*      SPRMCC (0x10048042)                                                     */
/*      SPRUCC (0x10048042)                                                     */
/* Register default value on SPRA0: 0x00000002                                  */
/* Register default value on SPRB0: 0x00000002                                  */
/* Register default value on SPRHBM: 0x00000002                                 */
/* Register default value on SPRC0: 0x00000002                                  */
/* Register default value on SPRMCC: 0x00000002                                 */
/* Register default value on SPRUCC: 0x00000002                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register stores the version number of the capability item and other base information contained in the capability structure.
*/


#define EXPCAP_IIO_PCIENTB_REG 0x0F010042

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 vn : 4;

                            /* Bits[3:0], Access Type=RO, default=0x00000002*/

                            /*
                               These bits indicate the version number of the
                               PCI Express capability structure.
                            */
    UINT16 dt : 4;

                            /* Bits[7:4], Access Type=RW/V/L, default=None*/

                            /*
                               0: PCI Express Endpoint 1: Legacy PCI Express
                               Endpoint Indicates the specific type of this PCI
                               Express Function. Note that different Functions
                               in a multi-Function device can generally be of
                               different types. Defined encodings for Functions
                               that implement a Type 00h PCI Configuration
                               Space header are: 0000b PCI Express Endpoint
                               0001b Legacy PCI Express Endpoint 1001b Root
                               Complex Integrated Endpoint 1010b Root Complex
                               Event Collector Defined encodings for Functions
                               that implement a Type 01h PCI Configuration
                               Space header are: 0100b Root Port of PCI Express
                               Root Complex 0101b Upstream Port of PCI Express
                               Switch 0110b Downstream Port of PCI Express
                               Switch 0111b PCI Express to PCI/PCI-X Bridge
                               1000b PCI/PCI-X to PCI Express Bridge All other
                               encodings are Reserved. Note: Lock Key bit is
                               located in the Personality Lock Key Control
                               Register .
                            */
    UINT16 si : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Hardwired to 0 for non root ports and non DP.
                               vRP/SP is defined not to have any slots
                               implemented as it communicates with an internal
                               multifunction device.
                            */
    UINT16 imn : 5;

                            /* Bits[13:9], Access Type=RO, default=0x00000000*/

                            /*
                               This field indicates the interrupt message
                               number that is generated from the PCI Express
                               port. When there is more than one MSI interrupt
                               number, this register is required to contain the
                               offset between the base Message Data and the MSI
                               Message that is generated when the status bits
                               in the slot status register or root port status
                               registers are set. The Chipset is required to
                               update this field if the number of MSI messages
                               change.
                            */
    UINT16 rsvd : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} EXPCAP_IIO_PCIENTB_STRUCT;

/* DEVCAP_IIO_PCIENTB_REG supported on:                                         */
/*      SPRA0 (0x20048044)                                                      */
/*      SPRB0 (0x20048044)                                                      */
/*      SPRHBM (0x20048044)                                                     */
/*      SPRC0 (0x20048044)                                                      */
/*      SPRMCC (0x20048044)                                                     */
/*      SPRUCC (0x20048044)                                                     */
/* Register default value on SPRA0: 0x00008000                                  */
/* Register default value on SPRB0: 0x00008000                                  */
/* Register default value on SPRHBM: 0x00008000                                 */
/* Register default value on SPRC0: 0x00008000                                  */
/* Register default value on SPRMCC: 0x00008000                                 */
/* Register default value on SPRUCC: 0x00008000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Device Capabilities
*/


#define DEVCAP_IIO_PCIENTB_REG 0x0F020044

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mpss : 3;

                            /* Bits[2:0], Access Type=RW/L, default=None*/

                            /*
                               The PCIe SIP can tolerate upto 512-byte packets
                               as maximum payload for project specific. Defined
                               encodings are: 000b 128 bytes max payload size
                               001b 256 bytes max payload size 010b 512 bytes
                               max payload size 011b 1024 bytes max payload
                               size 100b 2048 bytes max payload size 101b 4096
                               bytes max payload size 110b Reserved 111b
                               Reserved Note: Lock Key bit is located in the
                               Personality Lock Key Control Register .
                            */
    UINT32 pfs : 2;

                            /* Bits[4:3], Access Type=RO, default=0x00000000*/

                            /*
                               This field indicates the number of most
                               significant bits of the function number portion
                               of Requester ID in a TLP that are logically
                               combined with the Tag identifier. The PCIe
                               cluster does not support phantom functions
                            */
    UINT32 etfg : 1;

                            /* Bits[5:5], Access Type=RW/L, default=None*/

                            /*
                               This bit, in combination with the 10-Bit Tag
                               Requester Supported bit in the Device
                               Capabilities 2 register, indicates the maximum
                               supported size of the Tag field as a Requester.
                               This bit must be Set if the 10-Bit Tag Requester
                               Supported bit is Set. Defined encodings are: 0b
                               5-bit Tag field supported 1b 8-bit Tag field
                               supported Note that 8-bit Tag field generation
                               must be enabled by the Extended Tag Field Enable
                               bit in the Device Control register of the
                               Requester Function before 8-bit Tags can be
                               generated by the Requester. Note: Lock Key bit
                               is located in the Personality Lock Key Control
                               Register .
                            */
    UINT32 epl0al : 3;

                            /* Bits[8:6], Access Type=RW/V/L, default=None*/

                            /*
                               000b Maximum of 64 ns 001b Maximum of 128 ns
                               010b Maximum of 256 ns 011b Maximum of 512 ns
                               100b Maximum of 1 us 101b Maximum of 2 us 110b
                               Maximum of 4 us 111b No limit This field only
                               applies to Endpoint and should be configured by
                               BIOS/RTL accordingly. The minimal latency is
                               shown here as default. Note: Lock Key bit is
                               located in the Personality Lock Key Control
                               Register .
                            */
    UINT32 epl1al : 3;

                            /* Bits[11:9], Access Type=RW/V/L, default=None*/

                            /*
                               000b Maximum of 1 us 001b Maximum of 2 us 010b
                               Maximum of 4 us 011b Maximum of 8 us 100b
                               Maximum of 16 us 101b Maximum of 32 us 110b
                               Maximum of 64 us 111b No limit This field only
                               applies to Endpoint and should be configured by
                               BIOS/RTL accordingly. The minimal latency is
                               shown here as default. Note: Lock Key bit is
                               located in the Personality Lock Key Control
                               Register .
                            */
    UINT32 undefined : 3;

                            /* Bits[14:12], Access Type=RO, default=0x00000000*/

                            /* Undefined */
    UINT32 rber : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000001*/

                            /*
                               The PCIe cluster supports Role-based Error
                               Reporting.
                            */
    UINT32 rsvd : 2;

                            /* Bits[17:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 csplv : 8;

                            /* Bits[25:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               In combination with the Slot Power Limit Scale
                               value (bits[27:26]), this field specifies the
                               upper limit of the power supplied by slot. The
                               power limit (in Watts) is calculated by
                               multiplying the value in this field by the value
                               in the Slot Power Limit Scale field. This value
                               is set by the Set_Slot_Power_Limit message. Does
                               not apply to root ports or integrated devices
                               except for upstream ports
                            */
    UINT32 cspls : 2;

                            /* Bits[27:26], Access Type=RO/V, default=0x00000000*/

                            /*
                               In combination with the Slot Power Limit value
                               (bits[25:18], this field specifies the upper
                               limit of the power supplied by slot. The power
                               limit (in Watts) is calculated by multiplying
                               the value in this field by the value in the Slot
                               Power Limit Value field. This value is set by
                               the Set_Slot_Power_Limit message. Does not apply
                               to root ports or integrated devices except for
                               upstream ports
                            */
    UINT32 flr : 1;

                            /* Bits[28:28], Access Type=RW/L, default=None*/

                            /*
                               This field when set indicates this function
                               supports optional function Level Reset
                               mechanism. This field applies to Endpoints only.
                               For all other function types this bit must be
                               hardwired to 0b. Note: Lock Key bit is located
                               in the Personality Lock Key Control Register .
                            */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DEVCAP_IIO_PCIENTB_STRUCT;

/* DEVCTL_IIO_PCIENTB_REG supported on:                                         */
/*      SPRA0 (0x10048048)                                                      */
/*      SPRB0 (0x10048048)                                                      */
/*      SPRHBM (0x10048048)                                                     */
/*      SPRC0 (0x10048048)                                                      */
/*      SPRMCC (0x10048048)                                                     */
/*      SPRUCC (0x10048048)                                                     */
/* Register default value on SPRA0: 0x00002810                                  */
/* Register default value on SPRB0: 0x00002810                                  */
/* Register default value on SPRHBM: 0x00002810                                 */
/* Register default value on SPRC0: 0x00002810                                  */
/* Register default value on SPRMCC: 0x00002810                                 */
/* Register default value on SPRUCC: 0x00002810                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device Control
*/


#define DEVCTL_IIO_PCIENTB_REG 0x0F010048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 cere : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set, generation of the ERR_CORR
                               message is enabled.
                            */
    UINT16 nfere : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set, generation of the
                               ERR_NONFATAL message is enabled.
                            */
    UINT16 fere : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is set, generation of the
                               ERR_FATAL message is enabled.
                            */
    UINT16 urre : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               This bit controls the enabling of ERR_CORR,
                               ERR_NONFATAL or ERR_FATAL messages on PCI
                               Express for reporting 'Unsupported Request'
                               errors.
                            */
    UINT16 enro : 1;

                            /* Bits[4:4], Access Type=RW/V, default=0x00000001*/

                            /*
                               If this bit is Set, the Function is permitted to
                               set the Relaxed Ordering bit in the Attributes
                               field of transactions it initiates that do not
                               require strong write ordering. A Function is
                               permitted to hardwire this bit to 0b if it never
                               sets the Relaxed Ordering attribute in
                               transactions it initiates as a Requester.
                               Default value of this bit is 1b.
                            */
    UINT16 mps : 3;

                            /* Bits[7:5], Access Type=RW, default=0x00000000*/

                            /*
                               This field sets maximum TLP payload size for the
                               function. As a receiver, the function must
                               handle TLPs as large as the set value. As a
                               Transmitter, the function must not generate TLPs
                               exceeding the set value. 000b: 128 bytes maximum
                               payload size (default) 001b: 256 bytes maximum
                               payload size 010b: 512 bytes maximum payload
                               size 011b: 1024 bytes maximum payload size 100b:
                               2048 bytes maximum payload size 101b: 4096 bytes
                               maximum payload size Others: Reserved
                            */
    UINT16 etfe : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               When set, this bit enables a function to use an
                               8-bit tag field as a Requester. Functions that
                               do not implement this capability hardwire this
                               bit to 0b.
                            */
    UINT16 pfe : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, in combination with the 10-Bit Tag
                               Requester Enable bit in the Device Control 2
                               register, determines how many Tag field bits a
                               Requester is permitted to use. When the 10-Bit
                               Tag Requester Enable bit is Clear, the following
                               paragraph applies. See PCIe Spec. Section
                               2.2.6.2 for complete details. When Set, this bit
                               enables a Function to use unclaimed Functions as
                               Phantom Functions to extend the number of
                               outstanding transaction identifiers. If the bit
                               is Clear, the Function is not allowed to use
                               Phantom Functions. Software should not change
                               the value of this bit while the Function has
                               outstanding Non-Posted Requests, otherwise, the
                               result is undefined. Functions that do not
                               implement this capability hardwire this bit to
                               0b. Default value of this bit is 0b.
                            */
    UINT16 auxpme : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /* Not supported */
    UINT16 enosnp : 1;

                            /* Bits[11:11], Access Type=RW/V, default=0x00000001*/

                            /*
                               If this bit this is set, the function is
                               permitted to set the No Snoop bit in the
                               Requester attributes of transactions it
                               initiates that do not require hardware enforced
                               cache coherency.
                            */
    UINT16 mrrs : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000002*/

                            /*
                               This field sets the maximum Read Requests size
                               of the function as a requester. The Function
                               must not generate read requests with size
                               exceeding the set value. 000b: 128 bytes maximum
                               Read Request size 001b: 256 bytes maximum Read
                               Request size 010b: 512 bytes maximum Read
                               Request size 011b: 1024 bytes maximum Read
                               Request size 100b: 2048 bytes maximum Read
                               Request size 101b: 4096 bytes maximum Read
                               Request size Others: Reserved Functions that do
                               not generate Read Requests larger than 128B and
                               functions that do not generate Read Requests on
                               their own behalf are permitted to implement this
                               field as Read Only (RO) with a value of 000b.
                            */
    UINT16 iflr : 1;

                            /* Bits[15:15], Access Type=RW/V, default=0x00000000*/

                            /*
                               A write of 1 initiates Function Level Reset. The
                               value read by software from this bit is always
                               0b. Only applies to end point. This is a WO bit
                               that needs to return 0 when SW reads. If the
                               IFLR is not supported, them it can be left as
                               RO.
                            */

  } Bits;
  UINT16 Data;

} DEVCTL_IIO_PCIENTB_STRUCT;

/* DEVSTS_IIO_PCIENTB_REG supported on:                                         */
/*      SPRA0 (0x1004804a)                                                      */
/*      SPRB0 (0x1004804a)                                                      */
/*      SPRHBM (0x1004804a)                                                     */
/*      SPRC0 (0x1004804a)                                                      */
/*      SPRMCC (0x1004804a)                                                     */
/*      SPRUCC (0x1004804a)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device Status
*/


#define DEVSTS_IIO_PCIENTB_REG 0x0F01004A

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ced : 1;

                            /* Bits[0:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit indicates that this function has
                               detected a Correctable error. Errors are logged
                               in this register regardless of whether error
                               reporting is enabled or not in the Device
                               Control register.
                            */
    UINT16 nfed : 1;

                            /* Bits[1:1], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit indicates that this function has
                               detected a Non-Fatal error. Errors are logged in
                               this register regardless of whether error
                               reporting is enabled or not in the Device
                               Control register.
                            */
    UINT16 fed : 1;

                            /* Bits[2:2], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit indicates that this function has
                               detected a Fatal error. Errors are logged in
                               this register regardless of whether error
                               reporting is enabled or not in the Device
                               Control register.
                            */
    UINT16 urd : 1;

                            /* Bits[3:3], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               This bit indicates that this function received
                               an unsupported request from PCI Express link.
                               Errors are logged in this register regardless of
                               whether error reporting is enabled or not in the
                               Device Control register.
                            */
    UINT16 apd : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /* Auxiliary Power is not supported. */
    UINT16 tp : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               When set, this bit indicates that the function
                               has issued Non-Posted Requests that have not
                               been completed. For Root or Switch port, it
                               applies to Non-Posted Requests the port has
                               issued on its own behalf (Port's Request ID). A
                               function reports this bit cleared only when all
                               outstanding Non-Posted Requests have completed.
                               Functions that do not issue Non-Posted requests
                               on their own behalf should hardwire this bit to
                               0b. The implementation may set this bit based on
                               internal Request-Completion Tracking Table if
                               not empty for any outstanding/valid outbound or
                               inbound NP request (including split request).
                               Currently, it's only applicable to Root Ports.
                            */
    UINT16 rsvd : 10;

                            /* Bits[15:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} DEVSTS_IIO_PCIENTB_STRUCT;

/* DEVCAP2_IIO_PCIENTB_REG supported on:                                        */
/*      SPRA0 (0x20048064)                                                      */
/*      SPRB0 (0x20048064)                                                      */
/*      SPRHBM (0x20048064)                                                     */
/*      SPRC0 (0x20048064)                                                      */
/*      SPRMCC (0x20048064)                                                     */
/*      SPRUCC (0x20048064)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Device Capabilities 2
*/


#define DEVCAP2_IIO_PCIENTB_REG 0x0F020064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ctrs : 4;

                            /* Bits[3:0], Access Type=RW/L, default=None*/

                            /*
                               This field indicates device support for the
                               optional Completion Timeout programmability
                               mechanism. This mechanism allows system software
                               to modify the Completion Timeout value. This
                               field is applicable only to Root Ports,
                               Endpoints that issue requests on their own
                               behalf, and PCI Express to PCI/PCI-X Bridges
                               that take ownership of request issues on PCI
                               Express. Four time values ranges are defined:
                               Range A: 50us to 10ms Range B: 10ms to 250ms
                               Range C: 250ms to 4s Range D: 4s to 64s Bits
                               ares set according to table below to show
                               timeout value ranges supported. 0000b:
                               Completions Timeout programming not supported --
                               values is fixed by implementation in the range
                               50us to 50ms. 0001b: Range A 0010b: Range B
                               0011b: Range A and B 0110b: Range B and C 0111b:
                               Range A, B, and C 1110b: Range B, C, and D
                               1111b: Range A, B, C, and D All other values are
                               reserved. For all other devices this field is
                               reserved and maybe be hardwired to 0000b. Note:
                               Lock Key bit is located in the Personality Lock
                               Key Control Register .
                            */
    UINT32 ctds : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /*
                               A value of 1b indicates support for the
                               completion Timeout Disable Mechanism. Support of
                               completion timeout disable is optional for Root
                               Ports. The PCIe port supports completions
                               timeout disable.
                            */
    UINT32 ari : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Does not apply to endpoints and upstream ports
                               and is hardwired to 0.
                            */
    UINT32 ars : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               This bit must be set to 1b if the port supports
                               this optional capability. Applicable only to
                               switch upstream ports, Switch downstream ports,
                               and root ports,. Must be 00b for other function
                               types.
                            */
    UINT32 ad32acs : 1;

                            /* Bits[7:7], Access Type=RW/L, default=None*/

                            /*
                               This bit must be 0b for other functions types
                               (non root ports and memory BARS). Lock Key bit
                               is located in the Personality Lock Key Control
                               Register .
                            */
    UINT32 ad64acs : 1;

                            /* Bits[8:8], Access Type=RW/L, default=None*/

                            /*
                               This bit must be set to 0b for other functions
                               types (non root ports and memory BARS). Note:
                               Lock Key bit is located in the Personality Lock
                               Key Control Register .
                            */
    UINT32 ad128acs : 1;

                            /* Bits[9:9], Access Type=RW/L, default=None*/

                            /*
                               This bit must be set to 0b for other functions
                               types. Note: Lock Key bit is located in the
                               Personality Lock Key Control Register .
                            */
    UINT32 nroeprpass : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               If this bit is Set, the routing element never
                               carries out the passing permitted by Table 2-39
                               entry A2b that is associated with the Relaxed
                               Ordering Attribute field being Set. This bit
                               applies only for Switches and RCs that support
                               peer-topeer traffic between Root Ports. This bit
                               applies only to Posted Requests being forwarded
                               through the Switch or RC and does not apply to
                               traffic originating or terminating within the
                               Switch or RC itself. All Ports on a Switch or RC
                               must report the same value for this bit. For all
                               other functions, this bit must be 0b
                            */
    UINT32 ltrms : 1;

                            /* Bits[11:11], Access Type=RW/V/L, default=None*/

                            /*
                               This bit when set indicates support for the
                               Latency Tolerance Reporting (LTR) mechanism.
                               Note: Lock Key bit is located in the Personality
                               Lock Key Control Register . H/W will change this
                               field to 'RO' of zero when DISALBE_LTR fuse is
                               set to 1 if supported.
                            */
    UINT32 tphcs : 2;

                            /* Bits[13:12], Access Type=RW/L, default=None*/

                            /*
                               Value indicates Completer support for TPH or
                               Extended TPH. 00b:TPH and Extended TPH Completer
                               not supported 01b: TPH Completer supported,
                               Extended TPH completer not supported 10b:
                               Reserved 11b: Both TPH and Extended TPH
                               Completer supported Applicable only to Root
                               Ports and Endpoints. Must be 00b for other
                               function types. Note: Lock Key bit is located in
                               the Personality Lock Key Control Register .
                            */
    UINT32 rsvd : 2;

                            /* Bits[15:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 tenbtcs : 1;

                            /* Bits[16:16], Access Type=RW/L, default=None*/

                            /*
                               If this bit is Set, the Function supports 10-Bit
                               Tag Completer capability, otherwise, the
                               Function does not. Note: Lock Key bit is located
                               in the Personality Lock Key Control Register .
                            */
    UINT32 tenbtrs : 1;

                            /* Bits[17:17], Access Type=RW/L, default=None*/

                            /*
                               If this bit is Set, the Function supports 10-Bit
                               Tag Requester capability, otherwise, the
                               Function does not. This bit must not be Set if
                               the 10-Bit Tag Completer Supported bit is Clear.
                               Note that 10-Bit Tag field generation must be
                               enabled by the 10- Bit Tag Requester Enable bit
                               in the Device Control 2 register of the
                               Requester Function before 10-Bit Tags can be
                               generated by the Requester. Note: Lock Key bit
                               is located in the Personality Lock Key Control
                               Register .
                            */
    UINT32 obffs : 2;

                            /* Bits[19:18], Access Type=RW/L, default=None*/

                            /*
                               00b: OBFF Not Supported 01b: OBFF supported
                               using Message signaling only 10b: OBFF supported
                               using WAKE# signaling only 11b: OBFF supported
                               using WAKE# and Message signaling Applicable
                               only to Root Ports, Switch Ports, and Endpoints
                               that support this capability. Must be 00b for
                               other function types. Note: Lock Key bit is
                               located in the Personality Lock Key Control
                               Register .
                            */
    UINT32 exfmtfs : 1;

                            /* Bits[20:20], Access Type=RW/L, default=None*/

                            /*
                               0: The Function supports a 2-bit definition of
                               the Fmt field (default) 1: The Function supports
                               the 3-bit definition of the Fmt field. Must be
                               Set for Functions that support End-End TLP
                               Prefixes. Note: Lock Key bit is located in the
                               Personality Lock Key Control Register .
                            */
    UINT32 e2etlps : 1;

                            /* Bits[21:21], Access Type=RW/L, default=None*/

                            /*
                               Indicates whether End-End TLP Prefix support is
                               offered by a Function. 0: No Support 1: Support
                               is provided to receive TLPs containing End-End
                               TLP Prefixes. Note: Lock Key bit is located in
                               the Personality Lock Key Control Register .
                            */
    UINT32 maxe2etlp : 2;

                            /* Bits[23:22], Access Type=RW/V/L, default=None*/

                            /*
                               Indicates the maximum number of End-End TLP
                               Prefixes supported by this Function. TLPs
                               received by this Function that contain more End-
                               End TLP Prefixes than are supported must be
                               handled as Malformed TLPs. 01: 1 End-End TLP
                               Prefix 10: 2 End-End TLP Prefixes 11: 3 End-End
                               TLP Prefixes 00: 4 End-End TLP Prefixes If End-
                               End TLP Prefix Supported is Clear, this field is
                               Reserved. Note: Lock Key bit is located in the
                               Personality Lock Key Control Register .
                            */
    UINT32 rsvd_24 : 7;

                            /* Bits[30:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 frs : 1;

                            /* Bits[31:31], Access Type=RW/L, default=None*/

                            /*
                               When Set, indicates support for the optional
                               Function Readiness Status (FRS) capability. Must
                               be Set for all Functions that support generation
                               or receipt capabilities of FRS Messages. Must
                               not be Set by Switch Functions that do not
                               generate FRS Messages on their own behalf. Note:
                               Lock Key bit is located in the Personality Lock
                               Key Control Register .
                            */

  } Bits;
  UINT32 Data;

} DEVCAP2_IIO_PCIENTB_STRUCT;

/* DEVCTL2_IIO_PCIENTB_REG supported on:                                        */
/*      SPRA0 (0x10048068)                                                      */
/*      SPRB0 (0x10048068)                                                      */
/*      SPRHBM (0x10048068)                                                     */
/*      SPRC0 (0x10048068)                                                      */
/*      SPRMCC (0x10048068)                                                     */
/*      SPRUCC (0x10048068)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device Control 2
*/


#define DEVCTL2_IIO_PCIENTB_REG 0x0F010068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ctv : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /*
                               In devices that support completion timeout
                               programmability, this field allows system
                               software to modify the completion timeout range.
                               The following encodings and corresponding
                               timeout ranges are defined based on
                               implementation choice. This field provides finer
                               control based over the ranges chosen by
                               DEVAP2.CTRS. The implementation specific range
                               is shown in brackets 0000b: 50us to 50ms (16.8ms
                               - 25.2ms based on core clk period) 0001b: 50us
                               to 100us (65.5us - 99.3us based on core clk
                               period) 0010b: 1ms to 10ms (4.2ms - 6.3ms based
                               on core clk period) 0101b: 16ms to 55ms (33.6ms
                               - 50.3ms based on core clk period) 0110b: 65ms
                               to 210ms (134.2ms - 201.3ms based on core clk
                               period) 1001b: 260ms to 900ms (536.9ms - 805.3ms
                               based on core clk period) 1010b: 1s to 3.5s
                               (2.1s - 3.2s based on core clk period) 1101b: 4s
                               to 13s (8.6s - 12.9s based on core clk period)
                               1110b: 17s to 64s (34.4s - 51.5s based on core
                               clk period) All others are reserved. It is
                               highly recommended that the completion timeout
                               value not be less then 10ms. A small completion
                               timeout value may result in premature completion
                               timeout for slower responding devices.
                            */
    UINT16 ctd : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               1: Disable the completions timeout mechanism for
                               all NP transactions. 0: Completion timeout is
                               enabled for all NP transactions.
                            */
    UINT16 arie : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Hardwired to 0 for functions such as endpoints
                               and upstream ports
                            */
    UINT16 are : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Applicable only to Endpoints and Root Ports,
                               must be hardwired to 0b for other Function
                               types. The Function is allowed to initiate
                               AtomicOp Requests only if this bit and the Bus
                               Master Enable bit in the Command register are
                               both Set.
                            */
    UINT16 aeb : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /*
                               When this bit is set, AtomicOp requests that
                               target this out going Egress port must be
                               blocked. Applicable and mandatory for switch
                               Upstream Ports, Switch Downstream Ports, and
                               Root Ports that implement AtomicOp routing
                               capability. Otherwise, must be hardwired to 0b.
                            */
    UINT16 idore : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               This bit when set to 1b allows the function to
                               set the ID-Based Ordering (IDO) bit on requests
                               it initiates. Applicable only to Endpoints
                               including RC integrated Endpoints and Root
                               Ports.
                            */
    UINT16 idoce : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               This bit when set to 1b allows the function to
                               set the ID-Based Ordering (IDO) bit of
                               completions it returns. Applicable only to
                               Endpoints including RC integrated Endpoints and
                               Root Ports.
                            */
    UINT16 ltrme : 1;

                            /* Bits[10:10], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1b, this bit enables Upstream Ports
                               to send LTR messages and Downstream Ports to
                               process LTR Messages. Applicable to Root Ports,
                               Switches, and Endpoints that implement the LTR
                               capability must implement this bit. For
                               Downstream Ports, this bit must be reset to the
                               default value if the Port goes to DL_Down
                               status.
                            */
    UINT16 rsvd : 1;

                            /* Bits[11:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 tenbtre : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               This bit, in combination with the Extended Tag
                               Field Enable bit in the Device Control register,
                               determines how many Tag field bits a Requester
                               is permitted to use. When the 10-Bit Tag
                               Requester Enable bit is Set, the Requester is
                               permitted to use 10-Bit Tags. See PCIe Spec.
                               Section 2.2.6.2 for complete details. Software
                               should not change the value of this bit while
                               the Function has outstanding Non-Posted
                               Requests, otherwise, the result is undefined.
                               Functions that do not implement 10-Bit Tag
                               Requester capability are permitted to hardwire
                               this bit to 0b. Default value of this bit is 0b
                            */
    UINT16 obffe : 2;

                            /* Bits[14:13], Access Type=RW, default=0x00000000*/

                            /*
                               00b: OBFF Disabled 01b: OBFF Enabled using
                               Message signaling [Variation A] 10b: OBFF
                               Enabled using Message signaling [Variation B]
                               11b: OBFF Enabled using WAKE# signaling This
                               field is required for all Ports that support the
                               OBFF Capability. For a Multi-Function Device
                               associated with an Upstream Port of a Device
                               that implements OBFF, the field in Function 0 is
                               of type RW, and only Function 0 controls the
                               Component's behavior. In all other Functions of
                               that Device, this field is of type RsvdP.
                            */
    UINT16 e2etlpblk : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Controls whether the routing function is
                               permitted to forward TLPs containing an End-End
                               TLP Prefix. 0: Forwarding Enabled - Function is
                               permitted to send TLPs with End-End TLP
                               Prefixes. 1: Forwarding Blocked - Function is
                               not permitted to send TLPs with End-End TLP
                               Prefixes. This bit is applicable to Root Ports
                               and Switch Ports where the End-End TLP Prefix
                               Supported bit is Set. This bit is not applicable
                               and is RsvdP in all other cases
                            */

  } Bits;
  UINT16 Data;

} DEVCTL2_IIO_PCIENTB_STRUCT;

/* PMCAPLST_IIO_PCIENTB_REG supported on:                                       */
/*      SPRA0 (0x100480a0)                                                      */
/*      SPRB0 (0x100480a0)                                                      */
/*      SPRHBM (0x100480a0)                                                     */
/*      SPRC0 (0x100480a0)                                                      */
/*      SPRMCC (0x100480a0)                                                     */
/*      SPRUCC (0x100480a0)                                                     */
/* Register default value on SPRA0: 0x0000B001                                  */
/* Register default value on SPRB0: 0x0000B001                                  */
/* Register default value on SPRHBM: 0x0000B001                                 */
/* Register default value on SPRC0: 0x0000B001                                  */
/* Register default value on SPRMCC: 0x0000B001                                 */
/* Register default value on SPRUCC: 0x0000B001                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Power Management Capability List
*/


#define PMCAPLST_IIO_PCIENTB_REG 0x0F0100A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 capabilityid : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000001*/

                            /*
                               Identifies the function as PCI Power Management
                               capable.
                            */
    UINT16 nextpointer : 8;

                            /* Bits[15:8], Access Type=RO, default=0x000000B0*/

                            /*
                               Contains the offset of the next item in the
                               capabilities list. (MSIXCAPLST)
                            */

  } Bits;
  UINT16 Data;

} PMCAPLST_IIO_PCIENTB_STRUCT;

/* PMCAP_IIO_PCIENTB_REG supported on:                                          */
/*      SPRA0 (0x100480a2)                                                      */
/*      SPRB0 (0x100480a2)                                                      */
/*      SPRHBM (0x100480a2)                                                     */
/*      SPRC0 (0x100480a2)                                                      */
/*      SPRMCC (0x100480a2)                                                     */
/*      SPRUCC (0x100480a2)                                                     */
/* Register default value on SPRA0: 0x0000C803                                  */
/* Register default value on SPRB0: 0x0000C803                                  */
/* Register default value on SPRHBM: 0x0000C803                                 */
/* Register default value on SPRC0: 0x0000C803                                  */
/* Register default value on SPRMCC: 0x0000C803                                 */
/* Register default value on SPRUCC: 0x0000C803                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Power Management Capabilities
*/


#define PMCAP_IIO_PCIENTB_REG 0x0F0100A2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ver : 3;

                            /* Bits[2:0], Access Type=RO, default=0x00000003*/

                            /*
                               The PM implementation in the PCIe cluster is
                               compliant with PCI Bus Power Management
                               Interface Specification, Revision 1.2.
                            */
    UINT16 pmeclk : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /* Does not apply to PCI Express. Hard-wired to 0. */
    UINT16 rsvd : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 dsi : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /*
                               Device-specific initialization is not required
                               when transitioning to D0 from D3hot state. This
                               bit is zero.
                            */
    UINT16 ac : 3;

                            /* Bits[8:6], Access Type=RO, default=0x00000000*/

                            /* Auxiliary power is not supported. */
    UINT16 d1s : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* Not supported */
    UINT16 d2s : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /* Not supported */
    UINT16 pmes : 5;

                            /* Bits[15:11], Access Type=RO, default=0x00000019*/

                            /*
                               PME assertion is supported when in D3hot/D3cold.
                               Identifies power states in which the PCIe can
                               send an 'Assert_PMEGPE/ Deassert PMEGPE'
                               message. Bits 15, 14 and 11 must be set to '1'
                               for PCIPCI bridge structures representing ports
                               on root complexes. The definition of these bits
                               is taken from the PCI Bus Power Management
                               Interface Specification Revision 1.1. XXXX1b -
                               Assert_PMEGPE/Deassert PMEGPE can be sent from
                               D0 XXX1Xb - Assert_PMEGPE/Deassert PMEGPE can be
                               sent from D1 (Not supported by PCI-E) XX1XXb -
                               Assert_PMEGPE/Deassert PMEGPE can be sent from
                               D2 (Not supported by PCI-E) X1XXXb -
                               Assert_PMEGPE/Deassert PMEGPE can be sent from
                               D3 hot (Supported by PCI-E) 1XXXXb -
                               Assert_PMEGPE/Deassert PMEGPE can be sent from
                               D3 cold (Supported by PCI-E) In the EIP PCIe
                               implementation, there is no PME support for D3
                               cold as the part does not have the capability.
                               For PCIe compliance purposes, bit 15 is set to
                               1.
                            */

  } Bits;
  UINT16 Data;

} PMCAP_IIO_PCIENTB_STRUCT;

/* PMCSR_IIO_PCIENTB_REG supported on:                                          */
/*      SPRA0 (0x100480a4)                                                      */
/*      SPRB0 (0x100480a4)                                                      */
/*      SPRHBM (0x100480a4)                                                     */
/*      SPRC0 (0x100480a4)                                                      */
/*      SPRMCC (0x100480a4)                                                     */
/*      SPRUCC (0x100480a4)                                                     */
/* Register default value on SPRA0: 0x00000008                                  */
/* Register default value on SPRB0: 0x00000008                                  */
/* Register default value on SPRHBM: 0x00000008                                 */
/* Register default value on SPRC0: 0x00000008                                  */
/* Register default value on SPRMCC: 0x00000008                                 */
/* Register default value on SPRUCC: 0x00000008                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Power Management Control Status
*/


#define PMCSR_IIO_PCIENTB_REG 0x0F0100A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ps : 2;

                            /* Bits[1:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               This field is used both to determine the current
                               power state of a function and to set the
                               function into a new power state. The definition
                               of the supported values is given below: 0h - D0
                               3h - D3hot If software attempts to write an
                               unsupported, optional state to this field, the
                               write operation must complete normally, however,
                               the data is discarded and no state change
                               occurs.
                            */
    UINT16 rsvd : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 nsr : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000001*/

                            /*
                               This bit indicates the state of the Function
                               afer writing the PowerState field to transition
                               the Function from D3Hot to D0. When Set, this
                               transition preserves internal Function state.
                               The Function is in D0Active and no additional
                               sofware intervention is required. When Clear,
                               this transition results in undefined internal
                               Function state. Regardless of this bit,
                               Functions that transition from D3Hot to D0 by
                               Fundamental Reset will return to D0uninitialized
                               with only PME context preserved if PME is
                               supported and enabled. The value of this field
                               will be reflected by CAPSR.PMCSR_NSR.
                            */
    UINT16 rsvd_4 : 4;

                            /* Bits[7:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 pmeen : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               When Set, the Function is permitted to generate
                               a PME. When Clear, the Function is not permitted
                               to generate a PME.
                            */
    UINT16 ds : 4;

                            /* Bits[12:9], Access Type=RO, default=0x00000000*/

                            /* Not supported */
    UINT16 dc : 2;

                            /* Bits[14:13], Access Type=RO, default=0x00000000*/

                            /* Not supported */
    UINT16 pmests : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit is Set when the Function would normally
                               generate a PME signal. The value of this bit is
                               not affected by the value of the PME_En bit.
                            */

  } Bits;
  UINT16 Data;

} PMCSR_IIO_PCIENTB_STRUCT;

/* MSIXCAPLST_IIO_PCIENTB_REG supported on:                                     */
/*      SPRA0 (0x100480b0)                                                      */
/*      SPRB0 (0x100480b0)                                                      */
/*      SPRHBM (0x100480b0)                                                     */
/*      SPRC0 (0x100480b0)                                                      */
/*      SPRMCC (0x100480b0)                                                     */
/*      SPRUCC (0x100480b0)                                                     */
/* Register default value on SPRA0: 0x00000011                                  */
/* Register default value on SPRB0: 0x00000011                                  */
/* Register default value on SPRHBM: 0x00000011                                 */
/* Register default value on SPRC0: 0x00000011                                  */
/* Register default value on SPRMCC: 0x00000011                                 */
/* Register default value on SPRUCC: 0x00000011                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* MSI-X Capability List
*/


#define MSIXCAPLST_IIO_PCIENTB_REG 0x0F0100B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 capid : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000011*/

                            /* Identifies the function as MSI-X capable. */
    UINT16 np : 8;

                            /* Bits[15:8], Access Type=RW/L, default=None*/

                            /*
                               Contains the offset of the next item in the
                               capabilities list. Set to Null. Note: Lock Key
                               bit is located in the Personality Lock Key
                               Control Register .
                            */

  } Bits;
  UINT16 Data;

} MSIXCAPLST_IIO_PCIENTB_STRUCT;

/* MSIXCTL_IIO_PCIENTB_REG supported on:                                        */
/*      SPRA0 (0x100480b2)                                                      */
/*      SPRB0 (0x100480b2)                                                      */
/*      SPRHBM (0x100480b2)                                                     */
/*      SPRC0 (0x100480b2)                                                      */
/*      SPRMCC (0x100480b2)                                                     */
/*      SPRUCC (0x100480b2)                                                     */
/* Register default value on SPRA0: 0x00000020                                  */
/* Register default value on SPRB0: 0x00000020                                  */
/* Register default value on SPRHBM: 0x00000020                                 */
/* Register default value on SPRC0: 0x00000020                                  */
/* Register default value on SPRMCC: 0x00000020                                 */
/* Register default value on SPRUCC: 0x00000020                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* MSI-X Control
*/


#define MSIXCTL_IIO_PCIENTB_REG 0x0F0100B2

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 ts : 11;

                            /* Bits[10:0], Access Type=RO/V, default=0x00000020*/

                            /*
                               System software reads this field to determine
                               the MSI-X Table Size N, which is encoded as N-1.
                               For example, a returned value of '00000000011'
                               indicates a table size of 4. The MSI-X has 33
                               vectors as is encoded as 32 (decimal) as default
                               value. The value of this field will be reflected
                               by CAPSR.MSIXCTL_TS.
                            */
    UINT16 rsvd : 3;

                            /* Bits[13:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 fnmsk : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               This bit masks all vectors associated with the
                               Function, regardless of the per-vector Mask bit
                               states.
                            */
    UINT16 msixe : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               This bit enables MSI-X as the interrupt mode of
                               operation instead of the legacy interrupt
                               mechanism.
                            */

  } Bits;
  UINT16 Data;

} MSIXCTL_IIO_PCIENTB_STRUCT;

/* MSIXTOTB_IIO_PCIENTB_REG supported on:                                       */
/*      SPRA0 (0x200480b4)                                                      */
/*      SPRB0 (0x200480b4)                                                      */
/*      SPRHBM (0x200480b4)                                                     */
/*      SPRC0 (0x200480b4)                                                      */
/*      SPRMCC (0x200480b4)                                                     */
/*      SPRUCC (0x200480b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register 'MSI-X Table Offset/Table BIR Register (MSIXTOTB)'when read as a 32bit value would yield 0x4000 and SW can suitably extract the table offset by masking off the lower 3 bits.
The offsets need to be changed for SR-IOV through system page size register for post v0.5.
*/


#define MSIXTOTB_IIO_PCIENTB_REG 0x0F0200B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tbir : 3;

                            /* Bits[2:0], Access Type=RW/L, default=None*/

                            /*
                               Indicates that the Express Port Memory Base
                               Address Register 0/1 (MBAR01), located at 10h in
                               Configuration Space, is used to map the
                               function's MSI-X Table into Memory Space. Note:
                               Lock Key bit is located in the Personality Lock
                               Key Control Register .
                            */
    UINT32 toff : 29;

                            /* Bits[31:3], Access Type=RW/L, default=None*/

                            /*
                               Offset from the address contained by one of the
                               Function's Base Address registers to the base of
                               the MSI-X Table (MSIXTADR) in Memory Space.
                               MSI-X Table is located at offset of 4K. The
                               lower 3 bits of the offset are treated as zero
                               to form a 32-bit address and is 8-B aligned.
                               (1000h /8 = 200h) Note: Lock Key bit is located
                               in the Personality Lock Key Control Register .
                            */

  } Bits;
  UINT32 Data;

} MSIXTOTB_IIO_PCIENTB_STRUCT;

/* MSIXTPOPB_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x200480b8)                                                      */
/*      SPRB0 (0x200480b8)                                                      */
/*      SPRHBM (0x200480b8)                                                     */
/*      SPRC0 (0x200480b8)                                                      */
/*      SPRMCC (0x200480b8)                                                     */
/*      SPRUCC (0x200480b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* This register 'PBA Offset/PBA BIR Register for MSI-X (MSIXTPOPB)'when read as a 32bit value would yield 0x8000 and SW can suitably extract the table offset by masking off the lower 3 bits.
*/


#define MSIXTPOPB_IIO_PCIENTB_REG 0x0F0200B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tbir : 3;

                            /* Bits[2:0], Access Type=RW/L, default=None*/

                            /*
                               Indicates that the Express Port Memory Base
                               Address Register 0/1 (MBAR01), located at 10h in
                               Configuration Space, is used to map the
                               function's MSI-X PBA offset into Memory Space.
                               Note: Lock Key bit is located in the Personality
                               Lock Key Control Register .
                            */
    UINT32 toff : 29;

                            /* Bits[31:3], Access Type=RW/L, default=None*/

                            /*
                               Offset from the address contained by one of the
                               Function's Base Address registers to the base of
                               the MSI-X Table (MSIXTADR) in Memory Space. The
                               lower 3 bits of the offset are treated as zero
                               to form a 32-bit address and is 8-B aligned.
                               (2000h /8 = 400h) Note: Lock Key bit is located
                               in the Personality Lock Key Control Register .
                            */

  } Bits;
  UINT32 Data;

} MSIXTPOPB_IIO_PCIENTB_STRUCT;

/* IMBAR23SZ_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x480c4)                                                         */
/*      SPRB0 (0x480c4)                                                         */
/*      SPRHBM (0x480c4)                                                        */
/*      SPRC0 (0x480c4)                                                         */
/*      SPRMCC (0x480c4)                                                        */
/*      SPRUCC (0x480c4)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* The size registers live at 0xC0 in config space.
*/


#define IMBAR23SZ_IIO_PCIENTB_REG 0x0F0000C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 size : 8;

                            /* Bits[7:0], Access Type=RW/L, default=None*/

                            /*
                               Value indicating the size of 64-bit MBAR23_IP on
                               the internal side of the NTB. This value is
                               written by BIOS prior to enumeration. The value
                               indicates the lowest order address bit that is
                               writable in the BAR. Only legal settings are 12
                               to 62 (decimal) because the upper two bits of
                               address on NT Link are consumed by the bar-
                               select, and representing BAR sizes starting at
                               4KB. Note: Lock Key bit is located in the PLKCTL
                               Register . Programming a value less than 12 or
                               greater than 62 will result in the BAR being
                               disabled.
                            */

  } Bits;
  UINT8 Data;

} IMBAR23SZ_IIO_PCIENTB_STRUCT;

/* IMBAR45SZ_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x480c5)                                                         */
/*      SPRB0 (0x480c5)                                                         */
/*      SPRHBM (0x480c5)                                                        */
/*      SPRC0 (0x480c5)                                                         */
/*      SPRMCC (0x480c5)                                                        */
/*      SPRUCC (0x480c5)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Memory BAR45 Size Register - Internal End Point
*/


#define IMBAR45SZ_IIO_PCIENTB_REG 0x0F0000C5

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 size : 8;

                            /* Bits[7:0], Access Type=RW/L, default=None*/

                            /*
                               Value indicating the size of 64-bit MBAR45_IP on
                               the internal side of the NTB. This value is
                               written by BIOS prior to enumeration. The value
                               indicates the lowest order address bit that is
                               writable in the BAR. Only legal settings are 12
                               to 62 (decimal) because the upper two bits of
                               address on NT Link are consumed by the bar-
                               select, and representing BAR sizes starting at
                               4KB. Note: Lock Key bit is located in the PLKCTL
                               Register . Programming a value less than 12 or
                               greater than 62 will result in the BAR being
                               disabled.
                            */

  } Bits;
  UINT8 Data;

} IMBAR45SZ_IIO_PCIENTB_STRUCT;

/* EMBAR23SZ_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x480c6)                                                         */
/*      SPRB0 (0x480c6)                                                         */
/*      SPRHBM (0x480c6)                                                        */
/*      SPRC0 (0x480c6)                                                         */
/*      SPRMCC (0x480c6)                                                        */
/*      SPRUCC (0x480c6)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Memory BAR23 Size Register - External End Point
*/


#define EMBAR23SZ_IIO_PCIENTB_REG 0x0F0000C6

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 size : 8;

                            /* Bits[7:0], Access Type=RW/L, default=None*/

                            /*
                               Value indicating the size of 64-bit MBAR23 on
                               the internal side of the NTB. This value is
                               written by BIOS prior to enumeration. The value
                               indicates the lowest order address bit that is
                               writable in the BAR. Only legal settings are 12
                               to 62 (decimal) because the upper two bits of
                               address on NT Link are consumed by the bar-
                               select, and representing BAR sizes starting at
                               4KB. Note: Lock Key bit is located in the PLKCTL
                               Register . Programming a value less than 12 or
                               greater than 62 will result in the BAR being
                               disabled.
                            */

  } Bits;
  UINT8 Data;

} EMBAR23SZ_IIO_PCIENTB_STRUCT;

/* EMBAR45SZ_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x480c7)                                                         */
/*      SPRB0 (0x480c7)                                                         */
/*      SPRHBM (0x480c7)                                                        */
/*      SPRC0 (0x480c7)                                                         */
/*      SPRMCC (0x480c7)                                                        */
/*      SPRUCC (0x480c7)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Memory BAR45 Size Register - External End Point
*/


#define EMBAR45SZ_IIO_PCIENTB_REG 0x0F0000C7

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 size : 8;

                            /* Bits[7:0], Access Type=RW/L, default=None*/

                            /*
                               Value indicating the size of 64-bit MBAR45 on
                               the internal side of the NTB. This value is
                               written by BIOS prior to enumeration. The value
                               indicates the lowest order address bit that is
                               writable in the BAR. Only legal settings are 12
                               to 62 (decimal) because the upper two bits of
                               address on NT Link are consumed by the bar-
                               select, and representing BAR sizes starting at
                               4KB. Note: Lock Key bit is located in the PLKCTL
                               Register . Programming a value less than 12 or
                               greater than 62 will result in the BAR being
                               disabled.
                            */

  } Bits;
  UINT8 Data;

} EMBAR45SZ_IIO_PCIENTB_STRUCT;


/* SWLTRSEL_IIO_PCIENTB_REG supported on:                                       */
/*      SPRA0 (0x480ec)                                                         */
/*      SPRB0 (0x480ec)                                                         */
/*      SPRHBM (0x480ec)                                                        */
/*      SPRC0 (0x480ec)                                                         */
/*      SPRMCC (0x480ec)                                                        */
/*      SPRUCC (0x480ec)                                                        */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* SW LTR Select
*/


#define SWLTRSEL_IIO_PCIENTB_REG 0x0F0000EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 swltrsel : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               0: Choose Active SW LTR. Refer to ACTSWLTR
                               register below 1: Choose Idle SW LTR. Refer to
                               IDLESWLTR register below.
                            */
    UINT8 rsvd : 7;

                            /* Bits[7:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT8 Data;

} SWLTRSEL_IIO_PCIENTB_STRUCT;

/* ACTSWLTR_IIO_PCIENTB_REG supported on:                                       */
/*      SPRA0 (0x200480f0)                                                      */
/*      SPRB0 (0x200480f0)                                                      */
/*      SPRHBM (0x200480f0)                                                     */
/*      SPRC0 (0x200480f0)                                                      */
/*      SPRMCC (0x200480f0)                                                     */
/*      SPRUCC (0x200480f0)                                                     */
/* Register default value on SPRA0: 0x08000800                                  */
/* Register default value on SPRB0: 0x08000800                                  */
/* Register default value on SPRHBM: 0x08000800                                 */
/* Register default value on SPRC0: 0x08000800                                  */
/* Register default value on SPRMCC: 0x08000800                                 */
/* Register default value on SPRUCC: 0x08000800                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Active SW LTR
*/


#define ACTSWLTR_IIO_PCIENTB_REG 0x0F0200F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snval : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /*
                               The Snoop latency value is a 10-bit field that
                               is multiplied by the Snoop Latency Scale.
                               Setting the value and scale fields to all 0's
                               indicates that the device will be impacted by
                               any delay and that the best possible service is
                               requested.
                            */
    UINT32 snlatscale : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000002*/

                            /*
                               This field defines the Latency Scale multiplier:
                               000: SNVAL times 1 ns 001: SNVAL times 32 ns
                               010: SNVAL times 1,024 ns 011: SNVAL times
                               32,768 ns 100: SNVAL times 1,048,576 ns 101:
                               SNVAL times 33,554,432 ns Others: Not Permitted.
                               The value of this register is multiplied with
                               the Snoop Latency Value to provide the Snoop
                               Latency requirement.
                            */
    UINT32 rsvd : 2;

                            /* Bits[14:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 snreqmnt : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               If the Snoop Requirement bit is clear, then it
                               indicates that the device has no LTR requirement
                               for snoop traffic. i.e. it can wait for service
                               indefinitely.
                            */
    UINT32 nsnval : 10;

                            /* Bits[25:16], Access Type=RW, default=0x00000000*/

                            /*
                               The Non Snoop latency value is a 10-bit field
                               that is multiplied by the non Snoop Latency
                               Scale. Setting the value and scale fields to all
                               0's indicates that the device will be impacted
                               by any delay and that the best possible service
                               is requested.
                            */
    UINT32 nslatscale : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000002*/

                            /*
                               This field defines the non Snoop Latency Scale
                               multiplier: 000: NSNVAL times 1 ns 001: NSNVAL
                               times 32 ns 010: NSNVAL times 1,024 ns 011:
                               NSNVAL times 32,768 ns 100: NSNVAL times
                               1,048,576 ns 101: NSNVAL times 33,554,432 ns
                               Others: Not Permitted. The value of this
                               register is multiplied with the Non Snoop
                               Latency Value to provide the non-snoop latency
                               requirement.
                            */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 nsreqmnt : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               If the Non Snoop Requirement bit is clear, then
                               it indicates that the device has no LTR
                               requirement for non-snoop traffic. i.e. it can
                               wait for service indefinitely.
                            */

  } Bits;
  UINT32 Data;

} ACTSWLTR_IIO_PCIENTB_STRUCT;

/* IDLESWLTR_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x200480f4)                                                      */
/*      SPRB0 (0x200480f4)                                                      */
/*      SPRHBM (0x200480f4)                                                     */
/*      SPRC0 (0x200480f4)                                                      */
/*      SPRMCC (0x200480f4)                                                     */
/*      SPRUCC (0x200480f4)                                                     */
/* Register default value on SPRA0: 0x08000800                                  */
/* Register default value on SPRB0: 0x08000800                                  */
/* Register default value on SPRHBM: 0x08000800                                 */
/* Register default value on SPRC0: 0x08000800                                  */
/* Register default value on SPRMCC: 0x08000800                                 */
/* Register default value on SPRUCC: 0x08000800                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Idle SW LTR
*/


#define IDLESWLTR_IIO_PCIENTB_REG 0x0F0200F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 snval : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /*
                               The Snoop latency value is a 10-bit field that
                               is multiplied by the Snoop Latency Scale.
                               Setting the value and scale fields to all 0's
                               indicates that the device will be impacted by
                               any delay and that the best possible service is
                               requested.
                            */
    UINT32 snlatscale : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000002*/

                            /*
                               This field defines the Latency Scale multiplier:
                               000: SNVAL times 1 ns 001: SNVAL times 32 ns
                               010: SNVAL times 1,024 ns 011: SNVAL times
                               32,768 ns 100: SNVAL times 1,048,576 ns 101:
                               SNVAL times 33,554,432 ns Others: Not Permitted.
                               The value of this register is multiplied with
                               the Snoop Latency Value to provide the Snoop
                               Latency requirement.
                            */
    UINT32 rsvd : 2;

                            /* Bits[14:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 snreqmnt : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               If the Snoop Requirement bit is clear, then it
                               indicates that the device has no LTR requirement
                               for snoop traffic. i.e. it can wait for service
                               indefinitely.
                            */
    UINT32 nsnval : 10;

                            /* Bits[25:16], Access Type=RW, default=0x00000000*/

                            /*
                               The Non Snoop latency value is a 10-bit field
                               that is multiplied by the Non Snoop Latency
                               Scale. Setting the value and scale fields to all
                               0's indicates that the device will be impacted
                               by any delay and that the best possible service
                               is requested.
                            */
    UINT32 nslatscale : 3;

                            /* Bits[28:26], Access Type=RW, default=0x00000002*/

                            /*
                               This field defines the non Snoop Latency Scale
                               multiplier: 000: NSNVAL times 1 ns 001: NSNVAL
                               times 32 ns 010: NSNVAL times 1,024 ns 011:
                               NSNVAL times 32,768 ns 100: NSNVAL times
                               1,048,576 ns 101: NSNVAL times 33,554,432 ns
                               Others: Not Permitted. The value of this
                               register is multiplied with the Non Snoop
                               Latency Value to provide the non-snoop latency
                               requirement.
                            */
    UINT32 rsvd_29 : 2;

                            /* Bits[30:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 nsreqmnt : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               If the Non Snoop Requirement bit is clear, then
                               it indicates that the device has no LTR
                               requirement for non-snoop traffic. i.e. it can
                               wait for service indefinitely.
                            */

  } Bits;
  UINT32 Data;

} IDLESWLTR_IIO_PCIENTB_STRUCT;


/* AERCAPHDR_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x20048100)                                                      */
/*      SPRB0 (0x20048100)                                                      */
/*      SPRHBM (0x20048100)                                                     */
/*      SPRC0 (0x20048100)                                                      */
/*      SPRMCC (0x20048100)                                                     */
/*      SPRUCC (0x20048100)                                                     */
/* Register default value on SPRA0: 0x00010001                                  */
/* Register default value on SPRB0: 0x00010001                                  */
/* Register default value on SPRHBM: 0x00010001                                 */
/* Register default value on SPRC0: 0x00010001                                  */
/* Register default value on SPRMCC: 0x00010001                                 */
/* Register default value on SPRUCC: 0x00010001                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Advanced Error Reporting Extended Capability Header
*/


#define AERCAPHDR_IIO_PCIENTB_REG 0x0F020100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 excapid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000001*/

                            /*
                               Identifies the function as Advanced Error
                               Reporting capable.
                            */
    UINT32 cv : 4;

                            /* Bits[19:16], Access Type=RO/V, default=0x00000001*/

                            /*
                               This field is a PCI-SIG defined version number
                               that indicates the version of the Capability
                               structure present. This field must be 2h if the
                               End-End TLP Prefix Supported bit of Device
                               Capabilities 2 Register is Set and must be 1h or
                               2h otherwise.
                            */
    UINT32 nco : 12;

                            /* Bits[31:20], Access Type=RW/L, default=None*/

                            /*
                               Contains the offset of the next structure in the
                               Extended Capabilities list. Note: Lock Key bit
                               is located in the Personality Lock Key Control
                               Register . Software will need to program this
                               field appropriately for linked list of
                               capabilities if default is not supported.
                            */

  } Bits;
  UINT32 Data;

} AERCAPHDR_IIO_PCIENTB_STRUCT;

/* ERRUNCSTS_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x20048104)                                                      */
/*      SPRB0 (0x20048104)                                                      */
/*      SPRHBM (0x20048104)                                                     */
/*      SPRC0 (0x20048104)                                                      */
/*      SPRMCC (0x20048104)                                                     */
/*      SPRUCC (0x20048104)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register reports the error status of individual uncorrectable error sources. An individual error status bit that is set to 1 indicates that a particular error occurred. Software can clear an error status by writing a 1 to the respective bit.
*/


#define ERRUNCSTS_IIO_PCIENTB_REG 0x0F020104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dlpe : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit is set when a data link protocol error
                               is detected.
                            */
    UINT32 slde : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit is set when a surprise down error is
                               detected. This bit does not apply to the
                               upstream port. NTB should have RW1C attr. for
                               link error interrupt escalation support.
                            */
    UINT32 rsvd_6 : 6;

                            /* Bits[11:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ptlpe : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit is set and the bridge logs the header
                               when a poisoned TLP is received from PCI
                               Express.
                            */
    UINT32 fce : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit is set when a flow control protocol
                               error is detected.
                            */
    UINT32 cte : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               For Switch Ports, this bit is set if the Switch
                               Port issues Non-Posted Requests on its own
                               behalf (vs. only forwarding such as Requests
                               generated by other devices).
                            */
    UINT32 cae : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               The bridge sets this bit and logs the header
                               associated with the request when the
                               configuration unit signals a completer abort.
                            */
    UINT32 uce : 1;

                            /* Bits[16:16], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit is set whenever a completion is
                               received with a requestor ID that does not match
                               side A or side B, or when a completion is
                               received with a matching requestor ID but an
                               unexpected tag field. Header logging is
                               performed.
                            */
    UINT32 roe : 1;

                            /* Bits[17:17], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit is set when the PCI Express interface
                               unit receive buffers overflow.
                            */
    UINT32 mtlpe : 1;

                            /* Bits[18:18], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit is set when it receives a malformed
                               TLP. Header logging is performed.
                            */
    UINT32 ecrce : 1;

                            /* Bits[19:19], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* ECRC checking is optional for project specific. */
    UINT32 ure : 1;

                            /* Bits[20:20], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit is set whenever an unsupported request
                               is detected on PCI Express port
                            */
    UINT32 acse : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /*
                               This bit is set whenever an ACS violation is
                               detected by the PCI Express port.
                            */
    UINT32 uie : 1;

                            /* Bits[22:22], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit is set whenever an uncorrectable
                               internal error is detected.
                            */
    UINT32 mce : 1;

                            /* Bits[23:23], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit is set whenever a Multicast TLP is
                               blocked.
                            */
    UINT32 aebe : 1;

                            /* Bits[24:24], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               This bit is set whenever an Atomic OP TLP is
                               blocked on any egress port
                            */
    UINT32 tpbe : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /* TLP Prefix Blocked Error Status */
    UINT32 ptlpeb : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /* Poisoned TLP Egress Blocked Status */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERRUNCSTS_IIO_PCIENTB_STRUCT;

/* ERRUNCMSK_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x20048108)                                                      */
/*      SPRB0 (0x20048108)                                                      */
/*      SPRHBM (0x20048108)                                                     */
/*      SPRC0 (0x20048108)                                                      */
/*      SPRMCC (0x20048108)                                                     */
/*      SPRUCC (0x20048108)                                                     */
/* Register default value on SPRA0: 0x00400000                                  */
/* Register default value on SPRB0: 0x00400000                                  */
/* Register default value on SPRHBM: 0x00400000                                 */
/* Register default value on SPRC0: 0x00400000                                  */
/* Register default value on SPRMCC: 0x00400000                                 */
/* Register default value on SPRUCC: 0x00400000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register controls the reporting of individual uncorrectable errors by device to the host bridge via a PCI Express error message. This register also controls the logging of the header. Refer to the PCI Express specifications for details of how the mask bits function. A masked error (respective bit set in the mask register) is not reported to the host bridge by the switch, nor is the header logged (status bits unaffected by the mask bit). There is a mask bit per bit of the Uncorrectable Error Status Register.
The attr. of AER Error Mask bits in VRSP will be changed to be compliant with PCIe Spec.
*/


#define ERRUNCMSK_IIO_PCIENTB_REG 0x0F020108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dlpem : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /* Data Link Protocol Error Mask */
    UINT32 sldem : 1;

                            /* Bits[5:5], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               This bit does not apply to the USP, EP, vSP or
                               vRP. NTB should have RWS-V attr. for link error
                               interrupt escalation support.
                            */
    UINT32 rsvd_6 : 6;

                            /* Bits[11:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ptlpem : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* Poisoned TLP Received Mask */
    UINT32 fcem : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /* Flow Control Protocol Error Mask */
    UINT32 ctem : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* Completion Timeout Mask */
    UINT32 caem : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* Completer Abort Mask */
    UINT32 ucem : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* Unexpected Completion Mask */
    UINT32 roem : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /* Receiver Overflow Mask */
    UINT32 mtlpem : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /* Malformed TLP Mask */
    UINT32 ecrcem : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* ECRC Error Mask */
    UINT32 urem : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* Unsupported Request Error Mask */
    UINT32 acsem : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /* ACS Violation Mask */
    UINT32 uiem : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /* Uncorrectable Internal Error Mask */
    UINT32 mcem : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* MC Blocked TLP Mask */
    UINT32 aebem : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* AtomicOp Egress Blocked Mask */
    UINT32 tpbem : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /* TLP Prefix Blocked Error Mask */
    UINT32 ptlpebm : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /* Poisoned TLP Egress Blocked Mask */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERRUNCMSK_IIO_PCIENTB_STRUCT;

/* ERRUNCSEV_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x2004810c)                                                      */
/*      SPRB0 (0x2004810c)                                                      */
/*      SPRHBM (0x2004810c)                                                     */
/*      SPRC0 (0x2004810c)                                                      */
/*      SPRMCC (0x2004810c)                                                     */
/*      SPRUCC (0x2004810c)                                                     */
/* Register default value on SPRA0: 0x00462010                                  */
/* Register default value on SPRB0: 0x00462010                                  */
/* Register default value on SPRHBM: 0x00462010                                 */
/* Register default value on SPRC0: 0x00462010                                  */
/* Register default value on SPRMCC: 0x00462010                                 */
/* Register default value on SPRUCC: 0x00462010                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register controls whether an individual uncorrectable error is reported as a fatal error. An uncorrectable error is reported as fatal when the corresponding error bit in this register is set. When the bit is cleared, the corresponding error is considered non-fatal. 
The attr. of AER Error Severity bits in VRSP will be changed to be compliant with PCIe Spec.
*/


#define ERRUNCSEV_IIO_PCIENTB_REG 0x0F02010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dlpes : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /* Data Link Protocol Error Severity */
    UINT32 sldes : 1;

                            /* Bits[5:5], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               This bit does not apply to the USP, EP, vSP and
                               vRP. NTB should have RWS-V attr. for link error
                               interrupt escalation support.
                            */
    UINT32 rsvd_6 : 6;

                            /* Bits[11:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ptlpes : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* Poisoned TLP Received Severity */
    UINT32 fces : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /* Flow Control Protocol Error Severity */
    UINT32 ctes : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /* Completion Timeout Error Severity */
    UINT32 caes : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /* Completer Abort Error Severity */
    UINT32 uces : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /* Unexpected Completion Error Severity */
    UINT32 roes : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000001*/

                            /* Receiver Overflow Error Severity */
    UINT32 mtlpes : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000001*/

                            /* Malformed TLP Severity */
    UINT32 ecrces : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /* ECRC Error Severity */
    UINT32 ures : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /* Unsupported Request Error Severity */
    UINT32 acses : 1;

                            /* Bits[21:21], Access Type=RO, default=0x00000000*/

                            /* ACS Violation Severity */
    UINT32 uies : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000001*/

                            /* Uncorrectable Internal Error Severity */
    UINT32 mces : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /* MC Blocked TLP Severity */
    UINT32 aebes : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* AtomicOp Egress Blocked Severity */
    UINT32 tpbes : 1;

                            /* Bits[25:25], Access Type=RO, default=0x00000000*/

                            /* TLP Prefix Blocked Error Severity */
    UINT32 ptlpebs : 1;

                            /* Bits[26:26], Access Type=RO, default=0x00000000*/

                            /* Poisoned TLP Egress Blocked Severity */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERRUNCSEV_IIO_PCIENTB_STRUCT;

/* ERRCORSTS_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x20048110)                                                      */
/*      SPRB0 (0x20048110)                                                      */
/*      SPRHBM (0x20048110)                                                     */
/*      SPRC0 (0x20048110)                                                      */
/*      SPRMCC (0x20048110)                                                     */
/*      SPRUCC (0x20048110)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register reports the error status of individual correctable error sources on a PCI Express device. An individual error status bit set to 1 indicates that a particular error has occurred. Software can clear the error status by writing a 1 to the respective bit.
*/


#define ERRCORSTS_IIO_PCIENTB_REG 0x0F020110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 re : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               The PCIe sets this bit when the physical layer
                               detects a receiver error.
                            */
    UINT32 rsvd : 5;

                            /* Bits[5:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 btlpe : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* The switch sets this bit on CRC errors on TLP. */
    UINT32 bdllpe : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* The switch sets this bit on CRC errors on DLLP. */
    UINT32 rnre : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               The PCIe sets this bit when the replay number
                               rolls over from 11 to 00.
                            */
    UINT32 rsvd_9 : 3;

                            /* Bits[11:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rtte : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               The PCIe sets this bit when replay timer time-
                               out occurs.
                            */
    UINT32 anfe : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Advisory Non-Fatal Error */
    UINT32 cie : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Correctable Internal Error */
    UINT32 hloe : 1;

                            /* Bits[15:15], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Header Log Overflow Error */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERRCORSTS_IIO_PCIENTB_STRUCT;

/* ERRCORMSK_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x20048114)                                                      */
/*      SPRB0 (0x20048114)                                                      */
/*      SPRHBM (0x20048114)                                                     */
/*      SPRC0 (0x20048114)                                                      */
/*      SPRMCC (0x20048114)                                                     */
/*      SPRUCC (0x20048114)                                                     */
/* Register default value on SPRA0: 0x0000E000                                  */
/* Register default value on SPRB0: 0x0000E000                                  */
/* Register default value on SPRHBM: 0x0000E000                                 */
/* Register default value on SPRC0: 0x0000E000                                  */
/* Register default value on SPRMCC: 0x0000E000                                 */
/* Register default value on SPRUCC: 0x0000E000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register controls the reporting of individual correctable errors via ERR_COR message. A masked error (respective bit set in mask register) is not reported to the host bridge by the switch. There is a mask bit corresponding to every bit in the Correctable Error Status Register.
*/


#define ERRCORMSK_IIO_PCIENTB_REG 0x0F020114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rem : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Receiver Error Mask */
    UINT32 rsvd : 5;

                            /* Bits[5:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 btlpem : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /* Bad TLP Error Mask */
    UINT32 bdllpem : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /* Bad DLLP Error Mask */
    UINT32 rnrem : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /* Replay Number Rollover Error Mask */
    UINT32 rsvd_9 : 3;

                            /* Bits[11:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rttem : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /* Replay Timer Timeout Error Mask */
    UINT32 anfem : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /* Advisory Non-Fatal Error Mask */
    UINT32 ciem : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /* Correctable Internal Error Mask */
    UINT32 hloem : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000001*/

                            /* Header Log Overflow Error Mask */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERRCORMSK_IIO_PCIENTB_STRUCT;

/* AERCAPCTL_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x20048118)                                                      */
/*      SPRB0 (0x20048118)                                                      */
/*      SPRHBM (0x20048118)                                                     */
/*      SPRC0 (0x20048118)                                                      */
/*      SPRMCC (0x20048118)                                                     */
/*      SPRUCC (0x20048118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register gives the status and control for ECRC checks and also the pointer to the first uncorrectable error that happened.
*/


#define AERCAPCTL_IIO_PCIENTB_REG 0x0F020118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fep : 5;

                            /* Bits[4:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               This field identifies the bit position of the
                               first error reported in the Uncorrectable Error
                               Status Register (xref). This register re-arms
                               itself (which does not change its current value)
                               as soon as the error status bit indicated by the
                               pointer is cleared by the software by writing a
                               1 to that status bit.
                            */
    UINT32 egc : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               If Set, this bit indicates that the Function is
                               capable of generating ECRC. The value of this
                               field will be reflected by CAPSR.AERCAPCTL_EGC.
                            */
    UINT32 ege : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               When Set, ECRC generation is enabled. Functions
                               that do not implement the associated mechanism
                               are permitted to hardwire this bit to 0b.
                               Default value of this bit is 0b.
                            */
    UINT32 ecc : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               If Set, this bit indicates that the Function is
                               capable of checking ECRC. The value of this
                               field will be reflected by CAPSR.AERCAPCTL_ECC.
                            */
    UINT32 ece : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               When Set, ECRC checking is enabled. Functions
                               that do not implement the associated mechanism
                               are permitted to hardwire this bit to 0b.
                            */
    UINT32 mhrc : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* Supports more than one error header. */
    UINT32 mhre : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /*
                               When set, this bit enables the function to
                               record more than one error header.
                            */
    UINT32 tlpplp : 1;

                            /* Bits[11:11], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               If Set and the First Error Pointer is valid,
                               indicates that the TLP Prefix Log register
                               contains valid information. If Clear or if First
                               Error Pointer is invalid, the TLP Prefix Log
                               register is undefined.
                            */
    UINT32 ctphlc : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               If Set, this bit indicates that the Function
                               records the prefix/header of Request TLPs that
                               experience a Completion Timeout error.
                            */
    UINT32 rsvd : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} AERCAPCTL_IIO_PCIENTB_STRUCT;

/* AERHDRLOG1_IIO_PCIENTB_REG supported on:                                     */
/*      SPRA0 (0x2004811c)                                                      */
/*      SPRB0 (0x2004811c)                                                      */
/*      SPRHBM (0x2004811c)                                                     */
/*      SPRC0 (0x2004811c)                                                      */
/*      SPRMCC (0x2004811c)                                                     */
/*      SPRUCC (0x2004811c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BTSMM_W                                          */
/* SPRB0 Security PolicyGroup: BTSMM_W                                          */
/* SPRHBM Security PolicyGroup: BTSMM_W                                         */
/* SPRC0 Security PolicyGroup: BTSMM_W                                          */
/* SPRMCC Security PolicyGroup: BTSMM_W                                         */
/* SPRUCC Security PolicyGroup: BTSMM_W                                         */
/* This register logs the 4 Dwords of the transaction header for PCI Express errors.
*/


#define AERHDRLOG1_IIO_PCIENTB_REG 0x0F02011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlphdrlog : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               As soon as an error is logged in this register,
                               it remains locked for further error-logging
                               until the software clears the status bit that
                               caused the header log (in other words, until the
                               error pointer is re-armed for logging again).
                            */

  } Bits;
  UINT32 Data;

} AERHDRLOG1_IIO_PCIENTB_STRUCT;

/* AERHDRLOG2_IIO_PCIENTB_REG supported on:                                     */
/*      SPRA0 (0x20048120)                                                      */
/*      SPRB0 (0x20048120)                                                      */
/*      SPRHBM (0x20048120)                                                     */
/*      SPRC0 (0x20048120)                                                      */
/*      SPRMCC (0x20048120)                                                     */
/*      SPRUCC (0x20048120)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BTSMM_W                                          */
/* SPRB0 Security PolicyGroup: BTSMM_W                                          */
/* SPRHBM Security PolicyGroup: BTSMM_W                                         */
/* SPRC0 Security PolicyGroup: BTSMM_W                                          */
/* SPRMCC Security PolicyGroup: BTSMM_W                                         */
/* SPRUCC Security PolicyGroup: BTSMM_W                                         */
/* This register logs the 4 Dwords of the transaction header for PCI Express errors.
*/


#define AERHDRLOG2_IIO_PCIENTB_REG 0x0F020120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlphdrlog : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               As soon as an error is logged in this register,
                               it remains locked for further error-logging
                               until the software clears the status bit that
                               caused the header log (in other words, until the
                               error pointer is re-armed for logging again).
                            */

  } Bits;
  UINT32 Data;

} AERHDRLOG2_IIO_PCIENTB_STRUCT;

/* AERHDRLOG3_IIO_PCIENTB_REG supported on:                                     */
/*      SPRA0 (0x20048124)                                                      */
/*      SPRB0 (0x20048124)                                                      */
/*      SPRHBM (0x20048124)                                                     */
/*      SPRC0 (0x20048124)                                                      */
/*      SPRMCC (0x20048124)                                                     */
/*      SPRUCC (0x20048124)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BTSMM_W                                          */
/* SPRB0 Security PolicyGroup: BTSMM_W                                          */
/* SPRHBM Security PolicyGroup: BTSMM_W                                         */
/* SPRC0 Security PolicyGroup: BTSMM_W                                          */
/* SPRMCC Security PolicyGroup: BTSMM_W                                         */
/* SPRUCC Security PolicyGroup: BTSMM_W                                         */
/* This register logs the 4 Dwords of the transaction header for PCI Express errors.
*/


#define AERHDRLOG3_IIO_PCIENTB_REG 0x0F020124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlphdrlog : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               As soon as an error is logged in this register,
                               it remains locked for further error-logging
                               until the software clears the status bit that
                               caused the header log (in other words, until the
                               error pointer is re-armed for logging again).
                            */

  } Bits;
  UINT32 Data;

} AERHDRLOG3_IIO_PCIENTB_STRUCT;

/* AERHDRLOG4_IIO_PCIENTB_REG supported on:                                     */
/*      SPRA0 (0x20048128)                                                      */
/*      SPRB0 (0x20048128)                                                      */
/*      SPRHBM (0x20048128)                                                     */
/*      SPRC0 (0x20048128)                                                      */
/*      SPRMCC (0x20048128)                                                     */
/*      SPRUCC (0x20048128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BTSMM_W                                          */
/* SPRB0 Security PolicyGroup: BTSMM_W                                          */
/* SPRHBM Security PolicyGroup: BTSMM_W                                         */
/* SPRC0 Security PolicyGroup: BTSMM_W                                          */
/* SPRMCC Security PolicyGroup: BTSMM_W                                         */
/* SPRUCC Security PolicyGroup: BTSMM_W                                         */
/* This register logs the 4 Dwords of the transaction header for PCI Express errors.
*/


#define AERHDRLOG4_IIO_PCIENTB_REG 0x0F020128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlphdrlog : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               As soon as an error is logged in this register,
                               it remains locked for further error-logging
                               until the software clears the status bit that
                               caused the header log (in other words, until the
                               error pointer is re-armed for logging again).
                            */

  } Bits;
  UINT32 Data;

} AERHDRLOG4_IIO_PCIENTB_STRUCT;

/* EINJCAPHDR_IIO_PCIENTB_REG supported on:                                     */
/*      SPRA0 (0x20048190)                                                      */
/*      SPRB0 (0x20048190)                                                      */
/*      SPRHBM (0x20048190)                                                     */
/*      SPRC0 (0x20048190)                                                      */
/*      SPRMCC (0x20048190)                                                     */
/*      SPRUCC (0x20048190)                                                     */
/* Register default value on SPRA0: 0x0001000B                                  */
/* Register default value on SPRB0: 0x0001000B                                  */
/* Register default value on SPRHBM: 0x0001000B                                 */
/* Register default value on SPRC0: 0x0001000B                                  */
/* Register default value on SPRMCC: 0x0001000B                                 */
/* Register default value on SPRUCC: 0x0001000B                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* EINJ Extended Capability Header
*/


#define EINJCAPHDR_IIO_PCIENTB_REG 0x0F020190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ecid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000000B*/

                            /* Vendor Defined Capability */
    UINT32 cv : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               Indicates the version of the Capability
                               structure present.
                            */
    UINT32 nco : 12;

                            /* Bits[31:20], Access Type=RW/V/L, default=None*/

                            /*
                               Contains the offset of the next structure in the
                               Extended Capabilities. Note: Lock Key bit is
                               located in the Personality Lock Key Control
                               Register . H/W may skip over the linked
                               capabilities list when DISALBE_DPC fuse is set
                               to 1 if supported, or hardware-aware S/W will
                               need to program this field appropriately for
                               linked capabilities list if default is not
                               supported.
                            */

  } Bits;
  UINT32 Data;

} EINJCAPHDR_IIO_PCIENTB_STRUCT;

/* EINJCTL_IIO_PCIENTB_REG supported on:                                        */
/*      SPRA0 (0x10048198)                                                      */
/*      SPRB0 (0x10048198)                                                      */
/*      SPRHBM (0x10048198)                                                     */
/*      SPRC0 (0x10048198)                                                      */
/*      SPRMCC (0x10048198)                                                     */
/*      SPRUCC (0x10048198)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* EINJ Control
*/


#define EINJCTL_IIO_PCIENTB_REG 0x0F010198

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 einjdis : 1;

                            /* Bits[0:0], Access Type=RW/L, default=None*/

                            /*
                               This bit disables the usage of the PCIe Error
                               Injection bits for EINJCTL.INJRCVERR, INJCTOERR
                               Note: Lock Key bit is located in the EINJCTL
                               Register . This bit is self-locking. Once this
                               bit is set to a 1b, this key bit can not be
                               unlocked. Writing a 0b has no effect on this
                               bit.
                            */
    UINT16 injrcverr : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is written from 0 to 1, one and
                               only one error assertion pulse is produced on
                               the error source signal for the given port. This
                               error will appear equivalent to an actual error
                               assertion because this event is OR'd into the
                               existing error reporting structure. To log
                               another error, this bit must be cleared first,
                               before setting again. Leaving this bit in a 1
                               state does not produce a persistent error
                               condition.
                            */
    UINT16 injctoerr : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               When this bit is written from 0 to 1, one and
                               only one error assertion pulse is produced on
                               the error source signal for the given port. This
                               error will appear equivalent to an actual error
                               assertion because this event is OR'd into the
                               existing error reporting structure. To log
                               another error, this bit must be cleared first,
                               before setting again. Leaving this bit in a 1
                               state does not produce a persistent error
                               condition.
                            */
    UINT16 rsvd : 13;

                            /* Bits[15:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} EINJCTL_IIO_PCIENTB_STRUCT;

/* LTRCAPHDR_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x200481e8)                                                      */
/*      SPRB0 (0x200481e8)                                                      */
/*      SPRHBM (0x200481e8)                                                     */
/*      SPRC0 (0x200481e8)                                                      */
/*      SPRMCC (0x200481e8)                                                     */
/*      SPRUCC (0x200481e8)                                                     */
/* Register default value on SPRA0: 0x00010018                                  */
/* Register default value on SPRB0: 0x00010018                                  */
/* Register default value on SPRHBM: 0x00010018                                 */
/* Register default value on SPRC0: 0x00010018                                  */
/* Register default value on SPRMCC: 0x00010018                                 */
/* Register default value on SPRUCC: 0x00010018                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* LTR Extended Capability Header
*/


#define LTRCAPHDR_IIO_PCIENTB_REG 0x0F0201E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ecid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000018*/

                            /* Identifies the function as LTR capable. */
    UINT32 cv : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               Indicates the version of the Capability
                               structure present.
                            */
    UINT32 nco : 12;

                            /* Bits[31:20], Access Type=RW/V/L, default=None*/

                            /*
                               Contains the offset of the next structure in the
                               Extended Capabilities. Note: Lock Key bit is
                               located in the Personality Lock Key Control
                               Register . H/W may skip over the linked
                               capabilities list when DISALBE_MC fuse is set to
                               1 if supported, or hardware-aware S/W will need
                               to program this field appropriately for linked
                               capabilities list if default is not supported.
                            */

  } Bits;
  UINT32 Data;

} LTRCAPHDR_IIO_PCIENTB_STRUCT;

/* MAXSNPLAT_IIO_PCIENTB_REG supported on:                                      */
/*      SPRA0 (0x100481ec)                                                      */
/*      SPRB0 (0x100481ec)                                                      */
/*      SPRHBM (0x100481ec)                                                     */
/*      SPRC0 (0x100481ec)                                                      */
/*      SPRMCC (0x100481ec)                                                     */
/*      SPRUCC (0x100481ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Max Snoop Latency
*/


#define MAXSNPLAT_IIO_PCIENTB_REG 0x0F0101EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 maxsnpscl : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /*
                               This register provides a scale for the value
                               contained within the Maximum Snoop LatencyValue
                               field. Encoding is the same as the LatencyScale
                               fields in the LTR Message. The default value for
                               this field is 0. Hardware operation is undefined
                               if software writes a Not Permitted value to this
                               field.
                            */
    UINT16 maxsnplv : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000000*/

                            /*
                               Along with the Max Snoop LatencyScale field,
                               this register specifies the maximum snoop
                               latency that a device is permitted to request.
                               Software should set this to the platform's
                               maximum supported latency or less. The default
                               value for this field is 0
                            */
    UINT16 rsvd : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} MAXSNPLAT_IIO_PCIENTB_STRUCT;

/* MAXNSNPLAT_IIO_PCIENTB_REG supported on:                                     */
/*      SPRA0 (0x100481ee)                                                      */
/*      SPRB0 (0x100481ee)                                                      */
/*      SPRHBM (0x100481ee)                                                     */
/*      SPRC0 (0x100481ee)                                                      */
/*      SPRMCC (0x100481ee)                                                     */
/*      SPRUCC (0x100481ee)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Max No-Snoop Latency
*/


#define MAXNSNPLAT_IIO_PCIENTB_REG 0x0F0101EE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 maxnsnpscl : 10;

                            /* Bits[9:0], Access Type=RW, default=0x00000000*/

                            /*
                               This register provides a scale for the value
                               contained within the Maximum No Snoop Latency
                               Value field. Encoding is the same as the
                               LatencyScale fields in the LTR Message. The
                               default value for this field is 0. Hardware
                               operation is undefined if software writes a Not
                               Permitted value to this field.
                            */
    UINT16 maxnsnplv : 3;

                            /* Bits[12:10], Access Type=RW, default=0x00000000*/

                            /*
                               Along with the Max No Snoop LatencyScale field,
                               this register specifies the maximum no snoop
                               latency that a device is permitted to request.
                               Software should set this to the platform's
                               maximum supported latency or less. The default
                               value for this field is 0
                            */
    UINT16 rsvd : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} MAXNSNPLAT_IIO_PCIENTB_STRUCT;

/* MCSTCAPHDR_IIO_PCIENTB_REG supported on:                                     */
/*      SPRA0 (0x20048228)                                                      */
/*      SPRB0 (0x20048228)                                                      */
/*      SPRHBM (0x20048228)                                                     */
/*      SPRC0 (0x20048228)                                                      */
/*      SPRMCC (0x20048228)                                                     */
/*      SPRUCC (0x20048228)                                                     */
/* Register default value on SPRA0: 0x00010012                                  */
/* Register default value on SPRB0: 0x00010012                                  */
/* Register default value on SPRHBM: 0x00010012                                 */
/* Register default value on SPRC0: 0x00010012                                  */
/* Register default value on SPRMCC: 0x00010012                                 */
/* Register default value on SPRUCC: 0x00010012                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Multicast Extended Capability Header
*/


#define MCSTCAPHDR_IIO_PCIENTB_REG 0x0F020228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ecid : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000012*/

                            /*
                               This field is a PCISIG defined ID number that
                               indicates the nature and format of the Extended
                               Capability. PCI Express Extended Capability ID
                               for the Multicast Capability is 0012h.
                            */
    UINT32 cv : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               This field is a PCI-SIG defined version number
                               that indicates the version of the Capability
                               structure present. Must be 1h for this version
                               of the specification.
                            */
    UINT32 nco : 12;

                            /* Bits[31:20], Access Type=RW/L, default=None*/

                            /*
                               Contains the offset of the next structure in the
                               Extended Capabilities. Note: Lock Key bit is
                               located in the Personality Lock Key Control
                               Register . Software will need to program this
                               field appropriately for linked list of
                               capabilities if default is not supported.
                            */

  } Bits;
  UINT32 Data;

} MCSTCAPHDR_IIO_PCIENTB_STRUCT;

/* MCSTCAP_IIO_PCIENTB_REG supported on:                                        */
/*      SPRA0 (0x1004822c)                                                      */
/*      SPRB0 (0x1004822c)                                                      */
/*      SPRHBM (0x1004822c)                                                     */
/*      SPRC0 (0x1004822c)                                                      */
/*      SPRMCC (0x1004822c)                                                     */
/*      SPRUCC (0x1004822c)                                                     */
/* Register default value on SPRA0: 0x0000000F                                  */
/* Register default value on SPRB0: 0x0000000F                                  */
/* Register default value on SPRHBM: 0x0000000F                                 */
/* Register default value on SPRC0: 0x0000000F                                  */
/* Register default value on SPRMCC: 0x0000000F                                 */
/* Register default value on SPRUCC: 0x0000000F                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Multicast Capability
*/


#define MCSTCAP_IIO_PCIENTB_REG 0x0F01022C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 mcmg : 6;

                            /* Bits[5:0], Access Type=RO, default=0x0000000F*/

                            /*
                               This field indicates the maximum number of
                               Multicast Groups that the component supports,
                               encoded as M-1. A value of 00h indicates that
                               one multicast Group is supported.
                            */
    UINT16 rsvd : 2;

                            /* Bits[7:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 mcwsr : 6;

                            /* Bits[13:8], Access Type=RW/L, default=None*/

                            /*
                               In Endpoints, the log2 of the Multicast Window
                               size requested. RsvdP in Switch and Root Ports.
                               Lock Key bit is located in the Personality Lock
                               Key Control Register ('PLKCTL'). Note: Lock Key
                               bit is located in the Personality Lock Key
                               Control Register .
                            */
    UINT16 rsvd_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 mcers : 1;

                            /* Bits[15:15], Access Type=RW/L, default=None*/

                            /*
                               1: When set, this bit indicates that ECRC
                               regeneration is supported. Note: Lock Key bit is
                               located in the Personality Lock Key Control
                               Register .
                            */

  } Bits;
  UINT16 Data;

} MCSTCAP_IIO_PCIENTB_STRUCT;

/* MCSTCTL_IIO_PCIENTB_REG supported on:                                        */
/*      SPRA0 (0x1004822e)                                                      */
/*      SPRB0 (0x1004822e)                                                      */
/*      SPRHBM (0x1004822e)                                                     */
/*      SPRC0 (0x1004822e)                                                      */
/*      SPRMCC (0x1004822e)                                                     */
/*      SPRUCC (0x1004822e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Multicast Control
*/


#define MCSTCTL_IIO_PCIENTB_REG 0x0F01022E

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 mcng : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /*
                               This field indicates the number of Multicast
                               Groups configured for use, encoded as N-1. A
                               value of 00h indicates that one Multicast Group
                               is configured for use. Behavior is undefined if
                               value exceeds MC Max Group. This parameter
                               indirectly defines the upper limit of the
                               Multicast address range. This field is ignored
                               if MC Enable is Clear.
                            */
    UINT16 rsvd : 9;

                            /* Bits[14:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 mcen : 1;

                            /* Bits[15:15], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set, this bit indicates the Multicast
                               Capability is enabled for the component. This is
                               a global field that controls MC for the specific
                               function. H/W will change this field to 'RO' of
                               zero when DISALBE_MC fuse is set to 1 if
                               supported.
                            */

  } Bits;
  UINT16 Data;

} MCSTCTL_IIO_PCIENTB_STRUCT;

/* MCSTBAR_N0_IIO_PCIENTB_REG supported on:                                     */
/*      SPRA0 (0x20048230)                                                      */
/*      SPRB0 (0x20048230)                                                      */
/*      SPRHBM (0x20048230)                                                     */
/*      SPRC0 (0x20048230)                                                      */
/*      SPRMCC (0x20048230)                                                     */
/*      SPRUCC (0x20048230)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The MC_Base_Address register contains the MC_Base_Address and the MC_Index_Position for calculating whether a MC hit occurred or not. Refer to thePCI Local Bus Specification, Revision 3.0 for details on how the MC hit/miss and the MC group number is calculated.
This register is 4K aligned and hence only the top 20 bits are useable for the base address.
*/


#define MCSTBAR_N0_IIO_PCIENTB_REG 0x0F020230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mcip : 6;

                            /* Bits[5:0], Access Type=RW, default=0x0000000C*/

                            /*
                               This field indicates the location of the LSB of
                               the Multicast Group number within the address.
                               Behavior is undefined if this field is less than
                               12 when MC Enable is set.
                            */
    UINT32 rsvd : 6;

                            /* Bits[11:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 mcba : 20;

                            /* Bits[31:12], Access Type=RW, default=0x00000000*/

                            /*
                               The base address of the Multicast address range.
                               The behavior is undefined if MCSTCTL.MCEN is Set
                               and bits in this field corresponding to address
                               bits that contain the Multicast Group number or
                               address bits less than MC_Index_Position are
                               non-zero
                            */

  } Bits;
  UINT32 Data;

} MCSTBAR_N0_IIO_PCIENTB_STRUCT;

/* MCSTBAR_N1_IIO_PCIENTB_REG supported on:                                     */
/*      SPRA0 (0x20048234)                                                      */
/*      SPRB0 (0x20048234)                                                      */
/*      SPRHBM (0x20048234)                                                     */
/*      SPRC0 (0x20048234)                                                      */
/*      SPRMCC (0x20048234)                                                     */
/*      SPRUCC (0x20048234)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The MC_Base_Address register contains the MC_Base_Address and the MC_Index_Position for calculating whether a MC hit occurred or not. Refer to thePCI Local Bus Specification, Revision 3.0 for details on how the MC hit/miss and the MC group number is calculated.
This register is 4K aligned and hence only the top 20 bits are useable for the base address.
*/


#define MCSTBAR_N1_IIO_PCIENTB_REG 0x0F020234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mcba : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               The base address of the Multicast address range.
                               The behavior is undefined if MCSTCTL.MCEN is Set
                               and bits in this field corresponding to address
                               bits that contain the Multicast Group number or
                               address bits less than MC_Index_Position are
                               non-zero
                            */

  } Bits;
  UINT32 Data;

} MCSTBAR_N1_IIO_PCIENTB_STRUCT;

/* MCSTRCV_N0_IIO_PCIENTB_REG supported on:                                     */
/*      SPRA0 (0x20048238)                                                      */
/*      SPRB0 (0x20048238)                                                      */
/*      SPRHBM (0x20048238)                                                     */
/*      SPRC0 (0x20048238)                                                      */
/*      SPRMCC (0x20048238)                                                     */
/*      SPRUCC (0x20048238)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The MC_Receive register provides a bit vector denoting which Multicast groups the Function should accept
*/


#define MCSTRCV_N0_IIO_PCIENTB_REG 0x0F020238

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mcr : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               For each bit that's Set, this Function gets a
                               copy of any Multicast TLPs for the associated
                               Multicast Group. Bits above MC_Num_Group are
                               ignored by hardware. Default value of each bit
                               is 0b
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MCSTRCV_N0_IIO_PCIENTB_STRUCT;

/* MCSTRCV_N1_IIO_PCIENTB_REG supported on:                                     */
/*      SPRA0 (0x2004823c)                                                      */
/*      SPRB0 (0x2004823c)                                                      */
/*      SPRHBM (0x2004823c)                                                     */
/*      SPRC0 (0x2004823c)                                                      */
/*      SPRMCC (0x2004823c)                                                     */
/*      SPRUCC (0x2004823c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* The MC_Receive register provides a bit vector denoting which Multicast groups the Function should accept
*/


#define MCSTRCV_N1_IIO_PCIENTB_REG 0x0F02023C

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

} MCSTRCV_N1_IIO_PCIENTB_STRUCT;

/* MCSTBLKALL_N0_IIO_PCIENTB_REG supported on:                                  */
/*      SPRA0 (0x20048240)                                                      */
/*      SPRB0 (0x20048240)                                                      */
/*      SPRHBM (0x20048240)                                                     */
/*      SPRC0 (0x20048240)                                                      */
/*      SPRMCC (0x20048240)                                                     */
/*      SPRUCC (0x20048240)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Multicast Block All
*/


#define MCSTBLKALL_N0_IIO_PCIENTB_REG 0x0F020240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mcba : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               For each bit that is set, this function is
                               blocked from sending TLPs to the associated
                               Multicast Group. Bits above MC_Number_Group are
                               ignored by hardware.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MCSTBLKALL_N0_IIO_PCIENTB_STRUCT;

/* MCSTBLKALL_N1_IIO_PCIENTB_REG supported on:                                  */
/*      SPRA0 (0x20048244)                                                      */
/*      SPRB0 (0x20048244)                                                      */
/*      SPRHBM (0x20048244)                                                     */
/*      SPRC0 (0x20048244)                                                      */
/*      SPRMCC (0x20048244)                                                     */
/*      SPRUCC (0x20048244)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Multicast Block All
*/


#define MCSTBLKALL_N1_IIO_PCIENTB_REG 0x0F020244

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

} MCSTBLKALL_N1_IIO_PCIENTB_STRUCT;

/* MCSTOLBAR_N0_IIO_PCIENTB_REG supported on:                                   */
/*      SPRA0 (0x20048250)                                                      */
/*      SPRB0 (0x20048250)                                                      */
/*      SPRHBM (0x20048250)                                                     */
/*      SPRC0 (0x20048250)                                                      */
/*      SPRMCC (0x20048250)                                                     */
/*      SPRUCC (0x20048250)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Multicast Overlay Base Address
*/


#define MCSTOLBAR_N0_IIO_PCIENTB_REG 0x0F020250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mcovrsz : 6;

                            /* Bits[5:0], Access Type=RW, default=0x00000000*/

                            /*
                               If 6 or greater, specifies the size in bytes of
                               the overlay aperture as a power of 2. If less
                               than 6, disables the overlay mechanism. Default
                               is 0.
                            */
    UINT32 mcovrbar : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /*
                               Specifies the base address of the window onto
                               which MC TLPs passing through this function will
                               be overlaid. Default is 0.
                            */

  } Bits;
  UINT32 Data;

} MCSTOLBAR_N0_IIO_PCIENTB_STRUCT;

/* MCSTOLBAR_N1_IIO_PCIENTB_REG supported on:                                   */
/*      SPRA0 (0x20048254)                                                      */
/*      SPRB0 (0x20048254)                                                      */
/*      SPRHBM (0x20048254)                                                     */
/*      SPRC0 (0x20048254)                                                      */
/*      SPRMCC (0x20048254)                                                     */
/*      SPRUCC (0x20048254)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Multicast Overlay Base Address
*/


#define MCSTOLBAR_N1_IIO_PCIENTB_REG 0x0F020254

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mcovrbar : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Specifies the base address of the window onto
                               which MC TLPs passing through this function will
                               be overlaid. Default is 0.
                            */

  } Bits;
  UINT32 Data;

} MCSTOLBAR_N1_IIO_PCIENTB_STRUCT;

/* PLKCTL_IIO_PCIENTB_REG supported on:                                         */
/*      SPRA0 (0x10048558)                                                      */
/*      SPRB0 (0x10048558)                                                      */
/*      SPRHBM (0x10048558)                                                     */
/*      SPRC0 (0x10048558)                                                      */
/*      SPRMCC (0x10048558)                                                     */
/*      SPRUCC (0x10048558)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: DFX2                                             */
/* SPRB0 Security PolicyGroup: DFX2                                             */
/* SPRHBM Security PolicyGroup: DFX2                                            */
/* SPRC0 Security PolicyGroup: DFX2                                             */
/* SPRMCC Security PolicyGroup: DFX2                                            */
/* SPRUCC Security PolicyGroup: DFX2                                            */
/* Personality Lock Key Control
*/


#define PLKCTL_IIO_PCIENTB_REG 0x0F010558

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 cl : 1;

                            /* Bits[0:0], Access Type=RW/L, default=None*/

                            /*
                               Lock key bit for all RWS-L bits (capabilities,
                               next capability pointer, SSID/SVID, slot
                               register, etc) bits for the function. 1b: Lock
                               0b: Unlocked Note: Lock Key bit is located in
                               the PLKCTL Register . This bit is self-locking.
                               Once this bit is set to a 1b, this key bit can
                               not be unlocked. Writing a 0b has no effect on
                               this bit.
                            */
    UINT16 tlcl : 1;

                            /* Bits[1:1], Access Type=RW/L, default=None*/

                            /*
                               Lock key bit for all RWS-L bits located in
                               XPEINJ[1:0], XPCDTHROTTLEP,N,C, XPTDEF*,
                               MBAR01CTL EXPPTMBARCTL, XPERRINJCTL,
                               SWERRINJCTL, IDFERRINJCTL. 1b: Lock 0b: Unlocked
                               Note: Lock Key bit is located in the PLKCTL
                               Register . This bit is self-locking. Once this
                               bit is set to a 1b, this key bit can not be
                               unlocked. Writing a 0b has no effect on this
                               bit.
                            */
    UINT16 llcl : 1;

                            /* Bits[2:2], Access Type=RW/L, default=None*/

                            /*
                               Lock key bit for all RWS-L bits located in the
                               following CSRs. XPLDEF XPLDEF2 LLRBERRINJCTL
                               RXPERRINJCTL CRCERRINJCTL CRCERRINJMSK RTRYCTL
                               PACKTHRESH ECRCERRINJCTL 1b: Lock 0b: Unlocked
                               Note: Lock Key bit is located in the PLKCTL
                               Register . This bit is self-locking. Once this
                               bit is set to a 1b, this key bit can not be
                               unlocked. Writing a 0b has no effect on this
                               bit.
                            */
    UINT16 rsvd : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 plcl : 1;

                            /* Bits[4:4], Access Type=RW/L, default=None*/

                            /*
                               Lock key bit for all RWS-L bits located in the
                               following CSRs. OBEINJCTL 1b: Lock 0b: Unlocked
                               Note: Lock Key bit is located in the PLKCTL
                               Register . This bit is self-locking. Once this
                               bit is set to a 1b, this key bit can not be
                               unlocked. Writing a 0b has no effect on this
                               bit. Only applicable to PCIe Phy Layer, and
                               should be treated as reserved if not applicable.
                            */
    UINT16 rsvd_5 : 11;

                            /* Bits[15:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PLKCTL_IIO_PCIENTB_STRUCT;



/* CAPSR_IIO_PCIENTB_REG supported on:                                          */
/*      SPRA0 (0x20048748)                                                      */
/*      SPRB0 (0x20048748)                                                      */
/*      SPRHBM (0x20048748)                                                     */
/*      SPRC0 (0x20048748)                                                      */
/*      SPRMCC (0x20048748)                                                     */
/*      SPRUCC (0x20048748)                                                     */
/* Register default value on SPRA0: 0x00054041                                  */
/* Register default value on SPRB0: 0x00054041                                  */
/* Register default value on SPRHBM: 0x00054041                                 */
/* Register default value on SPRC0: 0x00054041                                  */
/* Register default value on SPRMCC: 0x00054041                                 */
/* Register default value on SPRUCC: 0x00054041                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: BOOT_W                                           */
/* SPRB0 Security PolicyGroup: BOOT_W                                           */
/* SPRHBM Security PolicyGroup: BOOT_W                                          */
/* SPRC0 Security PolicyGroup: BOOT_W                                           */
/* SPRMCC Security PolicyGroup: BOOT_W                                          */
/* SPRUCC Security PolicyGroup: BOOT_W                                          */
/* Capability Shadowed
*/


#define CAPSR_IIO_PCIENTB_REG 0x0F020748

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmcsr_nsr : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is utilized for Platform aware
                               Software to program the value based on
                               platform/device specific. Hardware will reflect
                               the value to the corresponding PMCSR.NSR once
                               this field was written. This field should be
                               treated as reserved if not supported by
                               project/device specific.
                            */
    UINT32 msixctl_ts : 11;

                            /* Bits[11:1], Access Type=RW/P, default=0x00000020*/

                            /*
                               This field is utilized for Platform aware
                               Software to program the value based on
                               platform/device specific. Hardware will reflect
                               the value to the corresponding MSIXCTL.TS once
                               this field was written. This field should be
                               treated as reserved if not supported by
                               project/device specific.
                            */
    UINT32 aercapctl_egc : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is utilized for Platform aware
                               Software to program the value based on
                               platform/device specific. Hardware will reflect
                               the value to the corresponding AERCAPCTL.EGC
                               once this field was written. This field should
                               be treated as reserved if not supported by
                               project/device specific.
                            */
    UINT32 aercapctl_ecc : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is utilized for Platform aware
                               Software to program the value based on
                               platform/device specific. Hardware will reflect
                               the value to the corresponding AERCAPCTL.ECC
                               once this field was written. This field should
                               be treated as reserved if not supported by
                               project/device specific.
                            */
    UINT32 aercapctl_ctphlc : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is utilized for Platform aware
                               Software to program the value based on
                               platform/device specific. Hardware will reflect
                               the value to the corresponding AERCAPCTL.CPTHLC
                               once this field was written. This field should
                               be treated as reserved if not supported by
                               project/device specific.
                            */
    UINT32 linkctl_ecpm : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is utilized for Platform aware
                               Software to program the value based on
                               platform/device specific. Hardware will reflect
                               the value to the corresponding LINKCTL.ECPM once
                               this field was written. This field should be
                               treated as reserved if not supported by
                               project/device specific.
                            */
    UINT32 linksts_scc : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is utilized for Platform aware
                               Software to program the value based on
                               platform/device specific. Hardware will reflect
                               the value to the corresponding LINKSTS.SCC once
                               this field was written. This field should be
                               treated as reserved if not supported by
                               project/device specific.
                            */
    UINT32 linkctl2_sd : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is utilized for Platform aware
                               Software to program the value based on
                               platform/device specific. Hardware will reflect
                               the value to the corresponding LINKCTL2.SD once
                               this field was written. This field should be
                               treated as reserved if not supported by
                               project/device specific.
                            */
    UINT32 msictl_pvm : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000001*/

                            /*
                               This field is utilized for Platform aware
                               Software to program the value based on
                               platform/device specific. Hardware will reflect
                               the value to the corresponding MSICTL.PVM once
                               this field was written. This field should be
                               treated as reserved if not supported by
                               project/device specific.
                            */
    UINT32 g5sts_txprecreq : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               This field is utilized for Platform aware
                               Software to program the value based on
                               platform/device specific. Hardware will reflect
                               the value to the corresponding G5STS.TXPREREQ
                               once this field was written. This field should
                               be treated as reserved if not supported by
                               project/device specific.
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CAPSR_IIO_PCIENTB_STRUCT;



/* CFGPCIECLKGCTL_IIO_PCIENTB_REG supported on:                                 */
/*      SPRA0 (0x100489ec)                                                      */
/*      SPRB0 (0x100489ec)                                                      */
/*      SPRHBM (0x100489ec)                                                     */
/*      SPRC0 (0x100489ec)                                                      */
/*      SPRMCC (0x100489ec)                                                     */
/*      SPRUCC (0x100489ec)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRB0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRHBM Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRC0 Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst     */
/* SPRMCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* SPRUCC Register File:    sprsp_top/pciernd_sprrnd4x16[0]/ntb_iep/cfg_inst    */
/* Struct generated from SPRA0 BDF: 0_9_0                                       */
/* SPRA0 Security PolicyGroup: DFX1                                             */
/* SPRB0 Security PolicyGroup: DFX1                                             */
/* SPRHBM Security PolicyGroup: DFX1                                            */
/* SPRC0 Security PolicyGroup: DFX1                                             */
/* SPRMCC Security PolicyGroup: DFX1                                            */
/* SPRUCC Security PolicyGroup: DFX1                                            */
/* Configuration PCIe Clock Gate Control
*/


#define CFGPCIECLKGCTL_IIO_PCIENTB_REG 0x0F0109EC

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

} CFGPCIECLKGCTL_IIO_PCIENTB_STRUCT;
#endif /* _IIO_PCIENTB_h */
