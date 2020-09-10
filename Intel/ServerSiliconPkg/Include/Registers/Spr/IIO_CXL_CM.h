
/** @file
  IIO_CXL_CM.h

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
/* BOOTSMM_RW                                                                   */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI */
/*    | OOB_MSM_SAI | UNCORE_PMA_SAI                                            */
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
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* BOOTSMM_RW                                                                   */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI */
/*    | OOB_MSM_SAI | UNCORE_PMA_SAI                                            */
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
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* BOOTSMM_RW                                                                   */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI */
/*    | OOB_MSM_SAI | UNCORE_PMA_SAI                                            */
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
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* BOOTSMM_RW                                                                   */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI */
/*    | OOB_MSM_SAI | UNCORE_PMA_SAI                                            */
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
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* BOOTSMM_RW                                                                   */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI */
/*    | OOB_MSM_SAI | UNCORE_PMA_SAI                                            */
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
/*  Security_Read_CP_Secured:                                                   */
/*    0                                                                         */
/* BOOTSMM_RW                                                                   */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI |                      */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |                  */
/*    DFX_THIRDPARTY_SAI | DFX_UNTRUSTED_SAI | HOSTIA_SMM_SAI | HOSTIA_BOOT_SAI */
/*    | OOB_MSM_SAI | UNCORE_PMA_SAI                                            */
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


#ifndef _IIO_CXL_CM_h
#define _IIO_CXL_CM_h
#include <Base.h>

/* INTEL_AL_CAPABILITY_HEADER_IIO_CXL_CM_REG supported on:                      */
/*      SPRA0 (0x20001000)                                                      */
/*      SPRB0 (0x20001000)                                                      */
/*      SPRHBM (0x20001000)                                                     */
/*      SPRC0 (0x20001000)                                                      */
/*      SPRMCC (0x20001000)                                                     */
/*      SPRUCC (0x20001000)                                                     */
/* Register default value on SPRA0: 0x02110001                                  */
/* Register default value on SPRB0: 0x02110001                                  */
/* Register default value on SPRHBM: 0x02110001                                 */
/* Register default value on SPRC0: 0x02110001                                  */
/* Register default value on SPRMCC: 0x02110001                                 */
/* Register default value on SPRUCC: 0x02110001                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Intel AL Capability Header
*/


#define INTEL_AL_CAPABILITY_HEADER_IIO_CXL_CM_REG 0x23121000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 intel_al_capability_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000001*/

                            /*
                               This defines the nature and format of the
                               Intel_AL_Capability register. For the
                               Intel_AL_Capability_Header register, this field
                               should be 0x1.
                            */
    UINT32 intel_al_capability_version : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               This defines the version number of the
                               Intel_AL_Capability structure present
                            */
    UINT32 intel_al_cache_mem_version : 4;

                            /* Bits[23:20], Access Type=RO, default=0x00000001*/

                            /*
                               This defines the version of the Intel AL Cache
                               Mem Protocol supported
                            */
    UINT32 array_size : 8;

                            /* Bits[31:24], Access Type=RO, default=0x00000002*/

                            /*
                               This defines the number of elements present in
                               the Intel_AL_Capability array, not including the
                               Intel_AL_Capability_Header element. Each element
                               is 1 DWORD in size and is located contiguous
                               with previous elements.
                            */

  } Bits;
  UINT32 Data;

} INTEL_AL_CAPABILITY_HEADER_IIO_CXL_CM_STRUCT;

/* INTEL_AL_RAS_CAPABILITY_POINTER_IIO_CXL_CM_REG supported on:                 */
/*      SPRA0 (0x20001004)                                                      */
/*      SPRB0 (0x20001004)                                                      */
/*      SPRHBM (0x20001004)                                                     */
/*      SPRC0 (0x20001004)                                                      */
/*      SPRMCC (0x20001004)                                                     */
/*      SPRUCC (0x20001004)                                                     */
/* Register default value on SPRA0: 0x10010002                                  */
/* Register default value on SPRB0: 0x10010002                                  */
/* Register default value on SPRHBM: 0x10010002                                 */
/* Register default value on SPRC0: 0x10010002                                  */
/* Register default value on SPRMCC: 0x10010002                                 */
/* Register default value on SPRUCC: 0x10010002                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Intel AL RAS Capability Pointer
*/


#define INTEL_AL_RAS_CAPABILITY_POINTER_IIO_CXL_CM_REG 0x23121004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 intel_al_capability_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000002*/

                            /*
                               This defines the nature and format of the
                               Intel_AL_Capability register. For the
                               Intel_AL_RAS_Capability_Pointer register, this
                               field should be 0x2
                            */
    UINT32 intel_al_capability_version : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               This defines the version number of the
                               Intel_AL_Capability structure present
                            */
    UINT32 ras_intel_al_capability_offset : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000100*/

                            /*
                               This defines the offset of the
                               Intel_AL_Capability relative to beginning of
                               Intel_AL_Capability_Header register
                            */

  } Bits;
  UINT32 Data;

} INTEL_AL_RAS_CAPABILITY_POINTER_IIO_CXL_CM_STRUCT;

/* INTEL_AL_SECURITY_CAPABILITY_POINTER_IIO_CXL_CM_REG supported on:            */
/*      SPRA0 (0x20001008)                                                      */
/*      SPRB0 (0x20001008)                                                      */
/*      SPRHBM (0x20001008)                                                     */
/*      SPRC0 (0x20001008)                                                      */
/*      SPRMCC (0x20001008)                                                     */
/*      SPRUCC (0x20001008)                                                     */
/* Register default value on SPRA0: 0x93010003                                  */
/* Register default value on SPRB0: 0x93010003                                  */
/* Register default value on SPRHBM: 0x93010003                                 */
/* Register default value on SPRC0: 0x93010003                                  */
/* Register default value on SPRMCC: 0x93010003                                 */
/* Register default value on SPRUCC: 0x93010003                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Intel AL Security Capability Pointer
*/


#define INTEL_AL_SECURITY_CAPABILITY_POINTER_IIO_CXL_CM_REG 0x23121008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 intel_al_capability_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000003*/

                            /*
                               This defines the nature and format of the
                               Intel_AL_Capability register. For the
                               Intel_AL_Security_Capability_Pointer register,
                               this field should be 0x3
                            */
    UINT32 intel_al_capability_version : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               This defines the version number of the
                               Intel_AL_Capability structure present
                            */
    UINT32 security_intel_al_capability_offset : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000930*/

                            /*
                               This defines the offset of the
                               Intel_AL_Capability relative to beginning of
                               Intel_AL_Capability_Header register
                            */

  } Bits;
  UINT32 Data;

} INTEL_AL_SECURITY_CAPABILITY_POINTER_IIO_CXL_CM_STRUCT;

/* INTEL_AL_LINK_CAPABILITY_POINTER_IIO_CXL_CM_REG supported on:                */
/*      SPRA0 (0x2000100c)                                                      */
/*      SPRB0 (0x2000100c)                                                      */
/*      SPRHBM (0x2000100c)                                                     */
/*      SPRC0 (0x2000100c)                                                      */
/*      SPRMCC (0x2000100c)                                                     */
/*      SPRUCC (0x2000100c)                                                     */
/* Register default value on SPRA0: 0x40010004                                  */
/* Register default value on SPRB0: 0x40010004                                  */
/* Register default value on SPRHBM: 0x40010004                                 */
/* Register default value on SPRC0: 0x40010004                                  */
/* Register default value on SPRMCC: 0x40010004                                 */
/* Register default value on SPRUCC: 0x40010004                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Intel AL Security Capability Pointer
*/


#define INTEL_AL_LINK_CAPABILITY_POINTER_IIO_CXL_CM_REG 0x2312100C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 intel_al_capability_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x00000004*/

                            /*
                               This defines the nature and format of the
                               Intel_AL_Capability register. For the
                               Intel_AL_Security_Capability_Pointer register,
                               this field should be 0x3
                            */
    UINT32 intel_al_capability_version : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               This defines the version number of the
                               Intel_AL_Capability structure present
                            */
    UINT32 link_intel_al_capability_offset : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000400*/

                            /*
                               This defines the offset of the
                               Intel_AL_Capability relative to beginning of
                               Intel_AL_Capability_Header register
                            */

  } Bits;
  UINT32 Data;

} INTEL_AL_LINK_CAPABILITY_POINTER_IIO_CXL_CM_STRUCT;

/* INTEL_AL_PMON_UNIT0_CAPABILITY_POINTER_IIO_CXL_CM_REG supported on:          */
/*      SPRA0 (0x20001010)                                                      */
/*      SPRB0 (0x20001010)                                                      */
/*      SPRHBM (0x20001010)                                                     */
/*      SPRC0 (0x20001010)                                                      */
/*      SPRMCC (0x20001010)                                                     */
/*      SPRUCC (0x20001010)                                                     */
/* Register default value on SPRA0: 0x3001000A                                  */
/* Register default value on SPRB0: 0x3001000A                                  */
/* Register default value on SPRHBM: 0x3001000A                                 */
/* Register default value on SPRC0: 0x3001000A                                  */
/* Register default value on SPRMCC: 0x3001000A                                 */
/* Register default value on SPRUCC: 0x3001000A                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Intel AL PMON Unit0 Capability Pointer
*/


#define INTEL_AL_PMON_UNIT0_CAPABILITY_POINTER_IIO_CXL_CM_REG 0x23121010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 intel_al_capability_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000000A*/

                            /*
                               This defines the nature and format of the
                               Intel_AL_Capability register. For the
                               Intel_AL_PMON_Unit0_Capability register, this
                               field should be 0x4
                            */
    UINT32 intel_al_capability_version : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               This defines the version number of the
                               Intel_AL_Capability structure present
                            */
    UINT32 pmon_unit0_intel_al_capability_offset : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000300*/

                            /*
                               This defines the offset of the
                               Intel_AL_Capability relative to beginning of
                               Intel_AL_Capability_Header register
                            */

  } Bits;
  UINT32 Data;

} INTEL_AL_PMON_UNIT0_CAPABILITY_POINTER_IIO_CXL_CM_STRUCT;

/* INTEL_AL_PMON_UNIT1_CAPABILITY_POINTER_IIO_CXL_CM_REG supported on:          */
/*      SPRA0 (0x20001014)                                                      */
/*      SPRB0 (0x20001014)                                                      */
/*      SPRHBM (0x20001014)                                                     */
/*      SPRC0 (0x20001014)                                                      */
/*      SPRMCC (0x20001014)                                                     */
/*      SPRUCC (0x20001014)                                                     */
/* Register default value on SPRA0: 0xB001000B                                  */
/* Register default value on SPRB0: 0xB001000B                                  */
/* Register default value on SPRHBM: 0xB001000B                                 */
/* Register default value on SPRC0: 0xB001000B                                  */
/* Register default value on SPRMCC: 0xB001000B                                 */
/* Register default value on SPRUCC: 0xB001000B                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Intel AL PMON Unit1 Capability Pointer
*/


#define INTEL_AL_PMON_UNIT1_CAPABILITY_POINTER_IIO_CXL_CM_REG 0x23121014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 intel_al_capability_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000000B*/

                            /*
                               This defines the nature and format of the
                               Intel_AL_Capability register. For the
                               Intel_AL_PMON_Unit1_Capability register, this
                               field should be 0x5
                            */
    UINT32 intel_al_capability_version : 4;

                            /* Bits[19:16], Access Type=RO, default=0x00000001*/

                            /*
                               This defines the version number of the
                               Intel_AL_Capability structure present
                            */
    UINT32 pmon_unit1_intel_al_capability_offset : 12;

                            /* Bits[31:20], Access Type=RO, default=0x00000B00*/

                            /*
                               This defines the offset of the
                               Intel_AL_Capability relative to beginning of
                               Intel_AL_Capability_Header register
                            */

  } Bits;
  UINT32 Data;

} INTEL_AL_PMON_UNIT1_CAPABILITY_POINTER_IIO_CXL_CM_STRUCT;

/* UNCORRECTABLE_ERROR_STATUS_REGISTER_IIO_CXL_CM_REG supported on:             */
/*      SPRA0 (0x20001100)                                                      */
/*      SPRB0 (0x20001100)                                                      */
/*      SPRHBM (0x20001100)                                                     */
/*      SPRC0 (0x20001100)                                                      */
/*      SPRMCC (0x20001100)                                                     */
/*      SPRUCC (0x20001100)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Status register for Uncorrectable Errors
*/


#define UNCORRECTABLE_ERROR_STATUS_REGISTER_IIO_CXL_CM_REG 0x23121100

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cache_data_parity : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Data Parity on IDI/IAL.cache. Header Log
                               contains H2D Data Header.
                            */
    UINT32 cache_address_parity : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Address Parity on IDI/IAL.cache. Header Log
                               contains H2D Request.
                            */
    UINT32 cache_be_parity : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               BE Parity on IDI/IAL.cache. Header Log contains
                               H2D Data Header.
                            */
    UINT32 cache_data_ecc : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Data ECC on IDI/IAL.cache. Header Log contains
                               H2D Data Header.
                            */
    UINT32 mem_data_parity : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Data Parity on IAL.mem. Header Log contains M2S
                               RwD Data Header.
                            */
    UINT32 mem_address_parity : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Address Parity on IAL.mem. If Bit 0 of Header
                               Log is '0, rest of Header Log contains M2S Req.
                               If Bit 0 of Header Log is '1, rest of Header Log
                               contains M2S RwD.
                            */
    UINT32 mem_be_parity : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               BE Parity on IAL.mem. Header Log contains M2S
                               RwD Data Header.
                            */
    UINT32 mem_data_ecc : 1;

                            /* Bits[7:7], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Data ECC on IAL.mem. Header Log contains M2S RwD
                               Data Header.
                            */
    UINT32 reinit_threshold : 1;

                            /* Bits[8:8], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* REINIT Threshold Hit. Header Log not applicable. */
    UINT32 rsvd_encoding_violation : 1;

                            /* Bits[9:9], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Received unrecognized encoding. Header Log
                               contains the entire flit received.
                            */
    UINT32 poison_received : 1;

                            /* Bits[10:10], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Received Poison from Device. Header Log contains
                               the entire flit received.
                            */
    UINT32 receiver_overflow : 1;

                            /* Bits[11:11], Access Type=RW/1C/V/P, default=0x00000000*/

                            /*
                               Receiver Overflow. First 3b of the Header Log
                               are relevant and should be interpreted as such:
                               3'b000 --> D2H Req 3'b001 --> D2H Rsp 3'b010 -->
                               D2H Data 3'b100 --> S2M NDR 3'b101 --> S2M DRS
                               The above shows which buffer had the overflow
                            */
    UINT32 llrb_crc : 1;

                            /* Bits[12:12], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* LLRB bad CRC condition */
    UINT32 duplicate_retry_ack : 1;

                            /* Bits[13:13], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Retry.Ack received when LRSM is in Normal State */
    UINT32 internal_error : 1;

                            /* Bits[14:14], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Internal_Error */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UNCORRECTABLE_ERROR_STATUS_REGISTER_IIO_CXL_CM_STRUCT;

/* UNCORRECTABLE_ERROR_MASK_REGISTER_IIO_CXL_CM_REG supported on:               */
/*      SPRA0 (0x20001104)                                                      */
/*      SPRB0 (0x20001104)                                                      */
/*      SPRHBM (0x20001104)                                                     */
/*      SPRC0 (0x20001104)                                                      */
/*      SPRMCC (0x20001104)                                                     */
/*      SPRUCC (0x20001104)                                                     */
/* Register default value on SPRA0: 0x00007FFF                                  */
/* Register default value on SPRB0: 0x00007FFF                                  */
/* Register default value on SPRHBM: 0x00007FFF                                 */
/* Register default value on SPRC0: 0x00007FFF                                  */
/* Register default value on SPRMCC: 0x00007FFF                                 */
/* Register default value on SPRUCC: 0x00007FFF                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Mask register for Uncorrectable Errors
*/


#define UNCORRECTABLE_ERROR_MASK_REGISTER_IIO_CXL_CM_REG 0x23121104

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cache_data_parity_mask : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /* Data Parity on IDI/IAL.cache Mask */
    UINT32 cache_address_parity_mask : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /* Address Parity on IDI/IAL.cache Mask */
    UINT32 cache_be_parity_mask : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /* BE Parity on IDI/IAL.cache Mask */
    UINT32 cache_data_ecc_mask : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /* Data ECC on IDI/IAL.cache Mask */
    UINT32 mem_data_parity_mask : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /* Data Parity on IAL.mem Mask */
    UINT32 mem_address_parity_mask : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* Address Parity on IAL.mem Mask */
    UINT32 mem_be_parity_mask : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /* BE Parity on IAL.mem Mask */
    UINT32 mem_data_ecc_mask : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /* Data ECC on IAL.mem Mask */
    UINT32 reinit_threshold_mask : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000001*/

                            /* REINIT Threshold Mask */
    UINT32 rsvd_encoding_violation_mask : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000001*/

                            /* Received unrecognized encoding Mask */
    UINT32 poison_received_mask : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000001*/

                            /* Received Poison from Device Mask */
    UINT32 receiver_overflow_mask : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000001*/

                            /* Receiver Overflow Mask */
    UINT32 llrb_crc_mask : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000001*/

                            /* LLRB bad CRC condition Mask */
    UINT32 duplicate_retry_ack_mask : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /*
                               Retry.Ack received when LRSM is in Normal State
                               Mask
                            */
    UINT32 internal_error_mask : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /* Internal_Error Mask */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UNCORRECTABLE_ERROR_MASK_REGISTER_IIO_CXL_CM_STRUCT;

/* UNCORRECTABLE_ERROR_SEVERITY_REGISTER_IIO_CXL_CM_REG supported on:           */
/*      SPRA0 (0x20001108)                                                      */
/*      SPRB0 (0x20001108)                                                      */
/*      SPRHBM (0x20001108)                                                     */
/*      SPRC0 (0x20001108)                                                      */
/*      SPRMCC (0x20001108)                                                     */
/*      SPRUCC (0x20001108)                                                     */
/* Register default value on SPRA0: 0x00007FFF                                  */
/* Register default value on SPRB0: 0x00007FFF                                  */
/* Register default value on SPRHBM: 0x00007FFF                                 */
/* Register default value on SPRC0: 0x00007FFF                                  */
/* Register default value on SPRMCC: 0x00007FFF                                 */
/* Register default value on SPRUCC: 0x00007FFF                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Severity register for Uncorrectable Errors
*/


#define UNCORRECTABLE_ERROR_SEVERITY_REGISTER_IIO_CXL_CM_REG 0x23121108

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cache_data_parity_severity : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /* Data Parity on IDI/IAL.cache Severity */
    UINT32 cache_address_parity_severity : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /* Address Parity on IDI/IAL.cache Severity */
    UINT32 cache_be_parity_severity : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /* BE Parity on IDI/IAL.cache Severity */
    UINT32 cache_data_ecc_severity : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /* Data ECC on IDI/IAL.cache Severity */
    UINT32 mem_data_parity_severity : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /* Data Parity on IAL.mem Severity */
    UINT32 mem_address_parity_severity : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* Address Parity on IAL.mem Severity */
    UINT32 mem_be_parity_severity : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /* BE Parity on IAL.mem Severity */
    UINT32 mem_data_ecc_severity : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /* Data ECC on IAL.mem Severity */
    UINT32 reinit_threshold_severity : 1;

                            /* Bits[8:8], Access Type=RW/P, default=0x00000001*/

                            /* REINIT Threshold Severity */
    UINT32 rsvd_encoding_violation_severity : 1;

                            /* Bits[9:9], Access Type=RW/P, default=0x00000001*/

                            /* Received unrecognized encoding Severity */
    UINT32 poison_received_severity : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000001*/

                            /* Received Poison from Device Severity */
    UINT32 receiver_overflow_severity : 1;

                            /* Bits[11:11], Access Type=RW/P, default=0x00000001*/

                            /* Receiver Overflow Severity */
    UINT32 llrb_crc_severity : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000001*/

                            /* LLRB bad CRC condition Severity */
    UINT32 duplicate_retry_ack_severity : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /*
                               Retry.Ack received when LRSM is in Normal State
                               Severity
                            */
    UINT32 internal_error_severity : 1;

                            /* Bits[14:14], Access Type=RW/P, default=0x00000001*/

                            /* Internal_Error Severity */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UNCORRECTABLE_ERROR_SEVERITY_REGISTER_IIO_CXL_CM_STRUCT;

/* CORRECTABLE_ERROR_STATUS_REGISTER_IIO_CXL_CM_REG supported on:               */
/*      SPRA0 (0x2000110c)                                                      */
/*      SPRB0 (0x2000110c)                                                      */
/*      SPRHBM (0x2000110c)                                                     */
/*      SPRC0 (0x2000110c)                                                      */
/*      SPRMCC (0x2000110c)                                                     */
/*      SPRUCC (0x2000110c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Status register for correctable Errors
*/


#define CORRECTABLE_ERROR_STATUS_REGISTER_IIO_CXL_CM_REG 0x2312110C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cache_data_ecc : 1;

                            /* Bits[0:0], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Data ECC on IDI/IAL.cache */
    UINT32 mem_data_ecc : 1;

                            /* Bits[1:1], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Data ECC on IAL.mem */
    UINT32 crc_threshold : 1;

                            /* Bits[2:2], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* CRC Error Threshold Hit */
    UINT32 retry_threshold : 1;

                            /* Bits[3:3], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Retry Threshold Hit - REINIT Triggered */
    UINT32 cache_data_poison : 1;

                            /* Bits[4:4], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Cache Data Poison Received */
    UINT32 mem_data_poison : 1;

                            /* Bits[5:5], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Mem Data Poison Received */
    UINT32 pl_error_received : 1;

                            /* Bits[6:6], Access Type=RW/1C/V/P, default=0x00000000*/

                            /* Received PL_CError or PL_Error on LPIF Interface */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CORRECTABLE_ERROR_STATUS_REGISTER_IIO_CXL_CM_STRUCT;

/* CORRECTABLE_ERROR_MASK_REGISTER_IIO_CXL_CM_REG supported on:                 */
/*      SPRA0 (0x20001110)                                                      */
/*      SPRB0 (0x20001110)                                                      */
/*      SPRHBM (0x20001110)                                                     */
/*      SPRC0 (0x20001110)                                                      */
/*      SPRMCC (0x20001110)                                                     */
/*      SPRUCC (0x20001110)                                                     */
/* Register default value on SPRA0: 0x0000007F                                  */
/* Register default value on SPRB0: 0x0000007F                                  */
/* Register default value on SPRHBM: 0x0000007F                                 */
/* Register default value on SPRC0: 0x0000007F                                  */
/* Register default value on SPRMCC: 0x0000007F                                 */
/* Register default value on SPRUCC: 0x0000007F                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Mask register for correctable Errors
*/


#define CORRECTABLE_ERROR_MASK_REGISTER_IIO_CXL_CM_REG 0x23121110

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cache_data_ecc_mask : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000001*/

                            /* Data ECC on IDI/IAL.cache Mask */
    UINT32 mem_data_ecc_mask : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /* Data ECC on IAL.mem Mask */
    UINT32 crc_threshold_mask : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /* CRC Error Threshold Mask */
    UINT32 retry_threshold_mask : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000001*/

                            /* Retry Threshold Hit - REINIT Triggered Mask */
    UINT32 cache_data_poison_mask : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000001*/

                            /* Cache Data Poison Mask */
    UINT32 mem_data_poison_mask : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000001*/

                            /* Mem Data Poison Mask */
    UINT32 pl_error_received_mask : 1;

                            /* Bits[6:6], Access Type=RW/P, default=0x00000001*/

                            /*
                               Received PL_CError or PL_Error on LPIF Interface
                               Mask
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CORRECTABLE_ERROR_MASK_REGISTER_IIO_CXL_CM_STRUCT;

/* ERROR_CAPABILITIES_AND_CONTROL_REGISTER_IIO_CXL_CM_REG supported on:         */
/*      SPRA0 (0x20001114)                                                      */
/*      SPRB0 (0x20001114)                                                      */
/*      SPRHBM (0x20001114)                                                     */
/*      SPRC0 (0x20001114)                                                      */
/*      SPRMCC (0x20001114)                                                     */
/*      SPRUCC (0x20001114)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Error Control Register
*/


#define ERROR_CAPABILITIES_AND_CONTROL_REGISTER_IIO_CXL_CM_REG 0x23121114

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 first_error_pointer : 5;

                            /* Bits[4:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               The First Error Pointer is a field that
                               identifies the bit position of the first error
                               reported in the Uncorrectable Error Status
                               register
                            */
    UINT32 rsvd : 4;

                            /* Bits[8:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 multiple_header_recording_capable : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000000*/

                            /*
                               This bit indicates that the $M stack is not
                               capable of recording multiple headers
                            */
    UINT32 rsvd_10 : 3;

                            /* Bits[12:10], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 poison_enabled : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000000*/

                            /* Poison Enable Control */
    UINT32 rsvd_14 : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERROR_CAPABILITIES_AND_CONTROL_REGISTER_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_0_IIO_CXL_CM_REG supported on:                                    */
/*      SPRA0 (0x20001118)                                                      */
/*      SPRB0 (0x20001118)                                                      */
/*      SPRHBM (0x20001118)                                                     */
/*      SPRC0 (0x20001118)                                                      */
/*      SPRMCC (0x20001118)                                                     */
/*      SPRUCC (0x20001118)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_0_IIO_CXL_CM_REG 0x23121118

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_0_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_1_IIO_CXL_CM_REG supported on:                                    */
/*      SPRA0 (0x2000111c)                                                      */
/*      SPRB0 (0x2000111c)                                                      */
/*      SPRHBM (0x2000111c)                                                     */
/*      SPRC0 (0x2000111c)                                                      */
/*      SPRMCC (0x2000111c)                                                     */
/*      SPRUCC (0x2000111c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_1_IIO_CXL_CM_REG 0x2312111C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_1_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_2_IIO_CXL_CM_REG supported on:                                    */
/*      SPRA0 (0x20001120)                                                      */
/*      SPRB0 (0x20001120)                                                      */
/*      SPRHBM (0x20001120)                                                     */
/*      SPRC0 (0x20001120)                                                      */
/*      SPRMCC (0x20001120)                                                     */
/*      SPRUCC (0x20001120)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_2_IIO_CXL_CM_REG 0x23121120

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_2_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_3_IIO_CXL_CM_REG supported on:                                    */
/*      SPRA0 (0x20001124)                                                      */
/*      SPRB0 (0x20001124)                                                      */
/*      SPRHBM (0x20001124)                                                     */
/*      SPRC0 (0x20001124)                                                      */
/*      SPRMCC (0x20001124)                                                     */
/*      SPRUCC (0x20001124)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_3_IIO_CXL_CM_REG 0x23121124

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_3_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_4_IIO_CXL_CM_REG supported on:                                    */
/*      SPRA0 (0x20001128)                                                      */
/*      SPRB0 (0x20001128)                                                      */
/*      SPRHBM (0x20001128)                                                     */
/*      SPRC0 (0x20001128)                                                      */
/*      SPRMCC (0x20001128)                                                     */
/*      SPRUCC (0x20001128)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_4_IIO_CXL_CM_REG 0x23121128

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_4_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_5_IIO_CXL_CM_REG supported on:                                    */
/*      SPRA0 (0x2000112c)                                                      */
/*      SPRB0 (0x2000112c)                                                      */
/*      SPRHBM (0x2000112c)                                                     */
/*      SPRC0 (0x2000112c)                                                      */
/*      SPRMCC (0x2000112c)                                                     */
/*      SPRUCC (0x2000112c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_5_IIO_CXL_CM_REG 0x2312112C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_5_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_6_IIO_CXL_CM_REG supported on:                                    */
/*      SPRA0 (0x20001130)                                                      */
/*      SPRB0 (0x20001130)                                                      */
/*      SPRHBM (0x20001130)                                                     */
/*      SPRC0 (0x20001130)                                                      */
/*      SPRMCC (0x20001130)                                                     */
/*      SPRUCC (0x20001130)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_6_IIO_CXL_CM_REG 0x23121130

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_6_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_7_IIO_CXL_CM_REG supported on:                                    */
/*      SPRA0 (0x20001134)                                                      */
/*      SPRB0 (0x20001134)                                                      */
/*      SPRHBM (0x20001134)                                                     */
/*      SPRC0 (0x20001134)                                                      */
/*      SPRMCC (0x20001134)                                                     */
/*      SPRUCC (0x20001134)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_7_IIO_CXL_CM_REG 0x23121134

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_7_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_8_IIO_CXL_CM_REG supported on:                                    */
/*      SPRA0 (0x20001138)                                                      */
/*      SPRB0 (0x20001138)                                                      */
/*      SPRHBM (0x20001138)                                                     */
/*      SPRC0 (0x20001138)                                                      */
/*      SPRMCC (0x20001138)                                                     */
/*      SPRUCC (0x20001138)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_8_IIO_CXL_CM_REG 0x23121138

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_8_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_9_IIO_CXL_CM_REG supported on:                                    */
/*      SPRA0 (0x2000113c)                                                      */
/*      SPRB0 (0x2000113c)                                                      */
/*      SPRHBM (0x2000113c)                                                     */
/*      SPRC0 (0x2000113c)                                                      */
/*      SPRMCC (0x2000113c)                                                     */
/*      SPRUCC (0x2000113c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_9_IIO_CXL_CM_REG 0x2312113C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_9_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_10_IIO_CXL_CM_REG supported on:                                   */
/*      SPRA0 (0x20001140)                                                      */
/*      SPRB0 (0x20001140)                                                      */
/*      SPRHBM (0x20001140)                                                     */
/*      SPRC0 (0x20001140)                                                      */
/*      SPRMCC (0x20001140)                                                     */
/*      SPRUCC (0x20001140)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_10_IIO_CXL_CM_REG 0x23121140

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_10_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_11_IIO_CXL_CM_REG supported on:                                   */
/*      SPRA0 (0x20001144)                                                      */
/*      SPRB0 (0x20001144)                                                      */
/*      SPRHBM (0x20001144)                                                     */
/*      SPRC0 (0x20001144)                                                      */
/*      SPRMCC (0x20001144)                                                     */
/*      SPRUCC (0x20001144)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_11_IIO_CXL_CM_REG 0x23121144

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_11_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_12_IIO_CXL_CM_REG supported on:                                   */
/*      SPRA0 (0x20001148)                                                      */
/*      SPRB0 (0x20001148)                                                      */
/*      SPRHBM (0x20001148)                                                     */
/*      SPRC0 (0x20001148)                                                      */
/*      SPRMCC (0x20001148)                                                     */
/*      SPRUCC (0x20001148)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_12_IIO_CXL_CM_REG 0x23121148

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_12_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_13_IIO_CXL_CM_REG supported on:                                   */
/*      SPRA0 (0x2000114c)                                                      */
/*      SPRB0 (0x2000114c)                                                      */
/*      SPRHBM (0x2000114c)                                                     */
/*      SPRC0 (0x2000114c)                                                      */
/*      SPRMCC (0x2000114c)                                                     */
/*      SPRUCC (0x2000114c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_13_IIO_CXL_CM_REG 0x2312114C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_13_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_14_IIO_CXL_CM_REG supported on:                                   */
/*      SPRA0 (0x20001150)                                                      */
/*      SPRB0 (0x20001150)                                                      */
/*      SPRHBM (0x20001150)                                                     */
/*      SPRC0 (0x20001150)                                                      */
/*      SPRMCC (0x20001150)                                                     */
/*      SPRUCC (0x20001150)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_14_IIO_CXL_CM_REG 0x23121150

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_14_IIO_CXL_CM_STRUCT;

/* HEADER_LOG_15_IIO_CXL_CM_REG supported on:                                   */
/*      SPRA0 (0x20001154)                                                      */
/*      SPRB0 (0x20001154)                                                      */
/*      SPRHBM (0x20001154)                                                     */
/*      SPRC0 (0x20001154)                                                      */
/*      SPRMCC (0x20001154)                                                     */
/*      SPRUCC (0x20001154)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Header log register
*/


#define HEADER_LOG_15_IIO_CXL_CM_REG 0x23121154

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 header : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Header associated with the first error */

  } Bits;
  UINT32 Data;

} HEADER_LOG_15_IIO_CXL_CM_STRUCT;

/* FATAL_ERROR_MASK_REGISTER_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001160)                                                      */
/*      SPRB0 (0x20001160)                                                      */
/*      SPRHBM (0x20001160)                                                     */
/*      SPRC0 (0x20001160)                                                      */
/*      SPRMCC (0x20001160)                                                     */
/*      SPRUCC (0x20001160)                                                     */
/* Register default value on SPRA0: 0x00007FFF                                  */
/* Register default value on SPRB0: 0x00007FFF                                  */
/* Register default value on SPRHBM: 0x00007FFF                                 */
/* Register default value on SPRC0: 0x00007FFF                                  */
/* Register default value on SPRMCC: 0x00007FFF                                 */
/* Register default value on SPRUCC: 0x00007FFF                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* This register enables fatal/viral signaling for uncorrectable errors detected by IAL.$M IP
*/


#define FATAL_ERROR_MASK_REGISTER_IIO_CXL_CM_REG 0x23121160

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cache_data_parity_mask : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Data Parity on IDI/IAL.cache Mask */
    UINT32 cache_address_parity_mask : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /* Address Parity on IDI/IAL.cache Mask */
    UINT32 cache_be_parity_mask : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /* BE Parity on IDI/IAL.cache Mask */
    UINT32 cache_data_ecc_mask : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /* Data ECC on IDI/IAL.cache Mask */
    UINT32 mem_data_parity_mask : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /* Data Parity on IAL.mem Mask */
    UINT32 mem_address_parity_mask : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /* Address Parity on IAL.mem Mask */
    UINT32 mem_be_parity_mask : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /* BE Parity on IAL.mem Mask */
    UINT32 mem_data_ecc_mask : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /* Data ECC on IAL.mem Mask */
    UINT32 reinit_threshold_mask : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /* REINIT Threshold Mask */
    UINT32 rsvd_encoding_violation_mask : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /* Received unrecognized encoding Mask */
    UINT32 poison_received_mask : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /* Received Poison from Device Mask */
    UINT32 receiver_overflow_mask : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /* Receiver Overflow Mask */
    UINT32 llrb_crc_mask : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /* LLRB bad CRC condition Mask */
    UINT32 duplicate_retry_ack_mask : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Retry.Ack received when LRSM is in Normal State
                               Mask
                            */
    UINT32 internal_error_mask : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /* Internal_Error Mask */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} FATAL_ERROR_MASK_REGISTER_IIO_CXL_CM_STRUCT;

/* RXHDRMATCH0_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x20001200)                                                      */
/*      SPRB0 (0x20001200)                                                      */
/*      SPRHBM (0x20001200)                                                     */
/*      SPRC0 (0x20001200)                                                      */
/*      SPRMCC (0x20001200)                                                     */
/*      SPRUCC (0x20001200)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the flit to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the flit is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define RXHDRMATCH0_IIO_CXL_CM_REG 0x23121200

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reqopcode : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /* IAL.$ Req Opcode */
    UINT32 rspopcode : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /* IAL.$ Rsp Opcode */
    UINT32 cqid : 12;

                            /* Bits[21:10], Access Type=RW, default=0x00000000*/

                            /* IAL.$ CQID */
    UINT32 clos : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /* IAL.$ CLOS */
    UINT32 sai : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /* IAL.$ SAI */
    UINT32 nt : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /* IAL.$ NT */
    UINT32 cachenear : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* IAL.$ CN */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXHDRMATCH0_IIO_CXL_CM_STRUCT;

/* RXHDRMATCH1_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x20001204)                                                      */
/*      SPRB0 (0x20001204)                                                      */
/*      SPRHBM (0x20001204)                                                     */
/*      SPRC0 (0x20001204)                                                      */
/*      SPRMCC (0x20001204)                                                     */
/*      SPRUCC (0x20001204)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the flit to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the flit is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define RXHDRMATCH1_IIO_CXL_CM_REG 0x23121204

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 uqid : 12;

                            /* Bits[11:0], Access Type=RW, default=0x00000000*/

                            /* IAL.$ UQID */
    UINT32 chunkvalid : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* IAL.$ ChunkValid */
    UINT32 bogus : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* IAL.$ Bogus */
    UINT32 poison : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* IAL.$ Poison */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXHDRMATCH1_IIO_CXL_CM_STRUCT;

/* RXHDRMATCH2_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x20001208)                                                      */
/*      SPRB0 (0x20001208)                                                      */
/*      SPRHBM (0x20001208)                                                     */
/*      SPRC0 (0x20001208)                                                      */
/*      SPRMCC (0x20001208)                                                     */
/*      SPRUCC (0x20001208)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the flit to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the flit is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define RXHDRMATCH2_IIO_CXL_CM_REG 0x23121208

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s2mndropcode : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem NDR Opcode */
    UINT32 s2mdrsopcode : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem DRS Opcode */
    UINT32 metafield : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem MetaField */
    UINT32 metavalue : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem MetaValue */
    UINT32 tag : 16;

                            /* Bits[25:10], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem Tag */
    UINT32 poison : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem Poison */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXHDRMATCH2_IIO_CXL_CM_STRUCT;

/* RXHDRMATCH3_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x2000120c)                                                      */
/*      SPRB0 (0x2000120c)                                                      */
/*      SPRHBM (0x2000120c)                                                     */
/*      SPRC0 (0x2000120c)                                                      */
/*      SPRMCC (0x2000120c)                                                     */
/*      SPRUCC (0x2000120c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the flit to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the flit is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define RXHDRMATCH3_IIO_CXL_CM_REG 0x2312120C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pcls : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem PCLS */
    UINT32 pre : 7;

                            /* Bits[10:4], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem PRE */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXHDRMATCH3_IIO_CXL_CM_STRUCT;

/* RXHDRMASK0_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001210)                                                      */
/*      SPRB0 (0x20001210)                                                      */
/*      SPRHBM (0x20001210)                                                     */
/*      SPRC0 (0x20001210)                                                      */
/*      SPRMCC (0x20001210)                                                     */
/*      SPRUCC (0x20001210)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* The header bits to be used in matching are selected by the mask bits in this register. A zero in a mask bit position causes the corresponding bit in the header to be ignored. A one in a mask bit position indicates that the corresponding bit in the flit must match the value of the same bit in the Match register for that bit to be considered to have matched. 
*/


#define RXHDRMASK0_IIO_CXL_CM_REG 0x23121210

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reqopcode : 5;

                            /* Bits[4:0], Access Type=RW, default=0x00000000*/

                            /* IAL.$ ReqOpcode */
    UINT32 rspopcode : 5;

                            /* Bits[9:5], Access Type=RW, default=0x00000000*/

                            /* IAL.$ RspOpcode */
    UINT32 cqid : 12;

                            /* Bits[21:10], Access Type=RW, default=0x00000000*/

                            /* IAL.$ CQID */
    UINT32 clos : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /* IAL.$ CLOS */
    UINT32 sai : 4;

                            /* Bits[27:24], Access Type=RW, default=0x00000000*/

                            /* IAL.$ SAI */
    UINT32 nt : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /* IAL.$ NT */
    UINT32 cachenear : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* IAL.$ CacheNear */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXHDRMASK0_IIO_CXL_CM_STRUCT;

/* RXHDRMASK1_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001214)                                                      */
/*      SPRB0 (0x20001214)                                                      */
/*      SPRHBM (0x20001214)                                                     */
/*      SPRC0 (0x20001214)                                                      */
/*      SPRMCC (0x20001214)                                                     */
/*      SPRUCC (0x20001214)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* The header bits to be used in matching are selected by the mask bits in this register. A zero in a mask bit position causes the corresponding bit in the header to be ignored. A one in a mask bit position indicates that the corresponding bit in the flit must match the value of the same bit in the Match register for that bit to be considered to have matched. 
*/


#define RXHDRMASK1_IIO_CXL_CM_REG 0x23121214

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 uqid : 12;

                            /* Bits[11:0], Access Type=RW, default=0x00000000*/

                            /* IAL.$ UQID */
    UINT32 chunkvalid : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* IAL.$ ChunkValid */
    UINT32 bogus : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* IAL.$ Bogus */
    UINT32 poison : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* IAL.$ Poison */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXHDRMASK1_IIO_CXL_CM_STRUCT;

/* RXHDRMASK2_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001218)                                                      */
/*      SPRB0 (0x20001218)                                                      */
/*      SPRHBM (0x20001218)                                                     */
/*      SPRC0 (0x20001218)                                                      */
/*      SPRMCC (0x20001218)                                                     */
/*      SPRUCC (0x20001218)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* The header bits to be used in matching are selected by the mask bits in this register. A zero in a mask bit position causes the corresponding bit in the header to be ignored. A one in a mask bit position indicates that the corresponding bit in the flit must match the value of the same bit in the Match register for that bit to be considered to have matched. 
*/


#define RXHDRMASK2_IIO_CXL_CM_REG 0x23121218

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s2mndropcode : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem S2MNDROpcode */
    UINT32 s2mdrsopcode : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem S2MDRSOpcode */
    UINT32 metafield : 2;

                            /* Bits[7:6], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem MetaField */
    UINT32 metavalue : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem MetaValue */
    UINT32 tag : 16;

                            /* Bits[25:10], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem Tag */
    UINT32 poison : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem Poison */
    UINT32 rsvd : 5;

                            /* Bits[31:27], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXHDRMASK2_IIO_CXL_CM_STRUCT;

/* RXHDRMASK3_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x2000121c)                                                      */
/*      SPRB0 (0x2000121c)                                                      */
/*      SPRHBM (0x2000121c)                                                     */
/*      SPRC0 (0x2000121c)                                                      */
/*      SPRMCC (0x2000121c)                                                     */
/*      SPRUCC (0x2000121c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* The header bits to be used in matching are selected by the mask bits in this register. A zero in a mask bit position causes the corresponding bit in the header to be ignored. A one in a mask bit position indicates that the corresponding bit in the flit must match the value of the same bit in the Match register for that bit to be considered to have matched. 
*/


#define RXHDRMASK3_IIO_CXL_CM_REG 0x2312121C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pcls : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem PCLS */
    UINT32 pre : 7;

                            /* Bits[10:4], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem PRE */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXHDRMASK3_IIO_CXL_CM_STRUCT;

/* TXHDRMATCH0_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x20001220)                                                      */
/*      SPRB0 (0x20001220)                                                      */
/*      SPRHBM (0x20001220)                                                     */
/*      SPRC0 (0x20001220)                                                      */
/*      SPRMCC (0x20001220)                                                     */
/*      SPRUCC (0x20001220)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the flit to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the flit is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define TXHDRMATCH0_IIO_CXL_CM_REG 0x23121220

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reqopcode : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* IAL.$ ReqOpcode */
    UINT32 rsvd : 2;

                            /* Bits[4:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rspopcode : 4;

                            /* Bits[8:5], Access Type=RW, default=0x00000000*/

                            /* IAL.$ RspOpcode */
    UINT32 rsvd_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cqid : 12;

                            /* Bits[21:10], Access Type=RW, default=0x00000000*/

                            /* IAL.$ CQID */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXHDRMATCH0_IIO_CXL_CM_STRUCT;

/* TXHDRMATCH1_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x20001224)                                                      */
/*      SPRB0 (0x20001224)                                                      */
/*      SPRHBM (0x20001224)                                                     */
/*      SPRC0 (0x20001224)                                                      */
/*      SPRMCC (0x20001224)                                                     */
/*      SPRUCC (0x20001224)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the flit to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the flit is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define TXHDRMATCH1_IIO_CXL_CM_REG 0x23121224

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 uqid : 12;

                            /* Bits[11:0], Access Type=RW, default=0x00000000*/

                            /* IAL.$ UQID */
    UINT32 chunkvalid : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* IAL.$ ChunkValid */
    UINT32 rsvd : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 poison : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* IAL.$ Poison */
    UINT32 rsp_pre : 2;

                            /* Bits[16:15], Access Type=RW, default=0x00000000*/

                            /* IAL.$ RSP_PRE */
    UINT32 data_pre : 7;

                            /* Bits[23:17], Access Type=RW, default=0x00000000*/

                            /* IAL.$ DATA_PRE */
    UINT32 go_err : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /* IAL.$ GO_ERR */
    UINT32 rsvd_25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXHDRMATCH1_IIO_CXL_CM_STRUCT;

/* TXHDRMATCH2_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x20001228)                                                      */
/*      SPRB0 (0x20001228)                                                      */
/*      SPRHBM (0x20001228)                                                     */
/*      SPRC0 (0x20001228)                                                      */
/*      SPRMCC (0x20001228)                                                     */
/*      SPRUCC (0x20001228)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the flit to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the flit is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define TXHDRMATCH2_IIO_CXL_CM_REG 0x23121228

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 m2sreqopcode : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem M2SREQOpcode */
    UINT32 m2srwdopcode : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem M2SRwDOpcode */
    UINT32 metafield : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem MetaField */
    UINT32 metavalue : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem MetaValue */
    UINT32 snptype : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem SnpType */
    UINT32 poison : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem Poison */
    UINT32 tc : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem TC */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXHDRMATCH2_IIO_CXL_CM_STRUCT;

/* TXHDRMATCH3_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x2000122c)                                                      */
/*      SPRB0 (0x2000122c)                                                      */
/*      SPRHBM (0x2000122c)                                                     */
/*      SPRC0 (0x2000122c)                                                      */
/*      SPRMCC (0x2000122c)                                                     */
/*      SPRUCC (0x2000122c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the flit to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the flit is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define TXHDRMATCH3_IIO_CXL_CM_REG 0x2312122C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tag : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem Tag */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXHDRMATCH3_IIO_CXL_CM_STRUCT;

/* TXHDRMASK0_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001230)                                                      */
/*      SPRB0 (0x20001230)                                                      */
/*      SPRHBM (0x20001230)                                                     */
/*      SPRC0 (0x20001230)                                                      */
/*      SPRMCC (0x20001230)                                                     */
/*      SPRUCC (0x20001230)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* The header bits to be used in matching are selected by the mask bits in this register. A zero in a mask bit position causes the corresponding bit in the header to be ignored. A one in a mask bit position indicates that the corresponding bit in the flit must match the value of the same bit in the Match register for that bit to be considered to have matched. 
*/


#define TXHDRMASK0_IIO_CXL_CM_REG 0x23121230

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 reqopcode : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* IAL.$ ReqOpcode */
    UINT32 rsvd : 2;

                            /* Bits[4:3], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rspopcode : 4;

                            /* Bits[8:5], Access Type=RW, default=0x00000000*/

                            /* IAL.$ RspOpcode */
    UINT32 rsvd_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 cqid : 12;

                            /* Bits[21:10], Access Type=RW, default=0x00000000*/

                            /* IAL.$ CQID */
    UINT32 rsvd_22 : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXHDRMASK0_IIO_CXL_CM_STRUCT;

/* TXHDRMASK1_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001234)                                                      */
/*      SPRB0 (0x20001234)                                                      */
/*      SPRHBM (0x20001234)                                                     */
/*      SPRC0 (0x20001234)                                                      */
/*      SPRMCC (0x20001234)                                                     */
/*      SPRUCC (0x20001234)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* The header bits to be used in matching are selected by the mask bits in this register. A zero in a mask bit position causes the corresponding bit in the header to be ignored. A one in a mask bit position indicates that the corresponding bit in the flit must match the value of the same bit in the Match register for that bit to be considered to have matched. 
*/


#define TXHDRMASK1_IIO_CXL_CM_REG 0x23121234

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 uqid : 12;

                            /* Bits[11:0], Access Type=RW, default=0x00000000*/

                            /* IAL.$ UQID */
    UINT32 chunkvalid : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* IAL.$ ChunkValid */
    UINT32 rsvd : 1;

                            /* Bits[13:13], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 poison : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* IAL.$ Poison */
    UINT32 rsp_pre : 2;

                            /* Bits[16:15], Access Type=RW, default=0x00000000*/

                            /* IAL.$ RSP_PRE */
    UINT32 data_pre : 7;

                            /* Bits[23:17], Access Type=RW, default=0x00000000*/

                            /* IAL.$ DATA_PRE */
    UINT32 go_err : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /* IAL.$ GO_ERR */
    UINT32 rsvd_25 : 7;

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXHDRMASK1_IIO_CXL_CM_STRUCT;

/* TXHDRMASK2_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001238)                                                      */
/*      SPRB0 (0x20001238)                                                      */
/*      SPRHBM (0x20001238)                                                     */
/*      SPRC0 (0x20001238)                                                      */
/*      SPRMCC (0x20001238)                                                     */
/*      SPRUCC (0x20001238)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* The header bits to be used in matching are selected by the mask bits in this register. A zero in a mask bit position causes the corresponding bit in the header to be ignored. A one in a mask bit position indicates that the corresponding bit in the flit must match the value of the same bit in the Match register for that bit to be considered to have matched. 
*/


#define TXHDRMASK2_IIO_CXL_CM_REG 0x23121238

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 m2sreqopcode : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem M2SREQOpcode */
    UINT32 m2srwdopcode : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /* IAL.mem RwDOpcode */
    UINT32 metafield : 2;

                            /* Bits[9:8], Access Type=RW, default=0x00000000*/

                            /* IAL.$ MetaField */
    UINT32 metavalue : 2;

                            /* Bits[11:10], Access Type=RW, default=0x00000000*/

                            /* IAL.$ MetaValue */
    UINT32 snptype : 3;

                            /* Bits[14:12], Access Type=RW, default=0x00000000*/

                            /* IAL.$ SnpType */
    UINT32 poison : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* IAL.$ Poison */
    UINT32 tc : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /* IAL.$ TC */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXHDRMASK2_IIO_CXL_CM_STRUCT;

/* TXHDRMASK3_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x2000123c)                                                      */
/*      SPRB0 (0x2000123c)                                                      */
/*      SPRHBM (0x2000123c)                                                     */
/*      SPRC0 (0x2000123c)                                                      */
/*      SPRMCC (0x2000123c)                                                     */
/*      SPRUCC (0x2000123c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* The header bits to be used in matching are selected by the mask bits in this register. A zero in a mask bit position causes the corresponding bit in the header to be ignored. A one in a mask bit position indicates that the corresponding bit in the flit must match the value of the same bit in the Match register for that bit to be considered to have matched. 
*/


#define TXHDRMASK3_IIO_CXL_CM_REG 0x2312123C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tag : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* IAL.Mem Tag */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXHDRMASK3_IIO_CXL_CM_STRUCT;

/* ADDRMATCH0_LO_START_IIO_CXL_CM_REG supported on:                             */
/*      SPRA0 (0x20001240)                                                      */
/*      SPRB0 (0x20001240)                                                      */
/*      SPRHBM (0x20001240)                                                     */
/*      SPRC0 (0x20001240)                                                      */
/*      SPRMCC (0x20001240)                                                     */
/*      SPRUCC (0x20001240)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* First Address Match Register Start - Lower Address Bits
*/


#define ADDRMATCH0_LO_START_IIO_CXL_CM_REG 0x23121240

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 5;

                            /* Bits[4:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addr_31_5 : 27;

                            /* Bits[31:5], Access Type=RW, default=0x00000000*/

                            /* Addr_31_5 */

  } Bits;
  UINT32 Data;

} ADDRMATCH0_LO_START_IIO_CXL_CM_STRUCT;

/* ADDRMATCH0_HI_START_IIO_CXL_CM_REG supported on:                             */
/*      SPRA0 (0x20001244)                                                      */
/*      SPRB0 (0x20001244)                                                      */
/*      SPRHBM (0x20001244)                                                     */
/*      SPRC0 (0x20001244)                                                      */
/*      SPRMCC (0x20001244)                                                     */
/*      SPRUCC (0x20001244)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* First Address Match Register Start - Upper Address Bits
*/


#define ADDRMATCH0_HI_START_IIO_CXL_CM_REG 0x23121244

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr_51_32 : 20;

                            /* Bits[19:0], Access Type=RW, default=0x00000000*/

                            /* Addr_51_32 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ADDRMATCH0_HI_START_IIO_CXL_CM_STRUCT;

/* ADDRMATCH0_LO_END_IIO_CXL_CM_REG supported on:                               */
/*      SPRA0 (0x20001248)                                                      */
/*      SPRB0 (0x20001248)                                                      */
/*      SPRHBM (0x20001248)                                                     */
/*      SPRC0 (0x20001248)                                                      */
/*      SPRMCC (0x20001248)                                                     */
/*      SPRUCC (0x20001248)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* First Address Match Register End - Lower Address Bits
*/


#define ADDRMATCH0_LO_END_IIO_CXL_CM_REG 0x23121248

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 5;

                            /* Bits[4:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addr_31_5 : 27;

                            /* Bits[31:5], Access Type=RW, default=0x00000000*/

                            /* Addr_31_5 */

  } Bits;
  UINT32 Data;

} ADDRMATCH0_LO_END_IIO_CXL_CM_STRUCT;

/* ADDRMATCH0_HI_END_IIO_CXL_CM_REG supported on:                               */
/*      SPRA0 (0x2000124c)                                                      */
/*      SPRB0 (0x2000124c)                                                      */
/*      SPRHBM (0x2000124c)                                                     */
/*      SPRC0 (0x2000124c)                                                      */
/*      SPRMCC (0x2000124c)                                                     */
/*      SPRUCC (0x2000124c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* First Address Match Register End - Upper Address Bits
*/


#define ADDRMATCH0_HI_END_IIO_CXL_CM_REG 0x2312124C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr_51_32 : 20;

                            /* Bits[19:0], Access Type=RW, default=0x00000000*/

                            /* Addr_51_32 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ADDRMATCH0_HI_END_IIO_CXL_CM_STRUCT;

/* ADDRMATCH1_LO_START_IIO_CXL_CM_REG supported on:                             */
/*      SPRA0 (0x20001250)                                                      */
/*      SPRB0 (0x20001250)                                                      */
/*      SPRHBM (0x20001250)                                                     */
/*      SPRC0 (0x20001250)                                                      */
/*      SPRMCC (0x20001250)                                                     */
/*      SPRUCC (0x20001250)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Second Address Match Register Start - Lower Address Bits
*/


#define ADDRMATCH1_LO_START_IIO_CXL_CM_REG 0x23121250

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 5;

                            /* Bits[4:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addr_31_5 : 27;

                            /* Bits[31:5], Access Type=RW, default=0x00000000*/

                            /* Addr_31_5 */

  } Bits;
  UINT32 Data;

} ADDRMATCH1_LO_START_IIO_CXL_CM_STRUCT;

/* ADDRMATCH1_HI_START_IIO_CXL_CM_REG supported on:                             */
/*      SPRA0 (0x20001254)                                                      */
/*      SPRB0 (0x20001254)                                                      */
/*      SPRHBM (0x20001254)                                                     */
/*      SPRC0 (0x20001254)                                                      */
/*      SPRMCC (0x20001254)                                                     */
/*      SPRUCC (0x20001254)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Second Address Match Register Start - Upper Address Bits
*/


#define ADDRMATCH1_HI_START_IIO_CXL_CM_REG 0x23121254

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr_51_32 : 20;

                            /* Bits[19:0], Access Type=RW, default=0x00000000*/

                            /* Addr_51_32 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ADDRMATCH1_HI_START_IIO_CXL_CM_STRUCT;

/* ADDRMATCH1_LO_END_IIO_CXL_CM_REG supported on:                               */
/*      SPRA0 (0x20001258)                                                      */
/*      SPRB0 (0x20001258)                                                      */
/*      SPRHBM (0x20001258)                                                     */
/*      SPRC0 (0x20001258)                                                      */
/*      SPRMCC (0x20001258)                                                     */
/*      SPRUCC (0x20001258)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Second Address Match Register End - Lower Address Bits
*/


#define ADDRMATCH1_LO_END_IIO_CXL_CM_REG 0x23121258

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 5;

                            /* Bits[4:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 addr_31_5 : 27;

                            /* Bits[31:5], Access Type=RW, default=0x00000000*/

                            /* Addr_31_5 */

  } Bits;
  UINT32 Data;

} ADDRMATCH1_LO_END_IIO_CXL_CM_STRUCT;

/* ADDRMATCH1_HI_END_IIO_CXL_CM_REG supported on:                               */
/*      SPRA0 (0x2000125c)                                                      */
/*      SPRB0 (0x2000125c)                                                      */
/*      SPRHBM (0x2000125c)                                                     */
/*      SPRC0 (0x2000125c)                                                      */
/*      SPRMCC (0x2000125c)                                                     */
/*      SPRUCC (0x2000125c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Second Address Match Register End - Upper Address Bits
*/


#define ADDRMATCH1_HI_END_IIO_CXL_CM_REG 0x2312125C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr_51_32 : 20;

                            /* Bits[19:0], Access Type=RW, default=0x00000000*/

                            /* Addr_51_32 */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ADDRMATCH1_HI_END_IIO_CXL_CM_STRUCT;

/* TXFLITMATCH0_IIO_CXL_CM_REG supported on:                                    */
/*      SPRA0 (0x20001260)                                                      */
/*      SPRB0 (0x20001260)                                                      */
/*      SPRHBM (0x20001260)                                                     */
/*      SPRC0 (0x20001260)                                                      */
/*      SPRMCC (0x20001260)                                                     */
/*      SPRUCC (0x20001260)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Tx Flit Match
*/


#define TXFLITMATCH0_IIO_CXL_CM_REG 0x23121260

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 slot_0_format_type : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Slot_0_Format_Type */
    UINT32 slot_1_format_type : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /* Slot_1_Format_Type */
    UINT32 slot_2_format_type : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /* Slot_2_Format_Type */
    UINT32 slot_3_format_type : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /* Slot_3_Format_Type */
    UINT32 be : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* BE */
    UINT32 sz : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* Sz */
    UINT32 ak : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Ak */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXFLITMATCH0_IIO_CXL_CM_STRUCT;

/* TXFLITMASK0_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x20001264)                                                      */
/*      SPRB0 (0x20001264)                                                      */
/*      SPRHBM (0x20001264)                                                     */
/*      SPRC0 (0x20001264)                                                      */
/*      SPRMCC (0x20001264)                                                     */
/*      SPRUCC (0x20001264)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Tx Flit Mask
*/


#define TXFLITMASK0_IIO_CXL_CM_REG 0x23121264

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 slot_0_format_type : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Slot_0_Format_Type */
    UINT32 slot_1_format_type : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /* Slot_1_Format_Type */
    UINT32 slot_2_format_type : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /* Slot_2_Format_Type */
    UINT32 slot_3_format_type : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /* Slot_3_Format_Type */
    UINT32 be : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* BE */
    UINT32 sz : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* Sz */
    UINT32 ak : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Ak */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TXFLITMASK0_IIO_CXL_CM_STRUCT;

/* RXFLITMATCH0_IIO_CXL_CM_REG supported on:                                    */
/*      SPRA0 (0x20001268)                                                      */
/*      SPRB0 (0x20001268)                                                      */
/*      SPRHBM (0x20001268)                                                     */
/*      SPRC0 (0x20001268)                                                      */
/*      SPRMCC (0x20001268)                                                     */
/*      SPRUCC (0x20001268)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Rx Flit Match
*/


#define RXFLITMATCH0_IIO_CXL_CM_REG 0x23121268

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 slot_0_format_type : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Slot_0_Format_Type */
    UINT32 slot_1_format_type : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /* Slot_1_Format_Type */
    UINT32 slot_2_format_type : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /* Slot_2_Format_Type */
    UINT32 slot_3_format_type : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /* Slot_3_Format_Type */
    UINT32 be : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* BE */
    UINT32 sz : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* Sz */
    UINT32 ak : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Ak */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXFLITMATCH0_IIO_CXL_CM_STRUCT;

/* RXFLITMASK0_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x2000126c)                                                      */
/*      SPRB0 (0x2000126c)                                                      */
/*      SPRHBM (0x2000126c)                                                     */
/*      SPRC0 (0x2000126c)                                                      */
/*      SPRMCC (0x2000126c)                                                     */
/*      SPRUCC (0x2000126c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Rx Flit Mask
*/


#define RXFLITMASK0_IIO_CXL_CM_REG 0x2312126C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 slot_0_format_type : 3;

                            /* Bits[2:0], Access Type=RW, default=0x00000000*/

                            /* Slot_0_Format_Type */
    UINT32 slot_1_format_type : 3;

                            /* Bits[5:3], Access Type=RW, default=0x00000000*/

                            /* Slot_1_Format_Type */
    UINT32 slot_2_format_type : 3;

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /* Slot_2_Format_Type */
    UINT32 slot_3_format_type : 3;

                            /* Bits[11:9], Access Type=RW, default=0x00000000*/

                            /* Slot_3_Format_Type */
    UINT32 be : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* BE */
    UINT32 sz : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /* Sz */
    UINT32 ak : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* Ak */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RXFLITMASK0_IIO_CXL_CM_STRUCT;

/* TX_CACHE_HDRMASK_IIO_CXL_CM_REG supported on:                                */
/*      SPRA0 (0x20001270)                                                      */
/*      SPRB0 (0x20001270)                                                      */
/*      SPRHBM (0x20001270)                                                     */
/*      SPRC0 (0x20001270)                                                      */
/*      SPRMCC (0x20001270)                                                     */
/*      SPRUCC (0x20001270)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the flit to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the flit is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define TX_CACHE_HDRMASK_IIO_CXL_CM_REG 0x23121270

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 h2ddh_cqid : 12;

                            /* Bits[11:0], Access Type=RW, default=0x00000000*/

                            /* H2DDH CQID Mask H2D Data transactions */
    UINT32 h2dreq_uqid : 12;

                            /* Bits[23:12], Access Type=RW, default=0x00000000*/

                            /* H2DReq UQID Mask for H2D Req transactions */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TX_CACHE_HDRMASK_IIO_CXL_CM_STRUCT;

/* RX_CACHE_HDRMASK_IIO_CXL_CM_REG supported on:                                */
/*      SPRA0 (0x20001274)                                                      */
/*      SPRB0 (0x20001274)                                                      */
/*      SPRHBM (0x20001274)                                                     */
/*      SPRC0 (0x20001274)                                                      */
/*      SPRMCC (0x20001274)                                                     */
/*      SPRUCC (0x20001274)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the flit to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the flit is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define RX_CACHE_HDRMASK_IIO_CXL_CM_REG 0x23121274

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 d2hrsp_uqid : 12;

                            /* Bits[11:0], Access Type=RW, default=0x00000000*/

                            /* D2HRsp UQID Mask Device Response */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RX_CACHE_HDRMASK_IIO_CXL_CM_STRUCT;

/* TX_MEM_HDRMASK_IIO_CXL_CM_REG supported on:                                  */
/*      SPRA0 (0x20001278)                                                      */
/*      SPRB0 (0x20001278)                                                      */
/*      SPRHBM (0x20001278)                                                     */
/*      SPRC0 (0x20001278)                                                      */
/*      SPRMCC (0x20001278)                                                     */
/*      SPRUCC (0x20001278)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the flit to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the flit is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define TX_MEM_HDRMASK_IIO_CXL_CM_REG 0x23121278

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mem_r_tag : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /* Mem Req/Rsp Mask for Tag */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TX_MEM_HDRMASK_IIO_CXL_CM_STRUCT;

/* LPIF_CTRL_IIO_CXL_CM_REG supported on:                                       */
/*      SPRA0 (0x200012a0)                                                      */
/*      SPRB0 (0x200012a0)                                                      */
/*      SPRHBM (0x200012a0)                                                     */
/*      SPRC0 (0x200012a0)                                                      */
/*      SPRMCC (0x200012a0)                                                     */
/*      SPRUCC (0x200012a0)                                                     */
/* Register default value on SPRA0: 0x00000090                                  */
/* Register default value on SPRB0: 0x00000090                                  */
/* Register default value on SPRHBM: 0x00000090                                 */
/* Register default value on SPRC0: 0x00000090                                  */
/* Register default value on SPRMCC: 0x00000090                                 */
/* Register default value on SPRUCC: 0x00000090                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* LPIF Control Register
*/


#define LPIF_CTRL_IIO_CXL_CM_REG 0x231212A0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lpif_arb_mode : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Controls the mode for LPIF Arbitration. Default
                               is Mode 0.
                            */
    UINT32 lpif_gnt_to : 6;

                            /* Bits[6:1], Access Type=RW/P, default=0x00000008*/

                            /*
                               Number of cycles the $M block will wait for a
                               gnt on a low priority arb before asserting high
                               priority arb
                            */
    UINT32 lpif_replay_roundrobin_disable : 1;

                            /* Bits[7:7], Access Type=RW/P, default=0x00000001*/

                            /*
                               LPIF lp_pri[1] is de-asserted during Link Layer
                               Retry mode
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LPIF_CTRL_IIO_CXL_CM_STRUCT;

/* RETRY_CTRL_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x200012a4)                                                      */
/*      SPRB0 (0x200012a4)                                                      */
/*      SPRHBM (0x200012a4)                                                     */
/*      SPRC0 (0x200012a4)                                                      */
/*      SPRMCC (0x200012a4)                                                     */
/*      SPRUCC (0x200012a4)                                                     */
/* Register default value on SPRA0: 0x0008294C                                  */
/* Register default value on SPRB0: 0x0008294C                                  */
/* Register default value on SPRHBM: 0x0008294C                                 */
/* Register default value on SPRC0: 0x0008294C                                  */
/* Register default value on SPRMCC: 0x0008294C                                 */
/* Register default value on SPRUCC: 0x0008294C                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Link Layer Retry Logic Control
*/


#define RETRY_CTRL_IIO_CXL_CM_REG 0x231212A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 max_retrytimeout : 5;

                            /* Bits[4:0], Access Type=RW/P, default=0x0000000C*/

                            /*
                               This is an exponentially encoded timeout value
                               (2^N flits) after which the Tx will retry
                               sending a RETRY.req. The HW default value for
                               this register is ?d12, which means, Link Layer
                               will send 4K flits before attempting to send
                               another Retry.Req sequence.
                            */
    UINT32 max_num_retry : 5;

                            /* Bits[9:5], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Number of times the Tx sends a retry without
                               receiving an ack before triggering REINIT
                            */
    UINT32 max_num_phy_reinit : 5;

                            /* Bits[14:10], Access Type=RW/P, default=0x0000000A*/

                            /*
                               Number of times the Tx tries a Phy REINIT before
                               giving up
                            */
    UINT32 crc_threshold : 8;

                            /* Bits[22:15], Access Type=RW/P, default=0x00000010*/

                            /* CRC Error Threshold Before Logging */
    UINT32 rsvd : 9;

                            /* Bits[31:23], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} RETRY_CTRL_IIO_CXL_CM_STRUCT;

/* LOCAL_RETRY_FSM_STS_IIO_CXL_CM_REG supported on:                             */
/*      SPRA0 (0x200012a8)                                                      */
/*      SPRB0 (0x200012a8)                                                      */
/*      SPRHBM (0x200012a8)                                                     */
/*      SPRC0 (0x200012a8)                                                      */
/*      SPRMCC (0x200012a8)                                                     */
/*      SPRUCC (0x200012a8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Local Retry FSM Status Observe
*/


#define LOCAL_RETRY_FSM_STS_IIO_CXL_CM_REG 0x231212A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 num_retry_sts : 5;

                            /* Bits[4:0], Access Type=RO/V, default=0x00000000*/

                            /* Current value of Num_Retry */
    UINT32 num_phy_reinit : 5;

                            /* Bits[9:5], Access Type=RO/V, default=0x00000000*/

                            /* Current value of Num_Phy_Reinit */
    UINT32 local_fsm_state_sts : 4;

                            /* Bits[13:10], Access Type=RO/V, default=0x00000000*/

                            /* Current Local Retry FSM State */
    UINT32 num_local_crc_detected : 8;

                            /* Bits[21:14], Access Type=RO/V, default=0x00000000*/

                            /* Count of the number of local CRC errors detected */
    UINT32 rsvd : 10;

                            /* Bits[31:22], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LOCAL_RETRY_FSM_STS_IIO_CXL_CM_STRUCT;

/* REMOTE_RETRY_FSM_STS_IIO_CXL_CM_REG supported on:                            */
/*      SPRA0 (0x200012ac)                                                      */
/*      SPRB0 (0x200012ac)                                                      */
/*      SPRHBM (0x200012ac)                                                     */
/*      SPRC0 (0x200012ac)                                                      */
/*      SPRMCC (0x200012ac)                                                     */
/*      SPRUCC (0x200012ac)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Remote Retry FSM Status Observe
*/


#define REMOTE_RETRY_FSM_STS_IIO_CXL_CM_REG 0x231212AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 remote_fsm_state_sts : 4;

                            /* Bits[3:0], Access Type=RO/V, default=0x00000000*/

                            /* Current Remote Retry FSM State */
    UINT32 num_remote_crc_detected : 8;

                            /* Bits[11:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               Count of the number of remote CRC errors
                               detected
                            */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} REMOTE_RETRY_FSM_STS_IIO_CXL_CM_STRUCT;

/* ERROR_INJECTION_REGISTER_IIO_CXL_CM_REG supported on:                        */
/*      SPRA0 (0x200012b0)                                                      */
/*      SPRB0 (0x200012b0)                                                      */
/*      SPRHBM (0x200012b0)                                                     */
/*      SPRC0 (0x200012b0)                                                      */
/*      SPRMCC (0x200012b0)                                                     */
/*      SPRUCC (0x200012b0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Error injection control register
*/


#define ERROR_INJECTION_REGISTER_IIO_CXL_CM_REG 0x231212B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cache_data_par_errinj : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               When set, one single parity error injection will
                               happen by setting the done bit. Once injection
                               is complete clear the done bit.
                            */
    UINT32 cache_data_par_errinj_done : 1;

                            /* Bits[1:1], Access Type=RO/V, default=0x00000000*/

                            /*
                               Run busy bit .Will be cleared when error
                               injection is done.
                            */
    UINT32 cache_addr_par_errinj : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               When set, one single parity error injection will
                               happen. Once injection is complete set the DONE
                               filed. S/W need clear both bits if it wants to
                               do another injection.)
                            */
    UINT32 cache_addr_par_errinj_done : 1;

                            /* Bits[3:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               Run busy bit .Will be cleared when error
                               injection is done.
                            */
    UINT32 cache_uqid_par_errinj : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               When set, one single parity error injection will
                               happen. Once injection is complete set the DONE
                               filed. S/W need clear both bits if it wants to
                               do another injection.)
                            */
    UINT32 cache_uqid_par_errinj_done : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               Run busy bit .Will be cleared when error
                               injection is done.
                            */
    UINT32 mem_data_par_errinj : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               When set, one single parity error injection will
                               happen. Once injection is complete set the DONE
                               filed. S/W need clear both bits if it wants to
                               do another injection.)
                            */
    UINT32 mem_data_par_errinj_done : 1;

                            /* Bits[7:7], Access Type=RO/V, default=0x00000000*/

                            /*
                               Run busy bit .Will be cleared when error
                               injection is done.
                            */
    UINT32 mem_data_be_par_errinj : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               When set, one single parity error injection will
                               happen. Once injection is complete set the DONE
                               filed. S/W need clear both bits if it wants to
                               do another injection.)
                            */
    UINT32 mem_data_be_par_errinj_done : 1;

                            /* Bits[9:9], Access Type=RO/V, default=0x00000000*/

                            /*
                               Run busy bit .Will be cleared when error
                               injection is done.
                            */
    UINT32 mem_addr_par_errinj : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               When set, one single parity error injection will
                               happen. Once injection is complete set the DONE
                               filed. S/W need clear both bits if it wants to
                               do another injection.)
                            */
    UINT32 mem_addr_par_errinj_done : 1;

                            /* Bits[11:11], Access Type=RO/V, default=0x00000000*/

                            /*
                               Run busy bit .Will be cleared when error
                               injection is done.
                            */
    UINT32 mem_ecc_errinj : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               When set , inject a single bit error. Once
                               injection is done and corrected set the DONE
                               field.S/W need clear both bits if it wants to do
                               another injection.)
                            */
    UINT32 mem_ecc_errinj_done : 1;

                            /* Bits[13:13], Access Type=RO/V, default=0x00000000*/

                            /*
                               Run busy bit .Will be cleared when error
                               injection is done.
                            */
    UINT32 cache_ecc_errinj : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               When set , inject a single bit error. Once
                               injection is done and corrected set the DONE
                               field.S/W need clear both bits if it wants to do
                               another injection.)
                            */
    UINT32 cache_ecc_errinj_done : 1;

                            /* Bits[15:15], Access Type=RO/V, default=0x00000000*/

                            /*
                               Run busy bit .Will be cleared when error
                               injection is done.
                            */
    UINT32 cache_poison_errinj : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Overrides the poison field in the data header
                               slot of Cache
                            */
    UINT32 cache_poison_errinj_done : 1;

                            /* Bits[17:17], Access Type=RO/V, default=0x00000000*/

                            /*
                               Run busy bit .Will be cleared when error
                               injection is done.
                            */
    UINT32 mem_poison_errinj : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Overrides the poison filed in the data header
                               slot of Mem
                            */
    UINT32 mem_poison_errinj_done : 1;

                            /* Bits[19:19], Access Type=RO/V, default=0x00000000*/

                            /*
                               Run busy bit .Will be cleared when error
                               injection is done.
                            */
    UINT32 cache_mem_viral_errinj : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               When set, drives the GlobalFatal Viral signal
                               coming into the $M boundary (DP TX).
                            */
    UINT32 cache_mem_viral_errinj_done : 1;

                            /* Bits[21:21], Access Type=RO/V, default=0x00000000*/

                            /*
                               Run busy bit .Will be cleared when error
                               injection is done.
                            */
    UINT32 crc_err_inj_entry : 2;

                            /* Bits[23:22], Access Type=RW, default=0x00000000*/

                            /*
                               00 - Disable 01-Single bit flipped 10-Two bits
                               flipped 11-three bits flipped Inject
                               single/double or triple bit error
                            */
    UINT32 crc_err_inj_count : 2;

                            /* Bits[25:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               00 - Disable 01 - Inject only one error
                               injection 10-two injections in a row. Done filed
                               is set after 2 injections are done 1. N number
                               of injections set by a register value until
                               cleared by SW Must be supporting injections in a
                               row so that persistent case is satisfied When
                               set one /two/three bits will be flipped as
                               defined in the Inj_Entry. Once Injection done
                               set the DONE field. S/W need clear both bits if
                               it wants to do another injection.
                            */
    UINT32 crc_err_inj_done : 1;

                            /* Bits[26:26], Access Type=RO/V, default=0x00000000*/

                            /*
                               Run busy bit .Will be cleared when error
                               injection is done.
                            */
    UINT32 force_phy_retrain : 1;

                            /* Bits[27:27], Access Type=RW/V, default=0x00000000*/

                            /*
                               Force the PHY to enter retrain only when in
                               ACTIVE state
                            */
    UINT32 force_phy_linkerror : 1;

                            /* Bits[28:28], Access Type=RW/V, default=0x00000000*/

                            /*
                               Force the PHY to enter into LINKERROR state only
                               when in ACTIVE state
                            */
    UINT32 force_rx_retry : 1;

                            /* Bits[29:29], Access Type=RW/V, default=0x00000000*/

                            /*
                               Force the Rx to Detect a CRC Error and Retry for
                               the next received flit
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} ERROR_INJECTION_REGISTER_IIO_CXL_CM_STRUCT;

/* CLK_GATE_CTRL_IIO_CXL_CM_REG supported on:                                   */
/*      SPRA0 (0x200012b4)                                                      */
/*      SPRB0 (0x200012b4)                                                      */
/*      SPRHBM (0x200012b4)                                                     */
/*      SPRC0 (0x200012b4)                                                      */
/*      SPRMCC (0x200012b4)                                                     */
/*      SPRUCC (0x200012b4)                                                     */
/* Register default value on SPRA0: 0x00000003                                  */
/* Register default value on SPRB0: 0x00000003                                  */
/* Register default value on SPRHBM: 0x00000003                                 */
/* Register default value on SPRC0: 0x00000003                                  */
/* Register default value on SPRMCC: 0x00000003                                 */
/* Register default value on SPRUCC: 0x00000003                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Control Dynamic Clock Gating 
*/


#define CLK_GATE_CTRL_IIO_CXL_CM_REG 0x231212B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 clock_gate_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /* Enable Dynamic Clock Gating */
    UINT32 visa_clock_gate_enable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /* Enable VISA Clock Gating */
    UINT32 force_clock_req : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* Force assert Clock Req for Txn and Link Clocks */
    UINT32 force_deassert_clock_req : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* Force deassert Clock Req for Txn and Link Clocks */
    UINT32 resetprepsx_auto_ack : 1;

                            /* Bits[4:4], Access Type=RW/P, default=0x00000000*/

                            /*
                               When this bit is set, CXLCM IP will respond back
                               to ResetPrep.Sx without waiting for vLSM to
                               enter into L2.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CLK_GATE_CTRL_IIO_CXL_CM_STRUCT;

/* LOCAL_RETRY_FSM_TO_STS_IIO_CXL_CM_REG supported on:                          */
/*      SPRA0 (0x200012b8)                                                      */
/*      SPRB0 (0x200012b8)                                                      */
/*      SPRHBM (0x200012b8)                                                     */
/*      SPRC0 (0x200012b8)                                                      */
/*      SPRMCC (0x200012b8)                                                     */
/*      SPRUCC (0x200012b8)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Local Retry FSM Timeout Status Observe
*/


#define LOCAL_RETRY_FSM_TO_STS_IIO_CXL_CM_REG 0x231212B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 retrytimeout_sts : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /* Current value of Retry TO counter */

  } Bits;
  UINT32 Data;

} LOCAL_RETRY_FSM_TO_STS_IIO_CXL_CM_STRUCT;

/* LINK_DEFEATURE_IIO_CXL_CM_REG supported on:                                  */
/*      SPRA0 (0x200012bc)                                                      */
/*      SPRB0 (0x200012bc)                                                      */
/*      SPRHBM (0x200012bc)                                                     */
/*      SPRC0 (0x200012bc)                                                      */
/*      SPRMCC (0x200012bc)                                                     */
/*      SPRUCC (0x200012bc)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* This register holds several Link Layer defeatures for IP specific functionality
*/


#define LINK_DEFEATURE_IIO_CXL_CM_REG 0x231212BC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 disallow_ial_version : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               IP accepts all IAL Versions as communicated in
                               Remote LL?s LLInit flit, with an exception of
                               IAL 0.5, in which case IP drops the Init flit
                               and logs an unrecoverable error. When this bit
                               is set, IP will drop Init flit and log an error
                               an error whenever IAL Version is not 1.0.
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LINK_DEFEATURE_IIO_CXL_CM_STRUCT;

/* VIRAL_LOG_IIO_CXL_CM_REG supported on:                                       */
/*      SPRA0 (0x200012c0)                                                      */
/*      SPRB0 (0x200012c0)                                                      */
/*      SPRHBM (0x200012c0)                                                     */
/*      SPRC0 (0x200012c0)                                                      */
/*      SPRMCC (0x200012c0)                                                     */
/*      SPRUCC (0x200012c0)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Viral Log Register
*/


#define VIRAL_LOG_IIO_CXL_CM_REG 0x231212C0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 port_generated_viral : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Port Generated Viral Indication */
    UINT32 soc_generated_viral : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /* SoC Generated Viral Indication */
    UINT32 link_received_viral : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /* Link Generated Viral Indication */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} VIRAL_LOG_IIO_CXL_CM_STRUCT;







/* OS_W_CP_N0_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x200012e8)                                                      */
/*      SPRB0 (0x200012e8)                                                      */
/*      SPRHBM (0x200012e8)                                                     */
/*      SPRC0 (0x200012e8)                                                      */
/*      SPRMCC (0x200012e8)                                                     */
/*      SPRUCC (0x200012e8)                                                     */
/* Register default value on SPRA0: 0x01000218                                  */
/* Register default value on SPRB0: 0x01000218                                  */
/* Register default value on SPRHBM: 0x01000218                                 */
/* Register default value on SPRC0: 0x01000218                                  */
/* Register default value on SPRMCC: 0x01000218                                 */
/* Register default value on SPRUCC: 0x01000218                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC.
*/


#define OS_W_CP_N0_IIO_CXL_CM_REG 0x231212E8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_ctrl_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_CP_N0_IIO_CXL_CM_STRUCT;

/* OS_W_CP_N1_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x200012ec)                                                      */
/*      SPRB0 (0x200012ec)                                                      */
/*      SPRHBM (0x200012ec)                                                     */
/*      SPRC0 (0x200012ec)                                                      */
/*      SPRMCC (0x200012ec)                                                     */
/*      SPRUCC (0x200012ec)                                                     */
/* Register default value on SPRA0: 0x00000400                                  */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access to RAC, WAC and CP registers for OS_W security policy group. This policy group covers registers set by OS, so all IA agents are allowed access in the WAC.
*/


#define OS_W_CP_N1_IIO_CXL_CM_REG 0x231212EC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_ctrl_pol_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */
    UINT32 os_w_ctrl_pol_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to OS_W WAC, RAC and CP
                               registers, based on the value from each agent's
                               6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_CP_N1_IIO_CXL_CM_STRUCT;

/* OS_W_RAC_N0_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x200012f0)                                                      */
/*      SPRB0 (0x200012f0)                                                      */
/*      SPRHBM (0x200012f0)                                                     */
/*      SPRC0 (0x200012f0)                                                      */
/*      SPRMCC (0x200012f0)                                                     */
/*      SPRUCC (0x200012f0)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access for registers in OS_W security policy group.
*/


#define OS_W_RAC_N0_IIO_CXL_CM_REG 0x231212F0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_RAC_N0_IIO_CXL_CM_STRUCT;

/* OS_W_RAC_N1_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x200012f4)                                                      */
/*      SPRB0 (0x200012f4)                                                      */
/*      SPRHBM (0x200012f4)                                                     */
/*      SPRC0 (0x200012f4)                                                      */
/*      SPRMCC (0x200012f4)                                                     */
/*      SPRUCC (0x200012f4)                                                     */
/* Register default value on SPRA0: 0xFFFFFFFF                                  */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the read access for registers in OS_W security policy group.
*/


#define OS_W_RAC_N1_IIO_CXL_CM_REG 0x231212F4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed read access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_RAC_N1_IIO_CXL_CM_STRUCT;

/* OS_W_WAC_N0_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x200012f8)                                                      */
/*      SPRB0 (0x200012f8)                                                      */
/*      SPRHBM (0x200012f8)                                                     */
/*      SPRC0 (0x200012f8)                                                      */
/*      SPRMCC (0x200012f8)                                                     */
/*      SPRUCC (0x200012f8)                                                     */
/* Register default value on SPRA0: 0x0300021F                                  */
/* Register default value on SPRB0: 0x0300021F                                  */
/* Register default value on SPRHBM: 0x0300021F                                 */
/* Register default value on SPRC0: 0x0300021F                                  */
/* Register default value on SPRMCC: 0x0300021F                                 */
/* Register default value on SPRUCC: 0x0300021F                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access for registers in OS_W security policy group.
*/


#define OS_W_WAC_N0_IIO_CXL_CM_REG 0x231212F8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_2 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_4 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_5 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_6 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_7 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_8 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_9 : 1;

                            /* Bits[9:9], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_10 : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_11 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_12 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_13 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_14 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_15 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_16 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_17 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_18 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_19 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_20 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_21 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_22 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_23 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_24 : 1;

                            /* Bits[24:24], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_25 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_26 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_27 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_28 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_29 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_30 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_31 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_WAC_N0_IIO_CXL_CM_STRUCT;

/* OS_W_WAC_N1_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x200012fc)                                                      */
/*      SPRB0 (0x200012fc)                                                      */
/*      SPRHBM (0x200012fc)                                                     */
/*      SPRC0 (0x200012fc)                                                      */
/*      SPRMCC (0x200012fc)                                                     */
/*      SPRUCC (0x200012fc)                                                     */
/* Register default value on SPRA0: 0x20000C00                                  */
/* Register default value on SPRB0: 0x20000C00                                  */
/* Register default value on SPRHBM: 0x20000C00                                 */
/* Register default value on SPRC0: 0x20000C00                                  */
/* Register default value on SPRMCC: 0x20000C00                                 */
/* Register default value on SPRUCC: 0x20000C00                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register configures the write access for registers in OS_W security policy group.
*/


#define OS_W_WAC_N1_IIO_CXL_CM_REG 0x231212FC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 os_w_sai_pol_32 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_33 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_34 : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_35 : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_36 : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_37 : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_38 : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_39 : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_40 : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_41 : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_42 : 1;

                            /* Bits[10:10], Access Type=RO, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_43 : 1;

                            /* Bits[11:11], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_44 : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_45 : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_46 : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_47 : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_48 : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_49 : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_50 : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_51 : 1;

                            /* Bits[19:19], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_52 : 1;

                            /* Bits[20:20], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_53 : 1;

                            /* Bits[21:21], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_54 : 1;

                            /* Bits[22:22], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_55 : 1;

                            /* Bits[23:23], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_56 : 1;

                            /* Bits[24:24], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_57 : 1;

                            /* Bits[25:25], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_58 : 1;

                            /* Bits[26:26], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_59 : 1;

                            /* Bits[27:27], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_60 : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_61 : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000001*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_62 : 1;

                            /* Bits[30:30], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */
    UINT32 os_w_sai_pol_63 : 1;

                            /* Bits[31:31], Access Type=RW, default=0x00000000*/

                            /*
                               Bit-vector used to determine which agents are
                               allowed write access to registers in OS_W
                               security policy group, based on the value from
                               each agent's 6bit SAI field.
                            */

  } Bits;
  UINT32 Data;

} OS_W_WAC_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_UNIT_CONTROL_N0_IIO_CXL_CM_REG supported on:                          */
/*      SPRA0 (0x20001300)                                                      */
/*      SPRB0 (0x20001300)                                                      */
/*      SPRHBM (0x20001300)                                                     */
/*      SPRC0 (0x20001300)                                                      */
/*      SPRMCC (0x20001300)                                                     */
/*      SPRUCC (0x20001300)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Perfmon Unit 0 Control Register
*/


#define PMON_0_UNIT_CONTROL_N0_IIO_CXL_CM_REG 0x23121300

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 freeze_counters : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               If set to 1 the counters in this unit will be
                               frozen.
                            */
    UINT32 rsvd : 7;

                            /* Bits[7:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 reset_control : 1;

                            /* Bits[8:8], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the Counter Control Registers
                               will be reset to 0.
                            */
    UINT32 reset_counters : 1;

                            /* Bits[9:9], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the Counter Registers will be
                               reset to 0.
                            */
    UINT32 rsvd_10 : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMON_0_UNIT_CONTROL_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_UNIT_CONTROL_N1_IIO_CXL_CM_REG supported on:                          */
/*      SPRA0 (0x20001304)                                                      */
/*      SPRB0 (0x20001304)                                                      */
/*      SPRHBM (0x20001304)                                                     */
/*      SPRC0 (0x20001304)                                                      */
/*      SPRMCC (0x20001304)                                                     */
/*      SPRUCC (0x20001304)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Perfmon Unit 0 Control Register
*/


#define PMON_0_UNIT_CONTROL_N1_IIO_CXL_CM_REG 0x23121304

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

} PMON_0_UNIT_CONTROL_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_OVERFLOW_STATUS_REGISTER_N0_IIO_CXL_CM_REG supported on:              */
/*      SPRA0 (0x20001308)                                                      */
/*      SPRB0 (0x20001308)                                                      */
/*      SPRHBM (0x20001308)                                                     */
/*      SPRC0 (0x20001308)                                                      */
/*      SPRMCC (0x20001308)                                                     */
/*      SPRUCC (0x20001308)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Overflow Status Register
*/


#define PMON_0_OVERFLOW_STATUS_REGISTER_N0_IIO_CXL_CM_REG 0x23121308

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_n_ovf : 8;

                            /* Bits[7:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               If an overflow is detected from the
                               corresponding data register, its overflow bit
                               will be set. NOTE: Write of 1 will clear the
                               bit.
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMON_0_OVERFLOW_STATUS_REGISTER_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_OVERFLOW_STATUS_REGISTER_N1_IIO_CXL_CM_REG supported on:              */
/*      SPRA0 (0x2000130c)                                                      */
/*      SPRB0 (0x2000130c)                                                      */
/*      SPRHBM (0x2000130c)                                                     */
/*      SPRC0 (0x2000130c)                                                      */
/*      SPRMCC (0x2000130c)                                                     */
/*      SPRUCC (0x2000130c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Overflow Status Register
*/


#define PMON_0_OVERFLOW_STATUS_REGISTER_N1_IIO_CXL_CM_REG 0x2312130C

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

} PMON_0_OVERFLOW_STATUS_REGISTER_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_0_N0_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001310)                                                      */
/*      SPRB0 (0x20001310)                                                      */
/*      SPRHBM (0x20001310)                                                     */
/*      SPRC0 (0x20001310)                                                      */
/*      SPRMCC (0x20001310)                                                     */
/*      SPRUCC (0x20001310)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 0
*/


#define PMON_0_CONTROL_REGISTER_0_N0_IIO_CXL_CM_REG 0x23121310

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 evslct : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Event Select Minimum field to select which of
                               the available events should be recorded in the
                               paired data register. Additional bits in the
                               control register may also be required to select
                               from the available events. 0x0 == Count Nothing
                               0x1 == Count Local Clockticks
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event. The selected sub-
                               events are bitwise OR-ed together to create
                               event. At least one sub-event must be selected
                               otherwise the PerfMon event signals will not
                               ever get asserted. Events with no sub-events
                               listed effectively have only one sub-event --
                               bit 8 must be set to 1 in this case.
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the corresponding counter will be
                               cleared to 0
                            */
    UINT32 edgedet : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1, rather than measuring the event
                               in each cycle it is active, the corresponding
                               counter will increment when a 0 to 1 transition
                               (i.e. rising edge) is detected. When 0, the
                               counter will increment in each cycle that the
                               event is asserted.
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovfenable : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               When an overflow is detected from this register,
                               a PMON overflow message is sent to the global
                               control. This bit will tell the global control
                               whether it should assert the global freeze for
                               all counters in the same domain.
                            */
    UINT32 rsvd_21 : 2;

                            /* Bits[22:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               Invert comparison against Threshold. 0 -
                               comparison will be is event increment >=
                               threshold? 1 - comparison is inverted - is event
                               increment < threshold? e.g. for a 64-entry
                               queue, if SW wanted to know how many cycles the
                               queue had fewer than 4 entries, SW should set
                               the threshold to 4 and set the invert bit to 1.
                            */
    UINT32 threshold : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               Threshold on Counter Increment. Threshold is
                               used, along with the invert bit, to compare
                               against the incoming increment value that will
                               be added to the counter. For events that
                               increment by more than 1 per cycle, if the
                               threshold is set to a value greater than 1, the
                               data register will accumulate instances in which
                               the event increment is >=threshold. e.g. say you
                               have an event to accumulate the occupancy of a
                               64-entry queue every cycle. By setting the
                               threshold value to 60, the data register would
                               count the number of cycles the queues occupancy
                               was >= 60.
                            */

  } Bits;
  UINT32 Data;

} PMON_0_CONTROL_REGISTER_0_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_0_N1_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001314)                                                      */
/*      SPRB0 (0x20001314)                                                      */
/*      SPRHBM (0x20001314)                                                     */
/*      SPRC0 (0x20001314)                                                      */
/*      SPRMCC (0x20001314)                                                     */
/*      SPRUCC (0x20001314)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 0
*/


#define PMON_0_CONTROL_REGISTER_0_N1_IIO_CXL_CM_REG 0x23121314

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

} PMON_0_CONTROL_REGISTER_0_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_1_N0_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001318)                                                      */
/*      SPRB0 (0x20001318)                                                      */
/*      SPRHBM (0x20001318)                                                     */
/*      SPRC0 (0x20001318)                                                      */
/*      SPRMCC (0x20001318)                                                     */
/*      SPRUCC (0x20001318)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 1
*/


#define PMON_0_CONTROL_REGISTER_1_N0_IIO_CXL_CM_REG 0x23121318

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 evslct : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Event Select Minimum field to select which of
                               the available events should be recorded in the
                               paired data register. Additional bits in the
                               control register may also be required to select
                               from the available events. 0x0 == Count Nothing
                               0x1 == Count Local Clockticks
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event. The selected sub-
                               events are bitwise OR-ed together to create
                               event. At least one sub-event must be selected
                               otherwise the PerfMon event signals will not
                               ever get asserted. Events with no sub-events
                               listed effectively have only one sub-event --
                               bit 8 must be set to 1 in this case.
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the corresponding counter will be
                               cleared to 0
                            */
    UINT32 edgedet : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1, rather than measuring the event
                               in each cycle it is active, the corresponding
                               counter will increment when a 0 to 1 transition
                               (i.e. rising edge) is detected. When 0, the
                               counter will increment in each cycle that the
                               event is asserted.
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovfenable : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               When an overflow is detected from this register,
                               a PMON overflow message is sent to the global
                               control. This bit will tell the global control
                               whether it should assert the global freeze for
                               all counters in the same domain.
                            */
    UINT32 rsvd_21 : 2;

                            /* Bits[22:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               Invert comparison against Threshold. 0 -
                               comparison will be is event increment >=
                               threshold? 1 - comparison is inverted - is event
                               increment < threshold? e.g. for a 64-entry
                               queue, if SW wanted to know how many cycles the
                               queue had fewer than 4 entries, SW should set
                               the threshold to 4 and set the invert bit to 1.
                            */
    UINT32 threshold : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               Threshold on Counter Increment. Threshold is
                               used, along with the invert bit, to compare
                               against the incoming increment value that will
                               be added to the counter. For events that
                               increment by more than 1 per cycle, if the
                               threshold is set to a value greater than 1, the
                               data register will accumulate instances in which
                               the event increment is >=threshold. e.g. say you
                               have an event to accumulate the occupancy of a
                               64-entry queue every cycle. By setting the
                               threshold value to 60, the data register would
                               count the number of cycles the queues occupancy
                               was >= 60.
                            */

  } Bits;
  UINT32 Data;

} PMON_0_CONTROL_REGISTER_1_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_1_N1_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x2000131c)                                                      */
/*      SPRB0 (0x2000131c)                                                      */
/*      SPRHBM (0x2000131c)                                                     */
/*      SPRC0 (0x2000131c)                                                      */
/*      SPRMCC (0x2000131c)                                                     */
/*      SPRUCC (0x2000131c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 1
*/


#define PMON_0_CONTROL_REGISTER_1_N1_IIO_CXL_CM_REG 0x2312131C

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

} PMON_0_CONTROL_REGISTER_1_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_2_N0_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001320)                                                      */
/*      SPRB0 (0x20001320)                                                      */
/*      SPRHBM (0x20001320)                                                     */
/*      SPRC0 (0x20001320)                                                      */
/*      SPRMCC (0x20001320)                                                     */
/*      SPRUCC (0x20001320)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 2
*/


#define PMON_0_CONTROL_REGISTER_2_N0_IIO_CXL_CM_REG 0x23121320

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 evslct : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Event Select Minimum field to select which of
                               the available events should be recorded in the
                               paired data register. Additional bits in the
                               control register may also be required to select
                               from the available events. 0x0 == Count Nothing
                               0x1 == Count Local Clockticks
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event. The selected sub-
                               events are bitwise OR-ed together to create
                               event. At least one sub-event must be selected
                               otherwise the PerfMon event signals will not
                               ever get asserted. Events with no sub-events
                               listed effectively have only one sub-event --
                               bit 8 must be set to 1 in this case.
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the corresponding counter will be
                               cleared to 0
                            */
    UINT32 edgedet : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1, rather than measuring the event
                               in each cycle it is active, the corresponding
                               counter will increment when a 0 to 1 transition
                               (i.e. rising edge) is detected. When 0, the
                               counter will increment in each cycle that the
                               event is asserted.
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovfenable : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               When an overflow is detected from this register,
                               a PMON overflow message is sent to the global
                               control. This bit will tell the global control
                               whether it should assert the global freeze for
                               all counters in the same domain.
                            */
    UINT32 rsvd_21 : 2;

                            /* Bits[22:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               Invert comparison against Threshold. 0 -
                               comparison will be is event increment >=
                               threshold?. 1 - comparison is inverted - is
                               event increment < threshold? e.g. for a 64-entry
                               queue, if SW wanted to know how many cycles the
                               queue had fewer than 4 entries, SW should set
                               the threshold to 4 and set the invert bit to 1.
                            */
    UINT32 threshold : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               Threshold on Counter Increment. Threshold is
                               used, along with the invert bit, to compare
                               against the incoming increment value that will
                               be added to the counter. For events that
                               increment by more than 1 per cycle, if the
                               threshold is set to a value greater than 1, the
                               data register will accumulate instances in which
                               the event increment is >=threshold. e.g. say you
                               have an event to accumulate the occupancy of a
                               64-entry queue every cycle. By setting the
                               threshold value to 60, the data register would
                               count the number of cycles the queues occupancy
                               was >= 60.
                            */

  } Bits;
  UINT32 Data;

} PMON_0_CONTROL_REGISTER_2_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_2_N1_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001324)                                                      */
/*      SPRB0 (0x20001324)                                                      */
/*      SPRHBM (0x20001324)                                                     */
/*      SPRC0 (0x20001324)                                                      */
/*      SPRMCC (0x20001324)                                                     */
/*      SPRUCC (0x20001324)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 2
*/


#define PMON_0_CONTROL_REGISTER_2_N1_IIO_CXL_CM_REG 0x23121324

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

} PMON_0_CONTROL_REGISTER_2_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_3_N0_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001328)                                                      */
/*      SPRB0 (0x20001328)                                                      */
/*      SPRHBM (0x20001328)                                                     */
/*      SPRC0 (0x20001328)                                                      */
/*      SPRMCC (0x20001328)                                                     */
/*      SPRUCC (0x20001328)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 3
*/


#define PMON_0_CONTROL_REGISTER_3_N0_IIO_CXL_CM_REG 0x23121328

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 evslct : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Event Select Minimum field to select which of
                               the available events should be recorded in the
                               paired data register. Additional bits in the
                               control register may also be required to select
                               from the available events. 0x0 == Count Nothing
                               0x1 == Count Local Clockticks
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event. The selected sub-
                               events are bitwise OR-ed together to create
                               event. At least one sub-event must be selected
                               otherwise the PerfMon event signals will not
                               ever get asserted. Events with no sub-events
                               listed effectively have only one sub-event --
                               bit 8 must be set to 1 in this case.
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the corresponding counter will be
                               cleared to 0
                            */
    UINT32 edgedet : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1, rather than measuring the event
                               in each cycle it is active, the corresponding
                               counter will increment when a 0 to 1 transition
                               (i.e. rising edge) is detected. When 0, the
                               counter will increment in each cycle that the
                               event is asserted.
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovfenable : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               When an overflow is detected from this register,
                               a PMON overflow message is sent to the global
                               control. This bit will tell the global control
                               whether it should assert the global freeze for
                               all counters in the same domain.
                            */
    UINT32 rsvd_21 : 2;

                            /* Bits[22:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               Invert comparison against Threshold. 0 -
                               comparison will be is event increment >=
                               threshold?. 1 - comparison is inverted - is
                               event increment < threshold? e.g. for a 64-entry
                               queue, if SW wanted to know how many cycles the
                               queue had fewer than 4 entries, SW should set
                               the threshold to 4 and set the invert bit to 1.
                            */
    UINT32 threshold : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               Threshold on Counter Increment. Threshold is
                               used, along with the invert bit, to compare
                               against the incoming increment value that will
                               be added to the counter. For events that
                               increment by more than 1 per cycle, if the
                               threshold is set to a value greater than 1, the
                               data register will accumulate instances in which
                               the event increment is >=threshold. e.g. say you
                               have an event to accumulate the occupancy of a
                               64-entry queue every cycle. By setting the
                               threshold value to 60, the data register would
                               count the number of cycles the queues occupancy
                               was >= 60.
                            */

  } Bits;
  UINT32 Data;

} PMON_0_CONTROL_REGISTER_3_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_3_N1_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x2000132c)                                                      */
/*      SPRB0 (0x2000132c)                                                      */
/*      SPRHBM (0x2000132c)                                                     */
/*      SPRC0 (0x2000132c)                                                      */
/*      SPRMCC (0x2000132c)                                                     */
/*      SPRUCC (0x2000132c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 3
*/


#define PMON_0_CONTROL_REGISTER_3_N1_IIO_CXL_CM_REG 0x2312132C

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

} PMON_0_CONTROL_REGISTER_3_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_4_N0_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001330)                                                      */
/*      SPRB0 (0x20001330)                                                      */
/*      SPRHBM (0x20001330)                                                     */
/*      SPRC0 (0x20001330)                                                      */
/*      SPRMCC (0x20001330)                                                     */
/*      SPRUCC (0x20001330)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 4
*/


#define PMON_0_CONTROL_REGISTER_4_N0_IIO_CXL_CM_REG 0x23121330

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 evslct : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Event Select Minimum field to select which of
                               the available events should be recorded in the
                               paired data register. Additional bits in the
                               control register may also be required to select
                               from the available events. 0x0 == Count Nothing
                               0x1 == Count Local Clockticks
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event. The selected sub-
                               events are bitwise OR-ed together to create
                               event. At least one sub-event must be selected
                               otherwise the PerfMon event signals will not
                               ever get asserted. Events with no sub-events
                               listed effectively have only one sub-event --
                               bit 8 must be set to 1 in this case.
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the corresponding counter will be
                               cleared to 0
                            */
    UINT32 edgedet : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1, rather than measuring the event
                               in each cycle it is active, the corresponding
                               counter will increment when a 0 to 1 transition
                               (i.e. rising edge) is detected. When 0, the
                               counter will increment in each cycle that the
                               event is asserted.
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovfenable : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               When an overflow is detected from this register,
                               a PMON overflow message is sent to the global
                               control. This bit will tell the global control
                               whether it should assert the global freeze for
                               all counters in the same domain.
                            */
    UINT32 rsvd_21 : 2;

                            /* Bits[22:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               Invert comparison against Threshold. 0 -
                               comparison will be is event increment >=
                               threshold?. 1 - comparison is inverted - is
                               event increment < threshold? e.g. for a 64-entry
                               queue, if SW wanted to know how many cycles the
                               queue had fewer than 4 entries, SW should set
                               the threshold to 4 and set the invert bit to 1.
                            */
    UINT32 threshold : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               Threshold on Counter Increment. Threshold is
                               used, along with the invert bit, to compare
                               against the incoming increment value that will
                               be added to the counter. For events that
                               increment by more than 1 per cycle, if the
                               threshold is set to a value greater than 1, the
                               data register will accumulate instances in which
                               the event increment is >=threshold. e.g. say you
                               have an event to accumulate the occupancy of a
                               64-entry queue every cycle. By setting the
                               threshold value to 60, the data register would
                               count the number of cycles the queues occupancy
                               was >= 60.
                            */

  } Bits;
  UINT32 Data;

} PMON_0_CONTROL_REGISTER_4_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_4_N1_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001334)                                                      */
/*      SPRB0 (0x20001334)                                                      */
/*      SPRHBM (0x20001334)                                                     */
/*      SPRC0 (0x20001334)                                                      */
/*      SPRMCC (0x20001334)                                                     */
/*      SPRUCC (0x20001334)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 4
*/


#define PMON_0_CONTROL_REGISTER_4_N1_IIO_CXL_CM_REG 0x23121334

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

} PMON_0_CONTROL_REGISTER_4_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_5_N0_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001338)                                                      */
/*      SPRB0 (0x20001338)                                                      */
/*      SPRHBM (0x20001338)                                                     */
/*      SPRC0 (0x20001338)                                                      */
/*      SPRMCC (0x20001338)                                                     */
/*      SPRUCC (0x20001338)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 5
*/


#define PMON_0_CONTROL_REGISTER_5_N0_IIO_CXL_CM_REG 0x23121338

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 evslct : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Event Select Minimum field to select which of
                               the available events should be recorded in the
                               paired data register. Additional bits in the
                               control register may also be required to select
                               from the available events. 0x0 == Count Nothing
                               0x1 == Count Local Clockticks
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event. The selected sub-
                               events are bitwise OR-ed together to create
                               event. At least one sub-event must be selected
                               otherwise the PerfMon event signals will not
                               ever get asserted. Events with no sub-events
                               listed effectively have only one sub-event --
                               bit 8 must be set to 1 in this case.
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the corresponding counter will be
                               cleared to 0
                            */
    UINT32 edgedet : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1, rather than measuring the event
                               in each cycle it is active, the corresponding
                               counter will increment when a 0 to 1 transition
                               (i.e. rising edge) is detected. When 0, the
                               counter will increment in each cycle that the
                               event is asserted.
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovfenable : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               When an overflow is detected from this register,
                               a PMON overflow message is sent to the global
                               control. This bit will tell the global control
                               whether it should assert the global freeze for
                               all counters in the same domain.
                            */
    UINT32 rsvd_21 : 2;

                            /* Bits[22:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               Invert comparison against Threshold. 0 -
                               comparison will be is event increment >=
                               threshold?. 1 - comparison is inverted - is
                               event increment < threshold? e.g. for a 64-entry
                               queue, if SW wanted to know how many cycles the
                               queue had fewer than 4 entries, SW should set
                               the threshold to 4 and set the invert bit to 1.
                            */
    UINT32 threshold : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               Threshold on Counter Increment. Threshold is
                               used, along with the invert bit, to compare
                               against the incoming increment value that will
                               be added to the counter. For events that
                               increment by more than 1 per cycle, if the
                               threshold is set to a value greater than 1, the
                               data register will accumulate instances in which
                               the event increment is >=threshold. e.g. say you
                               have an event to accumulate the occupancy of a
                               64-entry queue every cycle. By setting the
                               threshold value to 60, the data register would
                               count the number of cycles the queues occupancy
                               was >= 60.
                            */

  } Bits;
  UINT32 Data;

} PMON_0_CONTROL_REGISTER_5_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_5_N1_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x2000133c)                                                      */
/*      SPRB0 (0x2000133c)                                                      */
/*      SPRHBM (0x2000133c)                                                     */
/*      SPRC0 (0x2000133c)                                                      */
/*      SPRMCC (0x2000133c)                                                     */
/*      SPRUCC (0x2000133c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 5
*/


#define PMON_0_CONTROL_REGISTER_5_N1_IIO_CXL_CM_REG 0x2312133C

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

} PMON_0_CONTROL_REGISTER_5_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_6_N0_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001340)                                                      */
/*      SPRB0 (0x20001340)                                                      */
/*      SPRHBM (0x20001340)                                                     */
/*      SPRC0 (0x20001340)                                                      */
/*      SPRMCC (0x20001340)                                                     */
/*      SPRUCC (0x20001340)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 6
*/


#define PMON_0_CONTROL_REGISTER_6_N0_IIO_CXL_CM_REG 0x23121340

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 evslct : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Event Select Minimum field to select which of
                               the available events should be recorded in the
                               paired data register. Additional bits in the
                               control register may also be required to select
                               from the available events. 0x0 == Count Nothing
                               0x1 == Count Local Clockticks
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event. The selected sub-
                               events are bitwise OR-ed together to create
                               event. At least one sub-event must be selected
                               otherwise the PerfMon event signals will not
                               ever get asserted. Events with no sub-events
                               listed effectively have only one sub-event --
                               bit 8 must be set to 1 in this case.
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the corresponding counter will be
                               cleared to 0
                            */
    UINT32 edgedet : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1, rather than measuring the event
                               in each cycle it is active, the corresponding
                               counter will increment when a 0 to 1 transition
                               (i.e. rising edge) is detected. When 0, the
                               counter will increment in each cycle that the
                               event is asserted.
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovfenable : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               When an overflow is detected from this register,
                               a PMON overflow message is sent to the global
                               control. This bit will tell the global control
                               whether it should assert the global freeze for
                               all counters in the same domain.
                            */
    UINT32 rsvd_21 : 2;

                            /* Bits[22:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               Invert comparison against Threshold. 0 -
                               comparison will be is event increment >=
                               threshold?. 1 - comparison is inverted - is
                               event increment < threshold? e.g. for a 64-entry
                               queue, if SW wanted to know how many cycles the
                               queue had fewer than 4 entries, SW should set
                               the threshold to 4 and set the invert bit to 1.
                            */
    UINT32 threshold : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               Threshold on Counter Increment. Threshold is
                               used, along with the invert bit, to compare
                               against the incoming increment value that will
                               be added to the counter. For events that
                               increment by more than 1 per cycle, if the
                               threshold is set to a value greater than 1, the
                               data register will accumulate instances in which
                               the event increment is >=threshold. e.g. say you
                               have an event to accumulate the occupancy of a
                               64-entry queue every cycle. By setting the
                               threshold value to 60, the data register would
                               count the number of cycles the queues occupancy
                               was >= 60.
                            */

  } Bits;
  UINT32 Data;

} PMON_0_CONTROL_REGISTER_6_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_6_N1_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001344)                                                      */
/*      SPRB0 (0x20001344)                                                      */
/*      SPRHBM (0x20001344)                                                     */
/*      SPRC0 (0x20001344)                                                      */
/*      SPRMCC (0x20001344)                                                     */
/*      SPRUCC (0x20001344)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 6
*/


#define PMON_0_CONTROL_REGISTER_6_N1_IIO_CXL_CM_REG 0x23121344

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

} PMON_0_CONTROL_REGISTER_6_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_7_N0_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001348)                                                      */
/*      SPRB0 (0x20001348)                                                      */
/*      SPRHBM (0x20001348)                                                     */
/*      SPRC0 (0x20001348)                                                      */
/*      SPRMCC (0x20001348)                                                     */
/*      SPRUCC (0x20001348)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 7
*/


#define PMON_0_CONTROL_REGISTER_7_N0_IIO_CXL_CM_REG 0x23121348

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 evslct : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Event Select Minimum field to select which of
                               the available events should be recorded in the
                               paired data register. Additional bits in the
                               control register may also be required to select
                               from the available events. 0x0 == Count Nothing
                               0x1 == Count Local Clockticks
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event. The selected sub-
                               events are bitwise OR-ed together to create
                               event. At least one sub-event must be selected
                               otherwise the PerfMon event signals will not
                               ever get asserted. Events with no sub-events
                               listed effectively have only one sub-event --
                               bit 8 must be set to 1 in this case.
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the corresponding counter will be
                               cleared to 0
                            */
    UINT32 edgedet : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1, rather than measuring the event
                               in each cycle it is active, the corresponding
                               counter will increment when a 0 to 1 transition
                               (i.e. rising edge) is detected. When 0, the
                               counter will increment in each cycle that the
                               event is asserted.
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovfenable : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               When an overflow is detected from this register,
                               a PMON overflow message is sent to the global
                               control. This bit will tell the global control
                               whether it should assert the global freeze for
                               all counters in the same domain.
                            */
    UINT32 rsvd_21 : 2;

                            /* Bits[22:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               Invert comparison against Threshold. 0 -
                               comparison will be is event increment >=
                               threshold?. 1 - comparison is inverted - is
                               event increment < threshold? e.g. for a 64-entry
                               queue, if SW wanted to know how many cycles the
                               queue had fewer than 4 entries, SW should set
                               the threshold to 4 and set the invert bit to 1.
                            */
    UINT32 threshold : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               Threshold on Counter Increment. Threshold is
                               used, along with the invert bit, to compare
                               against the incoming increment value that will
                               be added to the counter. For events that
                               increment by more than 1 per cycle, if the
                               threshold is set to a value greater than 1, the
                               data register will accumulate instances in which
                               the event increment is >=threshold. e.g. say you
                               have an event to accumulate the occupancy of a
                               64-entry queue every cycle. By setting the
                               threshold value to 60, the data register would
                               count the number of cycles the queues occupancy
                               was >= 60.
                            */

  } Bits;
  UINT32 Data;

} PMON_0_CONTROL_REGISTER_7_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_CONTROL_REGISTER_7_N1_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x2000134c)                                                      */
/*      SPRB0 (0x2000134c)                                                      */
/*      SPRHBM (0x2000134c)                                                     */
/*      SPRC0 (0x2000134c)                                                      */
/*      SPRMCC (0x2000134c)                                                     */
/*      SPRUCC (0x2000134c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Counter Control Register 7
*/


#define PMON_0_CONTROL_REGISTER_7_N1_IIO_CXL_CM_REG 0x2312134C

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

} PMON_0_CONTROL_REGISTER_7_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_0_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001350)                                                      */
/*      SPRB0 (0x20001350)                                                      */
/*      SPRHBM (0x20001350)                                                     */
/*      SPRC0 (0x20001350)                                                      */
/*      SPRMCC (0x20001350)                                                     */
/*      SPRUCC (0x20001350)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 0
*/


#define PMON_0_DATA_REGISTER_0_N0_IIO_CXL_CM_REG 0x23121350

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_0_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_0_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001354)                                                      */
/*      SPRB0 (0x20001354)                                                      */
/*      SPRHBM (0x20001354)                                                     */
/*      SPRC0 (0x20001354)                                                      */
/*      SPRMCC (0x20001354)                                                     */
/*      SPRUCC (0x20001354)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 0
*/


#define PMON_0_DATA_REGISTER_0_N1_IIO_CXL_CM_REG 0x23121354

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_0_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_1_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001358)                                                      */
/*      SPRB0 (0x20001358)                                                      */
/*      SPRHBM (0x20001358)                                                     */
/*      SPRC0 (0x20001358)                                                      */
/*      SPRMCC (0x20001358)                                                     */
/*      SPRUCC (0x20001358)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 1
*/


#define PMON_0_DATA_REGISTER_1_N0_IIO_CXL_CM_REG 0x23121358

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_1_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_1_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x2000135c)                                                      */
/*      SPRB0 (0x2000135c)                                                      */
/*      SPRHBM (0x2000135c)                                                     */
/*      SPRC0 (0x2000135c)                                                      */
/*      SPRMCC (0x2000135c)                                                     */
/*      SPRUCC (0x2000135c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 1
*/


#define PMON_0_DATA_REGISTER_1_N1_IIO_CXL_CM_REG 0x2312135C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_1_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_2_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001360)                                                      */
/*      SPRB0 (0x20001360)                                                      */
/*      SPRHBM (0x20001360)                                                     */
/*      SPRC0 (0x20001360)                                                      */
/*      SPRMCC (0x20001360)                                                     */
/*      SPRUCC (0x20001360)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 2
*/


#define PMON_0_DATA_REGISTER_2_N0_IIO_CXL_CM_REG 0x23121360

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_2_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_2_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001364)                                                      */
/*      SPRB0 (0x20001364)                                                      */
/*      SPRHBM (0x20001364)                                                     */
/*      SPRC0 (0x20001364)                                                      */
/*      SPRMCC (0x20001364)                                                     */
/*      SPRUCC (0x20001364)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 2
*/


#define PMON_0_DATA_REGISTER_2_N1_IIO_CXL_CM_REG 0x23121364

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_2_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_3_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001368)                                                      */
/*      SPRB0 (0x20001368)                                                      */
/*      SPRHBM (0x20001368)                                                     */
/*      SPRC0 (0x20001368)                                                      */
/*      SPRMCC (0x20001368)                                                     */
/*      SPRUCC (0x20001368)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 3
*/


#define PMON_0_DATA_REGISTER_3_N0_IIO_CXL_CM_REG 0x23121368

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_3_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_3_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x2000136c)                                                      */
/*      SPRB0 (0x2000136c)                                                      */
/*      SPRHBM (0x2000136c)                                                     */
/*      SPRC0 (0x2000136c)                                                      */
/*      SPRMCC (0x2000136c)                                                     */
/*      SPRUCC (0x2000136c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 3
*/


#define PMON_0_DATA_REGISTER_3_N1_IIO_CXL_CM_REG 0x2312136C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_3_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_4_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001370)                                                      */
/*      SPRB0 (0x20001370)                                                      */
/*      SPRHBM (0x20001370)                                                     */
/*      SPRC0 (0x20001370)                                                      */
/*      SPRMCC (0x20001370)                                                     */
/*      SPRUCC (0x20001370)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 4
*/


#define PMON_0_DATA_REGISTER_4_N0_IIO_CXL_CM_REG 0x23121370

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_4_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_4_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001374)                                                      */
/*      SPRB0 (0x20001374)                                                      */
/*      SPRHBM (0x20001374)                                                     */
/*      SPRC0 (0x20001374)                                                      */
/*      SPRMCC (0x20001374)                                                     */
/*      SPRUCC (0x20001374)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 4
*/


#define PMON_0_DATA_REGISTER_4_N1_IIO_CXL_CM_REG 0x23121374

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_4_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_5_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001378)                                                      */
/*      SPRB0 (0x20001378)                                                      */
/*      SPRHBM (0x20001378)                                                     */
/*      SPRC0 (0x20001378)                                                      */
/*      SPRMCC (0x20001378)                                                     */
/*      SPRUCC (0x20001378)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 5
*/


#define PMON_0_DATA_REGISTER_5_N0_IIO_CXL_CM_REG 0x23121378

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_5_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_5_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x2000137c)                                                      */
/*      SPRB0 (0x2000137c)                                                      */
/*      SPRHBM (0x2000137c)                                                     */
/*      SPRC0 (0x2000137c)                                                      */
/*      SPRMCC (0x2000137c)                                                     */
/*      SPRUCC (0x2000137c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 5
*/


#define PMON_0_DATA_REGISTER_5_N1_IIO_CXL_CM_REG 0x2312137C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_5_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_6_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001380)                                                      */
/*      SPRB0 (0x20001380)                                                      */
/*      SPRHBM (0x20001380)                                                     */
/*      SPRC0 (0x20001380)                                                      */
/*      SPRMCC (0x20001380)                                                     */
/*      SPRUCC (0x20001380)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 6
*/


#define PMON_0_DATA_REGISTER_6_N0_IIO_CXL_CM_REG 0x23121380

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_6_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_6_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001384)                                                      */
/*      SPRB0 (0x20001384)                                                      */
/*      SPRHBM (0x20001384)                                                     */
/*      SPRC0 (0x20001384)                                                      */
/*      SPRMCC (0x20001384)                                                     */
/*      SPRUCC (0x20001384)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 6
*/


#define PMON_0_DATA_REGISTER_6_N1_IIO_CXL_CM_REG 0x23121384

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_6_N1_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_7_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001388)                                                      */
/*      SPRB0 (0x20001388)                                                      */
/*      SPRHBM (0x20001388)                                                     */
/*      SPRC0 (0x20001388)                                                      */
/*      SPRMCC (0x20001388)                                                     */
/*      SPRUCC (0x20001388)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 7
*/


#define PMON_0_DATA_REGISTER_7_N0_IIO_CXL_CM_REG 0x23121388

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_7_N0_IIO_CXL_CM_STRUCT;

/* PMON_0_DATA_REGISTER_7_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x2000138c)                                                      */
/*      SPRB0 (0x2000138c)                                                      */
/*      SPRHBM (0x2000138c)                                                     */
/*      SPRC0 (0x2000138c)                                                      */
/*      SPRMCC (0x2000138c)                                                     */
/*      SPRUCC (0x2000138c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 0 Performance Event counter 7
*/


#define PMON_0_DATA_REGISTER_7_N1_IIO_CXL_CM_REG 0x2312138C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_0_DATA_REGISTER_7_N1_IIO_CXL_CM_STRUCT;

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* BIOS_W_CP_N0_IIO_CXL_CM_REG supported on:                                    */
/*      SPRB0 (0x200013d0)                                                      */
/*      SPRHBM (0x200013d0)                                                     */
/*      SPRC0 (0x200013d0)                                                      */
/*      SPRMCC (0x200013d0)                                                     */
/*      SPRUCC (0x200013d0)                                                     */
/* Register default value on SPRB0: 0x01000208                                  */
/* Register default value on SPRHBM: 0x01000208                                 */
/* Register default value on SPRC0: 0x01000208                                  */
/* Register default value on SPRMCC: 0x01000208                                 */
/* Register default value on SPRUCC: 0x01000208                                 */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRB0 BDF: 0_0_0                                       */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register configures the write access to RAC, WAC and CP registers for BIOS_W security policy group.
*/

#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* BIOS_W_CP_N1_IIO_CXL_CM_REG supported on:                                    */
/*      SPRB0 (0x200013d4)                                                      */
/*      SPRHBM (0x200013d4)                                                     */
/*      SPRC0 (0x200013d4)                                                      */
/*      SPRMCC (0x200013d4)                                                     */
/*      SPRUCC (0x200013d4)                                                     */
/* Register default value on SPRB0: 0x00000400                                  */
/* Register default value on SPRHBM: 0x00000400                                 */
/* Register default value on SPRC0: 0x00000400                                  */
/* Register default value on SPRMCC: 0x00000400                                 */
/* Register default value on SPRUCC: 0x00000400                                 */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRB0 BDF: 0_0_0                                       */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register configures the write access to RAC, WAC and CP registers for BIOS_W security policy group.
*/

#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* BIOS_W_RAC_N0_IIO_CXL_CM_REG supported on:                                   */
/*      SPRB0 (0x200013d8)                                                      */
/*      SPRHBM (0x200013d8)                                                     */
/*      SPRC0 (0x200013d8)                                                      */
/*      SPRMCC (0x200013d8)                                                     */
/*      SPRUCC (0x200013d8)                                                     */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRB0 BDF: 0_0_0                                       */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register configures the read access for registers in BIOS_W security policy group.
*/

#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* BIOS_W_RAC_N1_IIO_CXL_CM_REG supported on:                                   */
/*      SPRB0 (0x200013dc)                                                      */
/*      SPRHBM (0x200013dc)                                                     */
/*      SPRC0 (0x200013dc)                                                      */
/*      SPRMCC (0x200013dc)                                                     */
/*      SPRUCC (0x200013dc)                                                     */
/* Register default value on SPRB0: 0xFFFFFFFF                                  */
/* Register default value on SPRHBM: 0xFFFFFFFF                                 */
/* Register default value on SPRC0: 0xFFFFFFFF                                  */
/* Register default value on SPRMCC: 0xFFFFFFFF                                 */
/* Register default value on SPRUCC: 0xFFFFFFFF                                 */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRB0 BDF: 0_0_0                                       */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register configures the read access for registers in BIOS_W security policy group.
*/

#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* BIOS_W_WAC_N0_IIO_CXL_CM_REG supported on:                                   */
/*      SPRB0 (0x200013e0)                                                      */
/*      SPRHBM (0x200013e0)                                                     */
/*      SPRC0 (0x200013e0)                                                      */
/*      SPRMCC (0x200013e0)                                                     */
/*      SPRUCC (0x200013e0)                                                     */
/* Register default value on SPRB0: 0x0100021A                                  */
/* Register default value on SPRHBM: 0x0100021A                                 */
/* Register default value on SPRC0: 0x0100021A                                  */
/* Register default value on SPRMCC: 0x0100021A                                 */
/* Register default value on SPRUCC: 0x0100021A                                 */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRB0 BDF: 0_0_0                                       */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register configures the write access for registers in BIOS_W security policy group.
*/

#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

#if defined(SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST)
/* BIOS_W_WAC_N1_IIO_CXL_CM_REG supported on:                                   */
/*      SPRB0 (0x200013e4)                                                      */
/*      SPRHBM (0x200013e4)                                                     */
/*      SPRC0 (0x200013e4)                                                      */
/*      SPRMCC (0x200013e4)                                                     */
/*      SPRUCC (0x200013e4)                                                     */
/* Register default value on SPRB0: 0x20000400                                  */
/* Register default value on SPRHBM: 0x20000400                                 */
/* Register default value on SPRC0: 0x20000400                                  */
/* Register default value on SPRMCC: 0x20000400                                 */
/* Register default value on SPRUCC: 0x20000400                                 */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRB0 BDF: 0_0_0                                       */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* This register configures the write access for registers in BIOS_W security policy group.
*/

#endif /* (SPRB0_HOST) || defined(SPRHBM_HOST) || defined(SPRC0_HOST) || defined(SPRMCC_HOST) || defined(SPRUCC_HOST) */

/* LL_LCP_N0_IIO_CXL_CM_REG supported on:                                       */
/*      SPRA0 (0x20001400)                                                      */
/*      SPRB0 (0x20001400)                                                      */
/*      SPRHBM (0x20001400)                                                     */
/*      SPRC0 (0x20001400)                                                      */
/*      SPRMCC (0x20001400)                                                     */
/*      SPRUCC (0x20001400)                                                     */
/* Register default value on SPRA0: 0x00006F01                                  */
/* Register default value on SPRB0: 0x00006F01                                  */
/* Register default value on SPRHBM: 0x00006F01                                 */
/* Register default value on SPRC0: 0x00006F01                                  */
/* Register default value on SPRMCC: 0x00006F01                                 */
/* Register default value on SPRUCC: 0x00006F01                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Link Layer Capability. This controls what's advertized by the LL on INIT.Param and logs what's received from the peer LL on INIT.Param
*/


#define LL_LCP_N0_IIO_CXL_CM_REG 0x23121400

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ial_link_version_supported : 4;

                            /* Bits[3:0], Access Type=RW/P, default=0x00000001*/

                            /*
                               Interconnect Version: Version of AL the port is
                               compliant with. Intel AL 0.5 = '0000 Intel AL
                               1.0 = '0001 Others Reserved
                            */
    UINT32 ial_link_version_received : 4;

                            /* Bits[7:4], Access Type=RO/V, default=0x00000000*/

                            /*
                               Interconnect Version: Version of AL the port is
                               compliant with. Intel AL 0.5 = '0000 Intel AL
                               1.0 = '0001 Others Reserved
                            */
    UINT32 llr_wrap_value_supported : 8;

                            /* Bits[15:8], Access Type=RW/P, default=0x0000006F*/

                            /*
                               LLR Wrap Value: Value after which LLR sequence
                               counter should wrap to zero.
                            */
    UINT32 llr_wrap_value_received : 8;

                            /* Bits[23:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               LLR Wrap Value: Value after which LLR sequence
                               counter should wrap to zero.
                            */
    UINT32 num_retry_received : 5;

                            /* Bits[28:24], Access Type=RO/V, default=0x00000000*/

                            /*
                               Num_Retry value reflected in the Retry.Req
                               message received from Remote
                            */
    UINT32 num_phy_reinit_received : 3;

                            /* Bits[31:29], Access Type=RO/V, default=0x00000000*/

                            /*
                               Num_Phy_Reinit value reflected in the Retry.Req
                               message received from Remote
                            */

  } Bits;
  UINT32 Data;

} LL_LCP_N0_IIO_CXL_CM_STRUCT;

/* LL_LCP_N1_IIO_CXL_CM_REG supported on:                                       */
/*      SPRA0 (0x20001404)                                                      */
/*      SPRB0 (0x20001404)                                                      */
/*      SPRHBM (0x20001404)                                                     */
/*      SPRC0 (0x20001404)                                                      */
/*      SPRMCC (0x20001404)                                                     */
/*      SPRUCC (0x20001404)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Link Layer Capability. This controls what's advertized by the LL on INIT.Param and logs what's received from the peer LL on INIT.Param
*/


#define LL_LCP_N1_IIO_CXL_CM_REG 0x23121404

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 num_phy_reinit_received : 2;

                            /* Bits[1:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Num_Phy_Reinit value reflected in the Retry.Req
                               message received from Remote
                            */
    UINT32 wr_ptr_value_received : 8;

                            /* Bits[9:2], Access Type=RO/V, default=0x00000000*/

                            /*
                               Wr_Ptr_Value reflected in the Retry.Ack message
                               received from Remote
                            */
    UINT32 echo_eseq_value_received : 8;

                            /* Bits[17:10], Access Type=RO/V, default=0x00000000*/

                            /*
                               Echo_Eseq_Value reflected in the Retry.Ack
                               message received from Remote
                            */
    UINT32 num_free_buf_value_received : 8;

                            /* Bits[25:18], Access Type=RO/V, default=0x00000000*/

                            /*
                               Num_Free_Buf reflected in the Retry.Ack message
                               received from Remote
                            */
    UINT32 rsvd : 6;

                            /* Bits[31:26], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LL_LCP_N1_IIO_CXL_CM_STRUCT;

/* LL_CTRL_N0_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001408)                                                      */
/*      SPRB0 (0x20001408)                                                      */
/*      SPRHBM (0x20001408)                                                     */
/*      SPRC0 (0x20001408)                                                      */
/*      SPRMCC (0x20001408)                                                     */
/*      SPRUCC (0x20001408)                                                     */
/* Register default value on SPRA0: 0x00000001                                  */
/* Register default value on SPRB0: 0x00000001                                  */
/* Register default value on SPRHBM: 0x00000001                                 */
/* Register default value on SPRC0: 0x00000001                                  */
/* Register default value on SPRMCC: 0x00000001                                 */
/* Register default value on SPRUCC: 0x00000001                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Link Control & Status
*/


#define LL_CTRL_N0_IIO_CXL_CM_REG 0x23121408

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ll_reset : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000001*/

                            /*
                               Re-initialize without resetting values in sticky
                               registers. Write '1 to reset link - this is a
                               destructive reset all link layer state. When
                               link layer reset complets, HW will clear the bit
                               to '0. Entity triggering soft reset should
                               ensure that link is quiesced, and Mem/Cache CABs
                               are enabled. There will not be a case when there
                               is transaction written to the Tx BGF, waiting to
                               be drained, and soft reset is triggered.
                            */
    UINT32 ll_init_stall : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               If '1, stall the transmission of LLCTRL-
                               INIT.Param flit until this bit is cleared
                            */
    UINT32 ll_crd_stall : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /*
                               If '1, stall the transmission of LLCRD flits
                               until this bit is cleared
                            */
    UINT32 init_state : 2;

                            /* Bits[4:3], Access Type=RO/V, default=0x00000000*/

                            /*
                               This field reflects the current initialization
                               status of the Link Layer, including any stall
                               conditions controlled by LL_CTRL[2:1] '00 -->
                               NOT_RDY_FOR_INIT (stalled or unstalled): LLCTRL-
                               INIT.Param flit not sent '01 --> PARAM_EX:
                               LLCTRL-INIT.Param sent and waiting to receive it
                               '10 --> CRD_RETURN_STALL: Parameter exchanged
                               successfully and Credit return is stalled '11
                               --> INIT_DONE: Link Initialization Done -
                               LLCTRL-INIT.Param flit sent and received, and
                               inital credit refund not stalled
                            */
    UINT32 ll_retry_buffer_consumed : 8;

                            /* Bits[12:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               Number of LLR buffer currently consumed while
                               waiting for ACK
                            */
    UINT32 rsvd : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LL_CTRL_N0_IIO_CXL_CM_STRUCT;

/* LL_CTRL_N1_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x2000140c)                                                      */
/*      SPRB0 (0x2000140c)                                                      */
/*      SPRHBM (0x2000140c)                                                     */
/*      SPRC0 (0x2000140c)                                                      */
/*      SPRMCC (0x2000140c)                                                     */
/*      SPRUCC (0x2000140c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Link Control & Status
*/


#define LL_CTRL_N1_IIO_CXL_CM_REG 0x2312140C

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

} LL_CTRL_N1_IIO_CXL_CM_STRUCT;

/* LL_LCCL_N0_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001410)                                                      */
/*      SPRB0 (0x20001410)                                                      */
/*      SPRHBM (0x20001410)                                                     */
/*      SPRC0 (0x20001410)                                                      */
/*      SPRMCC (0x20001410)                                                     */
/*      SPRUCC (0x20001410)                                                     */
/* Register default value on SPRA0: 0x47414480                                  */
/* Register default value on SPRB0: 0x47414480                                  */
/* Register default value on SPRHBM: 0x47414480                                 */
/* Register default value on SPRC0: 0x47414480                                  */
/* Register default value on SPRMCC: 0x47414480                                 */
/* Register default value on SPRUCC: 0x47414480                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Link Credit Control Register
*/


#define LL_LCCL_N0_IIO_CXL_CM_REG 0x23121410

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cache_req_crd : 10;

                            /* Bits[9:0], Access Type=RW/P, default=0x00000080*/

                            /*
                               Credits to advertise for Cache Request channel
                               at init; also, keeps running snapshot of
                               accumulated credits to be returned
                            */
    UINT32 cache_rsp_crd : 10;

                            /* Bits[19:10], Access Type=RW/P, default=0x00000051*/

                            /*
                               Credits to advertise for Cache Response channel
                               at init; also, keeps running snapshot of
                               accumulated credits to be returned
                            */
    UINT32 cache_dat_crd : 10;

                            /* Bits[29:20], Access Type=RW/P, default=0x00000074*/

                            /*
                               Credits to advertise for Cache Data channel at
                               init; also, keeps running snapshot of
                               accumulated credits to be returned
                            */
    UINT32 mem_r_crd : 2;

                            /* Bits[31:30], Access Type=RW/P, default=0x00000001*/

                            /*
                               Credits to advertise for Mem Req or Rsp channel
                               at init; also, keeps running snapshot of
                               accumulated credits to be returned
                            */

  } Bits;
  UINT32 Data;

} LL_LCCL_N0_IIO_CXL_CM_STRUCT;

/* LL_LCCL_N1_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001414)                                                      */
/*      SPRB0 (0x20001414)                                                      */
/*      SPRHBM (0x20001414)                                                     */
/*      SPRC0 (0x20001414)                                                      */
/*      SPRMCC (0x20001414)                                                     */
/*      SPRUCC (0x20001414)                                                     */
/* Register default value on SPRA0: 0x00007428                                  */
/* Register default value on SPRB0: 0x00007428                                  */
/* Register default value on SPRHBM: 0x00007428                                 */
/* Register default value on SPRC0: 0x00007428                                  */
/* Register default value on SPRMCC: 0x00007428                                 */
/* Register default value on SPRUCC: 0x00007428                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Link Credit Control Register
*/


#define LL_LCCL_N1_IIO_CXL_CM_REG 0x23121414

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mem_r_crd : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000028*/

                            /*
                               Credits to advertise for Mem Req or Rsp channel
                               at init; also, keeps running snapshot of
                               accumulated credits to be returned
                            */
    UINT32 mem_dat_crd : 10;

                            /* Bits[17:8], Access Type=RW/P, default=0x00000074*/

                            /*
                               Credits to advertise for Mem Data channel at
                               init; also, keeps running snapshot of
                               accumulated credits to be returned
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LL_LCCL_N1_IIO_CXL_CM_STRUCT;

/* LL_LCCL_STS_N0_IIO_CXL_CM_REG supported on:                                  */
/*      SPRA0 (0x20001418)                                                      */
/*      SPRB0 (0x20001418)                                                      */
/*      SPRHBM (0x20001418)                                                     */
/*      SPRC0 (0x20001418)                                                      */
/*      SPRMCC (0x20001418)                                                     */
/*      SPRUCC (0x20001418)                                                     */
/* Register default value on SPRA0: 0x47414480                                  */
/* Register default value on SPRB0: 0x47414480                                  */
/* Register default value on SPRHBM: 0x47414480                                 */
/* Register default value on SPRC0: 0x47414480                                  */
/* Register default value on SPRMCC: 0x47414480                                 */
/* Register default value on SPRUCC: 0x47414480                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Link Credit Control Register
*/


#define LL_LCCL_STS_N0_IIO_CXL_CM_REG 0x23121418

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cache_req_crd_sts : 10;

                            /* Bits[9:0], Access Type=RO/V, default=0x00000080*/

                            /*
                               Cache Req running snapshot of accumulated
                               credits to be returned
                            */
    UINT32 cache_rsp_crd_sts : 10;

                            /* Bits[19:10], Access Type=RO/V, default=0x00000051*/

                            /*
                               Cache Rsp running snapshot of accumulated
                               credits to be returned
                            */
    UINT32 cache_dat_crd_sts : 10;

                            /* Bits[29:20], Access Type=RO/V, default=0x00000074*/

                            /*
                               Cache Data running snapshot of accumulated
                               credits to be returned
                            */
    UINT32 mem_r_crd_sts : 2;

                            /* Bits[31:30], Access Type=RO/V, default=0x00000001*/

                            /*
                               Mem Req/Rsp running snapshot of accumulated
                               credits to be returned
                            */

  } Bits;
  UINT32 Data;

} LL_LCCL_STS_N0_IIO_CXL_CM_STRUCT;

/* LL_LCCL_STS_N1_IIO_CXL_CM_REG supported on:                                  */
/*      SPRA0 (0x2000141c)                                                      */
/*      SPRB0 (0x2000141c)                                                      */
/*      SPRHBM (0x2000141c)                                                     */
/*      SPRC0 (0x2000141c)                                                      */
/*      SPRMCC (0x2000141c)                                                     */
/*      SPRUCC (0x2000141c)                                                     */
/* Register default value on SPRA0: 0x00007428                                  */
/* Register default value on SPRB0: 0x00007428                                  */
/* Register default value on SPRHBM: 0x00007428                                 */
/* Register default value on SPRC0: 0x00007428                                  */
/* Register default value on SPRMCC: 0x00007428                                 */
/* Register default value on SPRUCC: 0x00007428                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Link Credit Control Register
*/


#define LL_LCCL_STS_N1_IIO_CXL_CM_REG 0x2312141C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mem_r_crd_sts : 8;

                            /* Bits[7:0], Access Type=RO/V, default=0x00000028*/

                            /*
                               Mem Req/Rsp running snapshot of accumulated
                               credits to be returned
                            */
    UINT32 mem_dat_crd_sts : 10;

                            /* Bits[17:8], Access Type=RO/V, default=0x00000074*/

                            /*
                               Mem Data running snapshot of accumulated credits
                               to be returned
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LL_LCCL_STS_N1_IIO_CXL_CM_STRUCT;

/* LL_LCSTS_N0_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x20001420)                                                      */
/*      SPRB0 (0x20001420)                                                      */
/*      SPRHBM (0x20001420)                                                     */
/*      SPRC0 (0x20001420)                                                      */
/*      SPRMCC (0x20001420)                                                     */
/*      SPRUCC (0x20001420)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Link Credit Status Register
*/


#define LL_LCSTS_N0_IIO_CXL_CM_REG 0x23121420

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cache_req_crd : 10;

                            /* Bits[9:0], Access Type=RO/V, default=0x00000000*/

                            /* Credits available for Cache Request channel */
    UINT32 cache_rsp_crd : 10;

                            /* Bits[19:10], Access Type=RO/V, default=0x00000000*/

                            /* Credits available for Cache Response channel */
    UINT32 cache_dat_crd : 10;

                            /* Bits[29:20], Access Type=RO/V, default=0x00000000*/

                            /* Credits available for Cache Data channel */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LL_LCSTS_N0_IIO_CXL_CM_STRUCT;

/* LL_LCSTS_N1_IIO_CXL_CM_REG supported on:                                     */
/*      SPRA0 (0x20001424)                                                      */
/*      SPRB0 (0x20001424)                                                      */
/*      SPRHBM (0x20001424)                                                     */
/*      SPRC0 (0x20001424)                                                      */
/*      SPRMCC (0x20001424)                                                     */
/*      SPRUCC (0x20001424)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Link Credit Status Register
*/


#define LL_LCSTS_N1_IIO_CXL_CM_REG 0x23121424

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mem_r_crd : 10;

                            /* Bits[9:0], Access Type=RO/V, default=0x00000000*/

                            /* Credits available for Mem Req or Rsp channel */
    UINT32 mem_dat_crd : 10;

                            /* Bits[19:10], Access Type=RO/V, default=0x00000000*/

                            /* Credits available for Mem Data channel */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LL_LCSTS_N1_IIO_CXL_CM_STRUCT;

/* LL_LCCL_TIMER_N0_IIO_CXL_CM_REG supported on:                                */
/*      SPRA0 (0x20001428)                                                      */
/*      SPRB0 (0x20001428)                                                      */
/*      SPRHBM (0x20001428)                                                     */
/*      SPRC0 (0x20001428)                                                      */
/*      SPRMCC (0x20001428)                                                     */
/*      SPRUCC (0x20001428)                                                     */
/* Register default value on SPRA0: 0x00004010                                  */
/* Register default value on SPRB0: 0x00004010                                  */
/* Register default value on SPRHBM: 0x00004010                                 */
/* Register default value on SPRC0: 0x00004010                                  */
/* Register default value on SPRMCC: 0x00004010                                 */
/* Register default value on SPRUCC: 0x00004010                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Link Ack and Credit Timer Control Register
*/


#define LL_LCCL_TIMER_N0_IIO_CXL_CM_REG 0x23121428

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ack_force_threshold : 8;

                            /* Bits[7:0], Access Type=RW/P, default=0x00000010*/

                            /*
                               Specifies how many Flit AKs should Link Layer
                               accumulate before injecting an LLCRD
                            */
    UINT32 ack_flush_retimer : 10;

                            /* Bits[17:8], Access Type=RW/P, default=0x00000040*/

                            /*
                               Specifies how many link clock cycles Link Layer
                               should reinitialize timer for before flushing
                               all outstanding AKs by injecting an LLCRD
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LL_LCCL_TIMER_N0_IIO_CXL_CM_STRUCT;

/* LL_LCCL_TIMER_N1_IIO_CXL_CM_REG supported on:                                */
/*      SPRA0 (0x2000142c)                                                      */
/*      SPRB0 (0x2000142c)                                                      */
/*      SPRHBM (0x2000142c)                                                     */
/*      SPRC0 (0x2000142c)                                                      */
/*      SPRMCC (0x2000142c)                                                     */
/*      SPRUCC (0x2000142c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Link Ack and Credit Timer Control Register
*/


#define LL_LCCL_TIMER_N1_IIO_CXL_CM_REG 0x2312142C

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

} LL_LCCL_TIMER_N1_IIO_CXL_CM_STRUCT;

/* TL_LL_DEFEATURE_IIO_CXL_CM_REG supported on:                                 */
/*      SPRA0 (0x20001430)                                                      */
/*      SPRB0 (0x20001430)                                                      */
/*      SPRHBM (0x20001430)                                                     */
/*      SPRC0 (0x20001430)                                                      */
/*      SPRMCC (0x20001430)                                                     */
/*      SPRUCC (0x20001430)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* This register holds several defeatures for IP specific functionality
*/


#define TL_LL_DEFEATURE_IIO_CXL_CM_REG 0x23121430

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 mdh_disable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Write '1 to disable MDH. Enabled by default.
                               BIOS needs to ensure it programs this value
                               consistently on the UP & DP. After programming,
                               a warm reset is required for the disable to take
                               effect.
                            */
    UINT32 m2s_req_bypass_disable : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Bypass disable for M2S Request Tx */
    UINT32 s2m_drs_bypass_disable : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* Bypass disable for S2M DRS Rx */
    UINT32 rsvd : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TL_LL_DEFEATURE_IIO_CXL_CM_STRUCT;

/* LPIFMATCH0_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001500)                                                      */
/*      SPRB0 (0x20001500)                                                      */
/*      SPRHBM (0x20001500)                                                     */
/*      SPRC0 (0x20001500)                                                      */
/*      SPRMCC (0x20001500)                                                     */
/*      SPRUCC (0x20001500)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the lpif event to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the event is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define LPIFMATCH0_IIO_CXL_CM_REG 0x23121500

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lp_state_req : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* lpif lp_state_req */
    UINT32 pl_state_sts : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /* lpif pl_state_sts */
    UINT32 vlsm_state : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /* lpif vlsm_state */
    UINT32 lp_pri : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /* lpif lp_pri */
    UINT32 lp_rcvd_crc_err : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* lpif lp_rcvd_crc_err */
    UINT32 pl_error : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* lpif pl_error */
    UINT32 pl_cerror : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /* lpif pl_cerror */
    UINT32 pl_trainerror : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /* lpif pl_trainerror */
    UINT32 pl_rxframe_errmask : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /* lpif pl_rxframe_errmask */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LPIFMATCH0_IIO_CXL_CM_STRUCT;

/* LPIFMASK0_IIO_CXL_CM_REG supported on:                                       */
/*      SPRA0 (0x20001504)                                                      */
/*      SPRB0 (0x20001504)                                                      */
/*      SPRHBM (0x20001504)                                                     */
/*      SPRC0 (0x20001504)                                                      */
/*      SPRMCC (0x20001504)                                                     */
/*      SPRUCC (0x20001504)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* The header bits to be used in matching are selected by the mask bits in this register. A zero in a mask bit position causes the corresponding bit in the header to be ignored. A one in a mask bit position indicates that the corresponding bit must match the value of the same bit in the Match register for that bit to be considered to have matched. 
*/


#define LPIFMASK0_IIO_CXL_CM_REG 0x23121504

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lp_state_req : 4;

                            /* Bits[3:0], Access Type=RW, default=0x00000000*/

                            /* lpif lp_state_req */
    UINT32 pl_state_sts : 4;

                            /* Bits[7:4], Access Type=RW, default=0x00000000*/

                            /* lpif pl_state_sts */
    UINT32 vlsm_state : 4;

                            /* Bits[11:8], Access Type=RW, default=0x00000000*/

                            /* lpif vlsm_state */
    UINT32 lp_pri : 2;

                            /* Bits[13:12], Access Type=RW, default=0x00000000*/

                            /* lpif lp_pri */
    UINT32 lp_rcvd_crc_err : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /* lpif lp_rcvd_crc_err */
    UINT32 pl_error : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /* lpif pl_error */
    UINT32 pl_cerror : 1;

                            /* Bits[16:16], Access Type=RW, default=0x00000000*/

                            /* lpif pl_cerror */
    UINT32 pl_trainerror : 1;

                            /* Bits[17:17], Access Type=RW, default=0x00000000*/

                            /* lpif pl_trainerror */
    UINT32 pl_rxframe_errmask : 1;

                            /* Bits[18:18], Access Type=RW, default=0x00000000*/

                            /* lpif pl_rxframe_errmask */
    UINT32 rsvd : 13;

                            /* Bits[31:19], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LPIFMASK0_IIO_CXL_CM_STRUCT;

/* LINK_LAYER_CTRLMATCH0_IIO_CXL_CM_REG supported on:                           */
/*      SPRA0 (0x20001508)                                                      */
/*      SPRB0 (0x20001508)                                                      */
/*      SPRHBM (0x20001508)                                                     */
/*      SPRC0 (0x20001508)                                                      */
/*      SPRMCC (0x20001508)                                                     */
/*      SPRUCC (0x20001508)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the link layer ctrl event to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the link layer ctrl event is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define LINK_LAYER_CTRLMATCH0_IIO_CXL_CM_REG 0x23121508

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ll_init_param_rcvd : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* ll_init_param_rcvd */
    UINT32 llr_idle_rcvd : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* llr_idle_rcvd */
    UINT32 llr_req_rcvd : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* llr_req_rcvd */
    UINT32 llr_ack_rcvd : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* llr_ack_rcvd */
    UINT32 llr_frame_rcvd : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* llr_frame_rcvd */
    UINT32 ll_crd_rcvd : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* ll_crd_rcvd */
    UINT32 scheck_fail : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* scheck_fail */
    UINT32 predict_adf : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* predict_adf */
    UINT32 is_adf : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* is_adf */
    UINT32 flit_type_select : 6;

                            /* Bits[14:9], Access Type=RW, default=0x00000000*/

                            /* flit_type_select */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LINK_LAYER_CTRLMATCH0_IIO_CXL_CM_STRUCT;

/* LINK_LAYER_CTRLMASK0_IIO_CXL_CM_REG supported on:                            */
/*      SPRA0 (0x2000150c)                                                      */
/*      SPRB0 (0x2000150c)                                                      */
/*      SPRHBM (0x2000150c)                                                     */
/*      SPRC0 (0x2000150c)                                                      */
/*      SPRMCC (0x2000150c)                                                     */
/*      SPRUCC (0x2000150c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* The header bits to be used in matching are selected by the mask bits in this register. A zero in a mask bit position causes the corresponding bit in the header to be ignored. A one in a mask bit position indicates that the corresponding bit must match the value of the same bit in the Match register for that bit to be considered to have matched. 
*/


#define LINK_LAYER_CTRLMASK0_IIO_CXL_CM_REG 0x2312150C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 ll_init_param_rcvd : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* ll_init_param_rcvd */
    UINT32 llr_idle_rcvd : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* llr_idle_rcvd */
    UINT32 llr_req_rcvd : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* llr_req_rcvd */
    UINT32 llr_ack_rcvd : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* llr_ack_rcvd */
    UINT32 llr_frame_rcvd : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* llr_frame_rcvd */
    UINT32 ll_crd_rcvd : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* ll_crd_rcvd */
    UINT32 scheck_fail : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* scheck_fail */
    UINT32 predict_adf : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* predict_adf */
    UINT32 is_adf : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* is_adf */
    UINT32 flit_type_select : 6;

                            /* Bits[14:9], Access Type=RW, default=0x00000000*/

                            /*
                               Bit 0: IALCM_PROTOCOL, headered protocol flit
                               arrives Bit 1: IALCM_ADF, when RTL is predicting
                               an ADF and a flit arrives Bit 2: IALCM_LLCRD,
                               when LLCRD Flit arrives Bit 3: IALCM_LLR_FRAME,
                               when LLR Frame Flit arrives Bit 4:
                               IALCM_LLR_REQ, when LLR Request Flit arrives Bit
                               5: IALCM_LLR_ACK, when LLR Ack Flit arrives
                            */
    UINT32 rsvd : 17;

                            /* Bits[31:15], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LINK_LAYER_CTRLMASK0_IIO_CXL_CM_STRUCT;

/* LINK_LAYER_RETRYMATCH0_IIO_CXL_CM_REG supported on:                          */
/*      SPRA0 (0x20001510)                                                      */
/*      SPRB0 (0x20001510)                                                      */
/*      SPRHBM (0x20001510)                                                     */
/*      SPRC0 (0x20001510)                                                      */
/*      SPRMCC (0x20001510)                                                     */
/*      SPRUCC (0x20001510)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* These bit fields indicate specifically the value to match against in the link layer retry event to produce a MatchOut assertion. It is expected that the assertion is valid only for the clock cycle that the link layer retry event is valid. This register is used with the mask register to produced ranges of searches for particular types of transactions.
*/


#define LINK_LAYER_RETRYMATCH0_IIO_CXL_CM_REG 0x23121510

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lrsm_num_retry_reached : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Retry Timeout Occurred */
    UINT32 lrsm_num_phyreinit_reached : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Max Num Phy ReInit Event */
    UINT32 lrsm_in_abort : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* LRSM is in Abort State */
    UINT32 rrsm_llrb_ack_empty : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* LLR Ack has Empty Bit Set */
    UINT32 llrb_rd_ptr : 8;

                            /* Bits[11:4], Access Type=RW, default=0x00000000*/

                            /* LLRB Read Pointer Match */
    UINT32 llrb_wr_ptr : 8;

                            /* Bits[19:12], Access Type=RW, default=0x00000000*/

                            /* LLRB Write Pointer Match */
    UINT32 llrb_num_free_buf : 8;

                            /* Bits[27:20], Access Type=RW, default=0x00000000*/

                            /* LLRB Num Free Buffer Match */
    UINT32 llrb_full_cond : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /* LLRB Full Condition */
    UINT32 llrb_empty_cond : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* LLRB Empty Condition */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LINK_LAYER_RETRYMATCH0_IIO_CXL_CM_STRUCT;

/* LINK_LAYER_RETRYMASK0_IIO_CXL_CM_REG supported on:                           */
/*      SPRA0 (0x20001514)                                                      */
/*      SPRB0 (0x20001514)                                                      */
/*      SPRHBM (0x20001514)                                                     */
/*      SPRC0 (0x20001514)                                                      */
/*      SPRMCC (0x20001514)                                                     */
/*      SPRUCC (0x20001514)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* The header bits to be used in matching are selected by the mask bits in this register. A zero in a mask bit position causes the corresponding bit in the header to be ignored. A one in a mask bit position indicates that the corresponding bit must match the value of the same bit in the Match register for that bit to be considered to have matched. 
*/


#define LINK_LAYER_RETRYMASK0_IIO_CXL_CM_REG 0x23121514

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 lrsm_num_retry_reached : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Retry Timeout Occurred */
    UINT32 lrsm_num_phyreinit_reached : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Max Num Phy ReInit Event */
    UINT32 lrsm_in_abort : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* LRSM is in Abort State */
    UINT32 rrsm_llrb_ack_empty : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* LLR Ack has Empty Bit Set */
    UINT32 llrb_rd_ptr : 8;

                            /* Bits[11:4], Access Type=RW, default=0x00000000*/

                            /* LLRB Read Pointer Match */
    UINT32 llrb_wr_ptr : 8;

                            /* Bits[19:12], Access Type=RW, default=0x00000000*/

                            /* LLRB Write Pointer Match */
    UINT32 llrb_num_free_buf : 8;

                            /* Bits[27:20], Access Type=RW, default=0x00000000*/

                            /* LLRB Num Free Buffer Match */
    UINT32 llrb_full_cond : 1;

                            /* Bits[28:28], Access Type=RW, default=0x00000000*/

                            /* LLRB Full Condition */
    UINT32 llrb_empty_cond : 1;

                            /* Bits[29:29], Access Type=RW, default=0x00000000*/

                            /* LLRB Empty Condition */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} LINK_LAYER_RETRYMASK0_IIO_CXL_CM_STRUCT;

/* PM_PKGC_INDICATIONS_IIO_CXL_CM_REG supported on:                             */
/*      SPRA0 (0x20001530)                                                      */
/*      SPRB0 (0x20001530)                                                      */
/*      SPRHBM (0x20001530)                                                     */
/*      SPRC0 (0x20001530)                                                      */
/*      SPRMCC (0x20001530)                                                     */
/*      SPRUCC (0x20001530)                                                     */
/* Register default value on SPRA0: 0x00000208                                  */
/* Register default value on SPRB0: 0x00000208                                  */
/* Register default value on SPRHBM: 0x00000208                                 */
/* Register default value on SPRC0: 0x00000208                                  */
/* Register default value on SPRMCC: 0x00000208                                 */
/* Register default value on SPRUCC: 0x00000208                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Different indication for PM defeatures in cab and tlll included also timers for PkcC indications on ps protocol
*/


#define PM_PKGC_INDICATIONS_IIO_CXL_CM_REG 0x23121530

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tlll_idle_defeature : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Ovrd for idle indication. A zero indicate normal
                               idle operation. A one will force to deasert idle
                               indication
                            */
    UINT32 ps_req_ack_timer : 4;

                            /* Bits[4:1], Access Type=RW, default=0x00000004*/

                            /*
                               Timer to wait for ack response after receiving
                               ps_req (2^n)
                            */
    UINT32 pkgc_clkreq_timer : 8;

                            /* Bits[12:5], Access Type=RW, default=0x00000010*/

                            /*
                               Timer to wait for deasserting clkreq after
                               entering L1 (2^n)
                            */
    UINT32 pkgc_clkreq_disable : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               Disable Timer after entering L1 (2^n). 0: timer
                               is active; 1: Clk req timer disable
                            */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PM_PKGC_INDICATIONS_IIO_CXL_CM_STRUCT;

/* LINK_PM_HYST_TIMER_N0_IIO_CXL_CM_REG supported on:                           */
/*      SPRA0 (0x20001538)                                                      */
/*      SPRB0 (0x20001538)                                                      */
/*      SPRHBM (0x20001538)                                                     */
/*      SPRC0 (0x20001538)                                                      */
/*      SPRMCC (0x20001538)                                                     */
/*      SPRUCC (0x20001538)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Describes Idle and Block Link PM timers
*/


#define LINK_PM_HYST_TIMER_N0_IIO_CXL_CM_REG 0x23121538

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cfg_wait_n : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Programmable idle timer denotes number of cycles
                               vLSM waits for (post idleness), before it
                               transitions from ACTIVE to L1_REQ state.
                            */

  } Bits;
  UINT32 Data;

} LINK_PM_HYST_TIMER_N0_IIO_CXL_CM_STRUCT;

/* LINK_PM_HYST_TIMER_N1_IIO_CXL_CM_REG supported on:                           */
/*      SPRA0 (0x2000153c)                                                      */
/*      SPRB0 (0x2000153c)                                                      */
/*      SPRHBM (0x2000153c)                                                     */
/*      SPRC0 (0x2000153c)                                                      */
/*      SPRMCC (0x2000153c)                                                     */
/*      SPRUCC (0x2000153c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Describes Idle and Block Link PM timers
*/


#define LINK_PM_HYST_TIMER_N1_IIO_CXL_CM_REG 0x2312153C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cfg_block_n : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Programmable block timer denotes number of
                               cycles vLSM may have had an opportunity to enter
                               L1, but did not, becauseCfg_Wait_Nvalue was
                               higher thanCfg_Block_N.
                            */

  } Bits;
  UINT32 Data;

} LINK_PM_HYST_TIMER_N1_IIO_CXL_CM_STRUCT;

/* LINK_PM_DISUSE_STS_N0_IIO_CXL_CM_REG supported on:                           */
/*      SPRA0 (0x20001540)                                                      */
/*      SPRB0 (0x20001540)                                                      */
/*      SPRHBM (0x20001540)                                                     */
/*      SPRC0 (0x20001540)                                                      */
/*      SPRMCC (0x20001540)                                                     */
/*      SPRUCC (0x20001540)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Provides Link PM Telemetry Status
*/


#define LINK_PM_DISUSE_STS_N0_IIO_CXL_CM_REG 0x23121540

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sts_disuse_n : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Holds a snapshot valueSts_Disuse_N,
                               whereSts_Disuse_Nis counting Block Timer Expiry
                               events(Shdw_Block_N == Cfg_Block_N). This means,
                               every time design senses that it has lost an
                               opportunity to enter L1.
                            */

  } Bits;
  UINT32 Data;

} LINK_PM_DISUSE_STS_N0_IIO_CXL_CM_STRUCT;

/* LINK_PM_DISUSE_STS_N1_IIO_CXL_CM_REG supported on:                           */
/*      SPRA0 (0x20001544)                                                      */
/*      SPRB0 (0x20001544)                                                      */
/*      SPRHBM (0x20001544)                                                     */
/*      SPRC0 (0x20001544)                                                      */
/*      SPRMCC (0x20001544)                                                     */
/*      SPRUCC (0x20001544)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Provides Link PM Telemetry Status
*/


#define LINK_PM_DISUSE_STS_N1_IIO_CXL_CM_REG 0x23121544

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sts_disuse_n : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Holds a snapshot valueSts_Disuse_N,
                               whereSts_Disuse_Nis counting Block Timer Expiry
                               events(Shdw_Block_N == Cfg_Block_N). This means,
                               every time design senses that it has lost an
                               opportunity to enter L1.
                            */

  } Bits;
  UINT32 Data;

} LINK_PM_DISUSE_STS_N1_IIO_CXL_CM_STRUCT;

/* LINK_PM_ABORT_STS_N0_IIO_CXL_CM_REG supported on:                            */
/*      SPRA0 (0x20001548)                                                      */
/*      SPRB0 (0x20001548)                                                      */
/*      SPRHBM (0x20001548)                                                     */
/*      SPRC0 (0x20001548)                                                      */
/*      SPRMCC (0x20001548)                                                     */
/*      SPRUCC (0x20001548)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Provides Link PM Telemetry Status
*/


#define LINK_PM_ABORT_STS_N0_IIO_CXL_CM_REG 0x23121548

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sts_abort_n : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Holds a snapshot valueSts_Abort_N,
                               whereSts_Abort_Nis counting an L1 Abort event.
                               This means, every time design senses an
                               unsuccessful attempt to enter L1
                            */

  } Bits;
  UINT32 Data;

} LINK_PM_ABORT_STS_N0_IIO_CXL_CM_STRUCT;

/* LINK_PM_ABORT_STS_N1_IIO_CXL_CM_REG supported on:                            */
/*      SPRA0 (0x2000154c)                                                      */
/*      SPRB0 (0x2000154c)                                                      */
/*      SPRHBM (0x2000154c)                                                     */
/*      SPRC0 (0x2000154c)                                                      */
/*      SPRMCC (0x2000154c)                                                     */
/*      SPRUCC (0x2000154c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Provides Link PM Telemetry Status
*/


#define LINK_PM_ABORT_STS_N1_IIO_CXL_CM_REG 0x2312154C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 sts_abort_n : 32;

                            /* Bits[31:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Holds a snapshot valueSts_Abort_N,
                               whereSts_Abort_Nis counting an L1 Abort event.
                               This means, every time design senses an
                               unsuccessful attempt to enter L1
                            */

  } Bits;
  UINT32 Data;

} LINK_PM_ABORT_STS_N1_IIO_CXL_CM_STRUCT;

/* PMON_UNIT_0_DISCOVERY0_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001600)                                                      */
/*      SPRB0 (0x20001600)                                                      */
/*      SPRHBM (0x20001600)                                                     */
/*      SPRC0 (0x20001600)                                                      */
/*      SPRMCC (0x20001600)                                                     */
/*      SPRUCC (0x20001600)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Discovery state register 0
*/


#define PMON_UNIT_0_DISCOVERY0_N0_IIO_CXL_CM_REG 0x23121600

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_unit_disc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* PMON Discovery Unit 0 Discovery 0 */

  } Bits;
  UINT32 Data;

} PMON_UNIT_0_DISCOVERY0_N0_IIO_CXL_CM_STRUCT;

/* PMON_UNIT_0_DISCOVERY0_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001604)                                                      */
/*      SPRB0 (0x20001604)                                                      */
/*      SPRHBM (0x20001604)                                                     */
/*      SPRC0 (0x20001604)                                                      */
/*      SPRMCC (0x20001604)                                                     */
/*      SPRUCC (0x20001604)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Discovery state register 0
*/


#define PMON_UNIT_0_DISCOVERY0_N1_IIO_CXL_CM_REG 0x23121604

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_unit_disc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* PMON Discovery Unit 0 Discovery 0 */

  } Bits;
  UINT32 Data;

} PMON_UNIT_0_DISCOVERY0_N1_IIO_CXL_CM_STRUCT;

/* PMON_UNIT_0_DISCOVERY1_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001608)                                                      */
/*      SPRB0 (0x20001608)                                                      */
/*      SPRHBM (0x20001608)                                                     */
/*      SPRC0 (0x20001608)                                                      */
/*      SPRMCC (0x20001608)                                                     */
/*      SPRUCC (0x20001608)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Discovery state register 1
*/


#define PMON_UNIT_0_DISCOVERY1_N0_IIO_CXL_CM_REG 0x23121608

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_unit_disc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* PMON Discovery Unit 0 Discovery 1 */

  } Bits;
  UINT32 Data;

} PMON_UNIT_0_DISCOVERY1_N0_IIO_CXL_CM_STRUCT;

/* PMON_UNIT_0_DISCOVERY1_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x2000160c)                                                      */
/*      SPRB0 (0x2000160c)                                                      */
/*      SPRHBM (0x2000160c)                                                     */
/*      SPRC0 (0x2000160c)                                                      */
/*      SPRMCC (0x2000160c)                                                     */
/*      SPRUCC (0x2000160c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Discovery state register 1
*/


#define PMON_UNIT_0_DISCOVERY1_N1_IIO_CXL_CM_REG 0x2312160C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_unit_disc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* PMON Discovery Unit 0 Discovery 1 */

  } Bits;
  UINT32 Data;

} PMON_UNIT_0_DISCOVERY1_N1_IIO_CXL_CM_STRUCT;

/* PMON_UNIT_0_DISCOVERY2_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001610)                                                      */
/*      SPRB0 (0x20001610)                                                      */
/*      SPRHBM (0x20001610)                                                     */
/*      SPRC0 (0x20001610)                                                      */
/*      SPRMCC (0x20001610)                                                     */
/*      SPRUCC (0x20001610)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Discovery state register 2
*/


#define PMON_UNIT_0_DISCOVERY2_N0_IIO_CXL_CM_REG 0x23121610

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_unit_disc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* PMON Discovery Unit 0 Discovery 2 */

  } Bits;
  UINT32 Data;

} PMON_UNIT_0_DISCOVERY2_N0_IIO_CXL_CM_STRUCT;

/* PMON_UNIT_0_DISCOVERY2_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001614)                                                      */
/*      SPRB0 (0x20001614)                                                      */
/*      SPRHBM (0x20001614)                                                     */
/*      SPRC0 (0x20001614)                                                      */
/*      SPRMCC (0x20001614)                                                     */
/*      SPRUCC (0x20001614)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Discovery state register 2
*/


#define PMON_UNIT_0_DISCOVERY2_N1_IIO_CXL_CM_REG 0x23121614

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_unit_disc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* PMON Discovery Unit 0 Discovery 2 */

  } Bits;
  UINT32 Data;

} PMON_UNIT_0_DISCOVERY2_N1_IIO_CXL_CM_STRUCT;

/* PMON_BLOCK_IDENTIFIER_0_N0_IIO_CXL_CM_REG supported on:                      */
/*      SPRA0 (0x20001618)                                                      */
/*      SPRB0 (0x20001618)                                                      */
/*      SPRHBM (0x20001618)                                                     */
/*      SPRC0 (0x20001618)                                                      */
/*      SPRMCC (0x20001618)                                                     */
/*      SPRUCC (0x20001618)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Block 0 Identifier
*/


#define PMON_BLOCK_IDENTIFIER_0_N0_IIO_CXL_CM_REG 0x23121618

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_blk_id : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               Local PMON Block Identifier. BIOS writes it with
                               the block ID, the PMON block logic uses that ID
                               for the overflow message.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMON_BLOCK_IDENTIFIER_0_N0_IIO_CXL_CM_STRUCT;

/* PMON_BLOCK_IDENTIFIER_0_N1_IIO_CXL_CM_REG supported on:                      */
/*      SPRA0 (0x2000161c)                                                      */
/*      SPRB0 (0x2000161c)                                                      */
/*      SPRHBM (0x2000161c)                                                     */
/*      SPRC0 (0x2000161c)                                                      */
/*      SPRMCC (0x2000161c)                                                     */
/*      SPRUCC (0x2000161c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Block 0 Identifier
*/


#define PMON_BLOCK_IDENTIFIER_0_N1_IIO_CXL_CM_REG 0x2312161C

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

} PMON_BLOCK_IDENTIFIER_0_N1_IIO_CXL_CM_STRUCT;

/* IDI_CAB_CONTROL_STATUS_IIO_CXL_CM_REG supported on:                          */
/*      SPRA0 (0x20001900)                                                      */
/*      SPRB0 (0x20001900)                                                      */
/*      SPRHBM (0x20001900)                                                     */
/*      SPRC0 (0x20001900)                                                      */
/*      SPRMCC (0x20001900)                                                     */
/*      SPRUCC (0x20001900)                                                     */
/* Register default value on SPRA0: 0x00000004                                  */
/* Register default value on SPRB0: 0x00000004                                  */
/* Register default value on SPRHBM: 0x00000004                                 */
/* Register default value on SPRC0: 0x00000004                                  */
/* Register default value on SPRMCC: 0x00000004                                 */
/* Register default value on SPRUCC: 0x00000004                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* IDI CAB Controls
*/


#define IDI_CAB_CONTROL_STATUS_IIO_CXL_CM_REG 0x23121900

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 force_idi_cab_active : 1;

                            /* Bits[0:0], Access Type=WO, default=0x00000000*/

                            /*
                               When written, forces the IDI CAB to active. Once
                               active, the CAB will respond to snoops
                            */
    UINT32 force_idi_cab_de_active : 1;

                            /* Bits[1:1], Access Type=WO, default=0x00000000*/

                            /*
                               When written, forces the IDI CAB to de-
                               activated. Once de-active, the CAB will not
                               respond to snoops
                            */
    UINT32 idi_cab_status : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000001*/

                            /*
                               This bit reflects the curent status of the IDI
                               CAB. 1 indicates active. 0 indicates inactive
                            */
    UINT32 block_new_idi_req : 1;

                            /* Bits[3:3], Access Type=WO, default=0x00000000*/

                            /*
                               Writing this bit will block new C2U Requests
                               from either channel
                            */
    UINT32 unblock_new_idi_req : 1;

                            /* Bits[4:4], Access Type=WO, default=0x00000000*/

                            /*
                               Writing this bit will unblock new C2U Requests
                               from either channel
                            */
    UINT32 idi_req_block_status : 1;

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /*
                               0 --> IDI Requests Unblocked 1 --> IDI Requests
                               Blocked
                            */
    UINT32 defeature_wbmtoi : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               0 --> DirtyEvict maps to WbPushHint 1 -->
                               DirtyEvict maps to WbMtoI
                            */
    UINT32 rsvd : 25;

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IDI_CAB_CONTROL_STATUS_IIO_CXL_CM_STRUCT;

/* MEM_CAB_CONTROL_STATUS_IIO_CXL_CM_REG supported on:                          */
/*      SPRA0 (0x20001904)                                                      */
/*      SPRB0 (0x20001904)                                                      */
/*      SPRHBM (0x20001904)                                                     */
/*      SPRC0 (0x20001904)                                                      */
/*      SPRMCC (0x20001904)                                                     */
/*      SPRUCC (0x20001904)                                                     */
/* Register default value on SPRA0: 0x0000000C                                  */
/* Register default value on SPRB0: 0x0000000C                                  */
/* Register default value on SPRHBM: 0x0000000C                                 */
/* Register default value on SPRC0: 0x0000000C                                  */
/* Register default value on SPRMCC: 0x0000000C                                 */
/* Register default value on SPRUCC: 0x0000000C                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* MEM CAB control status register
*/


#define MEM_CAB_CONTROL_STATUS_IIO_CXL_CM_REG 0x23121904

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 force_mem_cab_active : 1;

                            /* Bits[0:0], Access Type=WO, default=0x00000000*/

                            /*
                               When written, forces the MEM CAB to active. Once
                               active, the CAB will respond to MEM requests
                               from the host
                            */
    UINT32 force_mem_cab_de_active : 1;

                            /* Bits[1:1], Access Type=WO, default=0x00000000*/

                            /*
                               When written, forces the MEM CAB to de-
                               activated. Once de-active, the CAB will not
                               respond to MEM requests from the host
                            */
    UINT32 mem_cab_status : 1;

                            /* Bits[2:2], Access Type=RO/V, default=0x00000001*/

                            /*
                               This bit reflects the curent status of the MEM
                               CAB. 1 indicates active. 0 indicates inactive
                            */
    UINT32 poison_cab_response : 1;

                            /* Bits[3:3], Access Type=RW, default=0x00000001*/

                            /* Poison any data response from the MEM CAB */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MEM_CAB_CONTROL_STATUS_IIO_CXL_CM_STRUCT;

/* IDI_CRD_OBS_N0_IIO_CXL_CM_REG supported on:                                  */
/*      SPRA0 (0x20001908)                                                      */
/*      SPRB0 (0x20001908)                                                      */
/*      SPRHBM (0x20001908)                                                     */
/*      SPRC0 (0x20001908)                                                      */
/*      SPRMCC (0x20001908)                                                     */
/*      SPRUCC (0x20001908)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Defines observability for credits available for IDI
*/


#define IDI_CRD_OBS_N0_IIO_CXL_CM_REG 0x23121908

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 c2u_req0_avail : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /* Number of C2U Req Ch0 Credits available */
    UINT32 c2u_req1_avail : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /* Number of C2U Req Ch1 Credits available */
    UINT32 c2u_rsp_avail : 7;

                            /* Bits[20:14], Access Type=RO/V, default=0x00000000*/

                            /* Number of C2U Rsp Credits available */
    UINT32 c2u_dat_avail : 7;

                            /* Bits[27:21], Access Type=RO/V, default=0x00000000*/

                            /* Number of C2U Data Credits available */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IDI_CRD_OBS_N0_IIO_CXL_CM_STRUCT;

/* IDI_CRD_OBS_N1_IIO_CXL_CM_REG supported on:                                  */
/*      SPRA0 (0x2000190c)                                                      */
/*      SPRB0 (0x2000190c)                                                      */
/*      SPRHBM (0x2000190c)                                                     */
/*      SPRC0 (0x2000190c)                                                      */
/*      SPRMCC (0x2000190c)                                                     */
/*      SPRUCC (0x2000190c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Defines observability for credits available for IDI
*/


#define IDI_CRD_OBS_N1_IIO_CXL_CM_REG 0x2312190C

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

} IDI_CRD_OBS_N1_IIO_CXL_CM_STRUCT;

/* MEM_CRD_OBS_N0_IIO_CXL_CM_REG supported on:                                  */
/*      SPRA0 (0x20001910)                                                      */
/*      SPRB0 (0x20001910)                                                      */
/*      SPRHBM (0x20001910)                                                     */
/*      SPRC0 (0x20001910)                                                      */
/*      SPRMCC (0x20001910)                                                     */
/*      SPRUCC (0x20001910)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Defines observability for credits available for MEM
*/


#define MEM_CRD_OBS_N0_IIO_CXL_CM_REG 0x23121910

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 s2m_ndr_avail : 7;

                            /* Bits[6:0], Access Type=RO/V, default=0x00000000*/

                            /* Number of S2M NDR Credits available */
    UINT32 s2m_drs_avail : 7;

                            /* Bits[13:7], Access Type=RO/V, default=0x00000000*/

                            /* Number of S2M DRS Credits available */
    UINT32 rsvd : 18;

                            /* Bits[31:14], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MEM_CRD_OBS_N0_IIO_CXL_CM_STRUCT;

/* MEM_CRD_OBS_N1_IIO_CXL_CM_REG supported on:                                  */
/*      SPRA0 (0x20001914)                                                      */
/*      SPRB0 (0x20001914)                                                      */
/*      SPRHBM (0x20001914)                                                     */
/*      SPRC0 (0x20001914)                                                      */
/*      SPRMCC (0x20001914)                                                     */
/*      SPRUCC (0x20001914)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Defines observability for credits available for MEM
*/


#define MEM_CRD_OBS_N1_IIO_CXL_CM_REG 0x23121914

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

} MEM_CRD_OBS_N1_IIO_CXL_CM_STRUCT;

/* CAB_PKGC_CONTROL_IIO_CXL_CM_REG supported on:                                */
/*      SPRA0 (0x20001918)                                                      */
/*      SPRB0 (0x20001918)                                                      */
/*      SPRHBM (0x20001918)                                                     */
/*      SPRC0 (0x20001918)                                                      */
/*      SPRMCC (0x20001918)                                                     */
/*      SPRUCC (0x20001918)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Defeatures for various PM PkgC transitions
*/


#define CAB_PKGC_CONTROL_IIO_CXL_CM_REG 0x23121918

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cab_idle_defeature : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               Ovrd for idle indication. A zero indicate normal
                               idle operation. A one will force to deasert idle
                               indication
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CAB_PKGC_CONTROL_IIO_CXL_CM_STRUCT;

/* IDI_CAB_GNT_COUNTER_IIO_CXL_CM_REG supported on:                             */
/*      SPRA0 (0x2000191c)                                                      */
/*      SPRB0 (0x2000191c)                                                      */
/*      SPRHBM (0x2000191c)                                                     */
/*      SPRC0 (0x2000191c)                                                      */
/*      SPRMCC (0x2000191c)                                                     */
/*      SPRUCC (0x2000191c)                                                     */
/* Register default value on SPRA0: 0x0000001F                                  */
/* Register default value on SPRB0: 0x0000001F                                  */
/* Register default value on SPRHBM: 0x0000001F                                 */
/* Register default value on SPRC0: 0x0000001F                                  */
/* Register default value on SPRMCC: 0x0000001F                                 */
/* Register default value on SPRUCC: 0x0000001F                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Anti-starvation counter for cache CAB to get a grant to send C2U Rsp on IDI inteface
*/


#define IDI_CAB_GNT_COUNTER_IIO_CXL_CM_REG 0x2312191C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 gnt_counter : 5;

                            /* Bits[4:0], Access Type=RW, default=0x0000001F*/

                            /*
                               This defines the number of cycles after which
                               cache CAB gets a grant to send C2U Rsp on IDI
                               interface. Cache CAB gets a grant when D2H Rsp
                               BGF does not have anything to send or when this
                               counter expires.
                            */
    UINT32 rsvd : 27;

                            /* Bits[31:5], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IDI_CAB_GNT_COUNTER_IIO_CXL_CM_STRUCT;

/* IAL_SECURITY_N0_IIO_CXL_CM_REG supported on:                                 */
/*      SPRA0 (0x20001930)                                                      */
/*      SPRB0 (0x20001930)                                                      */
/*      SPRHBM (0x20001930)                                                     */
/*      SPRC0 (0x20001930)                                                      */
/*      SPRMCC (0x20001930)                                                     */
/*      SPRUCC (0x20001930)                                                     */
/* Register default value on SPRA0: 0x00000002                                  */
/* Register default value on SPRB0: 0x00000002                                  */
/* Register default value on SPRHBM: 0x00000002                                 */
/* Register default value on SPRC0: 0x00000002                                  */
/* Register default value on SPRMCC: 0x00000002                                 */
/* Register default value on SPRUCC: 0x00000002                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register can be used by the OS or VMM to assign trust privileges to the device. For a non-IAL aware OS, platform FW (BIOS) should configure this register if it wants to allow an IAL device to send requests to the Host on IAL.$
*/


#define IAL_SECURITY_N0_IIO_CXL_CM_REG 0x23121930

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 security_level : 2;

                            /* Bits[1:0], Access Type=RW, default=0x00000002*/

                            /*
                               0 --> Trusted IAL Device. At this setting, an
                               IAL Device will be able to get access on IAL.$
                               for host-attached and device attached memory
                               ranges in the WB address space. The Host will
                               still protect sensitive memory regions such as
                               PMRs. '1 --> Trusted for Device Attached Memory
                               Range Only. At this setting, an IAL Device will
                               be able to get access on IAL.$ for device
                               attached memory ranges only. Requests on IAL.$
                               for host-attached memory ranges will be aborted
                               by the Host. '2 --> Untrusted IAL Device. At
                               this setting, all requests on IAL.$ will be
                               aborted by the Host. Please note that these
                               settings only apply to requests on IAL.$. The
                               device can still source requests on IAL.io
                               regardless of these settings. Protection on
                               IAL.io will be implemented using IOMMU based
                               page tables.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} IAL_SECURITY_N0_IIO_CXL_CM_STRUCT;

/* IAL_SECURITY_N1_IIO_CXL_CM_REG supported on:                                 */
/*      SPRA0 (0x20001934)                                                      */
/*      SPRB0 (0x20001934)                                                      */
/*      SPRHBM (0x20001934)                                                     */
/*      SPRC0 (0x20001934)                                                      */
/*      SPRMCC (0x20001934)                                                     */
/*      SPRUCC (0x20001934)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* This register can be used by the OS or VMM to assign trust privileges to the device. For a non-IAL aware OS, platform FW (BIOS) should configure this register if it wants to allow an IAL device to send requests to the Host on IAL.$
*/


#define IAL_SECURITY_N1_IIO_CXL_CM_REG 0x23121934

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

} IAL_SECURITY_N1_IIO_CXL_CM_STRUCT;

/* DEVICE_MEM_BASE_N0_IIO_CXL_CM_REG supported on:                              */
/*      SPRA0 (0x20001950)                                                      */
/*      SPRB0 (0x20001950)                                                      */
/*      SPRHBM (0x20001950)                                                     */
/*      SPRC0 (0x20001950)                                                      */
/*      SPRMCC (0x20001950)                                                     */
/*      SPRUCC (0x20001950)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Device's coherent memory address base
*/


#define DEVICE_MEM_BASE_N0_IIO_CXL_CM_REG 0x23121950

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 6;

                            /* Bits[5:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dev_mem_base : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /* Device's coherent memory address base */

  } Bits;
  UINT32 Data;

} DEVICE_MEM_BASE_N0_IIO_CXL_CM_STRUCT;

/* DEVICE_MEM_BASE_N1_IIO_CXL_CM_REG supported on:                              */
/*      SPRA0 (0x20001954)                                                      */
/*      SPRB0 (0x20001954)                                                      */
/*      SPRHBM (0x20001954)                                                     */
/*      SPRC0 (0x20001954)                                                      */
/*      SPRMCC (0x20001954)                                                     */
/*      SPRUCC (0x20001954)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Device's coherent memory address base
*/


#define DEVICE_MEM_BASE_N1_IIO_CXL_CM_REG 0x23121954

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dev_mem_base : 20;

                            /* Bits[19:0], Access Type=RW, default=0x00000000*/

                            /* Device's coherent memory address base */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DEVICE_MEM_BASE_N1_IIO_CXL_CM_STRUCT;

/* DEVICE_MEM_LIMIT_N0_IIO_CXL_CM_REG supported on:                             */
/*      SPRA0 (0x20001958)                                                      */
/*      SPRB0 (0x20001958)                                                      */
/*      SPRHBM (0x20001958)                                                     */
/*      SPRC0 (0x20001958)                                                      */
/*      SPRMCC (0x20001958)                                                     */
/*      SPRUCC (0x20001958)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Device's coherent memory address limit
*/


#define DEVICE_MEM_LIMIT_N0_IIO_CXL_CM_REG 0x23121958

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 6;

                            /* Bits[5:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dev_mem_limit : 26;

                            /* Bits[31:6], Access Type=RW, default=0x00000000*/

                            /* Device's coherent memory address limit */

  } Bits;
  UINT32 Data;

} DEVICE_MEM_LIMIT_N0_IIO_CXL_CM_STRUCT;

/* DEVICE_MEM_LIMIT_N1_IIO_CXL_CM_REG supported on:                             */
/*      SPRA0 (0x2000195c)                                                      */
/*      SPRB0 (0x2000195c)                                                      */
/*      SPRHBM (0x2000195c)                                                     */
/*      SPRC0 (0x2000195c)                                                      */
/*      SPRMCC (0x2000195c)                                                     */
/*      SPRUCC (0x2000195c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* Device's coherent memory address limit
*/


#define DEVICE_MEM_LIMIT_N1_IIO_CXL_CM_REG 0x2312195C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dev_mem_limit : 20;

                            /* Bits[19:0], Access Type=RW, default=0x00000000*/

                            /* Device's coherent memory address limit */
    UINT32 rsvd : 12;

                            /* Bits[31:20], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DEVICE_MEM_LIMIT_N1_IIO_CXL_CM_STRUCT;

/* SNC_CONFIG_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001a00)                                                      */
/*      SPRB0 (0x20001a00)                                                      */
/*      SPRHBM (0x20001a00)                                                     */
/*      SPRC0 (0x20001a00)                                                      */
/*      SPRMCC (0x20001a00)                                                     */
/*      SPRUCC (0x20001a00)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SNC Configuration Register. DP only. Configured at Boot.
*/


#define SNC_CONFIG_IIO_CXL_CM_REG 0x23121A00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 full_snc_enable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Full SNC Enable */
    UINT32 snc_ind_enable : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* SNC IND enable */
    UINT32 num_clusters : 2;

                            /* Bits[3:2], Access Type=RW/P, default=0x00000000*/

                            /* Num of SNC Clusters */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_CONFIG_IIO_CXL_CM_STRUCT;

/* SNC_BASE_1_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001a04)                                                      */
/*      SPRB0 (0x20001a04)                                                      */
/*      SPRHBM (0x20001a04)                                                     */
/*      SPRC0 (0x20001a04)                                                      */
/*      SPRMCC (0x20001a04)                                                     */
/*      SPRUCC (0x20001a04)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SNC Hash Register. DP Only. Configured at Boot.
*/


#define SNC_BASE_1_IIO_CXL_CM_REG 0x23121A04

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Starting Address for Cluster 0 Base Address
                               [45:30]. This is the 1G aligned base address of
                               local cluster 0. Any address less than this
                               address will have SNLocal=0.
                            */
    UINT32 twolm_mask : 6;

                            /* Bits[21:16], Access Type=RW/P, default=0x00000000*/

                            /* 2LM Mask[45:40] */
    UINT32 hi_twolm_mask : 6;

                            /* Bits[27:22], Access Type=RW/P, default=0x00000000*/

                            /*
                               2LM Mask[51:46] (SPR only, any core that only
                               supports 46 address bits can treat this as
                               reserved)
                            */
    UINT32 rsvd : 4;

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_BASE_1_IIO_CXL_CM_STRUCT;

/* SNC_BASE_2_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001a08)                                                      */
/*      SPRB0 (0x20001a08)                                                      */
/*      SPRHBM (0x20001a08)                                                     */
/*      SPRC0 (0x20001a08)                                                      */
/*      SPRMCC (0x20001a08)                                                     */
/*      SPRUCC (0x20001a08)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SNC Base Register. DP Only. Configured at Boot.
*/


#define SNC_BASE_2_IIO_CXL_CM_REG 0x23121A08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Starting Address for Cluster 1 Base Address
                               [45:30]. This is the 1G aligned base address of
                               local cluster 0. Any address less than this
                               address will have SNLocal=0.
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 twolm_mask : 1;

                            /* Bits[17:17], Access Type=RW/P, default=0x00000000*/

                            /*
                               2LM Mask[28]. 2LM Mask[28]. KNH uses this. CNL
                               can treat this as reserved and simply assume 2LM
                               Mask[28] is always 0
                            */
    UINT32 hi_twolm_mask : 11;

                            /* Bits[28:18], Access Type=RW/P, default=0x00000000*/

                            /* 2LM Mask [39:29] */
    UINT32 rsvd_29 : 3;

                            /* Bits[31:29], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_BASE_2_IIO_CXL_CM_STRUCT;

/* SNC_BASE_3_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001a0c)                                                      */
/*      SPRB0 (0x20001a0c)                                                      */
/*      SPRHBM (0x20001a0c)                                                     */
/*      SPRC0 (0x20001a0c)                                                      */
/*      SPRMCC (0x20001a0c)                                                     */
/*      SPRUCC (0x20001a0c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SNC Base Register. DP Only. Configured at Boot.
*/


#define SNC_BASE_3_IIO_CXL_CM_REG 0x23121A0C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Starting Address for Cluster 2 Base Address
                               [45:30]. This is the 1G aligned base address of
                               local cluster 0. Any address less than this
                               address will have SNLocal=0
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_BASE_3_IIO_CXL_CM_STRUCT;

/* SNC_BASE_4_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001a10)                                                      */
/*      SPRB0 (0x20001a10)                                                      */
/*      SPRHBM (0x20001a10)                                                     */
/*      SPRC0 (0x20001a10)                                                      */
/*      SPRMCC (0x20001a10)                                                     */
/*      SPRUCC (0x20001a10)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SNC Base Register. DP Only. Configured at Boot.
*/


#define SNC_BASE_4_IIO_CXL_CM_REG 0x23121A10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Starting Address for Cluster 3 Base Address
                               [45:30]. This is the 1G aligned base address of
                               local cluster 0. Any address less than this
                               address will have SNLocal=0
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_BASE_4_IIO_CXL_CM_STRUCT;

/* SNC_BASE_5_IIO_CXL_CM_REG supported on:                                      */
/*      SPRA0 (0x20001a14)                                                      */
/*      SPRB0 (0x20001a14)                                                      */
/*      SPRHBM (0x20001a14)                                                     */
/*      SPRC0 (0x20001a14)                                                      */
/*      SPRMCC (0x20001a14)                                                     */
/*      SPRUCC (0x20001a14)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SNC Base Register. DP Only. Configured at Boot.
*/


#define SNC_BASE_5_IIO_CXL_CM_REG 0x23121A14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 addr : 16;

                            /* Bits[15:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Starting Address for Cluster 4 Base Address
                               [45:30]. This is the 1G aligned base address of
                               local cluster 0. Any address less than this
                               address will have SNLocal=0
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_BASE_5_IIO_CXL_CM_STRUCT;

/* SNC_UPPER_BASE_IIO_CXL_CM_REG supported on:                                  */
/*      SPRA0 (0x20001a18)                                                      */
/*      SPRB0 (0x20001a18)                                                      */
/*      SPRHBM (0x20001a18)                                                     */
/*      SPRC0 (0x20001a18)                                                      */
/*      SPRMCC (0x20001a18)                                                     */
/*      SPRUCC (0x20001a18)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SNC Base Register. DP Only. Configured at Boot.
*/


#define SNC_UPPER_BASE_IIO_CXL_CM_REG 0x23121A18

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 upper_base_1 : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Starting Address Upper Address bits [51:46] for
                               Cluster 1
                            */
    UINT32 upper_base_2 : 6;

                            /* Bits[11:6], Access Type=RW/P, default=0x00000000*/

                            /*
                               Starting Address Upper Address bits [51:46] for
                               Cluster 2
                            */
    UINT32 upper_base_3 : 6;

                            /* Bits[17:12], Access Type=RW/P, default=0x00000000*/

                            /*
                               Starting Address Upper Address bits [51:46] for
                               Cluster 3
                            */
    UINT32 upper_base_4 : 6;

                            /* Bits[23:18], Access Type=RW/P, default=0x00000000*/

                            /*
                               Starting Address Upper Address bits [51:46] for
                               Cluster 4
                            */
    UINT32 upper_base_5 : 6;

                            /* Bits[29:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Starting Address Upper Address bits [51:46] for
                               Cluster 5
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SNC_UPPER_BASE_IIO_CXL_CM_STRUCT;

/* UNCORE_SNC_CONFIG_IIO_CXL_CM_REG supported on:                               */
/*      SPRA0 (0x20001a1c)                                                      */
/*      SPRB0 (0x20001a1c)                                                      */
/*      SPRHBM (0x20001a1c)                                                     */
/*      SPRC0 (0x20001a1c)                                                      */
/*      SPRMCC (0x20001a1c)                                                     */
/*      SPRUCC (0x20001a1c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SNC Hash Register. DP Only. Configured at Boot.
*/


#define UNCORE_SNC_CONFIG_IIO_CXL_CM_REG 0x23121A1C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 num_chas_per_cluster : 6;

                            /* Bits[5:0], Access Type=RW/P, default=0x00000000*/

                            /* Number of CHA's per SNC Cluster */
    UINT32 base_cha_cluster_1 : 6;

                            /* Bits[11:6], Access Type=RW/P, default=0x00000000*/

                            /* Base of CHA Cluster 1 */
    UINT32 base_cha_cluster_2 : 6;

                            /* Bits[17:12], Access Type=RW/P, default=0x00000000*/

                            /* Base of CHA Cluster 2 */
    UINT32 base_cha_cluster_3 : 6;

                            /* Bits[23:18], Access Type=RW/P, default=0x00000000*/

                            /* Base of CHA Cluster 3 */
    UINT32 rsvd : 8;

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UNCORE_SNC_CONFIG_IIO_CXL_CM_STRUCT;

/* UMA_CLUSTER_CONFIG_IIO_CXL_CM_REG supported on:                              */
/*      SPRA0 (0x20001a20)                                                      */
/*      SPRB0 (0x20001a20)                                                      */
/*      SPRHBM (0x20001a20)                                                     */
/*      SPRC0 (0x20001a20)                                                      */
/*      SPRMCC (0x20001a20)                                                     */
/*      SPRUCC (0x20001a20)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SNC Hash Register. DP Only. Configured at Boot.
*/


#define UMA_CLUSTER_CONFIG_IIO_CXL_CM_REG 0x23121A20

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 uma_clustering_enable : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable bit for UMA based clustering mode.
                               Overrides FULL_SNC_ENABLE and SNC_IND_ENABLE if
                               set.
                            */
    UINT32 defeature_xor : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /*
                               Enable bit for XOR defeature mode. By default,
                               UMA mode clustering uses
                               Addr[26:25]^Addr[18:17]^Addr[12:11]^Addr[9:8].
                               But when this bit is set to 1, Addr[12:11] are
                               used.
                            */
    UINT32 rsvd : 30;

                            /* Bits[31:2], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} UMA_CLUSTER_CONFIG_IIO_CXL_CM_STRUCT;

/* HASHADDRMASK_IIO_CXL_CM_REG supported on:                                    */
/*      SPRA0 (0x20001a24)                                                      */
/*      SPRB0 (0x20001a24)                                                      */
/*      SPRHBM (0x20001a24)                                                     */
/*      SPRC0 (0x20001a24)                                                      */
/*      SPRMCC (0x20001a24)                                                     */
/*      SPRUCC (0x20001a24)                                                     */
/* Register default value on SPRA0: 0x00FFFFFF                                  */
/* Register default value on SPRB0: 0x00FFFFFF                                  */
/* Register default value on SPRHBM: 0x00FFFFFF                                 */
/* Register default value on SPRC0: 0x00FFFFFF                                  */
/* Register default value on SPRMCC: 0x00FFFFFF                                 */
/* Register default value on SPRUCC: 0x00FFFFFF                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRB0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRHBM Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRC0 Security PolicyGroup: BOOTSMM_RW                                       */
/* SPRMCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SPRUCC Security PolicyGroup: BOOTSMM_RW                                      */
/* SNC Hash Register. DP Only. Configured at Boot.
*/


#define HASHADDRMASK_IIO_CXL_CM_REG 0x23121A24

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 hashaddrmask : 24;

                            /* Bits[23:0], Access Type=RW/P, default=0x00FFFFFF*/

                            /* Hash Address [51:28] Mask */
    UINT32 numberofcbox : 6;

                            /* Bits[29:24], Access Type=RW/P, default=0x00000000*/

                            /*
                               Total number of Cbo's (1 -> 64) A value of 0 in
                               this field specifies a value of 64 CBo's.
                            */
    UINT32 rsvd : 2;

                            /* Bits[31:30], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} HASHADDRMASK_IIO_CXL_CM_STRUCT;

/* PMON_1_UNIT_CONTROL_N0_IIO_CXL_CM_REG supported on:                          */
/*      SPRA0 (0x20001b00)                                                      */
/*      SPRB0 (0x20001b00)                                                      */
/*      SPRHBM (0x20001b00)                                                     */
/*      SPRC0 (0x20001b00)                                                      */
/*      SPRMCC (0x20001b00)                                                     */
/*      SPRUCC (0x20001b00)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Perfmon Unit 1 Control Register
*/


#define PMON_1_UNIT_CONTROL_N0_IIO_CXL_CM_REG 0x23121B00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 freeze_counters : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               If set to 1 the counters in this unit will be
                               frozen.
                            */
    UINT32 rsvd : 7;

                            /* Bits[7:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 reset_control : 1;

                            /* Bits[8:8], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the Counter Control Registers
                               will be reset to 0.
                            */
    UINT32 reset_counters : 1;

                            /* Bits[9:9], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the Counter Registers will be
                               reset to 0.
                            */
    UINT32 rsvd_10 : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMON_1_UNIT_CONTROL_N0_IIO_CXL_CM_STRUCT;

/* PMON_1_UNIT_CONTROL_N1_IIO_CXL_CM_REG supported on:                          */
/*      SPRA0 (0x20001b04)                                                      */
/*      SPRB0 (0x20001b04)                                                      */
/*      SPRHBM (0x20001b04)                                                     */
/*      SPRC0 (0x20001b04)                                                      */
/*      SPRMCC (0x20001b04)                                                     */
/*      SPRUCC (0x20001b04)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* Perfmon Unit 1 Control Register
*/


#define PMON_1_UNIT_CONTROL_N1_IIO_CXL_CM_REG 0x23121B04

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

} PMON_1_UNIT_CONTROL_N1_IIO_CXL_CM_STRUCT;

/* PMON_1_OVERFLOW_STATUS_REGISTER_N0_IIO_CXL_CM_REG supported on:              */
/*      SPRA0 (0x20001b08)                                                      */
/*      SPRB0 (0x20001b08)                                                      */
/*      SPRHBM (0x20001b08)                                                     */
/*      SPRC0 (0x20001b08)                                                      */
/*      SPRMCC (0x20001b08)                                                     */
/*      SPRUCC (0x20001b08)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Counter Overflow Status Register
*/


#define PMON_1_OVERFLOW_STATUS_REGISTER_N0_IIO_CXL_CM_REG 0x23121B08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_n_ovf : 4;

                            /* Bits[3:0], Access Type=RW/1C/V, default=0x00000000*/

                            /*
                               If an overflow is detected from the
                               corresponding data register, its overflow bit
                               will be set. NOTE: Write of 1 will clear the
                               bit.
                            */
    UINT32 rsvd : 28;

                            /* Bits[31:4], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMON_1_OVERFLOW_STATUS_REGISTER_N0_IIO_CXL_CM_STRUCT;

/* PMON_1_OVERFLOW_STATUS_REGISTER_N1_IIO_CXL_CM_REG supported on:              */
/*      SPRA0 (0x20001b0c)                                                      */
/*      SPRB0 (0x20001b0c)                                                      */
/*      SPRHBM (0x20001b0c)                                                     */
/*      SPRC0 (0x20001b0c)                                                      */
/*      SPRMCC (0x20001b0c)                                                     */
/*      SPRUCC (0x20001b0c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Counter Overflow Status Register
*/


#define PMON_1_OVERFLOW_STATUS_REGISTER_N1_IIO_CXL_CM_REG 0x23121B0C

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

} PMON_1_OVERFLOW_STATUS_REGISTER_N1_IIO_CXL_CM_STRUCT;

/* PMON_1_CONTROL_REGISTER_0_N0_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001b10)                                                      */
/*      SPRB0 (0x20001b10)                                                      */
/*      SPRHBM (0x20001b10)                                                     */
/*      SPRC0 (0x20001b10)                                                      */
/*      SPRMCC (0x20001b10)                                                     */
/*      SPRUCC (0x20001b10)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Counter Control Register 0
*/


#define PMON_1_CONTROL_REGISTER_0_N0_IIO_CXL_CM_REG 0x23121B10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 evslct : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Event Select Minimum field to select which of
                               the available events should be recorded in the
                               paired data register. Additional bits in the
                               control register may also be required to select
                               from the available events. 0x0 == Count Nothing
                               0x1 == Count Local Clockticks
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event. The selected sub-
                               events are bitwise OR-ed together to create
                               event. At least one sub-event must be selected
                               otherwise the PerfMon event signals will not
                               ever get asserted. Events with no sub-events
                               listed effectively have only one sub-event --
                               bit 8 must be set to 1 in this case.
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the corresponding counter will be
                               cleared to 0
                            */
    UINT32 edgedet : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1, rather than measuring the event
                               in each cycle it is active, the corresponding
                               counter will increment when a 0 to 1 transition
                               (i.e. rising edge) is detected. When 0, the
                               counter will increment in each cycle that the
                               event is asserted.
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovfenable : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               When an overflow is detected from this register,
                               a PMON overflow message is sent to the global
                               control. This bit will tell the global control
                               whether it should assert the global freeze for
                               all counters in the same domain.
                            */
    UINT32 rsvd_21 : 2;

                            /* Bits[22:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               Invert comparison against Threshold. 0 -
                               comparison will be is event increment >=
                               threshold?. 1 - comparison is inverted - is
                               event increment < threshold? e.g. for a 64-entry
                               queue, if SW wanted to know how many cycles the
                               queue had fewer than 4 entries, SW should set
                               the threshold to 4 and set the invert bit to 1.
                            */
    UINT32 threshold : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               Threshold on Counter Increment. Threshold is
                               used, along with the invert bit, to compare
                               against the incoming increment value that will
                               be added to the counter. For events that
                               increment by more than 1 per cycle, if the
                               threshold is set to a value greater than 1, the
                               data register will accumulate instances in which
                               the event increment is >=threshold. e.g. say you
                               have an event to accumulate the occupancy of a
                               64-entry queue every cycle. By setting the
                               threshold value to 60, the data register would
                               count the number of cycles the queues occupancy
                               was >= 60.
                            */

  } Bits;
  UINT32 Data;

} PMON_1_CONTROL_REGISTER_0_N0_IIO_CXL_CM_STRUCT;

/* PMON_1_CONTROL_REGISTER_0_N1_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001b14)                                                      */
/*      SPRB0 (0x20001b14)                                                      */
/*      SPRHBM (0x20001b14)                                                     */
/*      SPRC0 (0x20001b14)                                                      */
/*      SPRMCC (0x20001b14)                                                     */
/*      SPRUCC (0x20001b14)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Counter Control Register 0
*/


#define PMON_1_CONTROL_REGISTER_0_N1_IIO_CXL_CM_REG 0x23121B14

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

} PMON_1_CONTROL_REGISTER_0_N1_IIO_CXL_CM_STRUCT;

/* PMON_1_CONTROL_REGISTER_1_N0_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001b18)                                                      */
/*      SPRB0 (0x20001b18)                                                      */
/*      SPRHBM (0x20001b18)                                                     */
/*      SPRC0 (0x20001b18)                                                      */
/*      SPRMCC (0x20001b18)                                                     */
/*      SPRUCC (0x20001b18)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Counter Control Register 1
*/


#define PMON_1_CONTROL_REGISTER_1_N0_IIO_CXL_CM_REG 0x23121B18

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 evslct : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Event Select Minimum field to select which of
                               the available events should be recorded in the
                               paired data register. Additional bits in the
                               control register may also be required to select
                               from the available events. 0x0 == Count Nothing
                               0x1 == Count Local Clockticks
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event. The selected sub-
                               events are bitwise OR-ed together to create
                               event. At least one sub-event must be selected
                               otherwise the PerfMon event signals will not
                               ever get asserted. Events with no sub-events
                               listed effectively have only one sub-event --
                               bit 8 must be set to 1 in this case.
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the corresponding counter will be
                               cleared to 0
                            */
    UINT32 edgedet : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1, rather than measuring the event
                               in each cycle it is active, the corresponding
                               counter will increment when a 0 to 1 transition
                               (i.e. rising edge) is detected. When 0, the
                               counter will increment in each cycle that the
                               event is asserted.
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovfenable : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               When an overflow is detected from this register,
                               a PMON overflow message is sent to the global
                               control. This bit will tell the global control
                               whether it should assert the global freeze for
                               all counters in the same domain.
                            */
    UINT32 rsvd_21 : 2;

                            /* Bits[22:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               Invert comparison against Threshold. 0 -
                               comparison will be is event increment >=
                               threshold?. 1 - comparison is inverted - is
                               event increment < threshold? e.g. for a 64-entry
                               queue, if SW wanted to know how many cycles the
                               queue had fewer than 4 entries, SW should set
                               the threshold to 4 and set the invert bit to 1.
                            */
    UINT32 threshold : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               Threshold on Counter Increment. Threshold is
                               used, along with the invert bit, to compare
                               against the incoming increment value that will
                               be added to the counter. For events that
                               increment by more than 1 per cycle, if the
                               threshold is set to a value greater than 1, the
                               data register will accumulate instances in which
                               the event increment is >=threshold. e.g. say you
                               have an event to accumulate the occupancy of a
                               64-entry queue every cycle. By setting the
                               threshold value to 60, the data register would
                               count the number of cycles the queues occupancy
                               was >= 60.
                            */

  } Bits;
  UINT32 Data;

} PMON_1_CONTROL_REGISTER_1_N0_IIO_CXL_CM_STRUCT;

/* PMON_1_CONTROL_REGISTER_1_N1_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001b1c)                                                      */
/*      SPRB0 (0x20001b1c)                                                      */
/*      SPRHBM (0x20001b1c)                                                     */
/*      SPRC0 (0x20001b1c)                                                      */
/*      SPRMCC (0x20001b1c)                                                     */
/*      SPRUCC (0x20001b1c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Counter Control Register 1
*/


#define PMON_1_CONTROL_REGISTER_1_N1_IIO_CXL_CM_REG 0x23121B1C

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

} PMON_1_CONTROL_REGISTER_1_N1_IIO_CXL_CM_STRUCT;

/* PMON_1_CONTROL_REGISTER_2_N0_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001b20)                                                      */
/*      SPRB0 (0x20001b20)                                                      */
/*      SPRHBM (0x20001b20)                                                     */
/*      SPRC0 (0x20001b20)                                                      */
/*      SPRMCC (0x20001b20)                                                     */
/*      SPRUCC (0x20001b20)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Counter Control Register 2
*/


#define PMON_1_CONTROL_REGISTER_2_N0_IIO_CXL_CM_REG 0x23121B20

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 evslct : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Event Select Minimum field to select which of
                               the available events should be recorded in the
                               paired data register. Additional bits in the
                               control register may also be required to select
                               from the available events. 0x0 == Count Nothing
                               0x1 == Count Local Clockticks
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event. The selected sub-
                               events are bitwise OR-ed together to create
                               event. At least one sub-event must be selected
                               otherwise the PerfMon event signals will not
                               ever get asserted. Events with no sub-events
                               listed effectively have only one sub-event --
                               bit 8 must be set to 1 in this case.
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the corresponding counter will be
                               cleared to 0
                            */
    UINT32 edgedet : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1, rather than measuring the event
                               in each cycle it is active, the corresponding
                               counter will increment when a 0 to 1 transition
                               (i.e. rising edge) is detected. When 0, the
                               counter will increment in each cycle that the
                               event is asserted.
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovfenable : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               When an overflow is detected from this register,
                               a PMON overflow message is sent to the global
                               control. This bit will tell the global control
                               whether it should assert the global freeze for
                               all counters in the same domain.
                            */
    UINT32 rsvd_21 : 2;

                            /* Bits[22:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               Invert comparison against Threshold. 0 -
                               comparison will be is event increment >=
                               threshold?. 1 - comparison is inverted - is
                               event increment < threshold? e.g. for a 64-entry
                               queue, if SW wanted to know how many cycles the
                               queue had fewer than 4 entries, SW should set
                               the threshold to 4 and set the invert bit to 1.
                            */
    UINT32 threshold : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               Threshold on Counter Increment. Threshold is
                               used, along with the invert bit, to compare
                               against the incoming increment value that will
                               be added to the counter. For events that
                               increment by more than 1 per cycle, if the
                               threshold is set to a value greater than 1, the
                               data register will accumulate instances in which
                               the event increment is >=threshold. e.g. say you
                               have an event to accumulate the occupancy of a
                               64-entry queue every cycle. By setting the
                               threshold value to 60, the data register would
                               count the number of cycles the queues occupancy
                               was >= 60.
                            */

  } Bits;
  UINT32 Data;

} PMON_1_CONTROL_REGISTER_2_N0_IIO_CXL_CM_STRUCT;

/* PMON_1_CONTROL_REGISTER_2_N1_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001b24)                                                      */
/*      SPRB0 (0x20001b24)                                                      */
/*      SPRHBM (0x20001b24)                                                     */
/*      SPRC0 (0x20001b24)                                                      */
/*      SPRMCC (0x20001b24)                                                     */
/*      SPRUCC (0x20001b24)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Counter Control Register 2
*/


#define PMON_1_CONTROL_REGISTER_2_N1_IIO_CXL_CM_REG 0x23121B24

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

} PMON_1_CONTROL_REGISTER_2_N1_IIO_CXL_CM_STRUCT;

/* PMON_1_CONTROL_REGISTER_3_N0_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001b28)                                                      */
/*      SPRB0 (0x20001b28)                                                      */
/*      SPRHBM (0x20001b28)                                                     */
/*      SPRC0 (0x20001b28)                                                      */
/*      SPRMCC (0x20001b28)                                                     */
/*      SPRUCC (0x20001b28)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Counter Control Register 3
*/


#define PMON_1_CONTROL_REGISTER_3_N0_IIO_CXL_CM_REG 0x23121B28

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 evslct : 8;

                            /* Bits[7:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               Event Select Minimum field to select which of
                               the available events should be recorded in the
                               paired data register. Additional bits in the
                               control register may also be required to select
                               from the available events. 0x0 == Count Nothing
                               0x1 == Count Local Clockticks
                            */
    UINT32 unitmask : 8;

                            /* Bits[15:8], Access Type=RW/V, default=0x00000000*/

                            /*
                               This mask selects the sub-events to be selected
                               for creation of the event. The selected sub-
                               events are bitwise OR-ed together to create
                               event. At least one sub-event must be selected
                               otherwise the PerfMon event signals will not
                               ever get asserted. Events with no sub-events
                               listed effectively have only one sub-event --
                               bit 8 must be set to 1 in this case.
                            */
    UINT32 rsvd : 1;

                            /* Bits[16:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 counterreset : 1;

                            /* Bits[17:17], Access Type=RW/1S/V, default=0x00000000*/

                            /*
                               When set to 1, the corresponding counter will be
                               cleared to 0
                            */
    UINT32 edgedet : 1;

                            /* Bits[18:18], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1, rather than measuring the event
                               in each cycle it is active, the corresponding
                               counter will increment when a 0 to 1 transition
                               (i.e. rising edge) is detected. When 0, the
                               counter will increment in each cycle that the
                               event is asserted.
                            */
    UINT32 rsvd_19 : 1;

                            /* Bits[19:19], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 ovfenable : 1;

                            /* Bits[20:20], Access Type=RW/V, default=0x00000000*/

                            /*
                               When an overflow is detected from this register,
                               a PMON overflow message is sent to the global
                               control. This bit will tell the global control
                               whether it should assert the global freeze for
                               all counters in the same domain.
                            */
    UINT32 rsvd_21 : 2;

                            /* Bits[22:21], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 invert : 1;

                            /* Bits[23:23], Access Type=RW/V, default=0x00000000*/

                            /*
                               Invert comparison against Threshold. 0 -
                               comparison will be is event increment >=
                               threshold?. 1 - comparison is inverted - is
                               event increment < threshold? e.g. for a 64-entry
                               queue, if SW wanted to know how many cycles the
                               queue had fewer than 4 entries, SW should set
                               the threshold to 4 and set the invert bit to 1.
                            */
    UINT32 threshold : 8;

                            /* Bits[31:24], Access Type=RW/V, default=0x00000000*/

                            /*
                               Threshold on Counter Increment. Threshold is
                               used, along with the invert bit, to compare
                               against the incoming increment value that will
                               be added to the counter. For events that
                               increment by more than 1 per cycle, if the
                               threshold is set to a value greater than 1, the
                               data register will accumulate instances in which
                               the event increment is >=threshold. e.g. say you
                               have an event to accumulate the occupancy of a
                               64-entry queue every cycle. By setting the
                               threshold value to 60, the data register would
                               count the number of cycles the queues occupancy
                               was >= 60.
                            */

  } Bits;
  UINT32 Data;

} PMON_1_CONTROL_REGISTER_3_N0_IIO_CXL_CM_STRUCT;

/* PMON_1_CONTROL_REGISTER_3_N1_IIO_CXL_CM_REG supported on:                    */
/*      SPRA0 (0x20001b2c)                                                      */
/*      SPRB0 (0x20001b2c)                                                      */
/*      SPRHBM (0x20001b2c)                                                     */
/*      SPRC0 (0x20001b2c)                                                      */
/*      SPRMCC (0x20001b2c)                                                     */
/*      SPRUCC (0x20001b2c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Counter Control Register 3
*/


#define PMON_1_CONTROL_REGISTER_3_N1_IIO_CXL_CM_REG 0x23121B2C

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

} PMON_1_CONTROL_REGISTER_3_N1_IIO_CXL_CM_STRUCT;

/* PMON_1_DATA_REGISTER_0_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001b30)                                                      */
/*      SPRB0 (0x20001b30)                                                      */
/*      SPRHBM (0x20001b30)                                                     */
/*      SPRC0 (0x20001b30)                                                      */
/*      SPRMCC (0x20001b30)                                                     */
/*      SPRUCC (0x20001b30)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Performance Event counter 0
*/


#define PMON_1_DATA_REGISTER_0_N0_IIO_CXL_CM_REG 0x23121B30

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_1_DATA_REGISTER_0_N0_IIO_CXL_CM_STRUCT;

/* PMON_1_DATA_REGISTER_0_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001b34)                                                      */
/*      SPRB0 (0x20001b34)                                                      */
/*      SPRHBM (0x20001b34)                                                     */
/*      SPRC0 (0x20001b34)                                                      */
/*      SPRMCC (0x20001b34)                                                     */
/*      SPRUCC (0x20001b34)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Performance Event counter 0
*/


#define PMON_1_DATA_REGISTER_0_N1_IIO_CXL_CM_REG 0x23121B34

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_1_DATA_REGISTER_0_N1_IIO_CXL_CM_STRUCT;

/* PMON_1_DATA_REGISTER_1_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001b38)                                                      */
/*      SPRB0 (0x20001b38)                                                      */
/*      SPRHBM (0x20001b38)                                                     */
/*      SPRC0 (0x20001b38)                                                      */
/*      SPRMCC (0x20001b38)                                                     */
/*      SPRUCC (0x20001b38)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Performance Event counter 1
*/


#define PMON_1_DATA_REGISTER_1_N0_IIO_CXL_CM_REG 0x23121B38

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_1_DATA_REGISTER_1_N0_IIO_CXL_CM_STRUCT;

/* PMON_1_DATA_REGISTER_1_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001b3c)                                                      */
/*      SPRB0 (0x20001b3c)                                                      */
/*      SPRHBM (0x20001b3c)                                                     */
/*      SPRC0 (0x20001b3c)                                                      */
/*      SPRMCC (0x20001b3c)                                                     */
/*      SPRUCC (0x20001b3c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Performance Event counter 1
*/


#define PMON_1_DATA_REGISTER_1_N1_IIO_CXL_CM_REG 0x23121B3C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_1_DATA_REGISTER_1_N1_IIO_CXL_CM_STRUCT;

/* PMON_1_DATA_REGISTER_2_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001b40)                                                      */
/*      SPRB0 (0x20001b40)                                                      */
/*      SPRHBM (0x20001b40)                                                     */
/*      SPRC0 (0x20001b40)                                                      */
/*      SPRMCC (0x20001b40)                                                     */
/*      SPRUCC (0x20001b40)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Performance Event counter 2
*/


#define PMON_1_DATA_REGISTER_2_N0_IIO_CXL_CM_REG 0x23121B40

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_1_DATA_REGISTER_2_N0_IIO_CXL_CM_STRUCT;

/* PMON_1_DATA_REGISTER_2_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001b44)                                                      */
/*      SPRB0 (0x20001b44)                                                      */
/*      SPRHBM (0x20001b44)                                                     */
/*      SPRC0 (0x20001b44)                                                      */
/*      SPRMCC (0x20001b44)                                                     */
/*      SPRUCC (0x20001b44)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Performance Event counter 2
*/


#define PMON_1_DATA_REGISTER_2_N1_IIO_CXL_CM_REG 0x23121B44

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_1_DATA_REGISTER_2_N1_IIO_CXL_CM_STRUCT;

/* PMON_1_DATA_REGISTER_3_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001b48)                                                      */
/*      SPRB0 (0x20001b48)                                                      */
/*      SPRHBM (0x20001b48)                                                     */
/*      SPRC0 (0x20001b48)                                                      */
/*      SPRMCC (0x20001b48)                                                     */
/*      SPRUCC (0x20001b48)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Performance Event counter 3
*/


#define PMON_1_DATA_REGISTER_3_N0_IIO_CXL_CM_REG 0x23121B48

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_1_DATA_REGISTER_3_N0_IIO_CXL_CM_STRUCT;

/* PMON_1_DATA_REGISTER_3_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001b4c)                                                      */
/*      SPRB0 (0x20001b4c)                                                      */
/*      SPRHBM (0x20001b4c)                                                     */
/*      SPRC0 (0x20001b4c)                                                      */
/*      SPRMCC (0x20001b4c)                                                     */
/*      SPRUCC (0x20001b4c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: OS_W                                             */
/* SPRHBM Security PolicyGroup: OS_W                                            */
/* SPRC0 Security PolicyGroup: OS_W                                             */
/* SPRMCC Security PolicyGroup: OS_W                                            */
/* SPRUCC Security PolicyGroup: OS_W                                            */
/* PMON Unit 1 Performance Event counter 3
*/


#define PMON_1_DATA_REGISTER_3_N1_IIO_CXL_CM_REG 0x23121B4C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 counter_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* This is the current value of the counter. */

  } Bits;
  UINT32 Data;

} PMON_1_DATA_REGISTER_3_N1_IIO_CXL_CM_STRUCT;

/* PMON_UNIT_1_DISCOVERY0_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001c00)                                                      */
/*      SPRB0 (0x20001c00)                                                      */
/*      SPRHBM (0x20001c00)                                                     */
/*      SPRC0 (0x20001c00)                                                      */
/*      SPRMCC (0x20001c00)                                                     */
/*      SPRUCC (0x20001c00)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Discovery state register 0
*/


#define PMON_UNIT_1_DISCOVERY0_N0_IIO_CXL_CM_REG 0x23121C00

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_unit_disc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* PMON Discovery Unit 1 Discovery 0 */

  } Bits;
  UINT32 Data;

} PMON_UNIT_1_DISCOVERY0_N0_IIO_CXL_CM_STRUCT;

/* PMON_UNIT_1_DISCOVERY0_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001c04)                                                      */
/*      SPRB0 (0x20001c04)                                                      */
/*      SPRHBM (0x20001c04)                                                     */
/*      SPRC0 (0x20001c04)                                                      */
/*      SPRMCC (0x20001c04)                                                     */
/*      SPRUCC (0x20001c04)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Discovery state register 0
*/


#define PMON_UNIT_1_DISCOVERY0_N1_IIO_CXL_CM_REG 0x23121C04

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_unit_disc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* PMON Discovery Unit 1 Discovery 0 */

  } Bits;
  UINT32 Data;

} PMON_UNIT_1_DISCOVERY0_N1_IIO_CXL_CM_STRUCT;

/* PMON_UNIT_1_DISCOVERY1_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001c08)                                                      */
/*      SPRB0 (0x20001c08)                                                      */
/*      SPRHBM (0x20001c08)                                                     */
/*      SPRC0 (0x20001c08)                                                      */
/*      SPRMCC (0x20001c08)                                                     */
/*      SPRUCC (0x20001c08)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Discovery state register 1
*/


#define PMON_UNIT_1_DISCOVERY1_N0_IIO_CXL_CM_REG 0x23121C08

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_unit_disc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* PMON Discovery Unit 1 Discovery 1 */

  } Bits;
  UINT32 Data;

} PMON_UNIT_1_DISCOVERY1_N0_IIO_CXL_CM_STRUCT;

/* PMON_UNIT_1_DISCOVERY1_N1_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001c0c)                                                      */
/*      SPRB0 (0x20001c0c)                                                      */
/*      SPRHBM (0x20001c0c)                                                     */
/*      SPRC0 (0x20001c0c)                                                      */
/*      SPRMCC (0x20001c0c)                                                     */
/*      SPRUCC (0x20001c0c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Discovery state register 1
*/


#define PMON_UNIT_1_DISCOVERY1_N1_IIO_CXL_CM_REG 0x23121C0C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_unit_disc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* PMON Discovery Unit 1 Discovery 1 */

  } Bits;
  UINT32 Data;

} PMON_UNIT_1_DISCOVERY1_N1_IIO_CXL_CM_STRUCT;

/* PMON_UNIT_1_DISCOVERY2_N0_IIO_CXL_CM_REG supported on:                       */
/*      SPRA0 (0x20001c10)                                                      */
/*      SPRB0 (0x20001c10)                                                      */
/*      SPRHBM (0x20001c10)                                                     */
/*      SPRC0 (0x20001c10)                                                      */
/*      SPRMCC (0x20001c10)                                                     */
/*      SPRUCC (0x20001c10)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Discovery state register 2
*/


#define PMON_UNIT_1_DISCOVERY2_N0_IIO_CXL_CM_REG 0x23121C10

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_unit_disc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* PMON Discovery Unit 2 Discovery 2 */

  } Bits;
  UINT32 Data;

} PMON_UNIT_1_DISCOVERY2_N0_IIO_CXL_CM_STRUCT;

/* PMON_UNIT_1_DISCOVERY2_N1_IIO_CXL_CM_REG supported on:                       */
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
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Discovery state register 2
*/


#define PMON_UNIT_1_DISCOVERY2_N1_IIO_CXL_CM_REG 0x23121C14

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_unit_disc : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* PMON Discovery Unit 2 Discovery 2 */

  } Bits;
  UINT32 Data;

} PMON_UNIT_1_DISCOVERY2_N1_IIO_CXL_CM_STRUCT;

/* PMON_BLOCK_IDENTIFIER_1_N0_IIO_CXL_CM_REG supported on:                      */
/*      SPRA0 (0x20001c18)                                                      */
/*      SPRB0 (0x20001c18)                                                      */
/*      SPRHBM (0x20001c18)                                                     */
/*      SPRC0 (0x20001c18)                                                      */
/*      SPRMCC (0x20001c18)                                                     */
/*      SPRUCC (0x20001c18)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Block 1 Identifier
*/


#define PMON_BLOCK_IDENTIFIER_1_N0_IIO_CXL_CM_REG 0x23121C18

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 pmon_blk_id : 16;

                            /* Bits[15:0], Access Type=RW, default=0x00000000*/

                            /*
                               Local PMON Block Identifier. BIOS writes it with
                               the block ID, the PMON block logic uses that ID
                               for the overflow message.
                            */
    UINT32 rsvd : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} PMON_BLOCK_IDENTIFIER_1_N0_IIO_CXL_CM_STRUCT;

/* PMON_BLOCK_IDENTIFIER_1_N1_IIO_CXL_CM_REG supported on:                      */
/*      SPRA0 (0x20001c1c)                                                      */
/*      SPRB0 (0x20001c1c)                                                      */
/*      SPRHBM (0x20001c1c)                                                     */
/*      SPRC0 (0x20001c1c)                                                      */
/*      SPRMCC (0x20001c1c)                                                     */
/*      SPRUCC (0x20001c1c)                                                     */
/* Register default value on SPRA0: 0x00000000                                  */
/* Register default value on SPRB0: 0x00000000                                  */
/* Register default value on SPRHBM: 0x00000000                                 */
/* Register default value on SPRC0: 0x00000000                                  */
/* Register default value on SPRMCC: 0x00000000                                 */
/* Register default value on SPRUCC: 0x00000000                                 */
/* SPRA0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRB0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRHBM Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRC0 Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst            */
/* SPRMCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* SPRUCC Register File:    sprsp_top/ialcm_dp_regs[0]/ialcm_mem_inst           */
/* Struct generated from SPRA0 BDF: 0_0_0                                       */
/* SPRA0 Security PolicyGroup: OS_W                                             */
/* SPRB0 Security PolicyGroup: BIOS_W                                           */
/* SPRHBM Security PolicyGroup: BIOS_W                                          */
/* SPRC0 Security PolicyGroup: BIOS_W                                           */
/* SPRMCC Security PolicyGroup: BIOS_W                                          */
/* SPRUCC Security PolicyGroup: BIOS_W                                          */
/* PMON Block 1 Identifier
*/


#define PMON_BLOCK_IDENTIFIER_1_N1_IIO_CXL_CM_REG 0x23121C1C

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

} PMON_BLOCK_IDENTIFIER_1_N1_IIO_CXL_CM_STRUCT;
#endif /* _IIO_CXL_CM_h */
