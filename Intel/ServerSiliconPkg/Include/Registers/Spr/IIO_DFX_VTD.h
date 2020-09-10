
/** @file
  IIO_DFX_VTD.h

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
/* DFX                                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
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
/* DFX                                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
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
/* DFX                                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
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
/* DFX                                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
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
/* DFX                                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
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
/* DFX                                                                          */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | UNCORE_PMA_SAI |     */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI | OOB_MSM_SAI      */
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


#ifndef _IIO_DFX_VTD_h
#define _IIO_DFX_VTD_h
#include <Base.h>

/* VID_IIO_DFX_VTD_REG supported on:                                            */
/*      SPRA0 (0x10803000)                                                      */
/*      SPRB0 (0x10803000)                                                      */
/*      SPRHBM (0x10803000)                                                     */
/*      SPRC0 (0x10803000)                                                      */
/*      SPRMCC (0x10803000)                                                     */
/*      SPRUCC (0x10803000)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Vendor ID
*/


#define VID_IIO_DFX_VTD_REG 0x12410000

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

} VID_IIO_DFX_VTD_STRUCT;

/* DID_IIO_DFX_VTD_REG supported on:                                            */
/*      SPRA0 (0x10803002)                                                      */
/*      SPRB0 (0x10803002)                                                      */
/*      SPRHBM (0x10803002)                                                     */
/*      SPRC0 (0x10803002)                                                      */
/*      SPRMCC (0x10803002)                                                     */
/*      SPRUCC (0x10803002)                                                     */
/* Register default value on SPRA0: 0x000009A5                                  */
/* Register default value on SPRB0: 0x000009A5                                  */
/* Register default value on SPRHBM: 0x000009A5                                 */
/* Register default value on SPRC0: 0x000009A5                                  */
/* Register default value on SPRMCC: 0x000009A5                                 */
/* Register default value on SPRUCC: 0x000009A5                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Device ID
*/


#define DID_IIO_DFX_VTD_REG 0x12410002

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 device_identification_number : 16;

                            /* Bits[15:0], Access Type=RO, default=0x000009A5*/

                            /* Device ID for the MS2IOSF Debug Registers */

  } Bits;
  UINT16 Data;

} DID_IIO_DFX_VTD_STRUCT;

/* PCICMD_IIO_DFX_VTD_REG supported on:                                         */
/*      SPRA0 (0x10803004)                                                      */
/*      SPRB0 (0x10803004)                                                      */
/*      SPRHBM (0x10803004)                                                     */
/*      SPRC0 (0x10803004)                                                      */
/*      SPRMCC (0x10803004)                                                     */
/*      SPRUCC (0x10803004)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Command
*/


#define PCICMD_IIO_DFX_VTD_REG 0x12410004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 iose : 1;

                            /* Bits[0:0], Access Type=RO, default=0x00000000*/

                            /*
                               IO Space Enable[br] Not applicable to DFx and is
                               hardwired to 0
                            */
    UINT16 mse : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Space Enable[br] Not applicable to DFx
                               and is hardwired to 0
                            */
    UINT16 bme : 1;

                            /* Bits[2:2], Access Type=RO, default=0x00000000*/

                            /*
                               Bus Master Enable[br] Not applicable to DFx and
                               is hardwired to 0
                            */
    UINT16 sce : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000000*/

                            /*
                               Special Cycle Enable[br] Not applicable to DFx
                               and is hardwired to 0
                            */
    UINT16 mwie : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000000*/

                            /*
                               Memory Write and Invalidate Enable[br] Not
                               applicable to DFx and is hardwired to 0
                            */
    UINT16 vga_palette_snoop_enable : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /* Not applicable to DFx and is hardwired to 0 */
    UINT16 perre : 1;

                            /* Bits[6:6], Access Type=RO, default=0x00000000*/

                            /*
                               Parity Error Response[br] Not applicable to DFx
                               and is hardwired to 0
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
                               SERR Enable[br] Not applicable to DFx and is
                               hardwired to 0
                            */
    UINT16 fast_back_to_back_enable : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /* Not applicable to DFx and is hardwired to 0 */
    UINT16 intx_disable : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000000*/

                            /* Not applicable to DFx and is hardwired to 0 */
    UINT16 rsvd : 5;

                            /* Bits[15:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} PCICMD_IIO_DFX_VTD_STRUCT;

/* PCISTS_IIO_DFX_VTD_REG supported on:                                         */
/*      SPRA0 (0x10803006)                                                      */
/*      SPRB0 (0x10803006)                                                      */
/*      SPRHBM (0x10803006)                                                     */
/*      SPRC0 (0x10803006)                                                      */
/*      SPRMCC (0x10803006)                                                     */
/*      SPRUCC (0x10803006)                                                     */
/* Register default value on SPRA0: 0x00000010                                  */
/* Register default value on SPRB0: 0x00000010                                  */
/* Register default value on SPRHBM: 0x00000010                                 */
/* Register default value on SPRC0: 0x00000010                                  */
/* Register default value on SPRMCC: 0x00000010                                 */
/* Register default value on SPRUCC: 0x00000010                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Status
*/


#define PCISTS_IIO_DFX_VTD_REG 0x12410006

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

                            /* Not applicable to DFx and is hardwired to 0 */
    UINT16 capabilities_list : 1;

                            /* Bits[4:4], Access Type=RO, default=0x00000001*/

                            /* Indicates there are capability structures. */
    UINT16 pci66mhz_capable : 1;

                            /* Bits[5:5], Access Type=RO, default=0x00000000*/

                            /* Not applicable to DFx and is hardwired to 0 */
    UINT16 rsvd_6 : 1;

                            /* Bits[6:6], Access Type=RO, default=None*/

                            /* Reserved */
    UINT16 fast_back_to_back : 1;

                            /* Bits[7:7], Access Type=RO, default=0x00000000*/

                            /* Not applicable to DFx and is hardwired to 0 */
    UINT16 mdpe : 1;

                            /* Bits[8:8], Access Type=RO, default=0x00000000*/

                            /*
                               Master Data Parity Error[br] Not applicable to
                               DFx and is hardwired to 0
                            */
    UINT16 devsel_timing : 2;

                            /* Bits[10:9], Access Type=RO, default=0x00000000*/

                            /* Not applicable to DFx and is hardwired to 0 */
    UINT16 sta : 1;

                            /* Bits[11:11], Access Type=RO, default=0x00000000*/

                            /*
                               Signaled Target Abort[br] Not applicable to DFx
                               and is hardwired to 0
                            */
    UINT16 rta : 1;

                            /* Bits[12:12], Access Type=RO, default=0x00000000*/

                            /*
                               Received Target Abort[br] Not applicable to DFx
                               and is hardwired to 0
                            */
    UINT16 rma : 1;

                            /* Bits[13:13], Access Type=RO, default=0x00000000*/

                            /*
                               Received Master Abort[br] Not applicable to DFx
                               and is hardwired to 0
                            */
    UINT16 sse : 1;

                            /* Bits[14:14], Access Type=RO, default=0x00000000*/

                            /*
                               Signaled System Error[br] Not applicable to DFx
                               and is hardwired to 0
                            */
    UINT16 dpe : 1;

                            /* Bits[15:15], Access Type=RO, default=0x00000000*/

                            /*
                               Detected Parity Error[br] Not applicable to DFx
                               and is hardwired to 0
                            */

  } Bits;
  UINT16 Data;

} PCISTS_IIO_DFX_VTD_STRUCT;

/* RID_CCR_IIO_DFX_VTD_REG supported on:                                        */
/*      SPRA0 (0x20803008)                                                      */
/*      SPRB0 (0x20803008)                                                      */
/*      SPRHBM (0x20803008)                                                     */
/*      SPRC0 (0x20803008)                                                      */
/*      SPRMCC (0x20803008)                                                     */
/*      SPRUCC (0x20803008)                                                     */
/* Register default value on SPRA0: 0x08800000                                  */
/* Register default value on SPRB0: 0x08800000                                  */
/* Register default value on SPRHBM: 0x08800000                                 */
/* Register default value on SPRC0: 0x08800000                                  */
/* Register default value on SPRMCC: 0x08800000                                 */
/* Register default value on SPRUCC: 0x08800000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCIe header Revision ID register and Class Code register
*/


#define RID_CCR_IIO_DFX_VTD_REG 0x12420008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 revision_id : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               MS2IOSF DFx Revision ID. Reflects the Uncore
                               Revision ID after reset. MS2IOSF loads this
                               value from a fuse.
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

} RID_CCR_IIO_DFX_VTD_STRUCT;

/* CLSR_IIO_DFX_VTD_REG supported on:                                           */
/*      SPRA0 (0x80300c)                                                        */
/*      SPRB0 (0x80300c)                                                        */
/*      SPRHBM (0x80300c)                                                       */
/*      SPRC0 (0x80300c)                                                        */
/*      SPRMCC (0x80300c)                                                       */
/*      SPRUCC (0x80300c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Cacheline Size Register
*/


#define CLSR_IIO_DFX_VTD_REG 0x1240000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 cacheline_size : 8;

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /*
                               This register is set as RW for compatibility
                               reasons only.
                            */

  } Bits;
  UINT8 Data;

} CLSR_IIO_DFX_VTD_STRUCT;

/* HDR_IIO_DFX_VTD_REG supported on:                                            */
/*      SPRA0 (0x80300e)                                                        */
/*      SPRB0 (0x80300e)                                                        */
/*      SPRHBM (0x80300e)                                                       */
/*      SPRC0 (0x80300e)                                                        */
/*      SPRMCC (0x80300e)                                                       */
/*      SPRUCC (0x80300e)                                                       */
/* Register default value on SPRA0: 0x00000080                                  */
/* Register default value on SPRB0: 0x00000080                                  */
/* Register default value on SPRHBM: 0x00000080                                 */
/* Register default value on SPRC0: 0x00000080                                  */
/* Register default value on SPRMCC: 0x00000080                                 */
/* Register default value on SPRUCC: 0x00000080                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header Type
*/


#define HDR_IIO_DFX_VTD_REG 0x1240000E

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

} HDR_IIO_DFX_VTD_STRUCT;

/* SVID_IIO_DFX_VTD_REG supported on:                                           */
/*      SPRA0 (0x1080302c)                                                      */
/*      SPRB0 (0x1080302c)                                                      */
/*      SPRHBM (0x1080302c)                                                     */
/*      SPRC0 (0x1080302c)                                                      */
/*      SPRMCC (0x1080302c)                                                     */
/*      SPRUCC (0x1080302c)                                                     */
/* Register default value on SPRA0: 0x00008086                                  */
/* Register default value on SPRB0: 0x00008086                                  */
/* Register default value on SPRHBM: 0x00008086                                 */
/* Register default value on SPRC0: 0x00008086                                  */
/* Register default value on SPRMCC: 0x00008086                                 */
/* Register default value on SPRUCC: 0x00008086                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem Vendor ID
*/


#define SVID_IIO_DFX_VTD_REG 0x1241002C

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

} SVID_IIO_DFX_VTD_STRUCT;

/* SID_IIO_DFX_VTD_REG supported on:                                            */
/*      SPRA0 (0x1080302e)                                                      */
/*      SPRB0 (0x1080302e)                                                      */
/*      SPRHBM (0x1080302e)                                                     */
/*      SPRC0 (0x1080302e)                                                      */
/*      SPRMCC (0x1080302e)                                                     */
/*      SPRUCC (0x1080302e)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Subsystem ID
*/


#define SID_IIO_DFX_VTD_REG 0x1241002E

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

} SID_IIO_DFX_VTD_STRUCT;

/* CAPPTR_IIO_DFX_VTD_REG supported on:                                         */
/*      SPRA0 (0x803034)                                                        */
/*      SPRB0 (0x803034)                                                        */
/*      SPRHBM (0x803034)                                                       */
/*      SPRC0 (0x803034)                                                        */
/*      SPRMCC (0x803034)                                                       */
/*      SPRUCC (0x803034)                                                       */
/* Register default value on SPRA0: 0x00000040                                  */
/* Register default value on SPRB0: 0x00000040                                  */
/* Register default value on SPRHBM: 0x00000040                                 */
/* Register default value on SPRC0: 0x00000040                                  */
/* Register default value on SPRMCC: 0x00000040                                 */
/* Register default value on SPRUCC: 0x00000040                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Capability Pointer
*/


#define CAPPTR_IIO_DFX_VTD_REG 0x12400034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT8 capability_pointer : 8;

                            /* Bits[7:0], Access Type=RO, default=0x00000040*/

                            /*
                               Points to the first capability structure for the
                               device
                            */

  } Bits;
  UINT8 Data;

} CAPPTR_IIO_DFX_VTD_STRUCT;

/* INTL_IIO_DFX_VTD_REG supported on:                                           */
/*      SPRA0 (0x80303c)                                                        */
/*      SPRB0 (0x80303c)                                                        */
/*      SPRHBM (0x80303c)                                                       */
/*      SPRC0 (0x80303c)                                                        */
/*      SPRMCC (0x80303c)                                                       */
/*      SPRUCC (0x80303c)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Interrupt Line Register
*/


#define INTL_IIO_DFX_VTD_REG 0x1240003C

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

} INTL_IIO_DFX_VTD_STRUCT;

/* INTPIN_IIO_DFX_VTD_REG supported on:                                         */
/*      SPRA0 (0x80303d)                                                        */
/*      SPRB0 (0x80303d)                                                        */
/*      SPRHBM (0x80303d)                                                       */
/*      SPRC0 (0x80303d)                                                        */
/*      SPRMCC (0x80303d)                                                       */
/*      SPRUCC (0x80303d)                                                       */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Interrupt Pin Register
*/


#define INTPIN_IIO_DFX_VTD_REG 0x1240003D

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

} INTPIN_IIO_DFX_VTD_STRUCT;

/* PXPCAP_IIO_DFX_VTD_REG supported on:                                         */
/*      SPRA0 (0x20803040)                                                      */
/*      SPRB0 (0x20803040)                                                      */
/*      SPRHBM (0x20803040)                                                     */
/*      SPRC0 (0x20803040)                                                      */
/*      SPRMCC (0x20803040)                                                     */
/*      SPRUCC (0x20803040)                                                     */
/* Register default value on SPRA0: 0x00920010                                  */
/* Register default value on SPRB0: 0x00920010                                  */
/* Register default value on SPRHBM: 0x00920010                                 */
/* Register default value on SPRC0: 0x00920010                                  */
/* Register default value on SPRMCC: 0x00920010                                 */
/* Register default value on SPRUCC: 0x00920010                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PCI Express Capability
*/


#define PXPCAP_IIO_DFX_VTD_REG 0x12420040

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
                               3.0 of the PCI Express Spec.
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

} PXPCAP_IIO_DFX_VTD_STRUCT;

/* PXPEXTCAP_IIO_DFX_VTD_REG supported on:                                      */
/*      SPRA0 (0x20803100)                                                      */
/*      SPRB0 (0x20803100)                                                      */
/*      SPRHBM (0x20803100)                                                     */
/*      SPRC0 (0x20803100)                                                      */
/*      SPRMCC (0x20803100)                                                     */
/*      SPRUCC (0x20803100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This field implements the Extended Capability Header required by section 7.9.1 in the PCIe 3.0 spec.
*/


#define PXPEXTCAP_IIO_DFX_VTD_REG 0x12420100

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

} PXPEXTCAP_IIO_DFX_VTD_STRUCT;

/* TC_CTRL0_IIO_DFX_VTD_REG supported on:                                       */
/*      SPRA0 (0x20803108)                                                      */
/*      SPRB0 (0x20803108)                                                      */
/*      SPRHBM (0x20803108)                                                     */
/*      SPRC0 (0x20803108)                                                      */
/*      SPRMCC (0x20803108)                                                     */
/*      SPRUCC (0x20803108)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000418C                                  */
/* Register default value on SPRMCC: 0x0000418C                                 */
/* Register default value on SPRUCC: 0x0000418C                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* itc_skid_buffer_credits[br]
Number of skid buffer credits available to ITC. Only used for post-Si debug. Requires a warm reset to take effect.
*/


#define TC_CTRL0_IIO_DFX_VTD_REG 0x12420108

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 itc_skid_buffer_crdt_cnt : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x0000000C*/

                            /*
                               itc_skid_buffer_credits[br] Number of skid
                               buffer credits available to ITC. Only used for
                               post-Si debug. Requires a warm reset to take
                               effect.
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TC_CTRL0_IIO_DFX_VTD_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 itc_pc_skid_buffer_crdt_cnt : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x0000000C*/

                            /*
                               itc_pc_skid_buffer_credits[br] Number of skid
                               buffer credits available to ITC for Posted and
                               Completions. Only used for post-Si debug.
                               Requires a warm reset to take effect.
                            */
    UINT32 itc_np_skid_buffer_crdt_cnt : 5;

                            /* Bits[9:5], Access Type=RW/P, default=0x0000000C*/

                            /*
                               itc_skid_buffer_credits[br] Number of skid
                               buffer credits available to ITC for NP
                               transactions. Only used for post-Si debug.
                               Requires a warm reset to take effect.
                            */
    UINT32 first_hw_init_csr_lvl : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /*
                               This is a flag which gets set at first edge of
                               hw init CSR tc_misc_ucr.hw_init and only clears
                               on reset.
                            */
    UINT32 itc_skid_buffer_freelist_depth : 5;

                            /* Bits[15:11], Access Type=RW/P, default=0x00000008*/

                            /*
                               Depth of the Itc skid buffer free list Note: In
                               wave3 this need not be the same as the Itc Skid
                               buffer depth which is currently 24 (12: P/Cmps
                               and 12: NPs)
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TC_CTRL0_IIO_DFX_VTD_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

/* BIOS_MISC_CTRL_IIO_DFX_VTD_REG supported on:                                 */
/*      SPRA0 (0x2080310c)                                                      */
/*      SPRB0 (0x2080310c)                                                      */
/*      SPRHBM (0x2080310c)                                                     */
/*      SPRC0 (0x2080310c)                                                      */
/*      SPRMCC (0x2080310c)                                                     */
/*      SPRUCC (0x2080310c)                                                     */
/* Register default value on SPRA0: 0x00000007                                  */
/* Register default value on SPRB0: 0x00000007                                  */
/* Register default value on SPRHBM: 0x00000007                                 */
/* Register default value on SPRC0: 0x00000007                                  */
/* Register default value on SPRMCC: 0x00000007                                 */
/* Register default value on SPRUCC: 0x00000007                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* BIOS Miscellaneous Control Register
*/


#define BIOS_MISC_CTRL_IIO_DFX_VTD_REG 0x1242010C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hf_comp_status : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000007*/

                            /*
                               IOSF Hard Fail completion status encoding.
                               MS2IOSF treats inbound completions on IOSF as
                               "Hard Fail" if the completion status equals the
                               value in this field.
                            */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} BIOS_MISC_CTRL_IIO_DFX_VTD_STRUCT;

/* DFX_HVM_IIO_DFX_VTD_REG supported on:                                        */
/*      SPRA0 (0x2080311c)                                                      */
/*      SPRB0 (0x2080311c)                                                      */
/*      SPRHBM (0x2080311c)                                                     */
/*      SPRC0 (0x2080311c)                                                      */
/*      SPRMCC (0x2080311c)                                                     */
/*      SPRUCC (0x2080311c)                                                     */
/* Register default value on SPRA0: 0x000A0808                                  */
/* Register default value on SPRB0: 0x000A0808                                  */
/* Register default value on SPRHBM: 0x000A0808                                 */
/* Register default value on SPRC0: 0x000A0808                                  */
/* Register default value on SPRMCC: 0x000A0808                                 */
/* Register default value on SPRUCC: 0x000A0808                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* DFx HVM
*/


#define DFX_HVM_IIO_DFX_VTD_REG 0x1242011C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 short_rst : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Short Reset[br] Enables short reset for HVM
                               testing. Counters are shortened in PCIe.[br]
                               Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 digital_loopback : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Digital Loopback[br] Enables digital loopback
                               for HVM testing[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT32 en_pllclkdistoff : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Enable PLL Clock Distribution Off[br] Notes:[br]
                               - Locked by DBGBUSLCK
                            */
    UINT32 cfg_invalid_dev_mmio_chk_dis : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               By default, reads to invalid device/functions
                               return -1 and writes are dropped. When this bit
                               is clear, MS2IOSF ignores the device/function in
                               the request, allowing reads and writes to work,
                               as long as the address hits an existing
                               register. This is a survivability feature.[br]
                               Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 msgchan_gateclkdis : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Message Channel Clock Gating Disable[br]
                               Disables clock gating for the message
                               channel[br] Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 cfg_clkgate_dis : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Config Logic Clock Gate Disable[br] Disables
                               clock gating on the config logic.[br] Notes:[br]
                               - Locked by DBGBUSLCK
                            */
    UINT32 cfg_fastpath_dis : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Config Fastpath Disable[br] Disables fast path
                               only on config ring. When enabled, decodes
                               config transaction and will send it down just
                               the fast path if able.[br] Notes:[br] - Locked
                               by DBGBUSLCK
                            */
    UINT32 reutmbpenable : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               REUT MBP Enable[br] MBP to REUT Enable[br]
                               Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 free_delay_count : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000008*/

                            /*
                               Free Delay Count[br] Provides backpressure on
                               message channel to separate back to back
                               transactions. Set to zero to disable.[br]
                               Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 dis_msgchan_pmrsblock : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               If 0, the PMRsBlock blocks traffic to the
                               message channel bgf.[br] If 1, the PMRsBlock
                               does not traffic to the message channel bgf, and
                               message channel requests can be lost during GV
                               flows.[br] Notes:[br] Locked by DBGBUSLCK
                            */
    UINT32 rsvd : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 spare_cfg : 2;

                            /* Bits[15:14], Access Type=RW, default=0x00000000*/

                            /*
                               Spare Config[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT32 disable_msgchan_msi_ordering : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               When 0, msg channel completions for requestors
                               selected by msi_request_ord will be ordered to
                               message signalled interrupts. [br] When 1, msg
                               channel completions for requestors selected by
                               msi_request_ord will not be ordered to message
                               signalled interrupts.[br] Notes:[br] Locked by
                               DBGBUSLCK
                            */
    UINT32 msi_ordered_request : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               When 0, all message channel operations with the
                               IA bit set will be orderable to MSIs.[br] When
                               1, only message channel operations with a ubox
                               srcid with the IA bit set will be orderable to
                               MSIs.[br] disable_msgchan_msi_ordering must be 0
                               for this CSR to have an effect.[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 msi_injection : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               When 1, enable short duration injections to the
                               msg channel logic as if an MSI were pending.
                               When 0, no injection is enabled.[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 block_msgchan_bgf : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               When 1, the IIMI msg channel wrapper blocks
                               incoming msg channel requests at the BGF and at
                               the router.[br] When 0, the IIMI msg channel
                               wrapper blocks incoming msg channel requests at
                               the router only.[br] Notes:[br] Locked by
                               DBGBUSLCK
                            */
    UINT32 aggressive_back_pressure : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               We use this CSR to agressively backpressure
                               messages from Router to EP.
                            */
    UINT32 rtr2ep_p_np_arb_en : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               We use this CSR to enable arbitration of p and
                               np messages from router to EP.
                            */
    UINT32 rtr2ep_p_np_arb_force_dis : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               We use this CSR to enable arbitration of p and
                               np messages from router to EP.
                            */
    UINT32 rsvd_23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DFX_HVM_IIO_DFX_VTD_STRUCT;

/* IMI_PMONDBG_IIO_DFX_VTD_REG supported on:                                    */
/*      SPRA0 (0x20803188)                                                      */
/*      SPRB0 (0x20803188)                                                      */
/*      SPRHBM (0x20803188)                                                     */
/*      SPRC0 (0x20803188)                                                      */
/*      SPRMCC (0x20803188)                                                     */
/*      SPRUCC (0x20803188)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Enable special PMON debug modes for IIMI (TC) PMON counters
*/


#define IMI_PMONDBG_IIO_DFX_VTD_REG 0x12420188

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 clr_en : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /*
                               1 bit per PMON counter. When set, the
                               corresponding PMON counter is cleared whenever
                               the counter selected by clr_source increments
                            */
    UINT32 clr_source : 8;

                            /* Bits[15:8], Access Type=RW, default=0x00000000*/

                            /*
                               2 bits per PMON counter. Selects which PMON
                               counter to use for triggering the clearing of
                               this counter.[br] For example, if
                               clr_source[3:2] = 3, then PMON counter 1 will be
                               cleared whenever PMON counter 3 increments.
                            */
    UINT32 count_en : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /*
                               1 bit per PMON counter. When set, the
                               corresponding PMON counter is enabled only if
                               the counter selected by count_source is non-zero
                            */
    UINT32 count_source : 8;

                            /* Bits[27:20], Access Type=RW, default=0x00000000*/

                            /*
                               2 bits per PMON counter. Selects which PMON
                               counter to use for triggering the clearing of
                               this counter.[br] For example, if
                               clr_source[3:2] = 3, then PMON counter 1 will be
                               cleared whenever PMON counter 3 increments.
                            */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IMI_PMONDBG_IIO_DFX_VTD_STRUCT;

/* TSWCTL0_IIO_DFX_VTD_REG supported on:                                        */
/*      SPRA0 (0x2080330c)                                                      */
/*      SPRB0 (0x2080330c)                                                      */
/*      SPRHBM (0x2080330c)                                                     */
/*      SPRC0 (0x2080330c)                                                      */
/*      SPRMCC (0x2080330c)                                                     */
/*      SPRUCC (0x2080330c)                                                     */
/* Register default value on SPRA0: 0x14700000                                  */
/* Register default value on SPRB0: 0x14700000                                  */
/* Register default value on SPRHBM: 0x14700000                                 */
/* Register default value on SPRC0: 0x14700000                                  */
/* Register default value on SPRMCC: 0x14700000                                 */
/* Register default value on SPRUCC: 0x14700000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Switch Control 0
*/


#define TSWCTL0_IIO_DFX_VTD_REG 0x1242030C

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 block_multiple_err_for_same_ent : 1;

                            /* Bits[2:2], Access Type=RW/O/P, default=0x00000000*/

                            /* Block Multiple Err For Same Ent */
    UINT32 me_disable : 1;

                            /* Bits[3:3], Access Type=RW/O/P, default=0x00000000*/

                            /* Disable ME region */
    UINT32 allow_ib_mmio_cfg : 1;

                            /* Bits[4:4], Access Type=RW/O/P, default=0x00000000*/

                            /* Allow Inbound MMIO Config */
    UINT32 ignore_acs_p2p_ma_lpbk : 1;

                            /* Bits[5:5], Access Type=RW/O, default=0x00000000*/

                            /* Ignore ACS p2p Master Abort Loopback */
    UINT32 shutdown_intack_cpl_abrt_cfg : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Shutdown INTACK Completion Abort Config[br]
                               Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 csr_vlw_throttle_cpl_disable : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               VLW Throttle Completion Disable[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT32 throttle_chnl8_on_iommu_colorstall : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Throttle miscblk initiated traffic on channel 8
                               requiring virtualization based on color stall
                               (SatIEH initiated MSIs, VMD initiated MSIs)
                            */
    UINT32 disable_throttle_chnl8_on_iommu_occupancy : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Disable throttle for miscblk initiated traffic
                               on channel 8 requiring virtualization based on
                               IOMMU PWT occupancy exceeding threshold (SatIEH
                               initiated MSIs, VMD initiated MSIs)
                            */
    UINT32 rsvd_10 : 3;

                            /* Bits[12:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 disable_jsm_lock_throttle : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Disable JSM Lock Throttle[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT32 me_eoibroad_en : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               ME EOI Broadcast Enable[br] Notes:[br] - Locked
                               by DBGBUSLCK
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 csi1_present : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               CSI1 Present[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT32 msi_pend_cpl_disable : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               MSI Pending Completion Disable[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT32 convert_ob_ca_to_ma : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Convert Outbound Completer Abort to Master
                               Abort[br] Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 convert_ib_ca_to_ma : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Convert Inbound Completer Abort to Master
                               Abort[br] Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 disable_vmd_rx_mailbox : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /* Disable Rx Mailbox for BMC->VMD communication */
    UINT32 disable_vmd_tx_mailbox : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /* Disable Tx Mailbox for VMD->BMC communication */
    UINT32 disable_vdm_mctp : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Disable Encapsulated MCTP VDM handling in OOBMSM
                               proxy sub-agent within miscblk
                            */
    UINT32 ignore_vmd_vdm_parerr : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Ignore data parity error from otc2misc within
                               OOBMSM sub-agent in miscblk
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lock_oldmode : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Lock Old Mode[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT32 lock_target_np : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Lock Target NP[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 suppress_mem_rd_lock : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Suppress Mem Read Lock[br] Notes:[br] - Locked
                               by DBGBUSLCK
                            */
    UINT32 rsvd_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 csr_dfx_dis_vphold_ignore_all : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Disable VPHold Ignore All[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSWCTL0_IIO_DFX_VTD_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 block_multiple_err_for_same_ent : 1;

                            /* Bits[2:2], Access Type=RW/O/P, default=0x00000000*/

                            /* Block Multiple Err For Same Ent */
    UINT32 me_disable : 1;

                            /* Bits[3:3], Access Type=RW/O/P, default=0x00000000*/

                            /* Disable ME region */
    UINT32 allow_ib_mmio_cfg : 1;

                            /* Bits[4:4], Access Type=RW/O/P, default=0x00000000*/

                            /* Allow Inbound MMIO Config */
    UINT32 ignore_acs_p2p_ma_lpbk : 1;

                            /* Bits[5:5], Access Type=RW/O, default=0x00000000*/

                            /* Ignore ACS p2p Master Abort Loopback */
    UINT32 shutdown_intack_cpl_abrt_cfg : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Shutdown INTACK Completion Abort Config[br]
                               Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 csr_vlw_throttle_cpl_disable : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               VLW Throttle Completion Disable[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT32 throttle_chnl8_on_iommu_colorstall : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Throttle miscblk initiated traffic on channel 8
                               requiring virtualization based on color stall
                               (SatIEH initiated MSIs, VMD initiated MSIs)
                            */
    UINT32 disable_throttle_chnl8_on_iommu_occupancy : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Disable throttle for miscblk initiated traffic
                               on channel 8 requiring virtualization based on
                               IOMMU PWT occupancy exceeding threshold (SatIEH
                               initiated MSIs, VMD initiated MSIs)
                            */
    UINT32 force_itcch8_quiesce_done : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Force quiesce done indication for ITC chnl8 for
                               StopReq4Misc state for quiesce lock
                            */
    UINT32 force_itcch9_quiesce_done : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Force quiesce done indication for ITC chnl9 for
                               StopReq4Misc state for quiesce lock
                            */
    UINT32 force_itcch10_quiesce_done : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Force quiesce done indication for ITC chnl10 for
                               StopReq4Misc state for quiesce lock
                            */
    UINT32 disable_jsm_lock_throttle : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Disable JSM Lock Throttle[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT32 me_eoibroad_en : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               ME EOI Broadcast Enable[br] Notes:[br] - Locked
                               by DBGBUSLCK
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 csi1_present : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               CSI1 Present[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT32 msi_pend_cpl_disable : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               MSI Pending Completion Disable[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT32 convert_ob_ca_to_ma : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Convert Outbound Completer Abort to Master
                               Abort[br] Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 convert_ib_ca_to_ma : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Convert Inbound Completer Abort to Master
                               Abort[br] Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 disable_vmd_rx_mailbox : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /* Disable Rx Mailbox for BMC->VMD communication */
    UINT32 disable_vmd_tx_mailbox : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /* Disable Tx Mailbox for VMD->BMC communication */
    UINT32 disable_vdm_mctp : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Disable Encapsulated MCTP VDM handling in OOBMSM
                               proxy sub-agent within miscblk
                            */
    UINT32 ignore_vmd_vdm_parerr : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Ignore data parity error from otc2misc within
                               OOBMSM sub-agent in miscblk
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lock_oldmode : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Lock Old Mode[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT32 lock_target_np : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Lock Target NP[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 suppress_mem_rd_lock : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Suppress Mem Read Lock[br] Notes:[br] - Locked
                               by DBGBUSLCK
                            */
    UINT32 rsvd_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 csr_dfx_dis_vphold_ignore_all : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Disable VPHold Ignore All[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSWCTL0_IIO_DFX_VTD_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 block_multiple_err_for_same_ent : 1;

                            /* Bits[2:2], Access Type=RW/O/P, default=0x00000000*/

                            /* Block Multiple Err For Same Ent */
    UINT32 me_disable : 1;

                            /* Bits[3:3], Access Type=RW/O/P, default=0x00000000*/

                            /* Disable ME region */
    UINT32 allow_ib_mmio_cfg : 1;

                            /* Bits[4:4], Access Type=RW/O/P, default=0x00000000*/

                            /* Allow Inbound MMIO Config */
    UINT32 ignore_acs_p2p_ma_lpbk : 1;

                            /* Bits[5:5], Access Type=RW/O, default=0x00000000*/

                            /* Ignore ACS p2p Master Abort Loopback */
    UINT32 shutdown_intack_cpl_abrt_cfg : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Shutdown INTACK Completion Abort Config[br]
                               Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 csr_vlw_throttle_cpl_disable : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               VLW Throttle Completion Disable[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT32 throttle_chnl8_on_iommu_colorstall : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Throttle miscblk initiated traffic on channel 8
                               requiring virtualization based on color stall
                               (SatIEH initiated MSIs, VMD initiated MSIs)
                            */
    UINT32 disable_throttle_chnl8_on_iommu_occupancy : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Disable throttle for miscblk initiated traffic
                               on channel 8 requiring virtualization based on
                               IOMMU PWT occupancy exceeding threshold (SatIEH
                               initiated MSIs, VMD initiated MSIs)
                            */
    UINT32 rsvd_10 : 3;

                            /* Bits[12:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 disable_jsm_lock_throttle : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Disable JSM Lock Throttle[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT32 me_eoibroad_en : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               ME EOI Broadcast Enable[br] Notes:[br] - Locked
                               by DBGBUSLCK
                            */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 csi1_present : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               CSI1 Present[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT32 msi_pend_cpl_disable : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               MSI Pending Completion Disable[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT32 convert_ob_ca_to_ma : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Convert Outbound Completer Abort to Master
                               Abort[br] Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 convert_ib_ca_to_ma : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Convert Inbound Completer Abort to Master
                               Abort[br] Notes:[br] - Locked by DBGBUSLCK
                            */
    UINT32 disable_vmd_rx_mailbox : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /* Disable Rx Mailbox for BMC->VMD communication */
    UINT32 disable_vmd_tx_mailbox : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /* Disable Tx Mailbox for VMD->BMC communication */
    UINT32 disable_vdm_mctp : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Disable Encapsulated MCTP VDM handling in OOBMSM
                               proxy sub-agent within miscblk
                            */
    UINT32 ignore_vmd_vdm_parerr : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Ignore data parity error from otc2misc within
                               OOBMSM sub-agent in miscblk
                            */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lock_oldmode : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Lock Old Mode[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT32 lock_target_np : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Lock Target NP[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT32 rsvd_27 : 1;

                            /* Bits[27:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 suppress_mem_rd_lock : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Suppress Mem Read Lock[br] Notes:[br] - Locked
                               by DBGBUSLCK
                            */
    UINT32 rsvd_29 : 1;

                            /* Bits[29:29], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 csr_dfx_dis_vphold_ignore_all : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Disable VPHold Ignore All[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSWCTL0_IIO_DFX_VTD_STRUCT;

/* IRP_DFX_NCS_NCB_MISC_IIO_DFX_VTD_REG supported on:                           */
/*      SPRA0 (0x20803310)                                                      */
/*      SPRB0 (0x20803310)                                                      */
/*      SPRHBM (0x20803310)                                                     */
/*      SPRC0 (0x20803310)                                                      */
/*      SPRMCC (0x20803310)                                                     */
/*      SPRUCC (0x20803310)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFC                                  */
/* Register default value on SPRB0: 0xFFFFFFFC                                  */
/* Register default value on SPRHBM: 0xFFFFFFFC                                 */
/* Register default value on SPRC0: 0xFFFFFFFC                                  */
/* Register default value on SPRMCC: 0xFFFFFFFC                                 */
/* Register default value on SPRUCC: 0xFFFFFFFC                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* IRP DFx NCS NCB Miscellaneous Control
*/


#define IRP_DFX_NCS_NCB_MISC_IIO_DFX_VTD_REG 0x12420310

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ncs_entry_enable : 6;

                            /* Bits[7:2], Access Type=RW, default=0x0000003F*/

                            /*
                               Entry 0 is pre-allocated after reset and will be
                               in use. Entry enable will only take effect after
                               first NCS has completed.
                            */
    UINT32 ncb_entry_enable : 24;

                            /* Bits[31:8], Access Type=RW, default=0x00FFFFFF*/

                            /*
                               Entry 0 is pre-allocated after reset and will be
                               in use. Entry enable will only take effect after
                               first NCB has completed.
                            */

  } Bits;
  UINT32 Data;

} IRP_DFX_NCS_NCB_MISC_IIO_DFX_VTD_STRUCT;

/* IRP_MISC_DFX2_IIO_DFX_VTD_REG supported on:                                  */
/*      SPRA0 (0x20803318)                                                      */
/*      SPRB0 (0x20803318)                                                      */
/*      SPRHBM (0x20803318)                                                     */
/*      SPRC0 (0x20803318)                                                      */
/*      SPRMCC (0x20803318)                                                     */
/*      SPRUCC (0x20803318)                                                     */
/* Register default value on SPRA0: 0xE019A278                                  */
/* Register default value on SPRB0: 0xE019A278                                  */
/* Register default value on SPRHBM: 0xE019A278                                 */
/* Register default value on SPRC0: 0xE019A278                                  */
/* Register default value on SPRMCC: 0xE019A278                                 */
/* Register default value on SPRUCC: 0xE019A278                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* DFX register 2 for IRP/IRPRING
*/


#define IRP_MISC_DFX2_IIO_DFX_VTD_REG 0x12420318

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 3;

                            /* Bits[2:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 p_pf_timeout_cnt : 5;

                            /* Bits[7:3], Access Type=RW, default=0x0000000F*/

                            /*
                               Controls the amount of time a posted prefetch to
                               coherent memory with a secondary will wait for
                               its fetch before giving up and passing ownership
                               to its secondary.[br] Encodings:[br] 0:
                               disabled[br] 1: something bigger than 0[br] 2:
                               something bigger than 1[br] etc. A value of 4hf
                               corresponds to a timeout between 6ms and 8ms.
                               This is the maximum possible.
                            */
    UINT32 rsvd_8 : 5;

                            /* Bits[12:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 enable_vc1 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/
    UINT32 rsvd_14 : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRP_MISC_DFX2_IIO_DFX_VTD_STRUCT;


/* IRPP0PAREINJCTL_IIO_DFX_VTD_REG supported on:                                */
/*      SPRA0 (0x20803320)                                                      */
/*      SPRB0 (0x20803320)                                                      */
/*      SPRHBM (0x20803320)                                                     */
/*      SPRC0 (0x20803320)                                                      */
/*      SPRMCC (0x20803320)                                                     */
/*      SPRUCC (0x20803320)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register contains the error injection mask register to determine which parity bits get corrupted for error detection testing.
*/


#define IRPP0PAREINJCTL_IIO_DFX_VTD_REG 0x12420320

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pf : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Protection Flip bits[br] This mask is used to
                               invert the corresponding parity bit assigned to
                               the byte, word, or dword of the data. Refer to
                               Table 14-67 for details.
                            */
    UINT32 hcnksel : 2;

                            /* Bits[17:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               This bit field selects which half-chunk to apply
                               the PF[15:0] bit field.[br] 00: chunk 0 (Bytes
                               15:00 of data) PF[3:0][br] 01: chunk 0 (Bytes
                               31:16 of data) PF[7:4][br] 10: chunk 1 (Bytes
                               15:00 of data) PF[3:0][br] 11: chunk 1 (Bytes
                               31:16 of data) PF[7:4][br] Half Chunk Select
                               allows partiy error injection on a 16 byte
                               granularity. Parity injection is done by
                               selecting the appropriate parity bits that
                               correspond to the PF field. When chunk1 is
                               selected, parity injection will take place on
                               the second pump of data. When selecting CQID
                               parity injection, this field is not used.
                            */
    UINT32 rsvd : 5;

                            /* Bits[22:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fpb : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /* NOT IMPLEMENTED. DO NOT USE. */
    UINT32 bps : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Buffer-Path select:[br] 000: Reserved[br] 001:
                               CQID parity[br] 010: 8b end-to-end parity for
                               rxdata0/rxdata1[br] 011: Inbound SDR parity[br]
                               100-111: Reserved
                            */
    UINT32 rsvd_27 : 2;

                            /* Bits[28:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 errinjst : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error injected status[br] 0: No error was
                               injected[br] 1: An error was injected
                            */
    UINT32 eirfsel : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               Error Injection Function Select[br] 0: Select
                               EINJ0 response function.[br] 1: Select EINJ1
                               response function.
                            */
    UINT32 eien : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               Error injection enable[br] 0: Disable error
                               injection[br] 1: Enable error injection
                            */

  } Bits;
  UINT32 Data;

} IRPP0PAREINJCTL_IIO_DFX_VTD_STRUCT;

/* IRPP1PAREINJCTL_IIO_DFX_VTD_REG supported on:                                */
/*      SPRA0 (0x20803324)                                                      */
/*      SPRB0 (0x20803324)                                                      */
/*      SPRHBM (0x20803324)                                                     */
/*      SPRC0 (0x20803324)                                                      */
/*      SPRMCC (0x20803324)                                                     */
/*      SPRUCC (0x20803324)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register contains the error injection mask register to determine which parity bits get corrupted for error detection testing.CB DMA Parity Error Usage Example
*/


#define IRPP1PAREINJCTL_IIO_DFX_VTD_REG 0x12420324

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pf : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Protection Flip bits[br] This mask is used to
                               invert the corresponding parity bit assigned to
                               the byte, word, or dword of the data. Refer to
                               Table 14-67 for details.
                            */
    UINT32 hcnksel : 2;

                            /* Bits[17:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Half Chunk Select[br] This bit field selects
                               which chunk to apply the PF[15:0] bit field.[br]
                               00: chunk 0 (Bytes 15:00 of data)[br] 01: chunk
                               1 (Bytes 31:16 of data)[br] When buffer path
                               select indicates Inbound Pkt RF FIFO then this
                               field set to 00 would select data bits 0 to 127
                               for parity error injection, and this field set
                               to 01 would select data bits 128 to 256 for
                               parity error injection.
                            */
    UINT32 rsvd : 5;

                            /* Bits[22:18], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fpb : 1;

                            /* Bits[23:23], Access Type=RO, default=0x00000000*/

                            /* Flip Poison Bit */
    UINT32 bps : 3;

                            /* Bits[26:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Buffer-Path select:[br] 000-010: Reserved[br]
                               011: Inbound Pkt RF FIFO[br] 100-111: Reserved
                            */
    UINT32 rsvd_27 : 2;

                            /* Bits[28:27], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 errinjst : 1;

                            /* Bits[29:29], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Error injected status[br] 0: No error was
                               injected[br] 1: An error was injected
                            */
    UINT32 eirfsel : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               Error Injection Function Select[br] 0: Select
                               EINJ0 response function.[br] 1: Select EINJ1
                               response function.[br] (Must enable the IIO
                               global error injection modules to generate the
                               EINJ0 and EINJ1 response functions to feed into
                               the IRP)
                            */
    UINT32 eien : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               Error injection enable[br] 0: Disable error
                               injection[br] 1: Enable error injection
                            */

  } Bits;
  UINT32 Data;

} IRPP1PAREINJCTL_IIO_DFX_VTD_STRUCT;


/* IIRPTHR0_IIO_DFX_VTD_REG supported on:                                       */
/*      SPRA0 (0x20803330)                                                      */
/*      SPRB0 (0x20803330)                                                      */
/*      SPRHBM (0x20803330)                                                     */
/*      SPRC0 (0x20803330)                                                      */
/*      SPRMCC (0x20803330)                                                     */
/*      SPRUCC (0x20803330)                                                     */
/* Register default value on SPRA0: 0x0FFFCACA                                  */
/* Register default value on SPRB0: 0x0FFFCACA                                  */
/* Register default value on SPRHBM: 0x0FFFCACA                                 */
/* Register default value on SPRC0: 0x0FFFCACA                                  */
/* Register default value on SPRMCC: 0x0FFFCACA                                 */
/* Register default value on SPRUCC: 0x0FFFCACA                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* The register is used for response logic to throttle AD and also to trigger aging timer timeouts, if the entries are in the correct state for that.
*/


#define IIRPTHR0_IIO_DFX_VTD_REG 0x12420330

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_sel : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Start Select[br] Selects the source which causes
                               the activation of the Inbound TXQ AD queue
                               throttling.[br] 0000: Global Event 0 (GE[0])[br]
                               0001: Global Event 1 (GE[1])[br] 0010: Global
                               Event 2 (GE[2])[br] 0011: Global Event 3
                               (GE[3])[br] 0100: Artificial Starvation Counter
                               0 (ASC[0])[br] 0101: Artificial Starvation
                               Counter 1 (ASC[1])[br] 0110: Artificial
                               Starvation Counter 2 (ASC[2])[br] 0111:
                               Artificial Starvation Counter 3 (ASC[3])[br]
                               1000: Global Event 4 (GE[4])[br] 1001: Global
                               Event 5 (GE[5])[br] 1010: Response never
                               occurs[br] 1011: Response occurs immediately[br]
                               1100-1111: Reserved
                            */
    UINT32 stop_sel : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Stop Select[br] Selects the source which causes
                               the deactivation of the Inbound TXQ AD queue
                               throttling.[br] 0000: Global Event 0 (GE[0])[br]
                               0001: Global Event 1 (GE[1])[br] 0010: Global
                               Event 2 (GE[2])[br] 0011: Global Event 3
                               (GE[3])[br] 0100: Artificial Starvation Counter
                               0 (ASC[0])[br] 0101: Artificial Starvation
                               Counter 1 (ASC[1])[br] 0110: Artificial
                               Starvation Counter 2 (ASC[2])[br] 0111:
                               Artificial Starvation Counter 3 (ASC[3])[br]
                               1000: Global Event 4 (GE[4])[br] 1001: Global
                               Event 5 (GE[5])[br] 1010: Deactivate one clock
                               after activation. Error injection on one
                               transaction will occur on the interface that is
                               enabled.[br] 1011: Response is never
                               deactivated[br] 1100: Deactivate the response
                               when the activating signal becomes false.[br]
                               1101-1111: Reserved
                            */
    UINT32 agt_start_sel : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Selects the source which causes the activation
                               of the response in the cluster.[br] 0000: Global
                               Event 0 (GE[0])[br] 0001: Global Event 1
                               (GE[1])[br] 0010: Global Event 2 (GE[2])[br]
                               0011: Global Event 3 (GE[3])[br] 0100:
                               Artificial Starvation Counter 0 (ASC[0])[br]
                               0101: Artificial Starvation Counter 1
                               (ASC[1])[br] 0110: Artificial Starvation Counter
                               2 (ASC[2])[br] 0111: Artificial Starvation
                               Counter 3 (ASC[3])[br] 1000: Global Event 4
                               (GE[4])[br] 1001: Global Event 5 (GE[5])[br]
                               1010: Response never occurs[br] 1011: Response
                               occurs immediately[br] 1100-1111: Reserved
                            */
    UINT32 agt_stop_sel : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Selects the source which causes the deactivation
                               of the response in the cluster.[br] 0000: Global
                               Event 0 (GE[0])[br] 0001: Global Event 1
                               (GE[1])[br] 0010: Global Event 2 (GE[2])[br]
                               0011: Global Event 3 (GE[3])[br] 0100:
                               Artificial Starvation Counter 0 (ASC[0])[br]
                               0101: Artificial Starvation Counter 1
                               (ASC[1])[br] 0110: Artificial Starvation Counter
                               2 (ASC[2])[br] 0111: Artificial Starvation
                               Counter 3 (ASC[3])[br] 1000: Global Event 4
                               (GE[4])[br] 1001: Global Event 5 (GE[5])[br]
                               1010: Deactivate one clock after activation.
                               Error injection on one transaction will occur on
                               the interface that is enabled.[br] 1011:
                               Response is never deactivated[br] 1100:
                               Deactivate the response when the activating
                               signal becomes false.[br] 1101-1111: Reserved
                            */
    UINT32 aging_timeout_trigger_cset_enables : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x0000000F*/

                            /*
                               Decides which cset is enabled for the aging
                               timeout trigger. [br] Bit 0 is for cset 0 and
                               bit 1 is for cset 1.
                            */
    UINT32 aging_timeout_trigger_entry_enable : 8;

                            /* Bits[27:20], Access Type=RW/P, default=0x000000FF*/

                            /*
                               Decides which group of ctag entries are enabled
                               for the aging timeout trigger. [br] Bit 0 is for
                               ctw 0, bit 1 is for ctw 1 etc.
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IIRPTHR0_IIO_DFX_VTD_STRUCT;

/* IIRPTHR1_IIO_DFX_VTD_REG supported on:                                       */
/*      SPRA0 (0x20803334)                                                      */
/*      SPRB0 (0x20803334)                                                      */
/*      SPRHBM (0x20803334)                                                     */
/*      SPRC0 (0x20803334)                                                      */
/*      SPRMCC (0x20803334)                                                     */
/*      SPRUCC (0x20803334)                                                     */
/* Register default value on SPRA0: 0x3280CACA                                  */
/* Register default value on SPRB0: 0x3280CACA                                  */
/* Register default value on SPRHBM: 0x3280CACA                                 */
/* Register default value on SPRC0: 0x3280CACA                                  */
/* Register default value on SPRMCC: 0x3280CACA                                 */
/* Register default value on SPRUCC: 0x3280CACA                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* IRP1 Throttle Control
*/


#define IIRPTHR1_IIO_DFX_VTD_REG 0x12420334

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_sel : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Selects the source which causes the activation
                               of the Inbound TXQ BL queue throttling.[br]
                               0000: Global Event 0 (GE[0])[br] 0001: Global
                               Event 1 (GE[1])[br] 0010: Global Event 2
                               (GE[2])[br] 0011: Global Event 3 (GE[3])[br]
                               0100: Artificial Starvation Counter 0
                               (ASC[0])[br] 0101: Artificial Starvation Counter
                               1 (ASC[1])[br] 0110: Artificial Starvation
                               Counter 2 (ASC[2])[br] 0111: Artificial
                               Starvation Counter 3 (ASC[3])[br] 1000: Global
                               Event 4 (GE[4])[br] 1001: Global Event 5
                               (GE[5])[br] 1010: Response never occurs[br]
                               1011: Response occurs immediately[br] 1100-1111:
                               Reserved
                            */
    UINT32 stop_sel : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Selects the source which causes the deactivation
                               of the Inbound TXQ BL queue throttling.[br]
                               0000: Global Event 0 (GE[0])[br] 0001: Global
                               Event 1 (GE[1])[br] 0010: Global Event 2
                               (GE[2])[br] 0011: Global Event 3 (GE[3])[br]
                               0100: Artificial Starvation Counter 0
                               (ASC[0])[br] 0101: Artificial Starvation Counter
                               1 (ASC[1])[br] 0110: Artificial Starvation
                               Counter 2 (ASC[2])[br] 0111: Artificial
                               Starvation Counter 3 (ASC[3])[br] 1000: Global
                               Event 4 (GE[4])[br] 1001: Global Event 5
                               (GE[5])[br] 1010: Deactivate one clock after
                               activation. Error injection on one transaction
                               will occur on the interface that is enabled.[br]
                               1011: Response is never deactivated[br] 1100:
                               Deactivate the response when the activating
                               signal becomes false.[br] 1101-1111: Reserved
                            */
    UINT32 iirpcdt_start_sel : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Selects the source which causes the activation
                               of the response in the cluster.[br] 0000: Global
                               Event 0 (GE[0])[br] 0001: Global Event 1
                               (GE[1])[br] 0010: Global Event 2 (GE[2])[br]
                               0011: Global Event 3 (GE[3])[br] 0100:
                               Artificial Starvation Counter 0 (ASC[0])[br]
                               0101: Artificial Starvation Counter 1
                               (ASC[1])[br] 0110: Artificial Starvation Counter
                               2 (ASC[2])[br] 0111: Artificial Starvation
                               Counter 3 (ASC[3])[br] 1000: Global Event 4
                               (GE[4])[br] 1001: Global Event 5 (GE[5])[br]
                               1010: Response never occurs[br] 1011: Response
                               occurs immediately[br] 1100-1111: Reserved
                            */
    UINT32 iirpcdt_stop_sel : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Selects the source which causes the deactivation
                               of the response in the cluster.[br] 0000: Global
                               Event 0 (GE[0])[br] 0001: Global Event 1
                               (GE[1])[br] 0010: Global Event 2 (GE[2])[br]
                               0011: Global Event 3 (GE[3])[br] 0100:
                               Artificial Starvation Counter 0 (ASC[0])[br]
                               0101: Artificial Starvation Counter 1
                               (ASC[1])[br] 0110: Artificial Starvation Counter
                               2 (ASC[2])[br] 0111: Artificial Starvation
                               Counter 3 (ASC[3])[br] 1000: Global Event 4
                               (GE[4])[br] 1001: Global Event 5 (GE[5])[br]
                               1010: Deactivate one clock after activation.
                               Error injection on one transaction will occur on
                               the interface that is enabled.[br] 1011:
                               Response is never deactivated[br] 1100:
                               Deactivate the response when the activating
                               signal becomes false.[br] 1101-1111: Reserved
                            */
    UINT32 hdr_cdt_throttle : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enables credit return throttling for CSET header
                               credits using the iirpcdt ASC
                            */
    UINT32 data_cdt_throttle : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enables credit return throttling for CSET Data
                               credits using the iirpcdt ASC
                            */
    UINT32 p2p_cdt_throttle : 2;

                            /* Bits[19:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enables credit return throttling for P2P credits
                               using the iirpcdt ASC. Bit 0 is for NCP2PS and
                               bit 1 is for NCP2PB
                            */
    UINT32 faf_cdt_throttle : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enables credit return throttling for FAF header
                               credits using the iirpcdt ASC
                            */
    UINT32 cpl_hdr_cdt_throttle : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enables credit return throttling for CPL header
                               credits using the iirpcdt ASC
                            */
    UINT32 ak_start_sel : 4;

                            /* Bits[25:22], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Selects the source which causes the activation
                               of the Inbound TXQ AK queue throttling.[br]
                               0000: Global Event 0 (GE[0])[br] 0001: Global
                               Event 1 (GE[1])[br] 0010: Global Event 2
                               (GE[2])[br] 0011: Global Event 3 (GE[3])[br]
                               0100: Artificial Starvation Counter 0
                               (ASC[0])[br] 0101: Artificial Starvation Counter
                               1 (ASC[1])[br] 0110: Artificial Starvation
                               Counter 2 (ASC[2])[br] 0111: Artificial
                               Starvation Counter 3 (ASC[3])[br] 1000: Global
                               Event 4 (GE[4])[br] 1001: Global Event 5
                               (GE[5])[br] 1010: Response never occurs[br]
                               1011: Response occurs immediately[br] 1100-1111:
                               Reserved
                            */
    UINT32 ak_stop_sel : 4;

                            /* Bits[29:26], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Selects the source which causes the deactivation
                               of the Inbound TXQ AK queue throttling.[br]
                               0000: Global Event 0 (GE[0])[br] 0001: Global
                               Event 1 (GE[1])[br] 0010: Global Event 2
                               (GE[2])[br] 0011: Global Event 3 (GE[3])[br]
                               0100: Artificial Starvation Counter 0
                               (ASC[0])[br] 0101: Artificial Starvation Counter
                               1 (ASC[1])[br] 0110: Artificial Starvation
                               Counter 2 (ASC[2])[br] 0111: Artificial
                               Starvation Counter 3 (ASC[3])[br] 1000: Global
                               Event 4 (GE[4])[br] 1001: Global Event 5
                               (GE[5])[br] 1010: Deactivate one clock after
                               activation. Error injection on one transaction
                               will occur on the interface that is enabled.[br]
                               1011: Response is never deactivated[br] 1100:
                               Deactivate the response when the activating
                               signal becomes false.[br] 1101-1111: Reserved
                            */
    UINT32 vfaf_cdt_throttle : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enables credit return throttling for FAF header
                               credits using the iirpcdt ASC
                            */
    UINT32 rsvd : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IIRPTHR1_IIO_DFX_VTD_STRUCT;

/* IIRPRTHRPARITY_IIO_DFX_VTD_REG supported on:                                 */
/*      SPRA0 (0x20803338)                                                      */
/*      SPRB0 (0x20803338)                                                      */
/*      SPRHBM (0x20803338)                                                     */
/*      SPRC0 (0x20803338)                                                      */
/*      SPRMCC (0x20803338)                                                     */
/*      SPRUCC (0x20803338)                                                     */
/* Register default value on SPRA0: 0x000CAFCA                                  */
/* Register default value on SPRB0: 0x000CAFCA                                  */
/* Register default value on SPRHBM: 0x000CAFCA                                 */
/* Register default value on SPRC0: 0x000CAFCA                                  */
/* Register default value on SPRMCC: 0x000CAFCA                                 */
/* Register default value on SPRUCC: 0x000CAFCA                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* IIRP Ring Throttle Parity Control
*/


#define IIRPRTHRPARITY_IIO_DFX_VTD_REG 0x12420338

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_sel : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Selects the source which causes the activation
                               of the response in the cluster.[br] 0000: Global
                               Event 0 (GE[0])[br] 0001: Global Event 1
                               (GE[1])[br] 0010: Global Event 2 (GE[2])[br]
                               0011: Global Event 3 (GE[3])[br] 0100:
                               Artificial Starvation Counter 0 (ASC[0])[br]
                               0101: Artificial Starvation Counter 1
                               (ASC[1])[br] 0110: Artificial Starvation Counter
                               2 (ASC[2])[br] 0111: Artificial Starvation
                               Counter 3 (ASC[3])[br] 1000: Global Event 4
                               (GE[4])[br] 1001: Global Event 5 (GE[5])[br]
                               1010: Response never occurs[br] 1011: Response
                               occurs immediately[br] 1100-1111: Reserved
                            */
    UINT32 stop_sel : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Selects the source which causes the deactivation
                               of the response in the cluster.[br] 0000: Global
                               Event 0 (GE[0])[br] 0001: Global Event 1
                               (GE[1])[br] 0010: Global Event 2 (GE[2])[br]
                               0011: Global Event 3 (GE[3])[br] 0100:
                               Artificial Starvation Counter 0 (ASC[0])[br]
                               0101: Artificial Starvation Counter 1
                               (ASC[1])[br] 0110: Artificial Starvation Counter
                               2 (ASC[2])[br] 0111: Artificial Starvation
                               Counter 3 (ASC[3])[br] 1000: Global Event 4
                               (GE[4])[br] 1001: Global Event 5 (GE[5])[br]
                               1010: Deactivate one clock after activation.
                               Error injection on one transaction will occur on
                               the interface that is enabled.[br] 1011:
                               Response is never deactivated[br] 1100:
                               Deactivate the response when the activating
                               signal becomes false.[br] 1101-1111: Reserved
                            */
    UINT32 bit_sel : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x0000000F*/

                            /*
                               Select which bits to inject parity on. Default
                               is all 4.
                            */
    UINT32 sdr_start_sel : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Selects the source which causes the activation
                               of the response in the cluster.[br] 0000: Global
                               Event 0 (GE[0])[br] 0001: Global Event 1
                               (GE[1])[br] 0010: Global Event 2 (GE[2])[br]
                               0011: Global Event 3 (GE[3])[br] 0100:
                               Artificial Starvation Counter 0 (ASC[0])[br]
                               0101: Artificial Starvation Counter 1
                               (ASC[1])[br] 0110: Artificial Starvation Counter
                               2 (ASC[2])[br] 0111: Artificial Starvation
                               Counter 3 (ASC[3])[br] 1000: Global Event 4
                               (GE[4])[br] 1001: Global Event 5 (GE[5])[br]
                               1010: Response never occurs[br] 1011: Response
                               occurs immediately[br] 1100-1111: Reserved
                            */
    UINT32 sdr_stop_sel : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x0000000C*/

                            /*
                               Selects the source which causes the deactivation
                               of the response in the cluster.[br] 0000: Global
                               Event 0 (GE[0])[br] 0001: Global Event 1
                               (GE[1])[br] 0010: Global Event 2 (GE[2])[br]
                               0011: Global Event 3 (GE[3])[br] 0100:
                               Artificial Starvation Counter 0 (ASC[0])[br]
                               0101: Artificial Starvation Counter 1
                               (ASC[1])[br] 0110: Artificial Starvation Counter
                               2 (ASC[2])[br] 0111: Artificial Starvation
                               Counter 3 (ASC[3])[br] 1000: Global Event 4
                               (GE[4])[br] 1001: Global Event 5 (GE[5])[br]
                               1010: Deactivate one clock after activation.
                               Error injection on one transaction will occur on
                               the interface that is enabled.[br] 1011:
                               Response is never deactivated[br] 1100:
                               Deactivate the response when the activating
                               signal becomes false.[br] 1101-1111: Reserved
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IIRPRTHRPARITY_IIO_DFX_VTD_STRUCT;

/* IRP_MISC_DFX4_IIO_DFX_VTD_REG supported on:                                  */
/*      SPRA0 (0x2080333c)                                                      */
/*      SPRB0 (0x2080333c)                                                      */
/*      SPRHBM (0x2080333c)                                                     */
/*      SPRC0 (0x2080333c)                                                      */
/*      SPRMCC (0x2080333c)                                                     */
/*      SPRUCC (0x2080333c)                                                     */
/* Register default value on SPRA0: 0x4410FFFF                                  */
/* Register default value on SPRB0: 0x8420FFFF                                  */
/* Register default value on SPRHBM: 0x8420FFFF                                 */
/* Register default value on SPRC0: 0x8420FFFF                                  */
/* Register default value on SPRMCC: 0x8420FFFF                                 */
/* Register default value on SPRUCC: 0x8420FFFF                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* DFX register 4 for IRP/IRPRING
*/


#define IRP_MISC_DFX4_IIO_DFX_VTD_REG 0x1242033C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 spec_i2m_dis : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Used to disable SpecI2M opcode. The I2M opcode
                               will be used instead
                            */
    UINT32 rsvd_17 : 15;

                            /* Bits[31:17], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRP_MISC_DFX4_IIO_DFX_VTD_STRUCT;






/* CSIPISOCRES_IIO_DFX_VTD_REG supported on:                                    */
/*      SPRA0 (0x20803388)                                                      */
/*      SPRB0 (0x20803388)                                                      */
/*      SPRHBM (0x20803388)                                                     */
/*      SPRC0 (0x20803388)                                                      */
/*      SPRMCC (0x20803388)                                                     */
/*      SPRUCC (0x20803388)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* CSIP Isoc Res
*/


#define CSIPISOCRES_IIO_DFX_VTD_REG 0x12420388

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 isoc_enabled : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /* Notes:[br] - Locked by DBGBUSLCK */
    UINT32 merge_vc0_cnt : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               If set, revert to legacy behavior of comparing
                               max_cache_vc0 to total cache usage instead of
                               VC0 cache usage.
                            */
    UINT32 rsvd_18 : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CSIPISOCRES_IIO_DFX_VTD_STRUCT;


/* LOCK_CTRL_IIO_DFX_VTD_REG supported on:                                      */
/*      SPRA0 (0x108033c0)                                                      */
/*      SPRB0 (0x108033c0)                                                      */
/*      SPRHBM (0x108033c0)                                                     */
/*      SPRC0 (0x108033c0)                                                      */
/*      SPRMCC (0x108033c0)                                                     */
/*      SPRUCC (0x108033c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ITC/OTC lock flow defeature bits
*/


#define LOCK_CTRL_IIO_DFX_VTD_REG 0x124103C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT16 allow_lock_vtd_vc0p : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Allow Lock VTd VC0p[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT16 allow_lock_vtd_vc1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Allow Lock VTd VC1[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT16 quiesce_done_dmisrc : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Quiesce Done DMI Source[br] Notes:[br] - Locked
                               by DBGBUSLCK
                            */
    UINT16 quiesce_done_misc_np : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Quiesce Done for OTC MISC NP[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT16 quiesce_done_misc_p : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Quiesce Done for OTC MISC P[br] Notes:[br] -
                               Locked by DBGBUSLCK
                            */
    UINT16 allow_msi : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Allow MSI[br] Notes:[br] - Locked by DBGBUSLCK */
    UINT16 set_msi_quiesce : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Set MSI Quiesce[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT16 rsvd : 9;

                            /* Bits[15:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT16 Data;

} LOCK_CTRL_IIO_DFX_VTD_STRUCT;

/* SWDBGCTL0_IIO_DFX_VTD_REG supported on:                                      */
/*      SPRA0 (0x208033c4)                                                      */
/*      SPRB0 (0x208033c4)                                                      */
/*      SPRHBM (0x208033c4)                                                     */
/*      SPRC0 (0x208033c4)                                                      */
/*      SPRMCC (0x208033c4)                                                     */
/*      SPRUCC (0x208033c4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Switch Debug Control 0
*/


#define SWDBGCTL0_IIO_DFX_VTD_REG 0x124203C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 12;

                            /* Bits[11:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 set_dmi_cfg_type0 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* Notes:[br] Locked by DBGBUSLCK */
    UINT32 rsvd_13 : 14;

                            /* Bits[26:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dfr_en_intremap : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Dfr En Intremap */
    UINT32 dis_jsm_squash : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /* Dis Jsm Squash */
    UINT32 dis_msi_ord_ecn152 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* Dis Msi Ord Ecn152 */
    UINT32 en_one_active_msi : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /* En One Active Msi */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SWDBGCTL0_IIO_DFX_VTD_STRUCT;

/* MISCBLK_ARB_CTRL_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208033d0)                                                      */
/*      SPRB0 (0x208033d0)                                                      */
/*      SPRHBM (0x208033d0)                                                     */
/*      SPRC0 (0x208033d0)                                                      */
/*      SPRMCC (0x208033d0)                                                     */
/*      SPRUCC (0x208033d0)                                                     */
/* Register default value on SPRA0: 0x0000018C                                  */
/* Register default value on SPRB0: 0x0000018C                                  */
/* Register default value on SPRHBM: 0x0000018C                                 */
/* Register default value on SPRC0: 0x0000018C                                  */
/* Register default value on SPRMCC: 0x0000018C                                 */
/* Register default value on SPRUCC: 0x0000018C                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Miscblk arbitration control
*/


#define MISCBLK_ARB_CTRL_IIO_DFX_VTD_REG 0x124203D0

#if defined(SPRA0_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc2misc_p_arb_block_counter : 5;

                            /* Bits[4:0], Access Type=RW, default=0x0000000C*/

                            /*
                               Block the OTC to Misc P arbitration (credit
                               avail) for these many cycles. [br] Allows us to
                               override the default value of 7 cycles which is
                               needed as the txn[br] passes through the
                               OTC->Misc shim[br] Notes: [br] Locked by
                               DBGBUSLCK
                            */
    UINT32 otc2misc_np_arb_block_counter : 5;

                            /* Bits[9:5], Access Type=RW, default=0x0000000C*/

                            /*
                               Block the OTC to Misc NP arbitration (credit
                               avail) for these many cycles. [br] Allows us to
                               override the default value of 7 cycles which is
                               needed as the[br] txn passes through the
                               OTC->Misc shim[br] Notes: [br] Locked by
                               DBGBUSLCK
                            */
    UINT32 gpsb_pcmsg_block_counter : 4;

                            /* Bits[13:10], Access Type=RW, default=0x00000000*/

                            /*
                               Block the arb for the posted messages that are
                               sent on GPSB for these many clocks[br] to allow
                               other OB transactions to make progress. This is
                               for the following messages - [br] PMON overflow,
                               Error messages and Intx/GPE [br] Notes: [br]
                               Locked by DBGBUSLCK
                            */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MISCBLK_ARB_CTRL_IIO_DFX_VTD_SPRA0_STRUCT;
#endif /* (SPRA0_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc2misc_p_arb_block_counter : 5;

                            /* Bits[4:0], Access Type=RW, default=0x0000000C*/

                            /*
                               Block the OTC to Misc P arbitration (credit
                               avail) for these many cycles. [br] Allows us to
                               override the default value of 7 cycles which is
                               needed as the txn[br] passes through the
                               OTC->Misc shim[br] Notes: [br] Locked by
                               DBGBUSLCK
                            */
    UINT32 otc2misc_np_arb_block_counter : 5;

                            /* Bits[9:5], Access Type=RW, default=0x0000000C*/

                            /*
                               Block the OTC to Misc NP arbitration (credit
                               avail) for these many cycles. [br] Allows us to
                               override the default value of 7 cycles which is
                               needed as the[br] txn passes through the
                               OTC->Misc shim[br] Notes: [br] Locked by
                               DBGBUSLCK
                            */
    UINT32 gpsb_pcmsg_block_counter : 4;

                            /* Bits[13:10], Access Type=RW, default=0x00000000*/

                            /*
                               Block the arb for the posted messages that are
                               sent on GPSB for these many clocks[br] to allow
                               other OB transactions to make progress. This is
                               for the following messages - [br] PMON overflow,
                               Error messages and Intx/GPE [br] Notes: [br]
                               Locked by DBGBUSLCK
                            */
    UINT32 csrdisprhdonemiscpthrottlesnnnh : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Disable PrhDone based qualification feeding into
                               the posted miscblock throttle indication
                            */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MISCBLK_ARB_CTRL_IIO_DFX_VTD_SPRB0_SPRHBM_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc2misc_p_arb_block_counter : 5;

                            /* Bits[4:0], Access Type=RW, default=0x0000000C*/

                            /*
                               Block the OTC to Misc P arbitration (credit
                               avail) for these many cycles. [br] Allows us to
                               override the default value of 7 cycles which is
                               needed as the txn[br] passes through the
                               OTC->Misc shim[br] Notes: [br] Locked by
                               DBGBUSLCK
                            */
    UINT32 otc2misc_np_arb_block_counter : 5;

                            /* Bits[9:5], Access Type=RW, default=0x0000000C*/

                            /*
                               Block the OTC to Misc NP arbitration (credit
                               avail) for these many cycles. [br] Allows us to
                               override the default value of 7 cycles which is
                               needed as the[br] txn passes through the
                               OTC->Misc shim[br] Notes: [br] Locked by
                               DBGBUSLCK
                            */
    UINT32 gpsb_pcmsg_block_counter : 4;

                            /* Bits[13:10], Access Type=RW, default=0x00000000*/

                            /*
                               Block the arb for the posted messages that are
                               sent on GPSB for these many clocks[br] to allow
                               other OB transactions to make progress. This is
                               for the following messages - [br] PMON overflow,
                               Error messages and Intx/GPE [br] Notes: [br]
                               Locked by DBGBUSLCK
                            */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MISCBLK_ARB_CTRL_IIO_DFX_VTD_STRUCT;

/* FAUXGV_IIO_DFX_VTD_REG supported on:                                         */
/*      SPRA0 (0x208033d8)                                                      */
/*      SPRB0 (0x208033d8)                                                      */
/*      SPRHBM (0x208033d8)                                                     */
/*      SPRC0 (0x208033d8)                                                      */
/*      SPRMCC (0x208033d8)                                                     */
/*      SPRUCC (0x208033d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Faux GV
*/


#define FAUXGV_IIO_DFX_VTD_REG 0x124203D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fauxgven : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Fauxgven */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} FAUXGV_IIO_DFX_VTD_STRUCT;

/* SPARE_SWNORTH_IIO_DFX_VTD_REG supported on:                                  */
/*      SPRA0 (0x208033e0)                                                      */
/*      SPRB0 (0x208033e0)                                                      */
/*      SPRHBM (0x208033e0)                                                     */
/*      SPRC0 (0x208033e0)                                                      */
/*      SPRMCC (0x208033e0)                                                     */
/*      SPRUCC (0x208033e0)                                                     */
/* Register default value on SPRA0: 0x00000240                                  */
/* Register default value on SPRB0: 0x00000240                                  */
/* Register default value on SPRHBM: 0x00000240                                 */
/* Register default value on SPRC0: 0x00000240                                  */
/* Register default value on SPRMCC: 0x00000240                                 */
/* Register default value on SPRUCC: 0x00000240                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Spare SWNorth
*/


#define SPARE_SWNORTH_IIO_DFX_VTD_REG 0x124203E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 knl_smpt_enable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Knl Smpt Enable */
    UINT32 rsvd_2 : 4;

                            /* Bits[5:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cpl_coalesce_type : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000001*/

                            /* Cpl Coalesce Type */
    UINT32 rsvd_8 : 1;

                            /* Bits[8:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 en_hsx_req_id_translation : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /* En Hsx Req Id Translation */
    UINT32 rsvd_10 : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SPARE_SWNORTH_IIO_DFX_VTD_STRUCT;

/* M2IOSF_SCRATCHPAD_IIO_DFX_VTD_REG supported on:                              */
/*      SPRA0 (0x208033fc)                                                      */
/*      SPRB0 (0x208033fc)                                                      */
/*      SPRHBM (0x208033fc)                                                     */
/*      SPRC0 (0x208033fc)                                                      */
/*      SPRMCC (0x208033fc)                                                     */
/*      SPRUCC (0x208033fc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Read-write bits used for validation and debug.
*/


#define M2IOSF_SCRATCHPAD_IIO_DFX_VTD_REG 0x124203FC

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

} M2IOSF_SCRATCHPAD_IIO_DFX_VTD_STRUCT;







/* ENABLE_B2B_OTC_VTDPIPE_0_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x20803418)                                                      */
/*      SPRB0 (0x20803418)                                                      */
/*      SPRHBM (0x20803418)                                                     */
/*      SPRC0 (0x20803418)                                                      */
/*      SPRMCC (0x20803418)                                                     */
/*      SPRUCC (0x20803418)                                                     */
/* Register default value on SPRA0: 0x00FFFFFF                                  */
/* Register default value on SPRB0: 0x00FFFFFF                                  */
/* Register default value on SPRHBM: 0x00FFFFFF                                 */
/* Register default value on SPRC0: 0x00FFFFFF                                  */
/* Register default value on SPRMCC: 0x00FFFFFF                                 */
/* Register default value on SPRUCC: 0x00FFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* enable b2b in otc vtd pipe
*/


#define ENABLE_B2B_OTC_VTDPIPE_0_IIO_DFX_VTD_REG 0x12420418

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 csrenablevtdpipeb2bsnnnh_chn0_p : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* enable b2b in otc vtd pipe */
    UINT32 csrenablevtdpipeb2bsnnnh_chn1_p : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /* enable b2b in otc vtd pipe */
    UINT32 csrenablevtdpipeb2bsnnnh_chn2_p : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /* enable b2b in otc vtd pipe */
    UINT32 csrenablevtdpipeb2bsnnnh_chn3_p : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /* enable b2b in otc vtd pipe */
    UINT32 csrenablevtdpipeb2bsnnnh_chn4_p : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /* enable b2b in otc vtd pipe */
    UINT32 csrenablevtdpipeb2bsnnnh_chn5_p : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /* enable b2b in otc vtd pipe */
    UINT32 csrenablevtdpipeb2bsnnnh_chn6_p : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /* enable b2b in otc vtd pipe */
    UINT32 csrenablevtdpipeb2bsnnnh_chn7_p : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* enable b2b in otc vtd pipe */
    UINT32 rsvd_8 : 16;

                            /* Bits[23:8], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ENABLE_B2B_OTC_VTDPIPE_0_IIO_DFX_VTD_STRUCT;

/* ENABLE_B2B_OTC_VTDPIPE_1_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x2080341c)                                                      */
/*      SPRB0 (0x2080341c)                                                      */
/*      SPRHBM (0x2080341c)                                                     */
/*      SPRC0 (0x2080341c)                                                      */
/*      SPRMCC (0x2080341c)                                                     */
/*      SPRUCC (0x2080341c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* enable b2b in otc vtd pipe
*/


#define ENABLE_B2B_OTC_VTDPIPE_1_IIO_DFX_VTD_REG 0x1242041C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 csrenablevtdpipeb2bsnnnh_chn8_p : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* enable b2b in otc vtd pipe */
    UINT32 csrenablevtdpipeb2bsnnnh_chn9_p : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* enable b2b in otc vtd pipe */
    UINT32 csrenablevtdpipeb2bsnnnh_chn10_p : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* enable b2b in otc vtd pipe */
    UINT32 csrenablevtdpipeb2bsnnnh_chn11_p : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* enable b2b in otc vtd pipe */
    UINT32 rsvd_4 : 8;

                            /* Bits[11:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ENABLE_B2B_OTC_VTDPIPE_1_IIO_DFX_VTD_STRUCT;

/* SW2IOSFARBBLOCK_IIO_DFX_VTD_REG supported on:                                */
/*      SPRA0 (0x208034d8)                                                      */
/*      SPRB0 (0x208034d8)                                                      */
/*      SPRHBM (0x208034d8)                                                     */
/*      SPRC0 (0x208034d8)                                                      */
/*      SPRMCC (0x208034d8)                                                     */
/*      SPRUCC (0x208034d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Block certain requestors from Arbing on IOSF
*/


#define SW2IOSFARBBLOCK_IIO_DFX_VTD_REG 0x124204D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disabled 1 - Always enabled 2 - Enable based
                               on response function 0 3 - Enable based on
                               response function 1 4 - Enable based on response
                               function 2
                            */
    UINT32 chnlmask : 12;

                            /* Bits[14:3], Access Type=RW, default=0x00000000*/

                            /* Channels to block */
    UINT32 fcmask : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /* FCs to block: [2] - C [1] - NP [0] - P */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SW2IOSFARBBLOCK_IIO_DFX_VTD_STRUCT;

/* SW2IOSFCRDTTHROTTLE_IIO_DFX_VTD_REG supported on:                            */
/*      SPRA0 (0x208034e0)                                                      */
/*      SPRB0 (0x208034e0)                                                      */
/*      SPRHBM (0x208034e0)                                                     */
/*      SPRC0 (0x208034e0)                                                      */
/*      SPRMCC (0x208034e0)                                                     */
/*      SPRUCC (0x208034e0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Throttle Credit Return from ITC into SW2IOSF
*/


#define SW2IOSFCRDTTHROTTLE_IIO_DFX_VTD_REG 0x124204E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cmd_enable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disabled 1 - Always enabled 2 - Enable based
                               on response function 0 3 - Enable based on
                               response function 1 4 - Enable based on response
                               function 2
                            */
    UINT32 data_enable : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disabled 1 - Always enabled 2 - Enable based
                               on response function 0 3 - Enable based on
                               response function 1 4 - Enable based on response
                               function 2
                            */
    UINT32 chnlmask : 12;

                            /* Bits[17:6], Access Type=RW, default=0x00000000*/

                            /* Channels to block */
    UINT32 fcmask : 3;

                            /* Bits[20:18], Access Type=RW, default=0x00000000*/

                            /* FCs to block: [2] - C [1] - NP [0] - P */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SW2IOSFCRDTTHROTTLE_IIO_DFX_VTD_STRUCT;

/* CSIPSMIC_0_IIO_DFX_VTD_REG supported on:                                     */
/*      SPRA0 (0x2080358c)                                                      */
/*      SPRB0 (0x2080358c)                                                      */
/*      SPRHBM (0x2080358c)                                                     */
/*      SPRC0 (0x2080358c)                                                      */
/*      SPRMCC (0x2080358c)                                                     */
/*      SPRUCC (0x2080358c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* CSI PSMIC 0
*/


#define CSIPSMIC_0_IIO_DFX_VTD_REG 0x1242058C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 interrupt_data_vector : 8;

                            /* Bits[23:16], Access Type=RW, default=0x00000000*/

                            /* Interrupt Data Vector */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CSIPSMIC_0_IIO_DFX_VTD_STRUCT;

/* CSIPSMIC_1_IIO_DFX_VTD_REG supported on:                                     */
/*      SPRA0 (0x20803590)                                                      */
/*      SPRB0 (0x20803590)                                                      */
/*      SPRHBM (0x20803590)                                                     */
/*      SPRC0 (0x20803590)                                                      */
/*      SPRMCC (0x20803590)                                                     */
/*      SPRUCC (0x20803590)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* CSI PSMIC 1
*/


#define CSIPSMIC_1_IIO_DFX_VTD_REG 0x12420590

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 destination_id : 32;

                            /* Bits[31:0], Access Type=RW, default=0xFFFFFFFF*/

                            /* Destination Id */

  } Bits;
  UINT32 Data;

} CSIPSMIC_1_IIO_DFX_VTD_STRUCT;

/* CSIPNMIC_1_IIO_DFX_VTD_REG supported on:                                     */
/*      SPRA0 (0x20803594)                                                      */
/*      SPRB0 (0x20803594)                                                      */
/*      SPRHBM (0x20803594)                                                     */
/*      SPRC0 (0x20803594)                                                      */
/*      SPRMCC (0x20803594)                                                     */
/*      SPRUCC (0x20803594)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* CSI PNMIC
*/


#define CSIPNMIC_1_IIO_DFX_VTD_REG 0x12420594

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 destination_id : 32;

                            /* Bits[31:0], Access Type=RW, default=0xFFFFFFFF*/

                            /* Destination Id */

  } Bits;
  UINT32 Data;

} CSIPNMIC_1_IIO_DFX_VTD_STRUCT;

/* CSIPMCAC_0_IIO_DFX_VTD_REG supported on:                                     */
/*      SPRA0 (0x20803598)                                                      */
/*      SPRB0 (0x20803598)                                                      */
/*      SPRHBM (0x20803598)                                                     */
/*      SPRC0 (0x20803598)                                                      */
/*      SPRMCC (0x20803598)                                                     */
/*      SPRUCC (0x20803598)                                                     */
/* Register default value on SPRA0: 0x00FF0000                                  */
/* Register default value on SPRB0: 0x00FF0000                                  */
/* Register default value on SPRHBM: 0x00FF0000                                 */
/* Register default value on SPRC0: 0x00FF0000                                  */
/* Register default value on SPRMCC: 0x00FF0000                                 */
/* Register default value on SPRUCC: 0x00FF0000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* CSI PMCAC 0
*/


#define CSIPMCAC_0_IIO_DFX_VTD_REG 0x12420598

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 16;

                            /* Bits[15:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 interrupt_data_vector : 8;

                            /* Bits[23:16], Access Type=RW, default=0x000000FF*/

                            /* Interrupt Data Vector */
    UINT32 rsvd_24 : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CSIPMCAC_0_IIO_DFX_VTD_STRUCT;

/* CSIPMCAC_1_IIO_DFX_VTD_REG supported on:                                     */
/*      SPRA0 (0x2080359c)                                                      */
/*      SPRB0 (0x2080359c)                                                      */
/*      SPRHBM (0x2080359c)                                                     */
/*      SPRC0 (0x2080359c)                                                      */
/*      SPRMCC (0x2080359c)                                                     */
/*      SPRUCC (0x2080359c)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* CSI PMCAC1
*/


#define CSIPMCAC_1_IIO_DFX_VTD_REG 0x1242059C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 destination_id : 32;

                            /* Bits[31:0], Access Type=RW, default=0xFFFFFFFF*/

                            /* Destination Id */

  } Bits;
  UINT32 Data;

} CSIPMCAC_1_IIO_DFX_VTD_STRUCT;

/* CSIPINITC_1_IIO_DFX_VTD_REG supported on:                                    */
/*      SPRA0 (0x208035a0)                                                      */
/*      SPRB0 (0x208035a0)                                                      */
/*      SPRHBM (0x208035a0)                                                     */
/*      SPRC0 (0x208035a0)                                                      */
/*      SPRMCC (0x208035a0)                                                     */
/*      SPRUCC (0x208035a0)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* CSI PINITC 1
*/


#define CSIPINITC_1_IIO_DFX_VTD_REG 0x124205A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 destination_id : 32;

                            /* Bits[31:0], Access Type=RW, default=0xFFFFFFFF*/

                            /* Destination Id */

  } Bits;
  UINT32 Data;

} CSIPINITC_1_IIO_DFX_VTD_STRUCT;

/* CSIPSUBSAD_IIO_DFX_VTD_REG supported on:                                     */
/*      SPRA0 (0x208035a8)                                                      */
/*      SPRB0 (0x208035a8)                                                      */
/*      SPRHBM (0x208035a8)                                                     */
/*      SPRC0 (0x208035a8)                                                      */
/*      SPRMCC (0x208035a8)                                                     */
/*      SPRUCC (0x208035a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Coherent Interface Sub Source Address Decoder
*/


#define CSIPSUBSAD_IIO_DFX_VTD_REG 0x124205A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 valid : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Valid */
    UINT32 rsvd : 7;

                            /* Bits[7:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 legacy_ioh_nodeid5 : 6;

                            /* Bits[13:8], Access Type=RW, default=0x00000000*/

                            /* legacy MS2IOSF Node ID */
    UINT32 rsvd_14 : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CSIPSUBSAD_IIO_DFX_VTD_STRUCT;

/* ITC_VISA_PREMUX_N0_IIO_DFX_VTD_REG supported on:                             */
/*      SPRA0 (0x20803660)                                                      */
/*      SPRB0 (0x20803660)                                                      */
/*      SPRHBM (0x20803660)                                                     */
/*      SPRC0 (0x20803660)                                                      */
/*      SPRMCC (0x20803660)                                                     */
/*      SPRUCC (0x20803660)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Register to control the various fields that are being pre-muxed in ITC before going into the Visa network
*/


#define ITC_VISA_PREMUX_N0_IIO_DFX_VTD_REG 0x12420660

#if defined(SPRA0_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sel_based_on_phase : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Phase select */
    UINT32 hdr_over_vtd : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               hdr_over_vtd: give priority to Hdr if both Hdr
                               and VTd pipes are valid in the same cycle[br]
                            */
    UINT32 del_hdr : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               del_hdr: delay the header 1 cycle before
                               capturing it[br]
                            */
    UINT32 sw2iosf_chnlb_sel : 4;

                            /* Bits[6:3], Access Type=RW, default=0x00000000*/

                            /* Psf Port Arb Chanel B select */
    UINT32 sw2iosf_chnla_sel : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /* Psf Port Arb Chanel A select */
    UINT32 sw2iosf_chnl_sel : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /* Sw2iosf psf buf master chanel select */
    UINT32 sw2iosf_fc_sel : 2;

                            /* Bits[16:15], Access Type=RW, default=0x00000000*/

                            /* sw2iosf psf buf master FC select */
    UINT32 sw2iosf_port_sel : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /* sw2iost port select */
    UINT32 sel_enq_pipe : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /* Enqueue pipe select */
    UINT32 sel_enq_misc_pipe : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /* Enqueue pipe select */
    UINT32 sel_norm_pipe : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /* Enqueue pipe select */
    UINT32 sel_fast_pipe : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* Enqueue pipe select */
    UINT32 rsvd : 2;

                            /* Bits[23:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sel_pump : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               sel_pump: choose which group of data to capture
                               within Vtd and Hdr pipes
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITC_VISA_PREMUX_N0_IIO_DFX_VTD_SPRA0_STRUCT;
#endif /* (SPRA0_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sel_based_on_phase : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Phase select */
    UINT32 hdr_over_vtd : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               hdr_over_vtd: give priority to Hdr if both Hdr
                               and VTd pipes are valid in the same cycle[br]
                            */
    UINT32 del_hdr : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               del_hdr: delay the header 1 cycle before
                               capturing it[br]
                            */
    UINT32 sw2iosf_chnlb_sel : 4;

                            /* Bits[6:3], Access Type=RW, default=0x00000000*/

                            /* Psf Port Arb Chanel B select */
    UINT32 sw2iosf_chnla_sel : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /* Psf Port Arb Chanel A select */
    UINT32 sw2iosf_chnl_sel : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /* Sw2iosf psf buf master chanel select */
    UINT32 sw2iosf_fc_sel : 2;

                            /* Bits[16:15], Access Type=RW, default=0x00000000*/

                            /* sw2iosf psf buf master FC select */
    UINT32 sw2iosf_port_sel : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /* sw2iost port select */
    UINT32 sel_enq_pipe : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /* Enqueue pipe select */
    UINT32 sel_enq_misc_pipe : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /* Enqueue pipe select */
    UINT32 sel_norm_pipe : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /* Enqueue pipe select */
    UINT32 sel_fast_pipe : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* Enqueue pipe select */
    UINT32 rsvd : 2;

                            /* Bits[23:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sel_pump : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               sel_pump: choose which group of data to capture
                               within Vtd and Hdr pipes
                            */
    UINT32 sel_hdr_pipe : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /* Select any hdr pipe */
    UINT32 rsvd_28 : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITC_VISA_PREMUX_N0_IIO_DFX_VTD_SPRB0_SPRHBM_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sel_based_on_phase : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Phase select */
    UINT32 hdr_over_vtd : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               hdr_over_vtd: give priority to Hdr if both Hdr
                               and VTd pipes are valid in the same cycle[br]
                            */
    UINT32 del_hdr : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               del_hdr: delay the header 1 cycle before
                               capturing it[br]
                            */
    UINT32 sw2iosf_chnlb_sel : 4;

                            /* Bits[6:3], Access Type=RW, default=0x00000000*/

                            /* Psf Port Arb Chanel B select */
    UINT32 sw2iosf_chnla_sel : 4;

                            /* Bits[10:7], Access Type=RW, default=0x00000000*/

                            /* Psf Port Arb Chanel A select */
    UINT32 sw2iosf_chnl_sel : 4;

                            /* Bits[14:11], Access Type=RW, default=0x00000000*/

                            /* Sw2iosf psf buf master chanel select */
    UINT32 sw2iosf_fc_sel : 2;

                            /* Bits[16:15], Access Type=RW, default=0x00000000*/

                            /* sw2iosf psf buf master FC select */
    UINT32 sw2iosf_port_sel : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /* sw2iost port select */
    UINT32 sel_enq_pipe : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /* Enqueue pipe select */
    UINT32 sel_enq_misc_pipe : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /* Enqueue pipe select */
    UINT32 sel_norm_pipe : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /* Enqueue pipe select */
    UINT32 sel_fast_pipe : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* Enqueue pipe select */
    UINT32 rsvd : 2;

                            /* Bits[23:22], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 sel_pump : 3;

                            /* Bits[26:24], Access Type=RW, default=0x00000000*/

                            /*
                               sel_pump: choose which group of data to capture
                               within Vtd and Hdr pipes
                            */
    UINT32 rsvd_27 : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITC_VISA_PREMUX_N0_IIO_DFX_VTD_STRUCT;

/* ITC_VISA_PREMUX_N1_IIO_DFX_VTD_REG supported on:                             */
/*      SPRA0 (0x20803664)                                                      */
/*      SPRB0 (0x20803664)                                                      */
/*      SPRHBM (0x20803664)                                                     */
/*      SPRC0 (0x20803664)                                                      */
/*      SPRMCC (0x20803664)                                                     */
/*      SPRUCC (0x20803664)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Register to control the various fields that are being pre-muxed in ITC before going into the Visa network
*/


#define ITC_VISA_PREMUX_N1_IIO_DFX_VTD_REG 0x12420664

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

} ITC_VISA_PREMUX_N1_IIO_DFX_VTD_STRUCT;

/* OTC_VISA_PREMUX_N0_IIO_DFX_VTD_REG supported on:                             */
/*      SPRA0 (0x20803668)                                                      */
/*      SPRB0 (0x20803668)                                                      */
/*      SPRHBM (0x20803668)                                                     */
/*      SPRC0 (0x20803668)                                                      */
/*      SPRMCC (0x20803668)                                                     */
/*      SPRUCC (0x20803668)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Register to control the various fields that are being pre-muxed in ITC before going into the Visa network
*/


#define OTC_VISA_PREMUX_N0_IIO_DFX_VTD_REG 0x12420668

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 intx_val : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000000*/

                            /* Visa premux intex val */
    UINT32 intx_sel : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Visa premux intex sel */
    UINT32 gpsb_asm_np_or_p_sel : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Visa premux gpsb_asm_np_or_p_sel */
    UINT32 trans_brdg_p_or_np_payload_sel : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Visa premux trans_brdg_p_or_np_payload_sel */
    UINT32 misc_dbgen : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Visa premux misc_dbgen */
    UINT32 vtd_hdrmux_sel0 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Visa premux vtd_hdrmux_sel0 */
    UINT32 vtd_hdrmux_sel1 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Visa premux vtd_hdrmux_sel1 */
    UINT32 cplcomb_chnl_sel : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Visa premux cplcomb_chnl_sel */
    UINT32 cplcomb_chnl : 4;

                            /* Bits[12:9], Access Type=RW, default=0x00000000*/

                            /* Visa premux cplcomb_chnl */
    UINT32 cplcomb_fc : 2;

                            /* Bits[14:13], Access Type=RW, default=0x00000000*/

                            /* Visa premux cplcomb_fc */
    UINT32 cplcomb_fc_sel : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* Visa premux cplcomb_fc_sel */
    UINT32 cplcount_chnl : 4;

                            /* Bits[19:16], Access Type=RW, default=0x00000000*/

                            /* Visa premux cplcount_chnl */
    UINT32 cplcount_chnl_sel : 2;

                            /* Bits[21:20], Access Type=RW, default=0x00000000*/

                            /* Visa premux cplcount_chnl_sel */
    UINT32 misc_pcrd : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /* Visa premux misc_pcrd */
    UINT32 misc_npcrd : 2;

                            /* Bits[25:24], Access Type=RW, default=0x00000000*/

                            /* Visa premux misc_npcrd */
    UINT32 misc_ccrd : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /* Visa premux misc_ccrd */
    UINT32 misc_cmd : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /* Visa premux misc_cmd */

  } Bits;
  UINT32 Data;

} OTC_VISA_PREMUX_N0_IIO_DFX_VTD_STRUCT;

/* OTC_VISA_PREMUX_N1_IIO_DFX_VTD_REG supported on:                             */
/*      SPRA0 (0x2080366c)                                                      */
/*      SPRB0 (0x2080366c)                                                      */
/*      SPRHBM (0x2080366c)                                                     */
/*      SPRC0 (0x2080366c)                                                      */
/*      SPRMCC (0x2080366c)                                                     */
/*      SPRUCC (0x2080366c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Register to control the various fields that are being pre-muxed in ITC before going into the Visa network
*/


#define OTC_VISA_PREMUX_N1_IIO_DFX_VTD_REG 0x1242066C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 misc_cmd : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Visa premux misc_cmd */
    UINT32 iotc_crdt_rtn_otc_occ_thrtl : 4;

                            /* Bits[4:1], Access Type=RW, default=0x00000000*/

                            /*
                               Channel select for otc occupancy throttle. 0-11:
                               Selects channel 0-11 12: selects NP and P to IRP
                               (global)
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTC_VISA_PREMUX_N1_IIO_DFX_VTD_STRUCT;

/* TSWCTL3_IIO_DFX_VTD_REG supported on:                                        */
/*      SPRA0 (0x20803670)                                                      */
/*      SPRB0 (0x20803670)                                                      */
/*      SPRHBM (0x20803670)                                                     */
/*      SPRC0 (0x20803670)                                                      */
/*      SPRMCC (0x20803670)                                                     */
/*      SPRUCC (0x20803670)                                                     */
/* Register default value on SPRA0: 0x0000060F                                  */
/* Register default value on SPRB0: 0x0000060F                                  */
/* Register default value on SPRHBM: 0x0000060F                                 */
/* Register default value on SPRC0: 0x0000460F                                  */
/* Register default value on SPRMCC: 0x0000060F                                 */
/* Register default value on SPRUCC: 0x0000460F                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ITC and OTC defeatures
*/


#define TSWCTL3_IIO_DFX_VTD_REG 0x12420670

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRMCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable_irp_intlv_iou2 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000003*/

                            /* Enable Irp Intlv Iou2 */
    UINT32 enable_irp_intlv_cbdma : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000003*/

                            /* Enable Irp Intlv Cbdma */
    UINT32 rsvd : 3;

                            /* Bits[6:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 csr_en_vc1_ma : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* For IVT only, which allow Vcm but not vc1 */
    UINT32 csr_en_ds_lt_in_vira : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Enable outbound address from processing
                               downstream lt transactions during vira[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 csr_en_ds_msg_in_vira : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Enable outbound address from processing
                               downstream msg transactions during vira[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 csr_en_ds_cfg_in_vira : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Enable outbound address from processing
                               downstream cfg transactions during vira[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 cfg_mabt_hint_disable : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Disable outbound address from processing MABT
                               Hint for downstream config transactions
                               initiated from coherent interface.[br] Locked by
                               DBGBUSLCK
                            */
    UINT32 lt_mabt_hint_disable : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Disable CSI FIFO controller from propagating the
                               MABT_HINT for LT transactions.[br] Locked by
                               DBGBUSLCK
                            */
    UINT32 lt_mabt_errlog_enable : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Enable error logging for LT cycles that were
                               MABTed[br] Locked by DBGBUSLCK
                            */
    UINT32 rsvd_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 csr_disable_bar_access_in_viral : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Disable MMIO access during viral[br] Locked by
                               DBGBUSLCK
                            */
    UINT32 rsvd_16 : 3;

                            /* Bits[18:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 irp_mabt_hint_disable : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Ignore the master abort hint from IRP on
                               outbound requests.[br] Locked by DBGBUSLCK
                            */
    UINT32 csr_iosf_cmd_par_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Chicken bit to disable packet dropping in
                               switch2iosf upon inbound cmd parity err
                            */
    UINT32 ras_err_func_zero : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               Defeature Bit to force RAS Error reporting in
                               IOTC Miscblk to send function zero on all IEH
                               Messages
                            */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSWCTL3_IIO_DFX_VTD_SPRA0_SPRB0_SPRHBM_SPRMCC_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRMCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable_irp_intlv_iou2 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000003*/

                            /* Enable Irp Intlv Iou2 */
    UINT32 enable_irp_intlv_cbdma : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000003*/

                            /* Enable Irp Intlv Cbdma */
    UINT32 rsvd : 3;

                            /* Bits[6:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 csr_en_vc1_ma : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* For IVT only, which allow Vcm but not vc1 */
    UINT32 csr_en_ds_lt_in_vira : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Enable outbound address from processing
                               downstream lt transactions during vira[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 csr_en_ds_msg_in_vira : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Enable outbound address from processing
                               downstream msg transactions during vira[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 csr_en_ds_cfg_in_vira : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Enable outbound address from processing
                               downstream cfg transactions during vira[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 cfg_mabt_hint_disable : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Disable outbound address from processing MABT
                               Hint for downstream config transactions
                               initiated from coherent interface.[br] Locked by
                               DBGBUSLCK
                            */
    UINT32 lt_mabt_hint_disable : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Disable CSI FIFO controller from propagating the
                               MABT_HINT for LT transactions.[br] Locked by
                               DBGBUSLCK
                            */
    UINT32 lt_mabt_errlog_enable : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Enable error logging for LT cycles that were
                               MABTed[br] Locked by DBGBUSLCK
                            */
    UINT32 block_vtbar_access_during_lock : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               When set, H/W applies same behavior to VTBAR
                               accesses as all other downstream MEM accesses
                               during a lock
                            */
    UINT32 csr_disable_bar_access_in_viral : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Disable MMIO access during viral[br] Locked by
                               DBGBUSLCK
                            */
    UINT32 rsvd_16 : 3;

                            /* Bits[18:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 irp_mabt_hint_disable : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Ignore the master abort hint from IRP on
                               outbound requests.[br] Locked by DBGBUSLCK
                            */
    UINT32 csr_iosf_cmd_par_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Chicken bit to disable packet dropping in
                               switch2iosf upon inbound cmd parity err
                            */
    UINT32 ras_err_func_zero : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               Defeature Bit to force RAS Error reporting in
                               IOTC Miscblk to send function zero on all IEH
                               Messages
                            */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSWCTL3_IIO_DFX_VTD_SPRC0_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable_irp_intlv_iou2 : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000003*/

                            /* Enable Irp Intlv Iou2 */
    UINT32 enable_irp_intlv_cbdma : 2;

                            /* Bits[3:2], Access Type=RW, default=0x00000003*/

                            /* Enable Irp Intlv Cbdma */
    UINT32 rsvd : 3;

                            /* Bits[6:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 csr_en_vc1_ma : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* For IVT only, which allow Vcm but not vc1 */
    UINT32 csr_en_ds_lt_in_vira : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Enable outbound address from processing
                               downstream lt transactions during vira[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 csr_en_ds_msg_in_vira : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Enable outbound address from processing
                               downstream msg transactions during vira[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 csr_en_ds_cfg_in_vira : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Enable outbound address from processing
                               downstream cfg transactions during vira[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 cfg_mabt_hint_disable : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Disable outbound address from processing MABT
                               Hint for downstream config transactions
                               initiated from coherent interface.[br] Locked by
                               DBGBUSLCK
                            */
    UINT32 lt_mabt_hint_disable : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Disable CSI FIFO controller from propagating the
                               MABT_HINT for LT transactions.[br] Locked by
                               DBGBUSLCK
                            */
    UINT32 lt_mabt_errlog_enable : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Enable error logging for LT cycles that were
                               MABTed[br] Locked by DBGBUSLCK
                            */
    UINT32 rsvd_14 : 1;

                            /* Bits[14:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 csr_disable_bar_access_in_viral : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Disable MMIO access during viral[br] Locked by
                               DBGBUSLCK
                            */
    UINT32 rsvd_16 : 3;

                            /* Bits[18:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 irp_mabt_hint_disable : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Ignore the master abort hint from IRP on
                               outbound requests.[br] Locked by DBGBUSLCK
                            */
    UINT32 csr_iosf_cmd_par_dis : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Chicken bit to disable packet dropping in
                               switch2iosf upon inbound cmd parity err
                            */
    UINT32 ras_err_func_zero : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               Defeature Bit to force RAS Error reporting in
                               IOTC Miscblk to send function zero on all IEH
                               Messages
                            */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSWCTL3_IIO_DFX_VTD_STRUCT;

/* TCSNAPSHOTCTL_IIO_DFX_VTD_REG supported on:                                  */
/*      SPRA0 (0x20803674)                                                      */
/*      SPRB0 (0x20803674)                                                      */
/*      SPRHBM (0x20803674)                                                     */
/*      SPRC0 (0x20803674)                                                      */
/*      SPRMCC (0x20803674)                                                     */
/*      SPRUCC (0x20803674)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Control for reading out the TC tracker contents into the itcsnapshotdata and otcsnapshotdata registers
*/


#define TCSNAPSHOTCTL_IIO_DFX_VTD_REG 0x12420674

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 automode : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               When set to 0, software must write the busy bit
                               along with the new address to initiate a new
                               read of the corresponding array.[br] When set to
                               1, each read of itcsnapshotdata or
                               otcsnapshotdata will increment the respective
                               address and initiate a new read by setting
                               itc_busy or otc_busy.
                            */
    UINT32 itc_busy : 1;

                            /* Bits[1:1], Access Type=RW/V, default=0x00000000*/

                            /*
                               When written to 1, hardware will look up the
                               array entry specified by itc_addr and load it
                               into itcsnapshotdata. Once the data has been
                               loaded, hardware will clear itc_busy.
                            */
    UINT32 otc_busy : 1;

                            /* Bits[2:2], Access Type=RW/V, default=0x00000000*/

                            /*
                               When written to 1, hardware will look up the
                               array entry specified by otc_addr and load it
                               into otcsnapshotdata. Once the data has been
                               loaded, hardware will clear otc_busy.
                            */
    UINT32 itc_err : 1;

                            /* Bits[3:3], Access Type=RW/V, default=0x00000000*/

                            /*
                               Indicates that itcsnapshotdata was read before
                               the data corresponding to itc_addr was loaded
                            */
    UINT32 otc_err : 1;

                            /* Bits[4:4], Access Type=RW/V, default=0x00000000*/

                            /*
                               Indicates that otcsnapshotdata was read before
                               the data corresponding to otc_addr was loaded
                            */
    UINT32 itc_addr : 7;

                            /* Bits[11:5], Access Type=RW/V, default=0x00000000*/

                            /*
                               Bits 5:4 - FC of the tracker array to read. Bits
                               3:0 - channel of the tracker array to read.
                            */
    UINT32 otc_addr : 7;

                            /* Bits[18:12], Access Type=RW/V, default=0x00000000*/

                            /*
                               Bits 5:4 - FC of the tracker array to read. Bits
                               3:0 - channel of the tracker array to read.
                            */
    UINT32 array_sel : 4;

                            /* Bits[22:19], Access Type=RW, default=0x00000000*/

                            /*
                               ID of the array to read,different values between
                               ITC and OTC:[br] 0-6: reserved[br] ITC [br] 7:
                               hit_early_counts and hit_head_ptrs ,sub chnl 0
                               and 1 [br] 8: hdr_counts and hdr_f_head_ptrs
                               subchnl 0 and 1 [br] 9: hdr_f_amap sub chnl 0
                               [br] 10: hdr_f_amap sub chnl 1 [br] 11:
                               hdr_pf_amap sub chnl 0[br] 12: hdr_pf_amap sub
                               chnl 1[br] 13: hdr_pf_head_ptrs(sub chnl 0 and
                               1), req_head_ptrs and tail_ptrs[br] 14:
                               hdr_pf_misc, dat_misc and hdr_pf_misc[br] 15:
                               OrderState[br] OTC [br] 7: vtd_counts and
                               vtd_f_head_ptrs [br] 8: hdr_counts and
                               hdr_f_head_ptrs [br] 9: hdr_f_amap [br] 12:
                               tail_ptrs[br] 13: hdr_f_misc[br] 14:
                               OrderState[br]
                            */
    UINT32 chunk_sel : 2;

                            /* Bits[24:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               Chunk of the array to read (for arrays that are
                               bigger than 32 bits per entry):[br] 0-13: not
                               applicable[br] Use 14 in tcctl for OTC and 15
                               for ITC 14/15: OrderState[br] 2b00:[br]
                               p_empty,[br] p_np_in,[br] p_c_in,[br]
                               np_inc_done,[br] c_inc_done,[br]
                               count_p_np[9:0],[br] count_p_c[9:0],[br]
                               np_head_p_bit,[br] c_head_p_bit,[br]
                               prh_head_done,[br] prh_head_ncp2pb,[br]
                               prh_head_hdr_crdt[br] 2b01:[br]
                               prh_head_ptr[7:0],[br] prh_2nd_ptr[7:0],[br]
                               prh_f_ptr[7:0],[br] prh_f_2nd_ptr[7:0][br]
                               2b10:[br] prh_f_ptr_vld,[br] p_dest_change,[br]
                               prh_f_pf_ack,[br] prh_tail_ptr[7:0],[br]
                               prh_empty,[br] prh_all_f_done,[br]
                               prh_all_f_done_destchange,[br]
                               prh_all_f_done_so,[br] p_block,[br]
                               p_head_order[1:0][br]
                            */
    UINT32 rsvd : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TCSNAPSHOTCTL_IIO_DFX_VTD_STRUCT;

/* ITCSNAPSHOTDATA_N0_IIO_DFX_VTD_REG supported on:                             */
/*      SPRA0 (0x20803678)                                                      */
/*      SPRB0 (0x20803678)                                                      */
/*      SPRHBM (0x20803678)                                                     */
/*      SPRC0 (0x20803678)                                                      */
/*      SPRMCC (0x20803678)                                                     */
/*      SPRUCC (0x20803678)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Data captured from the ITC array specified by tcsnapshotctl
*/


#define ITCSNAPSHOTDATA_N0_IIO_DFX_VTD_REG 0x12420678

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* Data */

  } Bits;
  UINT32 Data;

} ITCSNAPSHOTDATA_N0_IIO_DFX_VTD_STRUCT;

/* ITCSNAPSHOTDATA_N1_IIO_DFX_VTD_REG supported on:                             */
/*      SPRA0 (0x2080367c)                                                      */
/*      SPRB0 (0x2080367c)                                                      */
/*      SPRHBM (0x2080367c)                                                     */
/*      SPRC0 (0x2080367c)                                                      */
/*      SPRMCC (0x2080367c)                                                     */
/*      SPRUCC (0x2080367c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Data captured from the ITC array specified by tcsnapshotctl
*/


#define ITCSNAPSHOTDATA_N1_IIO_DFX_VTD_REG 0x1242067C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* Data */

  } Bits;
  UINT32 Data;

} ITCSNAPSHOTDATA_N1_IIO_DFX_VTD_STRUCT;

/* OTCSNAPSHOTDATA_N0_IIO_DFX_VTD_REG supported on:                             */
/*      SPRA0 (0x20803680)                                                      */
/*      SPRB0 (0x20803680)                                                      */
/*      SPRHBM (0x20803680)                                                     */
/*      SPRC0 (0x20803680)                                                      */
/*      SPRMCC (0x20803680)                                                     */
/*      SPRUCC (0x20803680)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Data captured from the OTC array specified by tcsnapshotctl
*/


#define OTCSNAPSHOTDATA_N0_IIO_DFX_VTD_REG 0x12420680

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* Data */

  } Bits;
  UINT32 Data;

} OTCSNAPSHOTDATA_N0_IIO_DFX_VTD_STRUCT;

/* OTCSNAPSHOTDATA_N1_IIO_DFX_VTD_REG supported on:                             */
/*      SPRA0 (0x20803684)                                                      */
/*      SPRB0 (0x20803684)                                                      */
/*      SPRHBM (0x20803684)                                                     */
/*      SPRC0 (0x20803684)                                                      */
/*      SPRMCC (0x20803684)                                                     */
/*      SPRUCC (0x20803684)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Data captured from the OTC array specified by tcsnapshotctl
*/


#define OTCSNAPSHOTDATA_N1_IIO_DFX_VTD_REG 0x12420684

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* Data */

  } Bits;
  UINT32 Data;

} OTCSNAPSHOTDATA_N1_IIO_DFX_VTD_STRUCT;

/* TSWCTL1_IIO_DFX_VTD_REG supported on:                                        */
/*      SPRA0 (0x2080368c)                                                      */
/*      SPRB0 (0x2080368c)                                                      */
/*      SPRHBM (0x2080368c)                                                     */
/*      SPRC0 (0x2080368c)                                                      */
/*      SPRMCC (0x2080368c)                                                     */
/*      SPRUCC (0x2080368c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ITC and OTC defeatures
*/


#define TSWCTL1_IIO_DFX_VTD_REG 0x1242068C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 6;

                            /* Bits[5:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 disable_otc_abort : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* set 1: OTC to disable MA/CA abort */
    UINT32 rsvd_7 : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSWCTL1_IIO_DFX_VTD_STRUCT;

/* TSWCTL2_IIO_DFX_VTD_REG supported on:                                        */
/*      SPRA0 (0x20803690)                                                      */
/*      SPRB0 (0x20803690)                                                      */
/*      SPRHBM (0x20803690)                                                     */
/*      SPRC0 (0x20803690)                                                      */
/*      SPRMCC (0x20803690)                                                     */
/*      SPRUCC (0x20803690)                                                     */
/* Register default value on SPRA0: 0x00100000                                  */
/* Register default value on SPRB0: 0x00100000                                  */
/* Register default value on SPRHBM: 0x00100000                                 */
/* Register default value on SPRC0: 0x00100000                                  */
/* Register default value on SPRMCC: 0x00100000                                 */
/* Register default value on SPRUCC: 0x00100000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ITC and OTC defeatures
*/


#define TSWCTL2_IIO_DFX_VTD_REG 0x12420690

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fastpath_disabled : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Notes:[br] Locked by DBGBUSLCK */
    UINT32 inb_crs_cmpl_disable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Defeature bit to disable logging for inbound
                               unsuccessful completions with CRS status[br]
                               Notes:[br] Locked by DBGBUSLCK
                            */
    UINT32 rsvd : 8;

                            /* Bits[9:2], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 global_nosnoop_disable : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Notes:[br] Locked by DBGBUSLCK */
    UINT32 rsvd_11 : 9;

                            /* Bits[19:11], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 override_routing_method : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /* Notes:[br] Locked by DBGBUSLCK */
    UINT32 rsvd_21 : 7;

                            /* Bits[27:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dis_abort_fdfe_range : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Disable hardware abort of requests to the
                               0xFD00-0xFEDF and 0xFEF0-0xFFFF ranges.[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSWCTL2_IIO_DFX_VTD_STRUCT;

/* TSWCTL4_IIO_DFX_VTD_REG supported on:                                        */
/*      SPRA0 (0x20803694)                                                      */
/*      SPRB0 (0x20803694)                                                      */
/*      SPRHBM (0x20803694)                                                     */
/*      SPRC0 (0x20803694)                                                      */
/*      SPRMCC (0x20803694)                                                     */
/*      SPRUCC (0x20803694)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ITC and OTC defeatures
*/


#define TSWCTL4_IIO_DFX_VTD_REG 0x12420694

#if defined(SPRA0_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 csr_jkt_enable_vtdcorruption_fix : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Notes:[br] Locked by DBGBUSLCK */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSWCTL4_IIO_DFX_VTD_SPRA0_STRUCT;
#endif /* (SPRA0_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 csr_jkt_enable_vtdcorruption_fix : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Notes:[br] Locked by DBGBUSLCK */
    UINT32 csr_dis_msgb_mcast_hdr_cdt_rtn : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Defeature to disable the bug fix for 2201788744
                               Disables the bug fix done to handle the fix
                               pipeline header credit return for MsgB/Mcast
                            */
    UINT32 lock_ignore_hdr_arbfvec_otc_np : 9;

                            /* Bits[10:2], Access Type=RW, default=0x00000000*/

                            /*
                               [7:0] - OTC Defeature to ignore hdr arbfvec
                               during lock for non-posted per channel [8] - OTC
                               Defeature to ignore hdr arbfvec during lock for
                               non-posted for misc channels
                            */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSWCTL4_IIO_DFX_VTD_SPRB0_SPRHBM_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 csr_jkt_enable_vtdcorruption_fix : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Notes:[br] Locked by DBGBUSLCK */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSWCTL4_IIO_DFX_VTD_STRUCT;

/* TSWCTL5_IIO_DFX_VTD_REG supported on:                                        */
/*      SPRA0 (0x20803698)                                                      */
/*      SPRB0 (0x20803698)                                                      */
/*      SPRHBM (0x20803698)                                                     */
/*      SPRC0 (0x20803698)                                                      */
/*      SPRMCC (0x20803698)                                                     */
/*      SPRUCC (0x20803698)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ITC and OTC defeatures
*/


#define TSWCTL5_IIO_DFX_VTD_REG 0x12420698

#if defined(SPRA0_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 en_tph_with_ro : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Enable TPH hints to propagate for RO=1 writes. */
    UINT32 force_no_snp_on_vc1_vcm : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Force VC1 and VCm traffic to be nonsnoop. */
    UINT32 vc1m_ns_disable : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Prevent VC1 and VCm traffic from being nonsnoop. */
    UINT32 disable_ma_ats_inv_msg : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Disable Master Abort of ATS invalidation message
                               when it it not confined p2p request.
                            */
    UINT32 disable_ma_prg_rsp_msg : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Disable Master Abort of Page Request Group
                               Response when it it not confined p2p request.
                            */
    UINT32 csr_dbg_tswctl5 : 27;

                            /* Bits[31:5], Access Type=RW, default=0x00000000*/

                            /*
                               Csr Dbg Tswctl5 [31:5] [5] - Defeature to
                               disable the bug fix for 2201788744 Disables the
                               bug fix done to handle the fix pipeline header
                               credit return for MsgB/Mcast [31:6] - Reserved
                            */

  } Bits;
  UINT32 Data;

} TSWCTL5_IIO_DFX_VTD_SPRA0_STRUCT;
#endif /* (SPRA0_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 en_tph_with_ro : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Enable TPH hints to propagate for RO=1 writes. */
    UINT32 force_no_snp_on_vc1_vcm : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Force VC1 and VCm traffic to be nonsnoop. */
    UINT32 vc1m_ns_disable : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Prevent VC1 and VCm traffic from being nonsnoop. */
    UINT32 disable_ma_ats_inv_msg : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Disable Master Abort of ATS invalidation message
                               when it it not confined p2p request.
                            */
    UINT32 disable_ma_prg_rsp_msg : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Disable Master Abort of Page Request Group
                               Response when it it not confined p2p request.
                            */
    UINT32 lock_ignore_hdr_arbfvec_p : 9;

                            /* Bits[13:5], Access Type=RW, default=0x00000000*/

                            /*
                               [7:0] - Defeature to ignore hdr arbfvec during
                               lock for posted per channel [8] - Defeature to
                               ignore hdr arbfvec during lock for posted for
                               misc channels Setting to 1 - enables defetaure
                            */
    UINT32 lock_ignore_hdr_arbfvec_np : 9;

                            /* Bits[22:14], Access Type=RW, default=0x00000000*/

                            /*
                               [7:0] - Defeature to ignore hdr arbfvec during
                               lock for non-posted per channel [8] - Defeature
                               to ignore hdr arbfvec during lock for non-posted
                               for misc channels Default value 0 - Sets/enables
                               defeature
                            */
    UINT32 lock_ignore_hdr_arbfvec_c : 9;

                            /* Bits[31:23], Access Type=RW, default=0x00000000*/

                            /*
                               [7:0] - Defeature to ignore hdr arbfvec during
                               lock for completion per channel [8] - Defeature
                               to ignore hdr arbfvec during lock for completion
                               for misc channels Default value 0 - Sets/enables
                               defeature
                            */

  } Bits;
  UINT32 Data;

} TSWCTL5_IIO_DFX_VTD_SPRB0_SPRHBM_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 en_tph_with_ro : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Enable TPH hints to propagate for RO=1 writes. */
    UINT32 force_no_snp_on_vc1_vcm : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Force VC1 and VCm traffic to be nonsnoop. */
    UINT32 vc1m_ns_disable : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Prevent VC1 and VCm traffic from being nonsnoop. */
    UINT32 disable_ma_ats_inv_msg : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Disable Master Abort of ATS invalidation message
                               when it it not confined p2p request.
                            */
    UINT32 disable_ma_prg_rsp_msg : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Disable Master Abort of Page Request Group
                               Response when it it not confined p2p request.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TSWCTL5_IIO_DFX_VTD_STRUCT;

/* SWDBGCTL1_IIO_DFX_VTD_REG supported on:                                      */
/*      SPRA0 (0x2080369c)                                                      */
/*      SPRB0 (0x2080369c)                                                      */
/*      SPRHBM (0x2080369c)                                                     */
/*      SPRC0 (0x2080369c)                                                      */
/*      SPRMCC (0x2080369c)                                                     */
/*      SPRUCC (0x2080369c)                                                     */
/* Register default value on SPRA0: 0x00C00000                                  */
/* Register default value on SPRB0: 0x00C00000                                  */
/* Register default value on SPRHBM: 0x00C00000                                 */
/* Register default value on SPRC0: 0x00C00000                                  */
/* Register default value on SPRMCC: 0x00C00000                                 */
/* Register default value on SPRUCC: 0x00C00000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* TC Defeature 01
*/


#define SWDBGCTL1_IIO_DFX_VTD_REG 0x1242069C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 22;

                            /* Bits[21:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 enable_irp_intlv : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Enable IRP INTLV[br] Notes:[br] - Locked by
                               DBGBUSLCK
                            */
    UINT32 enable_irp_set : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /* Enable Irp Set */
    UINT32 rsvd_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vtpf_delay_dis : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /* Vtpf Delay Dis */
    UINT32 rsvd_26 : 5;

                            /* Bits[30:26], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dis_hsx_hsd_4571261_fix : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /* Dis Hsx Hsd 4571261 Fix */

  } Bits;
  UINT32 Data;

} SWDBGCTL1_IIO_DFX_VTD_STRUCT;

/* SPARE_SWEAST_IIO_DFX_VTD_REG supported on:                                   */
/*      SPRA0 (0x208036a4)                                                      */
/*      SPRB0 (0x208036a4)                                                      */
/*      SPRHBM (0x208036a4)                                                     */
/*      SPRC0 (0x208036a4)                                                      */
/*      SPRMCC (0x208036a4)                                                     */
/*      SPRUCC (0x208036a4)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* ITC/OTC defeature bits
*/


#define SPARE_SWEAST_IIO_DFX_VTD_REG 0x124206A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 locks_skip_vtd_check : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Locks Skip Vtd Check */
    UINT32 locks_use_quiesce_flow : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Locks Use Quiesce Flow */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SPARE_SWEAST_IIO_DFX_VTD_STRUCT;

/* ITCDBG_HDRMASK_N0_IIO_DFX_VTD_REG supported on:                              */
/*      SPRA0 (0x208036a8)                                                      */
/*      SPRB0 (0x208036a8)                                                      */
/*      SPRHBM (0x208036a8)                                                     */
/*      SPRC0 (0x208036a8)                                                      */
/*      SPRMCC (0x208036a8)                                                     */
/*      SPRUCC (0x208036a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* header mask for ITC DFD logic
*/


#define ITCDBG_HDRMASK_N0_IIO_DFX_VTD_REG 0x124206A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask applied to one of the 4 32-bit chunks of
                               the incoming header. The chunk is selected by
                               TcDfdTagCtrl.ItcHdrChunkSel0.[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */

  } Bits;
  UINT32 Data;

} ITCDBG_HDRMASK_N0_IIO_DFX_VTD_STRUCT;

/* ITCDBG_HDRMASK_N1_IIO_DFX_VTD_REG supported on:                              */
/*      SPRA0 (0x208036ac)                                                      */
/*      SPRB0 (0x208036ac)                                                      */
/*      SPRHBM (0x208036ac)                                                     */
/*      SPRC0 (0x208036ac)                                                      */
/*      SPRMCC (0x208036ac)                                                     */
/*      SPRUCC (0x208036ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* header mask for ITC DFD logic
*/


#define ITCDBG_HDRMASK_N1_IIO_DFX_VTD_REG 0x124206AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask1 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask applied to one of the 4 32-bit chunks of
                               the incoming header. The chunk is selected by
                               TcDfdTagCtrl.ItcHdrChunkSel1.[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */

  } Bits;
  UINT32 Data;

} ITCDBG_HDRMASK_N1_IIO_DFX_VTD_STRUCT;

/* ITCDBG_HDRMATCH_N0_IIO_DFX_VTD_REG supported on:                             */
/*      SPRA0 (0x208036b0)                                                      */
/*      SPRB0 (0x208036b0)                                                      */
/*      SPRHBM (0x208036b0)                                                     */
/*      SPRC0 (0x208036b0)                                                      */
/*      SPRMCC (0x208036b0)                                                     */
/*      SPRUCC (0x208036b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* header match for ITC DFD logic
*/


#define ITCDBG_HDRMATCH_N0_IIO_DFX_VTD_REG 0x124206B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Match applied to one of the 4 32-bit chunks of
                               the incoming header. The chunk is selected by
                               TcDfdTagCtrl.ItcHdrChunkSel0.[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */

  } Bits;
  UINT32 Data;

} ITCDBG_HDRMATCH_N0_IIO_DFX_VTD_STRUCT;

/* ITCDBG_HDRMATCH_N1_IIO_DFX_VTD_REG supported on:                             */
/*      SPRA0 (0x208036b4)                                                      */
/*      SPRB0 (0x208036b4)                                                      */
/*      SPRHBM (0x208036b4)                                                     */
/*      SPRC0 (0x208036b4)                                                      */
/*      SPRMCC (0x208036b4)                                                     */
/*      SPRUCC (0x208036b4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* header match for ITC DFD logic
*/


#define ITCDBG_HDRMATCH_N1_IIO_DFX_VTD_REG 0x124206B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match1 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Match applied to one of the 4 32-bit chunks of
                               the incoming header. The chunk is selected by
                               TcDfdTagCtrl.ItcHdrChunkSel1.[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */

  } Bits;
  UINT32 Data;

} ITCDBG_HDRMATCH_N1_IIO_DFX_VTD_STRUCT;

/* SPARE_SWWEST2_IIO_DFX_VTD_REG supported on:                                  */
/*      SPRA0 (0x208036b8)                                                      */
/*      SPRB0 (0x208036b8)                                                      */
/*      SPRHBM (0x208036b8)                                                     */
/*      SPRC0 (0x208036b8)                                                      */
/*      SPRMCC (0x208036b8)                                                     */
/*      SPRUCC (0x208036b8)                                                     */
/* Register default value on SPRA0: 0x80000000                                  */
/* Register default value on SPRB0: 0x80000000                                  */
/* Register default value on SPRHBM: 0x80000000                                 */
/* Register default value on SPRC0: 0x80000000                                  */
/* Register default value on SPRMCC: 0x80000000                                 */
/* Register default value on SPRUCC: 0x80000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Spare SWWest2
*/


#define SPARE_SWWEST2_IIO_DFX_VTD_REG 0x124206B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 disable_intr_in_viral : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               0 : Enable interrupts in viral mode from APIC
                               subagent[br] 1 : Disable interrupts in viral
                               mode from APIC subagent
                            */
    UINT32 disable_pm_resp_in_viral : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               0 : Enable PM_RESP from DMI to flow during viral
                               mode.[br] 1 : Disable PM_RESP from DMI to flow
                               during viral mode.
                            */
    UINT32 enable_vlw_in_viral : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               0 : Disable VLW from DMI to flow during viral
                               mode.[br] 1 : Enable VLW from DMI to flow during
                               viral mode.
                            */
    UINT32 spare_csr : 27;

                            /* Bits[30:4], Access Type=RW, default=0x00000000*/

                            /*
                               4 - csr_disable_tgt_abort_confined_p2p[br] 5 -
                               csr_disable_tgt_abort_viral[br] 7: 6 -
                               sw2cb_ext_data_stall[br] 8 -
                               csr_disable_mailbox_to_nonlegacy_dmi: prevent
                               PECI mailbox access to downstream non-legacy DMI
                               registers[br] 13 - disable_pok_port0 14 -
                               disable_pok_port1
                            */
    UINT32 disable_ro_override : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /* Disable Ro Override */

  } Bits;
  UINT32 Data;

} SPARE_SWWEST2_IIO_DFX_VTD_STRUCT;

/* CSIPOOLDFX2_IIO_DFX_VTD_REG supported on:                                    */
/*      SPRA0 (0x208036bc)                                                      */
/*      SPRB0 (0x208036bc)                                                      */
/*      SPRHBM (0x208036bc)                                                     */
/*      SPRC0 (0x208036bc)                                                      */
/*      SPRMCC (0x208036bc)                                                     */
/*      SPRUCC (0x208036bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* CSI Pool DFX 2
*/


#define CSIPOOLDFX2_IIO_DFX_VTD_REG 0x124206BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 3;

                            /* Bits[2:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 datacdt_hoard : 5;

                            /* Bits[7:3], Access Type=RW, default=0x00000000*/

                            /* Datacdt Hoard */
    UINT32 rsvd_8 : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CSIPOOLDFX2_IIO_DFX_VTD_STRUCT;

/* CSIPCTRL_0_IIO_DFX_VTD_REG supported on:                                     */
/*      SPRA0 (0x208036c4)                                                      */
/*      SPRB0 (0x208036c4)                                                      */
/*      SPRHBM (0x208036c4)                                                     */
/*      SPRC0 (0x208036c4)                                                      */
/*      SPRMCC (0x208036c4)                                                     */
/*      SPRUCC (0x208036c4)                                                     */
/* Register default value on SPRA0: 0xC0000000                                  */
/* Register default value on SPRB0: 0xC0000000                                  */
/* Register default value on SPRHBM: 0xC0000000                                 */
/* Register default value on SPRC0: 0xC0000000                                  */
/* Register default value on SPRMCC: 0xC0000000                                 */
/* Register default value on SPRUCC: 0xC0000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* CSIP Control 0
*/


#define CSIPCTRL_0_IIO_DFX_VTD_REG 0x124206C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 26;

                            /* Bits[25:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vc0_priority : 2;

                            /* Bits[27:26], Access Type=RW, default=0x00000000*/

                            /* Vc0 Priority */
    UINT32 rsvd_28 : 2;

                            /* Bits[29:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vc1_priority : 2;

                            /* Bits[31:30], Access Type=RW, default=0x00000003*/

                            /* Vc1 Priority */

  } Bits;
  UINT32 Data;

} CSIPCTRL_0_IIO_DFX_VTD_STRUCT;

/* CSI0PRPRIVC1_IIO_DFX_VTD_REG supported on:                                   */
/*      SPRA0 (0x208036cc)                                                      */
/*      SPRB0 (0x208036cc)                                                      */
/*      SPRHBM (0x208036cc)                                                     */
/*      SPRC0 (0x208036cc)                                                      */
/*      SPRMCC (0x208036cc)                                                     */
/*      SPRUCC (0x208036cc)                                                     */
/* Register default value on SPRA0: 0x20000000                                  */
/* Register default value on SPRB0: 0x20000000                                  */
/* Register default value on SPRHBM: 0x20000000                                 */
/* Register default value on SPRC0: 0x20000000                                  */
/* Register default value on SPRMCC: 0x20000000                                 */
/* Register default value on SPRUCC: 0x20000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* CSI0 PRPRIVC
*/


#define CSI0PRPRIVC1_IIO_DFX_VTD_REG 0x124206CC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 29;

                            /* Bits[28:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 use_isoc_ovf_q : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000001*/

                            /* Use Isoc Ovf Q */
    UINT32 rsvd_30 : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CSI0PRPRIVC1_IIO_DFX_VTD_STRUCT;

/* ITCPIPEHAZARD_NEW_IIO_DFX_VTD_REG supported on:                              */
/*      SPRA0 (0x208036d0)                                                      */
/*      SPRB0 (0x208036d0)                                                      */
/*      SPRHBM (0x208036d0)                                                     */
/*      SPRC0 (0x208036d0)                                                      */
/*      SPRMCC (0x208036d0)                                                     */
/*      SPRUCC (0x208036d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Constrain the Hdr and VTd pipeline arbiters to avoid certain bypass conditions
*/


#define ITCPIPEHAZARD_NEW_IIO_DFX_VTD_REG 0x124206D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disabled[br] 1 - Always enabled[br] 2 -
                               Enable based on response function 0[br] 3 -
                               Enable based on response function 1[br] 4 -
                               Enable based on response function 2
                            */
    UINT32 mode : 4;

                            /* Bits[6:3], Access Type=RW, default=0x00000000*/

                            /*
                               [0] - When set, block only requests to the same
                               FC that caused the trigger[br] [1] - When set,
                               block only requests to the same chnl that caused
                               the trigger[br] [2] - When set, block only
                               requests to the same pipe (Hdr or Vtd) that
                               caused the trigger[br] [3] - When set, block
                               only requests to the same type (e.g. Pf, FP F,
                               normal F) that caused the trigger
                            */
    UINT32 lteq : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Block requests for Thresh cycles in a
                               row[br] 1 - Block requests exactly Thresh cycles
                               behind the trigger
                            */
    UINT32 thresh : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               Number of cycles that requests in Block mask
                               will be blocked after a request in Trigger mask
                               occurs
                            */
    UINT32 trigger : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               What classes of request should trigger the
                               blocking:[br] [9] - rsvd[br] [8] - HdrF NP[br]
                               [7] - HdrF FP[br] [6] - Hdr Data[br] [5] -
                               HdrPf[br] [4] - HdrF Norm[br] [3] - rsvd[br] [2]
                               - IOMMU hit pipeline[br] [1] - IOMMU request
                               pipeline [br] [0] - rsvd[br]
                            */
    UINT32 block : 10;

                            /* Bits[31:22], Access Type=RW, default=0x00000000*/

                            /*
                               What class of request should be blocked:[br] [9]
                               - rsvd[br] [8] - HdrF NP[br] [7] - HdrF FP[br]
                               [6] - Hdr Data[br] [5] - HdrPf[br] [4] - HdrF
                               Norm[br] [3] - rsvd[br] [2] - Set this to block
                               the IOMMU request pipeline in ITC for
                               P_NP_Cpl_8_11 [1] - Set this to block the IOMMU
                               Request pipeline in ITC for Cpl_0_7 [0] - Set
                               this to block the IOMMU Request Pipeline in ITC
                               for P_NP_0_7
                            */

  } Bits;
  UINT32 Data;

} ITCPIPEHAZARD_NEW_IIO_DFX_VTD_STRUCT;

/* CSIRTCTRL_IIO_DFX_VTD_REG supported on:                                      */
/*      SPRA0 (0x208036d4)                                                      */
/*      SPRB0 (0x208036d4)                                                      */
/*      SPRHBM (0x208036d4)                                                     */
/*      SPRC0 (0x208036d4)                                                      */
/*      SPRMCC (0x208036d4)                                                     */
/*      SPRUCC (0x208036d4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* CSI Routing Control
*/


#define CSIRTCTRL_IIO_DFX_VTD_REG 0x124206D4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 inbound_routing_method : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Inbound Routing Method */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CSIRTCTRL_IIO_DFX_VTD_STRUCT;

/* ITCERRDIS_IIO_DFX_VTD_REG supported on:                                      */
/*      SPRA0 (0x208036e4)                                                      */
/*      SPRB0 (0x208036e4)                                                      */
/*      SPRHBM (0x208036e4)                                                     */
/*      SPRC0 (0x208036e4)                                                      */
/*      SPRMCC (0x208036e4)                                                     */
/*      SPRUCC (0x208036e4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Traffic Controller Error Detection defeature
*/


#define ITCERRDIS_IIO_DFX_VTD_REG 0x124206E4

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

                            /* ITC SB OF */
    UINT32 itc_sb_uf : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /* ITC SB Uf */
    UINT32 itc_iosf_cred_uf : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               ITC IOSF credit underflow enable[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_iosf_cred_of : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               ITC IOSF credit overflow enable[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_enq_data_overflow : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               ITC inbound Data FIFO overflow[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 sw2iosf_mps_err : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               IOSF MPS ERROR[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 sw2iosf_req_buf_uf : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               SW2IOSF Req Buf Underflow[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 sw2iosf_req_buf_ov : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               SW2IOSF Req Buf Overflow[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_irp_cred_uf : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               ITC IRP credit underflow enable[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_irp_cred_of : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               ITC IRP credit overflow enable[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_par_iosf_dat : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Parity error enable in the incoming data from
                               IOSF[br] Lock: IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_par_hdr_rf : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               Parity error enable in the ITC hdr_q RF[br]
                               Lock: IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_vtmisc_hdr_rf : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               Parity error enable in the ITC vtd_misc_info
                               RF[br] Lock: IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_par_addr_rf : 1;

                            /* Bits[21:21], Access Type=RW/P, default=0x00000000*/

                            /*
                               Parity error enable in the ITC addr_q RF[br]
                               Lock: IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_ecc_cor_rf : 1;

                            /* Bits[22:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               ECC corrected error enable in the ITC dat_dword
                               RF[br] Lock: IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_ecc_uncor_rf : 1;

                            /* Bits[23:23], Access Type=RW/P, default=0x00000000*/

                            /*
                               ECC uncorrected error enable in the ITC
                               dat_dword RF[br] Lock: IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_cabort : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Completer abort enable[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_mabort : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000000*/

                            /*
                               Master abort enable[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 inb_unsuccessful_cmpl : 1;

                            /* Bits[26:26], Access Type=RW/P, default=0x00000000*/

                            /*
                               MS2IOSF received an unsuccessful completion
                               (status != 0) on the IOSF interface Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_enq_overflow : 1;

                            /* Bits[27:27], Access Type=RW/P, default=0x00000000*/

                            /*
                               ITC inbound FIFO overflow[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_misc_prh_overflow : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               ITC received a misc channel prh header when a
                               prh_complete on that same channel was still
                               pending[br] Lock: IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_hw_assert : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000000*/

                            /*
                               ITC hardware assertion[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_par_iosf_cmd : 1;

                            /* Bits[30:30], Access Type=RW/P, default=0x00000000*/

                            /*
                               Parity error enable in the incoming command from
                               IOSF[br] Lock: IIO_DFX_LCK_CTL.DBGBUSLCK
                            */
    UINT32 itc_msgd_illegal_size : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               Inbound message with data length greater than 16
                               DW [br] Lock: IIO_DFX_LCK_CTL.DBGBUSLCK
                            */

  } Bits;
  UINT32 Data;

} ITCERRDIS_IIO_DFX_VTD_STRUCT;

/* ITCARBBLOCK_NEW_IIO_DFX_VTD_REG supported on:                                */
/*      SPRA0 (0x208036e8)                                                      */
/*      SPRB0 (0x208036e8)                                                      */
/*      SPRHBM (0x208036e8)                                                     */
/*      SPRC0 (0x208036e8)                                                      */
/*      SPRMCC (0x208036e8)                                                     */
/*      SPRUCC (0x208036e8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Block certain requestors from the VTd and Hdr pipelines
*/


#define ITCARBBLOCK_NEW_IIO_DFX_VTD_REG 0x124206E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disabled[br] 1 - Always enabled[br] 2 -
                               Enable based on response function 0[br] 3 -
                               Enable based on response function 1[br] 4 -
                               Enable based on response function 2
                            */
    UINT32 chnlmask : 12;

                            /* Bits[14:3], Access Type=RW, default=0x00000000*/

                            /* Channels to block */
    UINT32 fcmask : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               FCs to block:[br] [2] - C[br] [1] - NP[br] [0] -
                               P
                            */
    UINT32 typemask : 12;

                            /* Bits[29:18], Access Type=RW, default=0x00000000*/

                            /*
                               Pipelines to block:[br] [11] - rsvd[br] [10] -
                               rsvd[br] [9] - rsvd[br] [8] - HdrF NP[br] [7] -
                               HdrF FP[br] [6] - Hdr Data[br] [5] - HdrPf[br]
                               [4] - HdrF Norm[br] [3] - rsvd[br] [2] - VTdF
                               that doesnt access VTd[br] [1] - VTdF that
                               accesses VTd[br] [0] - VTdPf
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITCARBBLOCK_NEW_IIO_DFX_VTD_STRUCT;

/* OTCCREDTHROTTLE_IIO_DFX_VTD_REG supported on:                                */
/*      SPRA0 (0x208036f0)                                                      */
/*      SPRB0 (0x208036f0)                                                      */
/*      SPRHBM (0x208036f0)                                                     */
/*      SPRC0 (0x208036f0)                                                      */
/*      SPRMCC (0x208036f0)                                                     */
/*      SPRUCC (0x208036f0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Throttle credit returns to IRP
*/


#define OTCCREDTHROTTLE_IIO_DFX_VTD_REG 0x124206F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disabled[br] 1 - Always enabled[br] 2 -
                               Enable based on response function 0[br] 3 -
                               Enable based on response function 1[br] 4 -
                               Enable based on response function 2
                            */
    UINT32 throttleprh : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Throttle return of Prh credits to IRP when
                               throttling is enabled via OtcCredThrottle.Enable
                            */
    UINT32 throttleprd : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Throttle return of Prd credits to IRP when
                               throttling is enabled via OtcCredThrottle.Enable
                            */
    UINT32 throttlenprh : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Throttle return of Nprh credits to IRP when
                               throttling is enabled via OtcCredThrottle.Enable
                            */
    UINT32 throttlenprd : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Throttle return of Nprd credits to IRP when
                               throttling is enabled via OtcCredThrottle.Enable
                            */
    UINT32 throttlecph : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Throttle return of Cph credits to IRP when
                               throttling is enabled via OtcCredThrottle.Enable
                            */
    UINT32 throttlecpd : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Throttle return of Cpd credits to IRP when
                               throttling is enabled via OtcCredThrottle.Enable
                            */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCCREDTHROTTLE_IIO_DFX_VTD_STRUCT;

/* ITCPIPEHAZARD_IIO_DFX_VTD_REG supported on:                                  */
/*      SPRA0 (0x208036f4)                                                      */
/*      SPRB0 (0x208036f4)                                                      */
/*      SPRHBM (0x208036f4)                                                     */
/*      SPRC0 (0x208036f4)                                                      */
/*      SPRMCC (0x208036f4)                                                     */
/*      SPRUCC (0x208036f4)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Constrain the Hdr and VTd pipeline arbiters to avoid certain bypass conditions
*/


#define ITCPIPEHAZARD_IIO_DFX_VTD_REG 0x124206F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disabled[br] 1 - Always enabled[br] 2 -
                               Enable based on response function 0[br] 3 -
                               Enable based on response function 1[br] 4 -
                               Enable based on response function 2
                            */
    UINT32 mode : 4;

                            /* Bits[6:3], Access Type=RW, default=0x00000000*/

                            /*
                               [0] - When set, block only requests to the same
                               FC that caused the trigger[br] [1] - When set,
                               block only requests to the same chnl that caused
                               the trigger[br] [2] - When set, block only
                               requests to the same pipe (Hdr or Vtd) that
                               caused the trigger[br] [3] - When set, block
                               only requests to the same type (e.g. Pf, FP F,
                               normal F) that caused the trigger
                            */
    UINT32 lteq : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Block requests for Thresh cycles in a
                               row[br] 1 - Block requests exactly Thresh cycles
                               behind the trigger
                            */
    UINT32 thresh : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               Number of cycles that requests in Block mask
                               will be blocked after a request in Trigger mask
                               occurs
                            */
    UINT32 trigger : 10;

                            /* Bits[21:12], Access Type=RW, default=0x00000000*/

                            /*
                               What classes of request should trigger the
                               blocking:[br] [9] - rsvd[br] [8] - HdrF NP[br]
                               [7] - HdrF FP[br] [6] - Hdr Data[br] [5] -
                               HdrPf[br] [4] - HdrF Norm[br] [3] - rsvd[br] [2]
                               - IOMMU hit pipeline[br] [1] - IOMMU request
                               pipeline [br] [0] - rsvd[br]
                            */
    UINT32 block : 10;

                            /* Bits[31:22], Access Type=RW, default=0x00000000*/

                            /*
                               What class of request should be blocked:[br] [9]
                               - rsvd[br] [8] - HdrF NP[br] [7] - HdrF FP[br]
                               [6] - Hdr Data[br] [5] - HdrPf[br] [4] - HdrF
                               Norm[br] [3] - rsvd[br] [2] - Set this to block
                               the IOMMU request pipeline in ITC for
                               P_NP_Cpl_8_11 [1] - Set this to block the IOMMU
                               Request pipeline in ITC for Cpl_0_7 [0] - Set
                               this to block the IOMMU Request Pipeline in ITC
                               for P_NP_0_7
                            */

  } Bits;
  UINT32 Data;

} ITCPIPEHAZARD_IIO_DFX_VTD_STRUCT;

/* ITCARBBLOCK_IIO_DFX_VTD_REG supported on:                                    */
/*      SPRA0 (0x208036f8)                                                      */
/*      SPRB0 (0x208036f8)                                                      */
/*      SPRHBM (0x208036f8)                                                     */
/*      SPRC0 (0x208036f8)                                                      */
/*      SPRMCC (0x208036f8)                                                     */
/*      SPRUCC (0x208036f8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Block certain requestors from the VTd and Hdr pipelines
*/


#define ITCARBBLOCK_IIO_DFX_VTD_REG 0x124206F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disabled[br] 1 - Always enabled[br] 2 -
                               Enable based on response function 0[br] 3 -
                               Enable based on response function 1[br] 4 -
                               Enable based on response function 2
                            */
    UINT32 chnlmask : 12;

                            /* Bits[14:3], Access Type=RW, default=0x00000000*/

                            /* Channels to block */
    UINT32 fcmask : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               FCs to block:[br] [2] - C[br] [1] - NP[br] [0] -
                               P
                            */
    UINT32 typemask : 12;

                            /* Bits[29:18], Access Type=RW, default=0x00000000*/

                            /*
                               Pipelines to block:[br] [11] - rsvd[br] [10] -
                               rsvd[br] [9] - rsvd[br] [8] - HdrF NP[br] [7] -
                               HdrF FP[br] [6] - Hdr Data[br] [5] - HdrPf[br]
                               [4] - HdrF Norm[br] [3] - rsvd[br] [2] - VTdF
                               that doesnt access VTd[br] [1] - VTdF that
                               accesses VTd[br] [0] - VTdPf
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITCARBBLOCK_IIO_DFX_VTD_STRUCT;

/* OTCPIPEHAZARD_IIO_DFX_VTD_REG supported on:                                  */
/*      SPRA0 (0x20803700)                                                      */
/*      SPRB0 (0x20803700)                                                      */
/*      SPRHBM (0x20803700)                                                     */
/*      SPRC0 (0x20803700)                                                      */
/*      SPRMCC (0x20803700)                                                     */
/*      SPRUCC (0x20803700)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Constrain the Hdr and VTd pipeline arbiters to avoid certain bypass conditions
*/


#define OTCPIPEHAZARD_IIO_DFX_VTD_REG 0x12420700

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disabled[br] 1 - Always enabled[br] 2 -
                               Enable based on response function 0[br] 3 -
                               Enable based on response function 1[br] 4 -
                               Enable based on response function 2
                            */
    UINT32 mode : 4;

                            /* Bits[6:3], Access Type=RW, default=0x00000000*/

                            /*
                               [0] - When set, block only requests to the same
                               FC that caused the trigger[br] [1] - When set,
                               block only requests to the same chnl that caused
                               the trigger[br] [2] - When set, block only
                               requests to the same pipe (Hdr or Vtd) that
                               caused the trigger[br] [3] - When set, block
                               only requests to the same type (e.g. Pf, FP F,
                               normal F) that caused the trigger
                            */
    UINT32 lteq : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Block requests for Thresh cycles in a
                               row[br] 1 - Block requests exactly Thresh cycles
                               behind the trigger
                            */
    UINT32 thresh : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /*
                               Number of cycles that requests in Block mask
                               will be blocked after a request in Trigger mask
                               occurs
                            */
    UINT32 trigger : 8;

                            /* Bits[19:12], Access Type=RW, default=0x00000000*/

                            /*
                               What class of request should trigger the
                               blocking:[br] [7] - HdrF Data (not used for
                               OTC)[br] [6] - HdrPf (not used for OTC)[br] [5]
                               - HdrF Fastpath (not used for OTC)[br] [4] -
                               HdrF Norm[br] [3] - rsvd[br] [2] - rsvd[br] [1]
                               - VTdF[br] [0] - VTdPf (not used in OTC)
                            */
    UINT32 block : 8;

                            /* Bits[27:20], Access Type=RW, default=0x00000000*/

                            /*
                               What class of request should be blocked:[br] [7]
                               - HdrF Data (not used for OTC)[br] [6] - HdrPf
                               (not used for OTC)[br] [5] - HdrF Fastpath (not
                               used for OTC)[br] [4] - HdrF Norm[br] [3] -
                               rsvd[br] [2] - rsvd[br] [1] - VTdF[br] [0] -
                               VTdPf (not used in OTC)
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCPIPEHAZARD_IIO_DFX_VTD_STRUCT;

/* OTCARBBLOCK_IIO_DFX_VTD_REG supported on:                                    */
/*      SPRA0 (0x20803704)                                                      */
/*      SPRB0 (0x20803704)                                                      */
/*      SPRHBM (0x20803704)                                                     */
/*      SPRC0 (0x20803704)                                                      */
/*      SPRMCC (0x20803704)                                                     */
/*      SPRUCC (0x20803704)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Block certain requestors from the VTd and Hdr pipelines
*/


#define OTCARBBLOCK_IIO_DFX_VTD_REG 0x12420704

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disabled[br] 1 - Always enabled[br] 2 -
                               Enable based on response function 0[br] 3 -
                               Enable based on response function 1[br] 4 -
                               Enable based on response function 2
                            */
    UINT32 chnlmask : 12;

                            /* Bits[14:3], Access Type=RW, default=0x00000000*/

                            /* Channels to block */
    UINT32 fcmask : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               FCs to block:[br] [2] - C[br] [1] - NP[br] [0] -
                               P
                            */
    UINT32 typemask : 8;

                            /* Bits[25:18], Access Type=RW, default=0x00000000*/

                            /*
                               Pipelines to block:[br] [7] - rsvd[br] [6] -
                               HdrPf[br] [5] - rsvd[br] [4] - HdrF Norm[br] [3]
                               - rsvd[br] [2] - VTdF[br] [1] - rsvd[br] [0] -
                               rsvd
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCARBBLOCK_IIO_DFX_VTD_STRUCT;

/* ITCFRCREJECT_IIO_DFX_VTD_REG supported on:                                   */
/*      SPRA0 (0x20803708)                                                      */
/*      SPRB0 (0x20803708)                                                      */
/*      SPRHBM (0x20803708)                                                     */
/*      SPRC0 (0x20803708)                                                      */
/*      SPRMCC (0x20803708)                                                     */
/*      SPRUCC (0x20803708)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Prevent requests from certain VTd and Hdr pipes from advancing to the next stage
*/


#define ITCFRCREJECT_IIO_DFX_VTD_REG 0x12420708

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disabled[br] 1 - Always enabled[br] 2 -
                               Enable based on response function 0[br] 3 -
                               Enable based on response function 1[br] 4 -
                               Enable based on response function 2
                            */
    UINT32 chnlmask : 8;

                            /* Bits[10:3], Access Type=RW, default=0x00000000*/

                            /* Channels to block */
    UINT32 fcmask : 3;

                            /* Bits[13:11], Access Type=RW, default=0x00000000*/

                            /*
                               FCs to block:[br] [2] - C[br] [1] - NP[br] [0] -
                               P
                            */
    UINT32 typemask : 6;

                            /* Bits[19:14], Access Type=RW, default=0x00000000*/

                            /*
                               Pipelines to block:[br] [5] - Hdr Data[br] [4] -
                               Hdr FP F[br] [3] - Hdr Normal F[br] [2] - Hdr
                               Pf[br] [1] - VTd F[br] [0] - VTd Pf
                            */
    UINT32 rejmode : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               When 0, always reject; when 1, reject only when
                               the following stage has more requests than the
                               configured threshold
                            */
    UINT32 threshold : 10;

                            /* Bits[30:21], Access Type=RW, default=0x00000000*/

                            /*
                               Number of requests in a stage above which the
                               previous stage will be backpressured
                            */
    UINT32 rsvd : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITCFRCREJECT_IIO_DFX_VTD_STRUCT;

/* OTCRSPFUNC_IIO_DFX_VTD_REG supported on:                                     */
/*      SPRA0 (0x2080370c)                                                      */
/*      SPRB0 (0x2080370c)                                                      */
/*      SPRHBM (0x2080370c)                                                     */
/*      SPRC0 (0x2080370c)                                                      */
/*      SPRMCC (0x2080370c)                                                     */
/*      SPRUCC (0x2080370c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Select the start and stop triggers for the response logic
*/


#define OTCRSPFUNC_IIO_DFX_VTD_REG 0x1242070C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 startsel0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* Startsel0 */
    UINT32 stopsel0 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /* Stopsel0 */
    UINT32 startsel1 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /* Startsel1 */
    UINT32 stopsel1 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /* Stopsel1 */
    UINT32 startsel2 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /* Startsel2 */
    UINT32 stopsel2 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /* Stopsel2 */
    UINT32 pto_en : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable OTC as a DFD trigger source, via the PTO
                               signal.[br] Currently, the only PTO input is the
                               enqueue header mask/match.
                            */
    UINT32 rsvd : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCRSPFUNC_IIO_DFX_VTD_STRUCT;

/* ITCRSPFUNC_IIO_DFX_VTD_REG supported on:                                     */
/*      SPRA0 (0x20803710)                                                      */
/*      SPRB0 (0x20803710)                                                      */
/*      SPRHBM (0x20803710)                                                     */
/*      SPRC0 (0x20803710)                                                      */
/*      SPRMCC (0x20803710)                                                     */
/*      SPRUCC (0x20803710)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Select the start and stop triggers for the response logic
*/


#define ITCRSPFUNC_IIO_DFX_VTD_REG 0x12420710

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 startsel0 : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000000*/

                            /* Startsel0 */
    UINT32 stopsel0 : 4;

                            /* Bits[7:4], Access Type=RW/P, default=0x00000000*/

                            /* Stopsel0 */
    UINT32 startsel1 : 4;

                            /* Bits[11:8], Access Type=RW/P, default=0x00000000*/

                            /* Startsel1 */
    UINT32 stopsel1 : 4;

                            /* Bits[15:12], Access Type=RW/P, default=0x00000000*/

                            /* Stopsel1 */
    UINT32 startsel2 : 4;

                            /* Bits[19:16], Access Type=RW/P, default=0x00000000*/

                            /* Startsel2 */
    UINT32 stopsel2 : 4;

                            /* Bits[23:20], Access Type=RW/P, default=0x00000000*/

                            /* Stopsel2 */
    UINT32 pto_en : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable ITC as a DFD trigger source, via the PTO
                               signal.[br] Currently, the only PTO input is the
                               enqueue header mask/match.
                            */
    UINT32 rsvd : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITCRSPFUNC_IIO_DFX_VTD_STRUCT;

/* OTCMISCARBBLOCK_IIO_DFX_VTD_REG supported on:                                */
/*      SPRA0 (0x20803714)                                                      */
/*      SPRB0 (0x20803714)                                                      */
/*      SPRHBM (0x20803714)                                                     */
/*      SPRC0 (0x20803714)                                                      */
/*      SPRMCC (0x20803714)                                                     */
/*      SPRUCC (0x20803714)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Block certain requestors from access to ITC
*/


#define OTCMISCARBBLOCK_IIO_DFX_VTD_REG 0x12420714

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disabled[br] 1 - Always enabled[br] 2 -
                               Enable based on response function 0[br] 3 -
                               Enable based on response function 1[br] 4 -
                               Enable based on response function 2
                            */
    UINT32 mask : 21;

                            /* Bits[23:3], Access Type=RW, default=0x00000000*/

                            /*
                               [0] - VC1 NP [1] - VC1 P[br] [2] - VC0 NP[br]
                               [3] - VC0 P[br] [4] - Msg P[br] [5] - APIC P[br]
                               [6] - IEH P[br] [7] - IEH C[br] [8] - INTEXGPE
                               P[br] [9] - Msg C[br] [10] - Cfg C[br] [11] -
                               OOBMSM NP[br] [12] - OOBMSM P[br] [13] - VC2
                               NP[br] [14] - GLBARB_VC1_NP [15] - GLBARB_VC1_P
                               [16] - GLBARB_VC0P_NP [77] - GLBARB_VC0_P [18] -
                               GLBARB_MISC_C [19] - GLBARB_MISC_NP [20] -
                               GLBARB_MISC_P
                            */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCMISCARBBLOCK_IIO_DFX_VTD_STRUCT;

/* OTCSKIDBUFFSURVIVEDFT_IIO_DFX_VTD_REG supported on:                          */
/*      SPRA0 (0x20803718)                                                      */
/*      SPRB0 (0x20803718)                                                      */
/*      SPRHBM (0x20803718)                                                     */
/*      SPRC0 (0x20803718)                                                      */
/*      SPRMCC (0x20803718)                                                     */
/*      SPRUCC (0x20803718)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* TC Skid buffer arb and credit throttling survivability hook
*/


#define OTCSKIDBUFFSURVIVEDFT_IIO_DFX_VTD_REG 0x12420718

#if defined(SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 primarbblksrc : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Select primary source for arb blocking used by
                               response function: 0 - Disabled 1 - Always
                               enabled 2 - Response function 0 3 - Response
                               function 1 4 - Response function 2
                            */
    UINT32 secondaryarbblksrcskew : 4;

                            /* Bits[6:3], Access Type=RW, default=0x00000000*/

                            /*
                               Select skew for secondary source for arb
                               blocking relative to primary source from 0 to 15
                               cycles
                            */
    UINT32 enitcarbblk : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Enable blocking in skid buffer for ITC traffic
                               into OTC
                            */
    UINT32 enirparbblk : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Enable blocking in skid buffer for IRP traffic
                               into OTC
                            */
    UINT32 itcarbblksrc : 2;

                            /* Bits[10:9], Access Type=RW, default=0x00000000*/

                            /*
                               Select source for ITC arb block 0 - primary
                               source 1 - inverted primary source 2 - secondary
                               source 3 - inverted secondary source
                            */
    UINT32 irparbblksrc : 2;

                            /* Bits[12:11], Access Type=RW, default=0x00000000*/

                            /*
                               Select source for IRP arb block 0 - primary
                               source 1 - inverted primary source 2 - secondary
                               source 3 - inverted secondary source
                            */
    UINT32 rsvd : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cdtthrottlesrc : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Select source for throttling for credit returns:
                               1 - Always enabled 2 - Enable based on response
                               function 0 3 - Enable based on response function
                               1 4 - Enable based on response function 2
                            */
    UINT32 itchdrcdtthrottle : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /* Enable credit throttling for ITC header credits */
    UINT32 itcdatcdtthrottle : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /* Enable credit throttling for ITC data credits */
    UINT32 irphdrcdtthrottle : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* Enable credit throttling for IRP header credits */
    UINT32 irpdatcdtthrottle : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* Enable credit throttling for IRP data credits */
    UINT32 rsvd_23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCSKIDBUFFSURVIVEDFT_IIO_DFX_VTD_SPRA0_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRA0_HOST) || defined(SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 primarbblksrc : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Select primary source for arb blocking used by
                               response function: 0 - Disabled 1 - Always
                               enabled 2 - Response function 0 3 - Response
                               function 1 4 - Response function 2
                            */
    UINT32 secondaryarbblksrcskew : 4;

                            /* Bits[6:3], Access Type=RW, default=0x00000000*/

                            /*
                               Select skew for secondary source for arb
                               blocking relative to primary source from 0 to 15
                               cycles
                            */
    UINT32 enitcarbblk : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Enable blocking in skid buffer for ITC traffic
                               into OTC
                            */
    UINT32 enirparbblk : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Enable blocking in skid buffer for IRP traffic
                               into OTC
                            */
    UINT32 itcarbblksrc : 2;

                            /* Bits[10:9], Access Type=RW, default=0x00000000*/

                            /*
                               Select source for ITC arb block 0 - primary
                               source 1 - inverted primary source 2 - secondary
                               source 3 - inverted secondary source
                            */
    UINT32 irparbblksrc : 2;

                            /* Bits[12:11], Access Type=RW, default=0x00000000*/

                            /*
                               Select source for IRP arb block 0 - primary
                               source 1 - inverted primary source 2 - secondary
                               source 3 - inverted secondary source
                            */
    UINT32 rsvd : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cdtthrottlesrc : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Select source for throttling for credit returns:
                               1 - Always enabled 2 - Enable based on response
                               function 0 3 - Enable based on response function
                               1 4 - Enable based on response function 2
                            */
    UINT32 itcpchdrcdtthrottle : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Enable credit throttling for ITC P/Cmp header
                               skid buffer credits
                            */
    UINT32 itcpcdatcdtthrottle : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Enable credit throttling for ITC P/Cmp data skid
                               buffer credits
                            */
    UINT32 irphdrcdtthrottle : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* Enable credit throttling for IRP header credits */
    UINT32 irpdatcdtthrottle : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* Enable credit throttling for IRP data credits */
    UINT32 itcnphdrcdtthrottle : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Enable credit throttling for ITC Np header skid
                               buffer credits
                            */
    UINT32 itcnpdatcdtthrottle : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Enable credit throttling for ITC Np data skid
                               buffer credits
                            */
    UINT32 rsvd_25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCSKIDBUFFSURVIVEDFT_IIO_DFX_VTD_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 primarbblksrc : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Select primary source for arb blocking used by
                               response function: 0 - Disabled 1 - Always
                               enabled 2 - Response function 0 3 - Response
                               function 1 4 - Response function 2
                            */
    UINT32 secondaryarbblksrcskew : 4;

                            /* Bits[6:3], Access Type=RW, default=0x00000000*/

                            /*
                               Select skew for secondary source for arb
                               blocking relative to primary source from 0 to 15
                               cycles
                            */
    UINT32 enitcarbblk : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Enable blocking in skid buffer for ITC traffic
                               into OTC
                            */
    UINT32 enirparbblk : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Enable blocking in skid buffer for IRP traffic
                               into OTC
                            */
    UINT32 itcarbblksrc : 2;

                            /* Bits[10:9], Access Type=RW, default=0x00000000*/

                            /*
                               Select source for ITC arb block 0 - primary
                               source 1 - inverted primary source 2 - secondary
                               source 3 - inverted secondary source
                            */
    UINT32 irparbblksrc : 2;

                            /* Bits[12:11], Access Type=RW, default=0x00000000*/

                            /*
                               Select source for IRP arb block 0 - primary
                               source 1 - inverted primary source 2 - secondary
                               source 3 - inverted secondary source
                            */
    UINT32 rsvd : 3;

                            /* Bits[15:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cdtthrottlesrc : 3;

                            /* Bits[18:16], Access Type=RW, default=0x00000000*/

                            /*
                               Select source for throttling for credit returns:
                               1 - Always enabled 2 - Enable based on response
                               function 0 3 - Enable based on response function
                               1 4 - Enable based on response function 2
                            */
    UINT32 rsvd_19 : 2;

                            /* Bits[20:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 irphdrcdtthrottle : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /* Enable credit throttling for IRP header credits */
    UINT32 irpdatcdtthrottle : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /* Enable credit throttling for IRP data credits */
    UINT32 rsvd_23 : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCSKIDBUFFSURVIVEDFT_IIO_DFX_VTD_STRUCT;

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* LOCKBLOCK_DISABLE_IIO_DFX_VTD_REG supported on:                              */
/*      SPRB0 (0x2080371c)                                                      */
/*      SPRHBM (0x2080371c)                                                     */
/*      SPRC0 (0x2080371c)                                                      */
/*      SPRMCC (0x2080371c)                                                     */
/*      SPRUCC (0x2080371c)                                                     */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRB0 BDF: 8_0_3                                       */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Controls for unblocking selected channel and fc momentarily
*/


#define LOCKBLOCK_DISABLE_IIO_DFX_VTD_REG 0x1242071C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               0 - Disabled[br] 1 - Always enabled[br] 2 -
                               Enable based on response function 0[br] 3 -
                               Enable based on response function 1[br] 4 -
                               Enable based on response function 2 5 - Enable
                               WatchDog Mode
                            */
    UINT32 chnlmask : 12;

                            /* Bits[14:3], Access Type=RW, default=0x00000000*/

                            /* Channels to unblock */
    UINT32 fcmask : 3;

                            /* Bits[17:15], Access Type=RW, default=0x00000000*/

                            /*
                               FCs to unblock:[br] [2] - C[br] [1] - NP[br] [0]
                               - P
                            */
    UINT32 watchdogperiod : 2;

                            /* Bits[19:18], Access Type=RW, default=0x00000000*/

                            /*
                               Period for Watchdog timer 00 - 32'hFFFFFFFF 01 -
                               32'hEFFFFFFF 10 - 32'hDFFFFFFF 11 - 32'hCFFFFFFF
                            */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LOCKBLOCK_DISABLE_IIO_DFX_VTD_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

/* OTCDBG_HDRMASK_N0_IIO_DFX_VTD_REG supported on:                              */
/*      SPRA0 (0x20803720)                                                      */
/*      SPRB0 (0x20803720)                                                      */
/*      SPRHBM (0x20803720)                                                     */
/*      SPRC0 (0x20803720)                                                      */
/*      SPRMCC (0x20803720)                                                     */
/*      SPRUCC (0x20803720)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* header mask for OTC DFD logic
*/


#define OTCDBG_HDRMASK_N0_IIO_DFX_VTD_REG 0x12420720

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask applied to one of the 4 32-bit chunks of
                               the incoming header. The chunk is selected by
                               TcDfdTagCtrl.OtcHdrChunkSel0.[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */

  } Bits;
  UINT32 Data;

} OTCDBG_HDRMASK_N0_IIO_DFX_VTD_STRUCT;

/* OTCDBG_HDRMASK_N1_IIO_DFX_VTD_REG supported on:                              */
/*      SPRA0 (0x20803724)                                                      */
/*      SPRB0 (0x20803724)                                                      */
/*      SPRHBM (0x20803724)                                                     */
/*      SPRC0 (0x20803724)                                                      */
/*      SPRMCC (0x20803724)                                                     */
/*      SPRUCC (0x20803724)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* header mask for OTC DFD logic
*/


#define OTCDBG_HDRMASK_N1_IIO_DFX_VTD_REG 0x12420724

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mask1 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Mask applied to one of the 4 32-bit chunks of
                               the incoming header. The chunk is selected by
                               TcDfdTagCtrl.OtcHdrChunkSel1.[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */

  } Bits;
  UINT32 Data;

} OTCDBG_HDRMASK_N1_IIO_DFX_VTD_STRUCT;

/* OTCDBG_HDRMATCH_N0_IIO_DFX_VTD_REG supported on:                             */
/*      SPRA0 (0x20803728)                                                      */
/*      SPRB0 (0x20803728)                                                      */
/*      SPRHBM (0x20803728)                                                     */
/*      SPRC0 (0x20803728)                                                      */
/*      SPRMCC (0x20803728)                                                     */
/*      SPRUCC (0x20803728)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* header match for OTC DFD logic
*/


#define OTCDBG_HDRMATCH_N0_IIO_DFX_VTD_REG 0x12420728

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match0 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Match applied to one of the 4 32-bit chunks of
                               the incoming header. The chunk is selected by
                               TcDfdTagCtrl.OtcHdrChunkSel0.[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */

  } Bits;
  UINT32 Data;

} OTCDBG_HDRMATCH_N0_IIO_DFX_VTD_STRUCT;

/* OTCDBG_HDRMATCH_N1_IIO_DFX_VTD_REG supported on:                             */
/*      SPRA0 (0x2080372c)                                                      */
/*      SPRB0 (0x2080372c)                                                      */
/*      SPRHBM (0x2080372c)                                                     */
/*      SPRC0 (0x2080372c)                                                      */
/*      SPRMCC (0x2080372c)                                                     */
/*      SPRUCC (0x2080372c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* header match for OTC DFD logic
*/


#define OTCDBG_HDRMATCH_N1_IIO_DFX_VTD_REG 0x1242072C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 match1 : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Match applied to one of the 4 32-bit chunks of
                               the incoming header. The chunk is selected by
                               TcDfdTagCtrl.OtcHdrChunkSel1.[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */

  } Bits;
  UINT32 Data;

} OTCDBG_HDRMATCH_N1_IIO_DFX_VTD_STRUCT;

/* TCDBG_TXNTAG_N0_IIO_DFX_VTD_REG supported on:                                */
/*      SPRA0 (0x20803730)                                                      */
/*      SPRB0 (0x20803730)                                                      */
/*      SPRHBM (0x20803730)                                                     */
/*      SPRC0 (0x20803730)                                                      */
/*      SPRMCC (0x20803730)                                                     */
/*      SPRUCC (0x20803730)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Control for transactional tagging
*/


#define TCDBG_TXNTAG_N0_IIO_DFX_VTD_REG 0x12420730

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 res_enable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable transactional tagging[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 itc_tag_requests : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable tagging of new inbound requests based on
                               the mask/match logic in itcdbg_hdrmask and
                               itcdbg_hdrmatch[br] Notes:[br] Locked by
                               DBGBUSLCK
                            */
    UINT32 otc_tag_requests : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable tagging of new outbound requests based on
                               the mask/match logic in otcdbg_hdrmask and
                               otcdbg_hdrmatch[br] Notes:[br] Locked by
                               DBGBUSLCK
                            */
    UINT32 frequency : 2;

                            /* Bits[4:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               Frequency of tagging (within ITC or OTC):[br] 00
                               - Tag all matching requests[br] 01 - Only allow
                               one outstanding tagged request within ITC or
                               OTC[br] 10 - Only allow one tagged request ever
                               (can be cleared by writing 0 to the tag_done
                               bit, or by doing a warm reset)[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 itc_tag_done : 1;

                            /* Bits[5:5], Access Type=RW/V, default=0x00000000*/

                            /*
                               Transaction has been tagged in ITC[br]
                               Notes:[br] Locked by DBGBUSLCK
                            */
    UINT32 otc_tag_done : 1;

                            /* Bits[6:6], Access Type=RW/V, default=0x00000000*/

                            /*
                               Transaction has been tagged in OTC[br]
                               Notes:[br] Locked by DBGBUSLCK
                            */
    UINT32 itc_tag_in_queue : 1;

                            /* Bits[7:7], Access Type=RW/V, default=0x00000000*/

                            /*
                               Tagged transaction is in ITC[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 otc_tag_in_queue : 1;

                            /* Bits[8:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               Tagged transaction is in OTC[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 itcchunksel0 : 2;

                            /* Bits[10:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               Which 32-bit chunk of the ITC header to apply
                               the itcdbg_hdr_mask/match0 to[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 itcchunksel1 : 2;

                            /* Bits[12:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               Which 32-bit chunk of the ITC header to apply
                               the itcdbg_hdr_mask/match1 to[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 otcchunksel0 : 2;

                            /* Bits[14:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               Which 32-bit chunk of the OTC header to apply
                               the otcdbg_hdr_mask/match0 to[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 otcchunksel1 : 2;

                            /* Bits[16:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               Which 32-bit chunk of the OTC header to apply
                               the otcdbg_hdr_mask/match1 to[br] Notes:[br]
                               Locked by DBGBUSLCK
                            */
    UINT32 en_irp_mem_tag : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable IRP tagging for Memory reads and writes.
                               Along with this, you need to reduce number of
                               ctag entries and number of FAF SQIDs to less
                               than 32.[br] Notes:[br] Locked by DBGBUSLCK
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TCDBG_TXNTAG_N0_IIO_DFX_VTD_STRUCT;

/* TCDBG_TXNTAG_N1_IIO_DFX_VTD_REG supported on:                                */
/*      SPRA0 (0x20803734)                                                      */
/*      SPRB0 (0x20803734)                                                      */
/*      SPRHBM (0x20803734)                                                     */
/*      SPRC0 (0x20803734)                                                      */
/*      SPRMCC (0x20803734)                                                     */
/*      SPRUCC (0x20803734)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Control for transactional tagging
*/


#define TCDBG_TXNTAG_N1_IIO_DFX_VTD_REG 0x12420734

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

} TCDBG_TXNTAG_N1_IIO_DFX_VTD_STRUCT;

/* OTCASSERTLOG_IIO_DFX_VTD_REG supported on:                                   */
/*      SPRA0 (0x20803780)                                                      */
/*      SPRB0 (0x20803780)                                                      */
/*      SPRHBM (0x20803780)                                                     */
/*      SPRC0 (0x20803780)                                                      */
/*      SPRMCC (0x20803780)                                                     */
/*      SPRUCC (0x20803780)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Inbound traffic controller hardware assert log
*/


#define OTCASSERTLOG_IIO_DFX_VTD_REG 0x12420780

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 assert_log0 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log1 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log2 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log3 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log4 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log5 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log6 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log7 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCASSERTLOG_IIO_DFX_VTD_STRUCT;

/* OTCASSERTMCA_IIO_DFX_VTD_REG supported on:                                   */
/*      SPRA0 (0x20803788)                                                      */
/*      SPRB0 (0x20803788)                                                      */
/*      SPRHBM (0x20803788)                                                     */
/*      SPRC0 (0x20803788)                                                      */
/*      SPRMCC (0x20803788)                                                     */
/*      SPRUCC (0x20803788)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Outbound traffic controller hardware assert MCA enable
*/


#define OTCASSERTMCA_IIO_DFX_VTD_REG 0x12420788

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0: No
                               MCA will be triggered if the check fails[br] 1:
                               OTC_HW_ASSERT machine check wiil be triggered if
                               check fails
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCASSERTMCA_IIO_DFX_VTD_STRUCT;

/* ITCASSERTLOG_IIO_DFX_VTD_REG supported on:                                   */
/*      SPRA0 (0x20803790)                                                      */
/*      SPRB0 (0x20803790)                                                      */
/*      SPRHBM (0x20803790)                                                     */
/*      SPRC0 (0x20803790)                                                      */
/*      SPRMCC (0x20803790)                                                     */
/*      SPRUCC (0x20803790)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Outbound traffic controller hardware assert log
*/


#define ITCASSERTLOG_IIO_DFX_VTD_REG 0x12420790

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 assert_log0 : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log1 : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log2 : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log3 : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log4 : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log5 : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log6 : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log7 : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log8 : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log9 : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log10 : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 assert_log11 : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0:
                               The check did not file since the log was
                               cleared[br] 1: The check failed since the log
                               was last cleared.
                            */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITCASSERTLOG_IIO_DFX_VTD_STRUCT;

/* ITCASSERTMCA_IIO_DFX_VTD_REG supported on:                                   */
/*      SPRA0 (0x20803798)                                                      */
/*      SPRB0 (0x20803798)                                                      */
/*      SPRHBM (0x20803798)                                                     */
/*      SPRC0 (0x20803798)                                                      */
/*      SPRMCC (0x20803798)                                                     */
/*      SPRUCC (0x20803798)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Outbound traffic controller hardware assert MCA enable
*/


#define ITCASSERTMCA_IIO_DFX_VTD_REG 0x12420798

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable : 12;

                            /* Bits[11:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Each bit represents a consistency check
                               implemented in hardware. For each bit:[br] 0: No
                               MCA will be triggered if the check fails[br] 1:
                               ITC_HW_ASSERT machine check wiil be triggered if
                               check fails
                            */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITCASSERTMCA_IIO_DFX_VTD_STRUCT;

/* IRP_MISC_DFX0_IIO_DFX_VTD_REG supported on:                                  */
/*      SPRA0 (0x20803800)                                                      */
/*      SPRB0 (0x20803800)                                                      */
/*      SPRHBM (0x20803800)                                                     */
/*      SPRC0 (0x20803800)                                                      */
/*      SPRMCC (0x20803800)                                                     */
/*      SPRUCC (0x20803800)                                                     */
/* Register default value on SPRA0: 0xC2FF3186                                  */
/* Register default value on SPRB0: 0xC0FF3186                                  */
/* Register default value on SPRHBM: 0xC0FF3186                                 */
/* Register default value on SPRC0: 0xC0FF3186                                  */
/* Register default value on SPRMCC: 0xC0FF3186                                 */
/* Register default value on SPRUCC: 0xC0FF3186                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* DFX register 0 for IRP/IRPRING
*/


#define IRP_MISC_DFX0_IIO_DFX_VTD_REG 0x12420800

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 p_pf_snp_rsp_holdoff_cnt : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000006*/
    UINT32 rsvd_4 : 25;

                            /* Bits[28:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 force_no_snp_on_vc1_vcm : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/
    UINT32 rsvd_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 en_poison_read_cmpl_for_hf : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

  } Bits;
  UINT32 Data;

} IRP_MISC_DFX0_IIO_DFX_VTD_STRUCT;

/* IRP_MISC_DFX1_IIO_DFX_VTD_REG supported on:                                  */
/*      SPRA0 (0x20803804)                                                      */
/*      SPRB0 (0x20803804)                                                      */
/*      SPRHBM (0x20803804)                                                     */
/*      SPRC0 (0x20803804)                                                      */
/*      SPRMCC (0x20803804)                                                     */
/*      SPRUCC (0x20803804)                                                     */
/* Register default value on SPRA0: 0xBD100000                                  */
/* Register default value on SPRB0: 0xBD100000                                  */
/* Register default value on SPRHBM: 0xBD100000                                 */
/* Register default value on SPRC0: 0xBD100000                                  */
/* Register default value on SPRMCC: 0xBD100000                                 */
/* Register default value on SPRUCC: 0xBD100000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* DFX register 1 for IRP/IRPRING
*/


#define IRP_MISC_DFX1_IIO_DFX_VTD_REG 0x12420804

#if defined(SPRA0_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 22;

                            /* Bits[21:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 spare3 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/
    UINT32 rsvd_23 : 7;

                            /* Bits[29:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 wb_arb_pri_ovr_pf_pri_disable : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               disables writeback arb priory over prefetch
                               priority for 10nm -- puts us back to SKX
                               priority
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRP_MISC_DFX1_IIO_DFX_VTD_SPRA0_STRUCT;
#endif /* (SPRA0_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 30;

                            /* Bits[29:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 wb_arb_pri_ovr_pf_pri_disable : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               disables writeback arb priory over prefetch
                               priority for 10nm -- puts us back to SKX
                               priority
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRP_MISC_DFX1_IIO_DFX_VTD_SPRB0_SPRHBM_STRUCT;
#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) */

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 30;

                            /* Bits[29:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 wb_arb_pri_ovr_pf_pri_disable : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               disables writeback arb priory over prefetch
                               priority for 10nm -- puts us back to SKX
                               priority
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRP_MISC_DFX1_IIO_DFX_VTD_SPRC0_SPRMCC_SPRUCC_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 30;

                            /* Bits[29:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 wb_arb_pri_ovr_pf_pri_disable : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               disables writeback arb priory over prefetch
                               priority for 10nm -- puts us back to SKX
                               priority
                            */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRP_MISC_DFX1_IIO_DFX_VTD_STRUCT;

/* IRPEGCREDITS_IIO_DFX_VTD_REG supported on:                                   */
/*      SPRA0 (0x20803840)                                                      */
/*      SPRB0 (0x20803840)                                                      */
/*      SPRHBM (0x20803840)                                                     */
/*      SPRC0 (0x20803840)                                                      */
/*      SPRMCC (0x20803840)                                                     */
/*      SPRUCC (0x20803840)                                                     */
/* Register default value on SPRA0: 0x8012A401                                  */
/* Register default value on SPRB0: 0x8012A401                                  */
/* Register default value on SPRHBM: 0x8012A401                                 */
/* Register default value on SPRC0: 0x8012A401                                  */
/* Register default value on SPRMCC: 0x8012A401                                 */
/* Register default value on SPRUCC: 0x8012A401                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Credits used by IRP when transmitting messages to various destinations on various rings. Pcode/BIOS should leave this register at default unless noted otherwise in the individual bit descriptions. These registers are made CSR only for the scenario that this might be needed for testing purposes.[br]

*/


#define IRPEGCREDITS_IIO_DFX_VTD_REG 0x12420840

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 qpi_ndr_shared_cdt_threshold : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000001*/

                            /*
                               Number of shared AD egress credits that UPI NDR
                               can consume.
                            */
    UINT32 ad_shared_cdt_threshold : 6;

                            /* Bits[10:5], Access Type=RW, default=0x00000020*/

                            /*
                               Number of shared AD egress credits per pool in
                               M2. M2 has 2 pools of 16, but 1 entry is for
                               ubox.[br] type | dedicated | uses shared |
                               max[br] | credits | credits? | outstanding[br] =
                               ================================================
                               ========[br] VC1 | 1 | no | 1[br] VC0p1 | 1 |
                               yes (AD) | 1 + ad_shared_cdt_threshold[br] VC0p0
                               | 0 | yes (AD) | vc0p0_shared_cdt_threshold[br]
                               qpi_ndr | 1 | yes (AD) | 1 +
                               qpi_ndr_shared_cdt_threshold[br] You must set
                               (ad_shared_cdt_threshold <= 12).[br] You must
                               set ((vc0p0_shared_cdt_threshold +
                               qpi_ndr_shared_cdt_threshold) <=
                               ad_shared_cdt_threshold).
                            */
    UINT32 ak_shared_cdt_threshold : 5;

                            /* Bits[15:11], Access Type=RW, default=0x00000014*/

                            /*
                               Number of shared AK egress credits per pool in
                               R2. R2 has 2 pools of 8, but 1 entry is for
                               ubox.[br] type | dedicated | uses shared |
                               max[br] | credits | credits? | outstanding[br] =
                               ================================================
                               ========[br] local_ndr | 1 | yes (AK) | 1 +
                               ak_shared_cdt_threshold[br] snp_rsp | 1 | yes
                               (AK) | 1 + ak_shared_cdt_threshold[br] You must
                               set (ak_shared_cdt_threshold <= 5).
                            */
    UINT32 ad0_ak0_plus_one_shared_cdt : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Increase the shared AD and AK credit thresholds
                               by 1 in AD0 and AK0 to take advantage of extra
                               poolA slots in R2 egress.Note that there is not
                               actually an extra slot in AK poolA (HSX HSD
                               244198).
                            */
    UINT32 en_cdt_bgf_backpressure : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Causes IRP to pay attention to BGF backpressure
                               when sending credit refunds to R2. Could be set
                               to 0 if this causes performance issues.
                            */
    UINT32 ddrt_shared_cdt_threshold : 5;

                            /* Bits[22:18], Access Type=RW, default=0x00000004*/

                            /*
                               Maximum number of shared AD egress credits that
                               M2PCIe can use for Low BW memory (DDRT)
                               requests. This does not reserve an egress entry
                               for Low BW traffic, rather it sets a ceiling on
                               how many can entries can be used for such
                               traffic.
                            */
    UINT32 rsvd : 5;

                            /* Bits[27:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fifo_cdt_threshold : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000008*/

                            /*
                               FIFO Credits[br] The IRP has a FIFO on the
                               inbound path feeding the R2PCIe. This is only a
                               staging FIFO to assist in the flow of inbound
                               traffic. This field pecifies the number of FIFO
                               entries to use in this IRP staging FIFO.
                            */

  } Bits;
  UINT32 Data;

} IRPEGCREDITS_IIO_DFX_VTD_STRUCT;

/* IRPEGCREDITS2_IIO_DFX_VTD_REG supported on:                                  */
/*      SPRA0 (0x20803844)                                                      */
/*      SPRB0 (0x20803844)                                                      */
/*      SPRHBM (0x20803844)                                                     */
/*      SPRC0 (0x20803844)                                                      */
/*      SPRMCC (0x20803844)                                                     */
/*      SPRUCC (0x20803844)                                                     */
/* Register default value on SPRA0: 0x033420D3                                  */
/* Register default value on SPRB0: 0x033420D3                                  */
/* Register default value on SPRHBM: 0x033420D3                                 */
/* Register default value on SPRC0: 0x033420D3                                  */
/* Register default value on SPRMCC: 0x033420D3                                 */
/* Register default value on SPRUCC: 0x033420D3                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Credits used by IRP when transmitting messages to various destinations on various rings.
*/


#define IRPEGCREDITS2_IIO_DFX_VTD_REG 0x12420844

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 idi_shared_cdt_threshold : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000013*/

                            /*
                               Number of shared BL egress credits that IDI can
                               consume.
                            */
    UINT32 qpi_drs_shared_cdt_threshold : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000006*/

                            /*
                               Number of shared BL egress credits that QPI DRS
                               can consume.
                            */
    UINT32 ncp2pb_shared_cdt_threshold : 5;

                            /* Bits[14:10], Access Type=RW, default=0x00000008*/

                            /*
                               Number of shared BL egress credits that NCP2PB
                               can consume.
                            */
    UINT32 ncp2ps_shared_cdt_threshold : 5;

                            /* Bits[19:15], Access Type=RW, default=0x00000008*/

                            /*
                               Number of shared BL egress credits that NCP2PS
                               can consume.
                            */
    UINT32 bl_shared_cdt_threshold : 5;

                            /* Bits[24:20], Access Type=RW, default=0x00000013*/

                            /*
                               Total number of shared BL egress credits that
                               IDI, QPI DRS, NCP2PB, and NCP2PS can use.
                            */
    UINT32 ubox_cdt_threshold : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /* BL egress credits threshold for UBOX */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IRPEGCREDITS2_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH00_N0_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x20803900)                                                      */
/*      SPRB0 (0x20803900)                                                      */
/*      SPRHBM (0x20803900)                                                     */
/*      SPRC0 (0x20803900)                                                      */
/*      SPRMCC (0x20803900)                                                     */
/*      SPRUCC (0x20803900)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH00_N0_IIO_DFX_VTD_REG 0x12420900

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 comp_hdr : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 np_hdr : 6;

                            /* Bits[11:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_hdr : 6;

                            /* Bits[17:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 comp_data : 8;

                            /* Bits[25:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH00_N0_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH00_N1_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x20803904)                                                      */
/*      SPRB0 (0x20803904)                                                      */
/*      SPRHBM (0x20803904)                                                     */
/*      SPRC0 (0x20803904)                                                      */
/*      SPRMCC (0x20803904)                                                     */
/*      SPRUCC (0x20803904)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH00_N1_IIO_DFX_VTD_REG 0x12420904

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 np_data : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_data : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH00_N1_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH01_N0_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x20803908)                                                      */
/*      SPRB0 (0x20803908)                                                      */
/*      SPRHBM (0x20803908)                                                     */
/*      SPRC0 (0x20803908)                                                      */
/*      SPRMCC (0x20803908)                                                     */
/*      SPRUCC (0x20803908)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH01_N0_IIO_DFX_VTD_REG 0x12420908

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 comp_hdr : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 np_hdr : 6;

                            /* Bits[11:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_hdr : 6;

                            /* Bits[17:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 comp_data : 8;

                            /* Bits[25:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH01_N0_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH01_N1_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x2080390c)                                                      */
/*      SPRB0 (0x2080390c)                                                      */
/*      SPRHBM (0x2080390c)                                                     */
/*      SPRC0 (0x2080390c)                                                      */
/*      SPRMCC (0x2080390c)                                                     */
/*      SPRUCC (0x2080390c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH01_N1_IIO_DFX_VTD_REG 0x1242090C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 np_data : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_data : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH01_N1_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH02_N0_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x20803910)                                                      */
/*      SPRB0 (0x20803910)                                                      */
/*      SPRHBM (0x20803910)                                                     */
/*      SPRC0 (0x20803910)                                                      */
/*      SPRMCC (0x20803910)                                                     */
/*      SPRUCC (0x20803910)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH02_N0_IIO_DFX_VTD_REG 0x12420910

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 comp_hdr : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 np_hdr : 6;

                            /* Bits[11:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_hdr : 6;

                            /* Bits[17:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 comp_data : 8;

                            /* Bits[25:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH02_N0_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH02_N1_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x20803914)                                                      */
/*      SPRB0 (0x20803914)                                                      */
/*      SPRHBM (0x20803914)                                                     */
/*      SPRC0 (0x20803914)                                                      */
/*      SPRMCC (0x20803914)                                                     */
/*      SPRUCC (0x20803914)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH02_N1_IIO_DFX_VTD_REG 0x12420914

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 np_data : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_data : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH02_N1_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH03_N0_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x20803918)                                                      */
/*      SPRB0 (0x20803918)                                                      */
/*      SPRHBM (0x20803918)                                                     */
/*      SPRC0 (0x20803918)                                                      */
/*      SPRMCC (0x20803918)                                                     */
/*      SPRUCC (0x20803918)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH03_N0_IIO_DFX_VTD_REG 0x12420918

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 comp_hdr : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 np_hdr : 6;

                            /* Bits[11:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_hdr : 6;

                            /* Bits[17:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 comp_data : 8;

                            /* Bits[25:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH03_N0_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH03_N1_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x2080391c)                                                      */
/*      SPRB0 (0x2080391c)                                                      */
/*      SPRHBM (0x2080391c)                                                     */
/*      SPRC0 (0x2080391c)                                                      */
/*      SPRMCC (0x2080391c)                                                     */
/*      SPRUCC (0x2080391c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH03_N1_IIO_DFX_VTD_REG 0x1242091C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 np_data : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_data : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH03_N1_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH04_N0_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x20803920)                                                      */
/*      SPRB0 (0x20803920)                                                      */
/*      SPRHBM (0x20803920)                                                     */
/*      SPRC0 (0x20803920)                                                      */
/*      SPRMCC (0x20803920)                                                     */
/*      SPRUCC (0x20803920)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH04_N0_IIO_DFX_VTD_REG 0x12420920

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 comp_hdr : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 np_hdr : 6;

                            /* Bits[11:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_hdr : 6;

                            /* Bits[17:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 comp_data : 8;

                            /* Bits[25:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH04_N0_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH04_N1_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x20803924)                                                      */
/*      SPRB0 (0x20803924)                                                      */
/*      SPRHBM (0x20803924)                                                     */
/*      SPRC0 (0x20803924)                                                      */
/*      SPRMCC (0x20803924)                                                     */
/*      SPRUCC (0x20803924)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH04_N1_IIO_DFX_VTD_REG 0x12420924

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 np_data : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_data : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH04_N1_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH05_N0_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x20803928)                                                      */
/*      SPRB0 (0x20803928)                                                      */
/*      SPRHBM (0x20803928)                                                     */
/*      SPRC0 (0x20803928)                                                      */
/*      SPRMCC (0x20803928)                                                     */
/*      SPRUCC (0x20803928)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH05_N0_IIO_DFX_VTD_REG 0x12420928

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 comp_hdr : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 np_hdr : 6;

                            /* Bits[11:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_hdr : 6;

                            /* Bits[17:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 comp_data : 8;

                            /* Bits[25:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH05_N0_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH05_N1_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x2080392c)                                                      */
/*      SPRB0 (0x2080392c)                                                      */
/*      SPRHBM (0x2080392c)                                                     */
/*      SPRC0 (0x2080392c)                                                      */
/*      SPRMCC (0x2080392c)                                                     */
/*      SPRUCC (0x2080392c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH05_N1_IIO_DFX_VTD_REG 0x1242092C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 np_data : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_data : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH05_N1_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH06_N0_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x20803930)                                                      */
/*      SPRB0 (0x20803930)                                                      */
/*      SPRHBM (0x20803930)                                                     */
/*      SPRC0 (0x20803930)                                                      */
/*      SPRMCC (0x20803930)                                                     */
/*      SPRUCC (0x20803930)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH06_N0_IIO_DFX_VTD_REG 0x12420930

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 comp_hdr : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 np_hdr : 6;

                            /* Bits[11:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_hdr : 6;

                            /* Bits[17:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 comp_data : 8;

                            /* Bits[25:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH06_N0_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH06_N1_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x20803934)                                                      */
/*      SPRB0 (0x20803934)                                                      */
/*      SPRHBM (0x20803934)                                                     */
/*      SPRC0 (0x20803934)                                                      */
/*      SPRMCC (0x20803934)                                                     */
/*      SPRUCC (0x20803934)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH06_N1_IIO_DFX_VTD_REG 0x12420934

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 np_data : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_data : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH06_N1_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH07_N0_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x20803938)                                                      */
/*      SPRB0 (0x20803938)                                                      */
/*      SPRHBM (0x20803938)                                                     */
/*      SPRC0 (0x20803938)                                                      */
/*      SPRMCC (0x20803938)                                                     */
/*      SPRUCC (0x20803938)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH07_N0_IIO_DFX_VTD_REG 0x12420938

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 comp_hdr : 6;

                            /* Bits[5:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 np_hdr : 6;

                            /* Bits[11:6], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_hdr : 6;

                            /* Bits[17:12], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted header credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 comp_data : 8;

                            /* Bits[25:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of completion data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH07_N0_IIO_DFX_VTD_STRUCT;

/* IOSF_TGT_CREDITS_CH07_N1_IIO_DFX_VTD_REG supported on:                       */
/*      SPRA0 (0x2080393c)                                                      */
/*      SPRB0 (0x2080393c)                                                      */
/*      SPRHBM (0x2080393c)                                                     */
/*      SPRC0 (0x2080393c)                                                      */
/*      SPRMCC (0x2080393c)                                                     */
/*      SPRUCC (0x2080393c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register reports the credits accumulated by MS2IOSF on its IOSF target interface. This is for observability only.
*/


#define IOSF_TGT_CREDITS_CH07_N1_IIO_DFX_VTD_REG 0x1242093C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 np_data : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of non-posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 p_data : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               Current number of posted data credits
                               accumulated for this channel on the target
                               interface.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IOSF_TGT_CREDITS_CH07_N1_IIO_DFX_VTD_STRUCT;

/* ADEGRINITCRD_IIO_DFX_VTD_REG supported on:                                   */
/*      SPRA0 (0x20803990)                                                      */
/*      SPRB0 (0x20803990)                                                      */
/*      SPRHBM (0x20803990)                                                     */
/*      SPRC0 (0x20803990)                                                      */
/*      SPRMCC (0x20803990)                                                     */
/*      SPRUCC (0x20803990)                                                     */
/* Register default value on SPRA0: 0x00060180                                  */
/* Register default value on SPRB0: 0x00060180                                  */
/* Register default value on SPRHBM: 0x00060180                                 */
/* Register default value on SPRC0: 0x00060180                                  */
/* Register default value on SPRMCC: 0x00060180                                 */
/* Register default value on SPRUCC: 0x00060180                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* AD Egr Init Crd registers
*/


#define ADEGRINITCRD_IIO_DFX_VTD_REG 0x12420990

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ad0port0 : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /* AD0 Egr Init crd registers for Port0 */
    UINT32 ad0port1 : 5;

                            /* Bits[9:5], Access Type=RW/P, default=0x0000000C*/

                            /* AD0 Egr Init crd registers for Port1 */
    UINT32 ad1port0 : 5;

                            /* Bits[14:10], Access Type=RW/P, default=0x00000000*/

                            /* AD1 Egr Init crd registers for Port0 */
    UINT32 ad1port1 : 5;

                            /* Bits[19:15], Access Type=RW/P, default=0x0000000C*/

                            /* AD1 Egr Init crd registers for Port1 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ADEGRINITCRD_IIO_DFX_VTD_STRUCT;

/* AKEGRINITCRD_IIO_DFX_VTD_REG supported on:                                   */
/*      SPRA0 (0x20803994)                                                      */
/*      SPRB0 (0x20803994)                                                      */
/*      SPRHBM (0x20803994)                                                     */
/*      SPRC0 (0x20803994)                                                      */
/*      SPRMCC (0x20803994)                                                     */
/*      SPRUCC (0x20803994)                                                     */
/* Register default value on SPRA0: 0x00002600                                  */
/* Register default value on SPRB0: 0x00002600                                  */
/* Register default value on SPRHBM: 0x00002600                                 */
/* Register default value on SPRC0: 0x00002600                                  */
/* Register default value on SPRMCC: 0x00002600                                 */
/* Register default value on SPRUCC: 0x00002600                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* AK Egr Init Crd registers
*/


#define AKEGRINITCRD_IIO_DFX_VTD_REG 0x12420994

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ak0port0 : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /* AD0 Egr Init crd registers for Port0 */
    UINT32 ak0port1 : 5;

                            /* Bits[9:5], Access Type=RW/P, default=0x00000010*/

                            /* AK0 Egr Init crd registers for Port0 */
    UINT32 ak1port0 : 5;

                            /* Bits[14:10], Access Type=RW/P, default=0x00000009*/

                            /* AK1 Egr Init crd registers for Port0 */
    UINT32 ak1port1 : 5;

                            /* Bits[19:15], Access Type=RW/P, default=0x00000000*/

                            /* AK1 Egr Init crd registers for Port1 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} AKEGRINITCRD_IIO_DFX_VTD_STRUCT;

/* BLEGRINITCRD_IIO_DFX_VTD_REG supported on:                                   */
/*      SPRA0 (0x20803998)                                                      */
/*      SPRB0 (0x20803998)                                                      */
/*      SPRHBM (0x20803998)                                                     */
/*      SPRC0 (0x20803998)                                                      */
/*      SPRMCC (0x20803998)                                                     */
/*      SPRUCC (0x20803998)                                                     */
/* Register default value on SPRA0: 0x000002E0                                  */
/* Register default value on SPRB0: 0x000002E0                                  */
/* Register default value on SPRHBM: 0x000002E0                                 */
/* Register default value on SPRC0: 0x000002E0                                  */
/* Register default value on SPRMCC: 0x000002E0                                 */
/* Register default value on SPRUCC: 0x000002E0                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* BL Egr Init Crd registers, for Port1 the value is one less than  the required (i.e, 'd23 instead of 'd24), since there is a pulse stretcher before sending out to IAL IP
*/


#define BLEGRINITCRD_IIO_DFX_VTD_REG 0x12420998

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 bl0port0 : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /* BL0 Egr Init crd registers for Port0 */
    UINT32 bl0port1 : 5;

                            /* Bits[9:5], Access Type=RW/P, default=0x00000017*/

                            /* BL0 Egr Init crd registers for Port1 */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} BLEGRINITCRD_IIO_DFX_VTD_STRUCT;

/* AKCEGRINITCRD_IIO_DFX_VTD_REG supported on:                                  */
/*      SPRA0 (0x2080399c)                                                      */
/*      SPRB0 (0x2080399c)                                                      */
/*      SPRHBM (0x2080399c)                                                     */
/*      SPRC0 (0x2080399c)                                                      */
/*      SPRMCC (0x2080399c)                                                     */
/*      SPRUCC (0x2080399c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* AKC Egr Init Crd registers
*/


#define AKCEGRINITCRD_IIO_DFX_VTD_REG 0x1242099C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 akc0port0 : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x00000000*/

                            /* AKC Egr Init crd registers for Port0 */
    UINT32 akc0port1 : 5;

                            /* Bits[9:5], Access Type=RW/P, default=0x00000000*/

                            /* AKC Egr Init crd registers for Port1 */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} AKCEGRINITCRD_IIO_DFX_VTD_STRUCT;

/* R2P2PDRSDEDCRED0_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208039a0)                                                      */
/*      SPRB0 (0x208039a0)                                                      */
/*      SPRHBM (0x208039a0)                                                     */
/*      SPRC0 (0x208039a0)                                                      */
/*      SPRMCC (0x208039a0)                                                     */
/*      SPRUCC (0x208039a0)                                                     */
/* Register default value on SPRA0: 0x01010101                                  */
/* Register default value on SPRB0: 0x01010101                                  */
/* Register default value on SPRHBM: 0x01010101                                 */
/* Register default value on SPRC0: 0x01010101                                  */
/* Register default value on SPRMCC: 0x01010101                                 */
/* Register default value on SPRUCC: 0x01010101                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This is the dedicated number of credits this MS2IOSF has to the corresponding Mesh agent for P2P DRS traffic
*/


#define R2P2PDRSDEDCRED0_IIO_DFX_VTD_REG 0x124209A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent0_drsdedcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */
    UINT32 meshagent1_drsdedcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */
    UINT32 meshagent2_drsdedcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */
    UINT32 meshagent3_drsdedcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */

  } Bits;
  UINT32 Data;

} R2P2PDRSDEDCRED0_IIO_DFX_VTD_STRUCT;

/* R2P2PNCBDEDCRED0_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208039a4)                                                      */
/*      SPRB0 (0x208039a4)                                                      */
/*      SPRHBM (0x208039a4)                                                     */
/*      SPRC0 (0x208039a4)                                                      */
/*      SPRMCC (0x208039a4)                                                     */
/*      SPRUCC (0x208039a4)                                                     */
/* Register default value on SPRA0: 0x01010101                                  */
/* Register default value on SPRB0: 0x01010101                                  */
/* Register default value on SPRHBM: 0x01010101                                 */
/* Register default value on SPRC0: 0x01010101                                  */
/* Register default value on SPRMCC: 0x01010101                                 */
/* Register default value on SPRUCC: 0x01010101                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This is the dedicated number of credits this MS2IOSF has to the corresponding Mesh agent for P2P NCB traffic
*/


#define R2P2PNCBDEDCRED0_IIO_DFX_VTD_REG 0x124209A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent0_ncbdedcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */
    UINT32 meshagent1_ncbdedcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */
    UINT32 meshagent2_ncbdedcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */
    UINT32 meshagent3_ncbdedcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */

  } Bits;
  UINT32 Data;

} R2P2PNCBDEDCRED0_IIO_DFX_VTD_STRUCT;

/* R2P2PNCSDEDCRED0_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208039a8)                                                      */
/*      SPRB0 (0x208039a8)                                                      */
/*      SPRHBM (0x208039a8)                                                     */
/*      SPRC0 (0x208039a8)                                                      */
/*      SPRMCC (0x208039a8)                                                     */
/*      SPRUCC (0x208039a8)                                                     */
/* Register default value on SPRA0: 0x01010101                                  */
/* Register default value on SPRB0: 0x01010101                                  */
/* Register default value on SPRHBM: 0x01010101                                 */
/* Register default value on SPRC0: 0x01010101                                  */
/* Register default value on SPRMCC: 0x01010101                                 */
/* Register default value on SPRUCC: 0x01010101                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This is the dedicated number of credits this MS2IOSF has to the corresponding Mesh agent for P2P NCS traffic
*/


#define R2P2PNCSDEDCRED0_IIO_DFX_VTD_REG 0x124209A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent0_ncsdedcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */
    UINT32 meshagent1_ncsdedcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */
    UINT32 meshagent2_ncsdedcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */
    UINT32 meshagent3_ncsdedcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */

  } Bits;
  UINT32 Data;

} R2P2PNCSDEDCRED0_IIO_DFX_VTD_STRUCT;

/* R2P2PDRSDEDCRED1_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208039ac)                                                      */
/*      SPRB0 (0x208039ac)                                                      */
/*      SPRHBM (0x208039ac)                                                     */
/*      SPRC0 (0x208039ac)                                                      */
/*      SPRMCC (0x208039ac)                                                     */
/*      SPRUCC (0x208039ac)                                                     */
/* Register default value on SPRA0: 0x01010101                                  */
/* Register default value on SPRB0: 0x01010101                                  */
/* Register default value on SPRHBM: 0x01010101                                 */
/* Register default value on SPRC0: 0x01010101                                  */
/* Register default value on SPRMCC: 0x01010101                                 */
/* Register default value on SPRUCC: 0x01010101                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This is the dedicated number of credits this MS2IOSF has to the corresponding Mesh agent for P2P DRS traffic
*/


#define R2P2PDRSDEDCRED1_IIO_DFX_VTD_REG 0x124209AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent4_drsdedcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */
    UINT32 meshagent5_drsdedcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */
    UINT32 meshagent6_drsdedcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */
    UINT32 meshagent7_drsdedcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */

  } Bits;
  UINT32 Data;

} R2P2PDRSDEDCRED1_IIO_DFX_VTD_STRUCT;

/* R2P2PNCBDEDCRED1_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208039b0)                                                      */
/*      SPRB0 (0x208039b0)                                                      */
/*      SPRHBM (0x208039b0)                                                     */
/*      SPRC0 (0x208039b0)                                                      */
/*      SPRMCC (0x208039b0)                                                     */
/*      SPRUCC (0x208039b0)                                                     */
/* Register default value on SPRA0: 0x01010101                                  */
/* Register default value on SPRB0: 0x01010101                                  */
/* Register default value on SPRHBM: 0x01010101                                 */
/* Register default value on SPRC0: 0x01010101                                  */
/* Register default value on SPRMCC: 0x01010101                                 */
/* Register default value on SPRUCC: 0x01010101                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This is the dedicated number of credits this MS2IOSF has to the corresponding Mesh agent for P2P NCB traffic
*/


#define R2P2PNCBDEDCRED1_IIO_DFX_VTD_REG 0x124209B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent4_ncbdedcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */
    UINT32 meshagent5_ncbdedcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */
    UINT32 meshagent6_ncbdedcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */
    UINT32 meshagent7_ncbdedcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */

  } Bits;
  UINT32 Data;

} R2P2PNCBDEDCRED1_IIO_DFX_VTD_STRUCT;

/* R2P2PNCSDEDCRED1_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208039b4)                                                      */
/*      SPRB0 (0x208039b4)                                                      */
/*      SPRHBM (0x208039b4)                                                     */
/*      SPRC0 (0x208039b4)                                                      */
/*      SPRMCC (0x208039b4)                                                     */
/*      SPRUCC (0x208039b4)                                                     */
/* Register default value on SPRA0: 0x01010101                                  */
/* Register default value on SPRB0: 0x01010101                                  */
/* Register default value on SPRHBM: 0x01010101                                 */
/* Register default value on SPRC0: 0x01010101                                  */
/* Register default value on SPRMCC: 0x01010101                                 */
/* Register default value on SPRUCC: 0x01010101                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This is the dedicated number of credits this MS2IOSF has to the corresponding Mesh agent for P2P NCS traffic
*/


#define R2P2PNCSDEDCRED1_IIO_DFX_VTD_REG 0x124209B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent4_ncsdedcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */
    UINT32 meshagent5_ncsdedcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */
    UINT32 meshagent6_ncsdedcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */
    UINT32 meshagent7_ncsdedcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */

  } Bits;
  UINT32 Data;

} R2P2PNCSDEDCRED1_IIO_DFX_VTD_STRUCT;

/* OTCERRDIS_IIO_DFX_VTD_REG supported on:                                      */
/*      SPRA0 (0x208039b8)                                                      */
/*      SPRB0 (0x208039b8)                                                      */
/*      SPRHBM (0x208039b8)                                                     */
/*      SPRC0 (0x208039b8)                                                      */
/*      SPRMCC (0x208039b8)                                                     */
/*      SPRUCC (0x208039b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Traffic Controller Error Detection defeature
*/


#define OTCERRDIS_IIO_DFX_VTD_REG 0x124209B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 otc_crdt_uf : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] OTC CRDT underflow[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_crdt_of : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] OTC CRDT overflow[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_par_hdr_rf : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] Parity error enable in the OTC hdr_q RF[br]
                               Lock: IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_par_addr_rf : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] Parity error enable in the OTC addr_q
                               RF[br] Lock: IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_ecc_uncor_rf : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] ECC uncorrected error enable in the OTC
                               dat_dword RF[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_cabort : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] Completer abort enable[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_mabort : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] Master abort enable[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_mtc_tgt_err : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] Multicast target error for ITC only[br]
                               Lock: IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_ecc_cor_rf : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] ECC corrected error enable in the OTC
                               dat_dword RF[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_mctp_bcast_to_dmi : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] MCTP broadcast request to DMI enable[br]
                               Lock: IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_par_rte : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] IOAPIC RTE parity error enable[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_irp_dat_par : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] parity error on incoming data from IRP
                               enable[br] Lock: IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_irp_addr_par : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] Parity error on incoming addr from IRP
                               enable[br] Lock: IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_hw_assert : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] OTC Hardware Assert[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_txn_dur_lock : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] M2IOSF received a DnS Txn during Lock[br]
                               Lock: IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_gpsb_par : 1;

                            /* Bits[15:15], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] GPSBP Parity Error[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_misc_snarf_fifo_of : 1;

                            /* Bits[16:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] OTC Misc Snarf Fifo Overflow[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_misc_snarf_fifo_uf : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] OTC Misc Snarf Fifo Underflow[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_misc_oobmsm_mult_pend : 1;

                            /* Bits[18:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] OTC Misc OOBMSM agent got a request when
                               one was pending already[br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 vmd_cfgbar_overflow : 1;

                            /* Bits[19:19], Access Type=RW/P, default=0x00000000*/

                            /*
                               MS2IOSF received a request targeting VMD CFGBAR
                               when two requests were already in progress.
                               Lock: IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 otc_poison_mabort : 1;

                            /* Bits[20:20], Access Type=RW/P, default=0x00000000*/

                            /*
                               [br] Disable Poison MAbort [br] Lock:
                               IIO_DFX_LCK_CTL.DBGBUSLCK[br]
                            */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCERRDIS_IIO_DFX_VTD_STRUCT;

/* R2P2PDRSDEDCRED2_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208039c0)                                                      */
/*      SPRB0 (0x208039c0)                                                      */
/*      SPRHBM (0x208039c0)                                                     */
/*      SPRC0 (0x208039c0)                                                      */
/*      SPRMCC (0x208039c0)                                                     */
/*      SPRUCC (0x208039c0)                                                     */
/* Register default value on SPRA0: 0x01010101                                  */
/* Register default value on SPRB0: 0x01010101                                  */
/* Register default value on SPRHBM: 0x01010101                                 */
/* Register default value on SPRC0: 0x01010101                                  */
/* Register default value on SPRMCC: 0x01010101                                 */
/* Register default value on SPRUCC: 0x01010101                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This is the dedicated number of credits this MS2IOSF has to the corresponding Mesh agent for P2P DRS traffic
*/


#define R2P2PDRSDEDCRED2_IIO_DFX_VTD_REG 0x124209C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent8_drsdedcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */
    UINT32 meshagent9_drsdedcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               M2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */
    UINT32 meshagent10_drsdedcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               M2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */
    UINT32 meshagent11_drsdedcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               M2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */

  } Bits;
  UINT32 Data;

} R2P2PDRSDEDCRED2_IIO_DFX_VTD_STRUCT;

/* R2P2PNCBDEDCRED2_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208039c4)                                                      */
/*      SPRB0 (0x208039c4)                                                      */
/*      SPRHBM (0x208039c4)                                                     */
/*      SPRC0 (0x208039c4)                                                      */
/*      SPRMCC (0x208039c4)                                                     */
/*      SPRUCC (0x208039c4)                                                     */
/* Register default value on SPRA0: 0x01010101                                  */
/* Register default value on SPRB0: 0x01010101                                  */
/* Register default value on SPRHBM: 0x01010101                                 */
/* Register default value on SPRC0: 0x01010101                                  */
/* Register default value on SPRMCC: 0x01010101                                 */
/* Register default value on SPRUCC: 0x01010101                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This is the dedicated number of credits this MS2IOSF has to the corresponding Mesh agent for P2P NCB traffic
*/


#define R2P2PNCBDEDCRED2_IIO_DFX_VTD_REG 0x124209C4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent8_ncbdedcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */
    UINT32 meshagent9_ncbdedcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */
    UINT32 meshagent10_ncbdedcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */
    UINT32 meshagent11_ncbdedcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */

  } Bits;
  UINT32 Data;

} R2P2PNCBDEDCRED2_IIO_DFX_VTD_STRUCT;

/* R2P2PNCSDEDCRED2_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208039c8)                                                      */
/*      SPRB0 (0x208039c8)                                                      */
/*      SPRHBM (0x208039c8)                                                     */
/*      SPRC0 (0x208039c8)                                                      */
/*      SPRMCC (0x208039c8)                                                     */
/*      SPRUCC (0x208039c8)                                                     */
/* Register default value on SPRA0: 0x01010101                                  */
/* Register default value on SPRB0: 0x01010101                                  */
/* Register default value on SPRHBM: 0x01010101                                 */
/* Register default value on SPRC0: 0x01010101                                  */
/* Register default value on SPRMCC: 0x01010101                                 */
/* Register default value on SPRUCC: 0x01010101                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This is the dedicated number of credits this MS2IOSF has to the corresponding Mesh agent for P2P NCS traffic
*/


#define R2P2PNCSDEDCRED2_IIO_DFX_VTD_REG 0x124209C8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent8_ncsdedcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */
    UINT32 meshagent9_ncsdedcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */
    UINT32 meshagent10_ncsdedcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */
    UINT32 meshagent11_ncsdedcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */

  } Bits;
  UINT32 Data;

} R2P2PNCSDEDCRED2_IIO_DFX_VTD_STRUCT;

/* M2IOSFFORCEEMPTY_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208039d0)                                                      */
/*      SPRB0 (0x208039d0)                                                      */
/*      SPRHBM (0x208039d0)                                                     */
/*      SPRC0 (0x208039d0)                                                      */
/*      SPRMCC (0x208039d0)                                                     */
/*      SPRUCC (0x208039d0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Force Empty Indication from MS2IOSF for Survivavibility
*/


#define M2IOSFFORCEEMPTY_IIO_DFX_VTD_REG 0x124209D0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 enable_force_irp_empty : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Enable Force Irp Empty */
    UINT32 enable_force_otc_empty : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Enable Force Otc Empty */
    UINT32 enable_force_itc_empty : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Enable Force Itc Empty */
    UINT32 enable_force_m2pcie_empty : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* Enable Force M2Pcie Empty */
    UINT32 enable_force_iosf_prim0_empty : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Enable Force Iosf Prim0 Empty */
    UINT32 enable_force_iosf_prim1_empty : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Enable Force Iosf Prim1 Empty */
    UINT32 enable_force_iosf_prim2_empty : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Enable Force Iosf Prim2 Empty */
    UINT32 enable_force_miscblk_empty : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Enable Force Miscblk Empty */
    UINT32 enable_force_gpsb_empty : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Enable Force Gpsb Empty */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} M2IOSFFORCEEMPTY_IIO_DFX_VTD_STRUCT;

/* M2IOSFQUIESCESTATUS_IIO_DFX_VTD_REG supported on:                            */
/*      SPRA0 (0x208039d8)                                                      */
/*      SPRB0 (0x208039d8)                                                      */
/*      SPRHBM (0x208039d8)                                                     */
/*      SPRC0 (0x208039d8)                                                      */
/*      SPRMCC (0x208039d8)                                                     */
/*      SPRUCC (0x208039d8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: DFX                                              */
/* SPRB0 Security PolicyGroup: DFX                                              */
/* SPRHBM Security PolicyGroup: DFX                                             */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* This register is used to indicate the quiesce status of individual structures in different blocks in MS2IOSF 
For eg:
 M2P AD EGR non-empty
- M2P BL EGR non-empty
- M2P AK EGR non-empty
- M2P AKc EGR non-empty
- M2P BL IGR non-empty
- IRP Ctag non-empty
- IRP NCS non-empty
- IRP NCB non-empty
- IRP FAF non-empty
- IRP P2P non-empty
- ITC has C pending
- ITC has NP pending
- ITC has P pending
- ITC has request pending to IOMMU
- OTC has C pending
- OTC has NP pending
- OTC has P pending
- Miscblk FSMs active:
  - Cfg
  - Lock
  - MsgBlk
  - What other ones still apply in 10nm? (e.g. IOAPIC, GPE, JSM)
- Primary fabric ISM (3 bits * 2 agents)
*/


#define M2IOSFQUIESCESTATUS_IIO_DFX_VTD_REG 0x124209D8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 irp_ncs_empty_b : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* Irp Ncs Empty B */
    UINT32 irp_ncb_empty_b : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /* Irp Ncb Empty B */
    UINT32 irp_p2p_empty_b : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000000*/

                            /* Irp P2P Empty B */
    UINT32 irp_faf_empty_b : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /* Irp Faf Empty B */
    UINT32 irp_ctag_empty_b : 1;

                            /* Bits[4:4], Access Type=RO/V, default=0x00000000*/

                            /* Irp Ctag Empty B */
    UINT32 m2pcie_adegr_empty_b : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /* M2Pcie Adegr Empty B */
    UINT32 m2pcie_blegr_empty_b : 1;

                            /* Bits[6:6], Access Type=RO/V, default=0x00000000*/

                            /* M2Pcie Blegr Empty B */
    UINT32 m2pcie_akegr_empty_b : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /* M2Pcie Akegr Empty B */
    UINT32 m2pcie_akcegr_empty_b : 1;

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /* M2Pcie Akcegr Empty B */
    UINT32 m2pcie_blingr_empty_b : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /* M2Pcie Blingr Empty B */
    UINT32 itc_posted_empty_b : 1;

                            /* Bits[10:10], Access Type=RO/V, default=0x00000000*/

                            /* Itc Posted Empty B */
    UINT32 itc_np_empty_b : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /* Itc Np Empty B */
    UINT32 itc_cpl_empty_b : 1;

                            /* Bits[12:12], Access Type=RO/V, default=0x00000000*/

                            /* Itc Cpl Empty B */
    UINT32 iommu_empty_b : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /* Iommu Empty B */
    UINT32 otc_posted_empty_b : 1;

                            /* Bits[14:14], Access Type=RO/V, default=0x00000000*/

                            /* Otc Posted Empty B */
    UINT32 otc_np_empty_b : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /* Otc Np Empty B */
    UINT32 otc_cpl_empty_b : 1;

                            /* Bits[16:16], Access Type=RO/V, default=0x00000000*/

                            /* Otc Cpl Empty B */
    UINT32 iosf_prim0_ism : 3;

                            /* Bits[19:17], Access Type=RO/V, default=0x00000000*/

                            /* Iosf Prim0 Ism */
    UINT32 iosf_prim1_ism : 3;

                            /* Bits[22:20], Access Type=RO/V, default=0x00000000*/

                            /* Iosf Prim1 Ism */
    UINT32 irp_vfaf_empty_b : 1;

                            /* Bits[23:23], Access Type=RO/V, default=0x00000000*/

                            /* Irp VFaf Empty B */
    UINT32 rsvd : 2;

                            /* Bits[25:24], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 misc_gpsb_empty_b : 1;

                            /* Bits[26:26], Access Type=RO/V, default=0x00000000*/

                            /* Misc Gpsb Empty B */
    UINT32 miscblk_lock_fsm_empty_b : 1;

                            /* Bits[27:27], Access Type=RO/V, default=0x00000000*/

                            /* Miscblk Lock Fsm Empty B */
    UINT32 miscblk_ras_fsm_empty_b : 1;

                            /* Bits[28:28], Access Type=RO/V, default=0x00000000*/

                            /* Miscblk Ras Fsm Empty B */
    UINT32 miscblk_msgblk_fsm_empty_b : 1;

                            /* Bits[29:29], Access Type=RO/V, default=0x00000000*/

                            /* Miscblk Msgblk Fsm Empty B */
    UINT32 miscblk_ieh_fsm_empty_b : 1;

                            /* Bits[30:30], Access Type=RO/V, default=0x00000000*/

                            /* Miscblk Ieh Fsm Empty B */
    UINT32 miscblk_obmsm_fsm_empty_b : 1;

                            /* Bits[31:31], Access Type=RO/V, default=0x00000000*/

                            /* Miscblk Obmsm Fsm Empty B */

  } Bits;
  UINT32 Data;

} M2IOSFQUIESCESTATUS_IIO_DFX_VTD_STRUCT;

#if defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* LOCKFSMANDQUEUEQUIESCESTATUS_IIO_DFX_VTD_REG supported on:                   */
/*      SPRC0 (0x208039dc)                                                      */
/*      SPRMCC (0x208039dc)                                                     */
/*      SPRUCC (0x208039dc)                                                     */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRC0 BDF: 8_0_3                                       */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRMCC Security PolicyGroup: DFX                                             */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Visibility into Lock FSM state and queisce done status for posted, non-posted and completion queues (inbound and outbound direction)
*/


#define LOCKFSMANDQUEUEQUIESCESTATUS_IIO_DFX_VTD_REG 0x124209DC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 inb_posted_quiesce_done : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Quiesce done indication for inbound posted
                               channels
                            */
    UINT32 inb_nonposted_quiesce_done : 8;

                            /* Bits[15:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               Quiesce done indication for inbound non-posted
                               channels
                            */
    UINT32 outb_cmp_quiesce_done : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               Quiesce done indication for outbound completion
                               channels
                            */
    UINT32 lock_fsm_state : 5;

                            /* Bits[28:24], Access Type=RO/V, default=0x00000000*/

                            /* Lock FSM state */
    UINT32 rsvd : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LOCKFSMANDQUEUEQUIESCESTATUS_IIO_DFX_VTD_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

/* R2P2PDRSDEDCRED3_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208039e0)                                                      */
/*      SPRB0 (0x208039e0)                                                      */
/*      SPRHBM (0x208039e0)                                                     */
/*      SPRC0 (0x208039e0)                                                      */
/*      SPRMCC (0x208039e0)                                                     */
/*      SPRUCC (0x208039e0)                                                     */
/* Register default value on SPRA0: 0x01010101                                  */
/* Register default value on SPRB0: 0x01010101                                  */
/* Register default value on SPRHBM: 0x01010101                                 */
/* Register default value on SPRC0: 0x01010101                                  */
/* Register default value on SPRMCC: 0x01010101                                 */
/* Register default value on SPRUCC: 0x01010101                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This is the dedicated number of credits this MS2IOSF has to the corresponding Mesh agent for P2P DRS traffic
*/


#define R2P2PDRSDEDCRED3_IIO_DFX_VTD_REG 0x124209E0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent12_drsdedcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */
    UINT32 meshagent13_drsdedcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               M2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */
    UINT32 meshagent14_drsdedcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               M2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */
    UINT32 meshagent15_drsdedcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               M2IOSF has to the corresponding Mesh agent for
                               P2P DRS traffic
                            */

  } Bits;
  UINT32 Data;

} R2P2PDRSDEDCRED3_IIO_DFX_VTD_STRUCT;

/* R2P2PNCBDEDCRED3_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208039e4)                                                      */
/*      SPRB0 (0x208039e4)                                                      */
/*      SPRHBM (0x208039e4)                                                     */
/*      SPRC0 (0x208039e4)                                                      */
/*      SPRMCC (0x208039e4)                                                     */
/*      SPRUCC (0x208039e4)                                                     */
/* Register default value on SPRA0: 0x01010101                                  */
/* Register default value on SPRB0: 0x01010101                                  */
/* Register default value on SPRHBM: 0x01010101                                 */
/* Register default value on SPRC0: 0x01010101                                  */
/* Register default value on SPRMCC: 0x01010101                                 */
/* Register default value on SPRUCC: 0x01010101                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This is the dedicated number of credits this MS2IOSF has to the corresponding Mesh agent for P2P NCB traffic
*/


#define R2P2PNCBDEDCRED3_IIO_DFX_VTD_REG 0x124209E4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent12_ncbdedcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */
    UINT32 meshagent13_ncbdedcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */
    UINT32 meshagent14_ncbdedcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */
    UINT32 meshagent15_ncbdedcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCB traffic
                            */

  } Bits;
  UINT32 Data;

} R2P2PNCBDEDCRED3_IIO_DFX_VTD_STRUCT;

/* R2P2PNCSDEDCRED3_IIO_DFX_VTD_REG supported on:                               */
/*      SPRA0 (0x208039e8)                                                      */
/*      SPRB0 (0x208039e8)                                                      */
/*      SPRHBM (0x208039e8)                                                     */
/*      SPRC0 (0x208039e8)                                                      */
/*      SPRMCC (0x208039e8)                                                     */
/*      SPRUCC (0x208039e8)                                                     */
/* Register default value on SPRA0: 0x01010101                                  */
/* Register default value on SPRB0: 0x01010101                                  */
/* Register default value on SPRHBM: 0x01010101                                 */
/* Register default value on SPRC0: 0x01010101                                  */
/* Register default value on SPRMCC: 0x01010101                                 */
/* Register default value on SPRUCC: 0x01010101                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* This is the dedicated number of credits this MS2IOSF has to the corresponding Mesh agent for P2P NCS traffic
*/


#define R2P2PNCSDEDCRED3_IIO_DFX_VTD_REG 0x124209E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 meshagent12_ncsdedcr : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */
    UINT32 meshagent13_ncsdedcr : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */
    UINT32 meshagent14_ncsdedcr : 8;

                            /* Bits[23:16], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */
    UINT32 meshagent15_ncsdedcr : 8;

                            /* Bits[31:24], Access Type=RW/P, default=0x00000001*/

                            /*
                               This is the dedicated number of credits this
                               MS2IOSF has to the corresponding Mesh agent for
                               P2P NCS traffic
                            */

  } Bits;
  UINT32 Data;

} R2P2PNCSDEDCRED3_IIO_DFX_VTD_STRUCT;

/* OQOSLOTDFX_IITC_IIO_DFX_VTD_REG supported on:                                */
/*      SPRA0 (0x208039f0)                                                      */
/*      SPRB0 (0x208039f0)                                                      */
/*      SPRHBM (0x208039f0)                                                     */
/*      SPRC0 (0x208039f0)                                                      */
/*      SPRMCC (0x208039f0)                                                     */
/*      SPRUCC (0x208039f0)                                                     */
/* Register default value on SPRA0: 0x00000078                                  */
/* Register default value on SPRB0: 0x00000078                                  */
/* Register default value on SPRHBM: 0x00000078                                 */
/* Register default value on SPRC0: 0x00000078                                  */
/* Register default value on SPRMCC: 0x00000078                                 */
/* Register default value on SPRUCC: 0x00000078                                 */
/* SPRA0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRB0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRHBM Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRMCC Register File:    sprsp_top/m2iosf_top[3]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRA0 BDF: 8_0_3                                       */
/* SPRA0 Security PolicyGroup: LB_W                                             */
/* SPRB0 Security PolicyGroup: LB_W                                             */
/* SPRHBM Security PolicyGroup: LB_W                                            */
/* SPRC0 Security PolicyGroup: LB_W                                             */
/* SPRMCC Security PolicyGroup: LB_W                                            */
/* SPRUCC Security PolicyGroup: LB_W                                            */
/* OqoSlot defeature CSR
*/


#define OQOSLOTDFX_IITC_IIO_DFX_VTD_REG 0x124209F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 oqoslotarbdisable : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /*
                               Disable oqoslot arb in ITC bit[0] -> Norm pipe
                               bit[1] -> Fast pipe bit[2] -> Np pipe
                            */
    UINT32 oqoslotarb_gnt_threshold : 6;

                            /* Bits[8:3], Access Type=RW, default=0x0000000F*/

                            /* Configure the grant threshold for oqoslot arb.. */
    UINT32 oqoslotblockdisable : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /*
                               Disable oqoslot block in ITC bit[0] -> Norm pipe
                               bit[1] -> Fast pipe bit[2] -> Np pipe
                            */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OQOSLOTDFX_IITC_IIO_DFX_VTD_STRUCT;

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* ITCSHAREDCREDITPOOLSTATUS_IIO_DFX_VTD_REG supported on:                      */
/*      SPRC0 (0x208039f8)                                                      */
/*      SPRUCC (0x208039f8)                                                     */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRC0 BDF: 8_0_3                                       */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Debug visibility for ITC Data credit shared pool
*/


#define ITCSHAREDCREDITPOOLSTATUS_IIO_DFX_VTD_REG 0x124209F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 freehdrcnt : 11;

                            /* Bits[10:0], Access Type=RO/V, default=0x00000000*/

                            /* Free shared hdr credits */
    UINT32 freedatacnt : 11;

                            /* Bits[21:11], Access Type=RO/V, default=0x00000000*/

                            /* Free shared data credits */
    UINT32 rsvd : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ITCSHAREDCREDITPOOLSTATUS_IIO_DFX_VTD_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRC0_HOST) || defined(SPRUCC_HOST)
/* OTCSHAREDCREDITPOOLSTATUS_IIO_DFX_VTD_REG supported on:                      */
/*      SPRC0 (0x208039fc)                                                      */
/*      SPRUCC (0x208039fc)                                                     */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRC0 Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG */
/* SPRUCC Register File:    sprsp_top/m2iosf_top[0]/m2iosf_reg/CFG_BDF_1_0_3_CFG*/
/* Struct generated from SPRC0 BDF: 8_0_3                                       */
/* SPRC0 Security PolicyGroup: DFX                                              */
/* SPRUCC Security PolicyGroup: DFX                                             */
/* Debug visibility for OTC shared credit pool
*/


#define OTCSHAREDCREDITPOOLSTATUS_IIO_DFX_VTD_REG 0x124209FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 freecnt : 10;

                            /* Bits[9:0], Access Type=RO/V, default=0x00000000*/

                            /* Free shared credits */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} OTCSHAREDCREDITPOOLSTATUS_IIO_DFX_VTD_STRUCT;
#endif /* (SPRC0_HOST) || defined(SPRUCC_HOST) */
#endif /* _IIO_DFX_VTD_h */
